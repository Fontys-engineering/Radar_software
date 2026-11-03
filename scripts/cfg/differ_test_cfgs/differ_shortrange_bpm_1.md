# differ_shortrange_bpm_1.cfg

Copy of `differ_shortrange_hires_1.cfg` with one change: all 4 TX transmit in
**every** chirp, each with a ±180° phase code (BPM, binary phase modulation).
The code is a 4×4 Hadamard matrix over the 4 chirps of a loop. Decoding it
offline gives back the same 16 TX-RX channels as the TDM config, each with
about **6 dB more SNR** for the same frame time. Those channels form the virtual
array used for angle, so the lateral plasma position is measured with that extra
SNR.

Run it with:

```
scripts/radar_cli.sh -f scripts/cfg/differ_test_cfgs/differ_shortrange_bpm_1.cfg
```

No firmware change is needed: `bpmCfgAdvanced` is part of the SDK CLI table
(`cli_mmwave.c`, compiled in via `Makefile`).

## Why BPM and not beam steering

The AWR2944 has 6-bit TX phase shifters (5.625°/LSB). We looked at using them
to steer, and rejected it for now:

- The CLI can only set them per profile (`profileCfg` field 7, packed integer).
  The firmware accepts one profile per frame, so a sweep would need one cfg and
  one run per angle. Per-chirp phase shifting only exists in the DDM build, and
  we build TDM.
- TX0, TX2 and TX3 are 2λ apart, so a TX-only beam has grating lobes every 30°.
- Steering does not add resolution. Resolution is set by the aperture, and the
  12-element virtual array we already capture covers it: ~9.5° at boresight,
  ~17 cm cross-range at 1 m.

TDM captures already hold every TX-RX channel, so any beam can be formed
offline. What a physical change can still add is **SNR**, which the plasma
return needs. BPM gives that without giving up the channel separation.

## The code

| Chirp | TX0 | TX1 | TX2 | TX3 | `bpmCfgAdvanced` |
|-------|-----|-----|-----|-----|------------------|
| 0     | +   | +   | +   | +   | `0 0 0`          |
| 1     | +   | −   | +   | −   | `1 1 136` (b3 TX1 + b7 TX3) |
| 2     | +   | +   | −   | −   | `2 2 160` (b5 TX2 + b7 TX3) |
| 3     | +   | −   | −   | +   | `3 3 40`  (b3 TX1 + b5 TX2) |

`constBpmVal` bits: b1 TX0, b3 TX1, b5 TX2, b7 TX3 (set = 180°).
Decode per loop, RX and range bin: `x_tx = ¼ Σ_c H[c,tx] · y_c`.

Each decoded channel gets the energy of 4 chirps instead of 1, while the noise
is averaged over 4 chirps. Noise per channel drops by 4, which is +6 dB
compared to TDM.

## Changes from differ_shortrange_hires_1.cfg

| Parameter      | Before           | After                       |
|----------------|------------------|-----------------------------|
| chirpCfg mask  | 1, 4, 8, 2       | 15, 15, 15, 15              |
| bpmCfgAdvanced | —                | 4 lines (table above)       |

Everything else is identical, including the data size: profile (76.55 GHz,
4 GHz, 80 samples @ 2 Msps, ~3.75 cm, R_max 1.5 m), `frameCfg 0 3 16`,
640 B/chirp, 40960 B/frame and the LVDS/plasma lines. `channelCfg 15 15 0` stays
the same, so switching between this cfg and hires_1 needs no NRST.

The demo's rfparser sees txMask 15 on every chirp and treats the frame as SIMO
(4 virtual antennas on the DSS). That is valid and does not touch the raw ADC
stream.

## Session procedure

1. **Bench check** (plate at ~1 m). Send the cfg and confirm `sensorStart`
   answers `Done`. Then record a few seconds and run:

   ```
   scripts/dca1000/parse_capture_mimo.py scripts/cfg/differ_test_cfgs/differ_shortrange_bpm_1.cfg \
       <bench>_Raw_0.bin --no-plot
   ```

   `Tx power` must show all 4 TX within a few dB of each other. If TX1–TX3
   are >10 dB down, the board ignored the code: all energy then lands in TX0
   and the script warns about it.
2. **Calibration** (needed for absolute angles). Put a corner reflector or plate
   at boresight, inside the gate, with nothing else close. Record ~10 s and run:

   ```
   parse_capture_mimo.py <cfg> <calib>_Raw_0.bin --gate 0.8 1.2 --calib-out bpm_calib.npz
   ```

   If the reflector is off-axis, give its azimuth with `--calib-az DEG`. Then
   move it to one side and check that the angle goes the right way; if not,
   use `--flip-az` from then on. Do the same in elevation (`--flip-el`).
   The board's TX phases do not depend on the code, so a calibration from
   hires_1 (TDM) also works for this cfg, and the other way round.
3. **Empty chamber**, ~10 s: the clutter reference (window, flange, back wall).
4. **Plasma**: BPM runs. Alternate with a short hires_1 run as the TDM
   reference for the SNR comparison (`channel SNR` line of the summary).
5. **Record the cfg file name and the distances for every capture.** The
   existing `record_data/` folders hold neither.

## Analysing: parse_capture_mimo.py

The same script reads TDM and BPM captures. It builds the code from the cfg's
`chirpCfg` masks and `bpmCfgAdvanced` lines, and the virtual array from
`antGeometryCfg`. It also works on the saved TDM captures, for example:

```
parse_capture_mimo.py scripts/cfg/plasma_capture_4tx.cfg \
    scripts/dca1000/record_data/test2/4tx4rx_Raw_0.bin --gate 0.3 2.9 --png figs/
```

Typical plasma run:

```
parse_capture_mimo.py <cfg> <meas>_Raw_0.bin --calib bpm_calib.npz \
    --clutter mean --dopcomp --capon --csv out.csv --png figs/
```

| Option        | Use |
|---------------|-----|
| `--calib`     | always, once a calibration exists |
| `--clutter mean` | removes everything static (window, flange, back wall), so the faint changing plasma return is not hidden under their sidelobes |
| `--dopcomp`   | target moves between the 4 chirps of a loop; removes the leakage between decoded TX (BPM) or the array tilt (TDM) |
| `--capon`     | MVDR azimuth spectrum, sharper than the ~9.5° beamformer when SNR allows |
| `--gate`      | defaults to the `plasmaCfg` gate of the cfg |

Per frame it reports range, azimuth, lateral offset `R·sin(az)`, elevation,
SNR and the pulse-pair velocity (`--csv`).

Figures:

1. **Angle**:
   - range-azimuth map and top view
   - azimuth spectrum at the peak
   - phase over the virtual array: a straight line means one target, and steps
     at the TX boundaries mean it needs calibration
2. **Over time**: azimuth-time map at the peak range, azimuth/elevation, lateral
   offset, range and SNR, velocity.
3. **Health**: decoded power per TX vs range, SNR per virtual channel, the code
   matrix.

## Caveats

- **Motion inside a loop.** The 4 chirps are 54 µs apart, so motion leaks
  between the decoded channels. At 0.5 m/s this costs ~1–2 dB on some TX and a
  small angle bias. `--dopcomp` removes it, as checked on synthetic data.
- **Strong static returns.** Static returns (TX-RX coupling, window) also add up
  over 4 TX. Watch for clipping in the near range.
- **Elevation.** It comes from the 4 TX1 channels only, which sit 0.8λ above the
  azimuth row. It is unambiguous within ±39° and coarse.
- **Sign conventions.** The sign of azimuth/elevation depends on how the board
  is mounted. Check it with a reflector to one side (step 2).
- **What this cfg does not change.** Electron density still comes from the
  O-mode cutoff and range per sub-band (Doc/05). This cfg keeps the hires_1
  sweep, so that analysis is unchanged.

## Without hardware

`--synth` writes a fake capture for any cfg (TDM or BPM). It takes point
targets, motion and per-TX phase errors:

```
parse_capture_mimo.py <cfg> --synth t_Raw_0.bin --target 0.9 12 5 --target 1.2 -20 0 -6 \
    --tx-phase 0 30 -50 80 --noise 2000
```

Checked on synthetic data:

| Case | Result |
|------|--------|
| Same scene and noise, hires_1 vs bpm_1 | channel SNR 17.5 → 23.5 dB (+6.0 dB) |
| TX phase errors 0/30/−50/80°, calibration from a boresight reflector | recovered exactly. Target at az 12°, el 5° measured 11.96°/4.98° (TDM) and 11.95°/5.00° (BPM, with the TDM calibration) |
| BPM, target at 0.5 m/s | TX leakage up to −1.7 dB, az 12.07° → with `--dopcomp` 0.0 dB, 12.00° |
| Recorded without the code, decoded as BPM | TX1–TX3 at −56 dB, warning printed |

## Fallbacks

| Problem | Change |
|---------|--------|
| `sensorStart` fails after the BPM lines | remove the 4 `bpmCfgAdvanced` lines. All TX then transmit in phase: a boresight beam with more SNR, but no angle |
| still fails, or TX power/thermal issues | use `differ_shortrange_hires_1.cfg` (TDM) |
| 76.55 GHz start rejected | `profileCfg 0 77 10 2.5 40 0 0 100 1 72 2000 0 0 30` + `frameCfg` 5th field 72 |
