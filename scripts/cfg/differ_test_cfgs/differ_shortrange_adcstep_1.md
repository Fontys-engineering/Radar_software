# differ_shortrange_adcstep_1.cfg

Copy of `differ_shortrange_hires_1.cfg` that samples the ramp in three ADC windows,
each starting 16 µs later than the previous one. One window is taken per repeat of
the 4-TX chirp sequence. The goal is to stitch the windows back together offline.

Run it with:

```
scripts/radar_cli.sh -f scripts/cfg/differ_test_cfgs/differ_shortrange_adcstep_1.cfg
```

## How the step works

`chirpCfg` field 7 (`adcStartTimeVar`) adds an extra ADC start delay to each chirp,
in µs with a 10 ns step. The hardware allows at most ~40.96 µs, so with a 16 µs step
only **three offsets fit: +0, +16, +32 µs**.

All loops in a frame repeat the same chirps, so the TX sequence (TX0, TX2, TX3, TX1)
is defined three times as chirps 0–11, once per offset. `frameCfg` loops over all 12.

| Chirps | ADC start offset | ADC window | RF band          |
|--------|------------------|------------|------------------|
| 0–3    | +0 µs            | 3–19 µs    | 76.80–78.15 GHz  |
| 4–7    | +16 µs           | 19–35 µs   | 78.15–79.49 GHz  |
| 8–11   | +32 µs           | 35–51 µs   | 79.49–80.83 GHz  |

Each window is 600 samples at 37.5 Msps = 16 µs, so the windows meet edge to edge,
with no gap and no overlap.

## Changes from differ_shortrange_hires_1.cfg

| Parameter   | Before        | After          | Why |
|-------------|---------------|----------------|-----|
| rampEnd     | 44 µs         | 52 µs          | fits the last window: 3 + 32 + 16 + 1 µs margin |
| slope       | 100 MHz/µs    | 84 MHz/µs      | keeps the sweep inside the VCO range (76.55–80.918 GHz) |
| numSamples  | 80            | 600            | 16 µs window |
| Fs          | 2000 ksps     | 37500 ksps     | |
| chirpCfg    | chirps 0–3    | chirps 0–11    | TX sequence × 3 offsets |
| frameCfg    | `0 3 16`      | `0 11 4`       | 12 chirps/loop, 4 loops (48 chirps/frame) |

Loops go from 16 to 4 so the firmware's radar cube stays smaller than in the
600-sample DIFFER configs that already run (64 chirps/frame). The firmware
(`Radar_Program_mss/utils/mmwdemo_rfparser.c`) counts unique TX bits, so 12 chirps
per loop with 4 TX is accepted as 4 TX antennas.

## What to expect

- **Per window:** 84 MHz/µs × 16 µs = 1.344 GHz → ~11.2 cm resolution.
- **Stitched:** 3–51 µs = 4.03 GHz → ~3.72 cm.
- **Max range:** (37.5 Msps / 2) · c / (2 · 84 MHz/µs) ≈ 33.5 m.
- **Range FFT:** 1024 points → 512 bins, 6.5 cm bin spacing (zero padded).

Range resolution depends only on the total swept bandwidth.
`differ_shortrange_hires_1.cfg` already samples 4.0 GHz in a single window, so
stitching gives a different sampling of the same band, not finer resolution than
that config. The VCO bandwidth (~4.4 GHz) is the real limit.

## Frame geometry and budgets

- 600 samples real int16, 4 Rx, 12 chirps/loop, 4 loops
- 48 chirps/frame, 4800 B/chirp, 230400 B/frame
- 33.333 ms frame → 6.9 MB/s (~55 Mbit/s) over Ethernet
- Chirping 48 × 62 µs = 3.0 ms of the 33.3 ms frame (9 %, demo rule ≤ 50 %)
- LVDS: Tc = 62 µs → 9300 B ≥ 4800 B per chirp, OK

## Caveats

- `parse_capture*.py` and the firmware compute the center frequency and range axis
  from the profile adcStart (3 µs) only. Chirps 4–11 sit 1.344 / 2.688 GHz higher in
  RF, so the wavelength (phase → displacement) is off for those chirps unless the
  parser corrects each chirp group.
- `parse_capture.py` / `parse_capture_adv.py` treat the 12 chirps as separate
  chirps. Use `parse_capture_adcstep.py` (below) for this config.

## Analysing captures: parse_capture_adcstep.py

`scripts/dca1000/parse_capture_adcstep.py` puts the three windows of each Tx/Rx
channel back on the ramp time axis and processes them as one 48 µs chirp. A
target's beat tone depends only on the time since ramp start, so the windows
continue each other. Each window's FFT is referenced to its own start time and
the three are summed under one Hann window over the full 1800 samples.

The windows of one Tx come from **different chirps** (248 µs and 496 µs apart).
They only add up correctly if the phase between those chirps is consistent. Two
things can break that, and the script measures both:

- **Chip offsets:** a fixed phase/gain difference between chirps with different
  ADC start. This is constant over time.
- **Motion:** the target moves between the chirps. The window phase then follows
  4π·v·Δt/λ, which the script predicts from the pulse-pair velocity.

### Recommended procedure

1. **Calibration capture.** Put one metal plate or corner reflector in the gate
   (nothing else close to it) and record ~10 s with this cfg. Then:

   ```
   scripts/dca1000/parse_capture_adcstep.py scripts/cfg/differ_test_cfgs/differ_shortrange_adcstep_1.cfg \
       <calib>_Raw_0.bin --calib-out adcstep_calib.npz
   ```

   The summary prints the window phase/gain. If it is ~0°, the chip is coherent
   across chirps and no calibration is needed.
2. **Measurement capture** (plasma):

   ```
   scripts/dca1000/parse_capture_adcstep.py scripts/cfg/differ_test_cfgs/differ_shortrange_adcstep_1.cfg \
       <meas>_Raw_0.bin --align calib --calib adcstep_calib.npz --csv out.csv --png figs/
   ```

   Use `--align none` instead if step 1 showed ~0°.
3. Read the `== hint ==` line at the end of the summary.

### Align modes

| Mode    | Removes | Use when |
|---------|---------|----------|
| `none`  | nothing | static scene, coherent chip |
| `calib` | stored chip phase/gain per window (`--calib`) | always safe once calibrated; does not touch the scene |
| `fixed` | this capture's mean window phase | single-target captures only |
| `peak`  | window phase per frame and loop | target moves between chirps (hint says so) |

Two targets closer than one window resolution cell (~16 cm) beat inside each
16 µs window, and that looks exactly like a window phase step. That beat is the
information stitching uses to separate them, so `fixed`/`peak` on such a scene
destroys resolution. The script detects this (the windows report the peak at
different ranges) and tells you to stay on `none`/`calib`.

### Figures

1. **Stitching:** the raw ADC trace on the ramp time axis with zooms of the
   seams, where a jump means offset or motion. Also the mean range profile of
   each window, their incoherent sum and every stitched variant with the
   −3 dB widths; the window phase over time with the motion prediction; window
   gain and coherence η (1 = in phase); and DC/RMS per window.
2. **Over time:** the stitched range-time map, peak range (stitched vs each window
   alone), power/SNR, phase displacement and velocity.
3. **Doppler and health:** the velocity spectrum at the target bin, a
   range-Doppler map, the profile per Rx, power per Tx-Rx channel, and ADC
   RMS/clipping/loss.

### Without hardware

`--synth` writes a fake capture for the cfg, with options for targets, motion
and chip offsets:

```
parse_capture_adcstep.py <cfg> --synth t_Raw_0.bin --range 0.80 0.87 --win-phase 0 40 -70
parse_capture_adcstep.py <cfg> --synth v_Raw_0.bin --range 0.8 --vib-mm 2 --vib-hz 50
```

Results checked on synthetic data:

| Case | Result |
|------|--------|
| One static target | window phase 0.0°, η = 1.000, stitched width 5.30 cm (theory 5.4 cm), range exact |
| Calibration with chip offsets 0/40/−70° | measured 0.0/40.0/−70.0° |
| Two targets 7 cm apart, with chip offsets, `--align calib` | same profile as the offset-free capture |
| 2 mm / 50 Hz vibration | window-phase scatter drops from 42° to 1° after removing the pulse-pair motion |

## Fallbacks

| Problem at `sensorStart`        | Change |
|---------------------------------|--------|
| out of memory                   | `frameCfg 0 11 2 0 600 33.333 1 0` |
| 76.55 GHz start rejected        | `profileCfg 0 77 10 3 52 0 0 75 1 600 37500 0 0 30` (77 + 75 MHz/µs × 52 µs = 80.9 GHz) |
