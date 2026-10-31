# 10 - Raw ADC Data Capture (DCA1000EVM)

How to record raw ADC data from the AWR2944EVM with a DCA1000EVM, while the current demo firmware runs. No firmware changes are needed: the demo's LVDS streaming (`Radar_Program_mss/mss/mmw_lvds_stream.c`, TI code) is always compiled in and is switched on by one cfg line.

**Not verified on our hardware yet.** Do a bench dry run (section 7) before a measurement session.

## 1. What gets captured

| Item            | Value                                                                                                                  |
| --------------- | ---------------------------------------------------------------------------------------------------------------------- |
| Data            | Raw ADC samples of every chirp. **Real only**, int16 (AWR294x has no complex ADC mode).                                |
| Order           | Per chirp `[Rx0 N samples][Rx1]..[Rx3]` (non-interleaved, `adcbufCfg -1 1 1 1 1`). Chirps in cfg order, loops, frames. |
| Not included    | No header, no frame markers, no detected objects. File = samples only. Frame boundaries come from the cfg.             |
| Link            | 2 LVDS lanes, 16 bit, DDR clock -> DCA1000 -> UDP over gigabit Ethernet -> `<prefix>_Raw_0.bin` on the PC              |
| UART TLV output | Still works in parallel (Visualizer / our scripts).                                                                    |

For `scripts/cfg/plasma_capture.cfg`: 144 samples × 4 Rx × 2 B = 1152 B/chirp, 48 chirps/frame = **55 296 B/frame**, 30 fps ≈ 13 Mbit/s.

## 2. Enabling it in the cfg

Add before `sensorStart` (it is ignored when the sensor is already running):

```
lvdsStreamCfg -1 0 1 0      % <subFrameIdx> <enableHeader> <dataFmt> <enableSW>
```

| Arg            | Use   | Why                                                                                           |
| -------------- | ----- | --------------------------------------------------------------------------------------------- |
| `enableHeader` | **0** | The demo does not support the HSI header for HW streaming. Parsers assume no header.          |
| `dataFmt`      | **1** | 1 = ADC. 4 = CP+ADC+CQ (adds chirp params/quality, not needed). 2, 3 assert at `sensorStart`. |
| `enableSW`     | **0** | 1 stops the stream after the first frame (`mmw_lvds_stream.c:639`).                           |

Also required: `adcCfg 2 0` (16 bit, real), `adcbufCfg ... 1` (chirpThreshold 1), `numAdcSamples ≥ 64`.

**LVDS budget per chirp:** `Tc × 2 lanes × 600 Mbps / 8 ≥ samples × Rx × 2 B`, with `Tc = idleTime + rampEndTime`. For plasma_capture.cfg: 157 µs → 23 550 B ≥ 1152 B. The DCA1000 → PC link gives ~325 Mbit/s at the default 25 µs packet delay.

## 3. Hardware setup

| Connection           | Detail                                                                                                     |
| -------------------- | ---------------------------------------------------------------------------------------------------------- |
| EVM power            | 12 V (≥ 2.5 A) on J12. **Power the EVM before plugging in USB.**                                           |
| DCA1000 power        | 5 V on DCA1000 J2, **SW3 set to the DC jack**. The AWR2944EVM does not supply power over the 60-pin cable. |
| LVDS                 | 60-pin Samtec cable: EVM J7 -> DCA1000 J3                                                                  |
| Ethernet             | DCA1000 J6 -> **gigabit** adapter on the PC                                                                |
| CLI UART             | EVM J10 (XDS110 USB), "Application/User UART", 115200 baud (`/dev/ttyACM0` on Linux)                       |
| DCA1000 USB (J1, J4) | Not needed                                                                                                 |

**DCA1000 switches:** SW2.5 = `SW_CONFIG` (software config, the other SW2 switches are then ignored), SW2.6 = default FPGA IP. Compare with the AWR2944 photo in the EVM guide (spruj22c, Fig. 2-27).

**EVM SOP jumpers** (J17/J18/J20, closed = 1, press NRST after changing):

| SOP[2:0] | Mode        | Use                                               |
| -------- | ----------- | ------------------------------------------------- |
| 101      | Flashing    | UniFlash. Unplug the Samtec cable while flashing. |
| 001      | Functional  | Run the demo = capture mode                       |
| 011      | Development | mmWave Studio only (not used here)                |

## 4. PC network setup

- Set the DCA1000 adapter to static **192.168.33.30 / 255.255.255.0**. DCA1000 is 192.168.33.180, config port 4096, data port 4098.
- Disable the firewall (Windows) or allow UDP 4096/4098. Turn off Wi-Fi if it uses the same subnet.
- Linux: `sudo ip addr add 192.168.33.30/24 dev <eth>` (not persistent, gone after reboot or a NetworkManager reconnect). Going through a gigabit switch works.
- Check with `scripts/dca1000/dca1000_ctl.py version`, which should print the FPGA version (2.9 on ours). **`ping` gets no reply**: the FPGA does not answer ICMP, so a failed ping means nothing. `ip neigh show 192.168.33.180` showing a MAC also confirms the link.

## 5. Capture sequence

Recording must start **before** `sensorStart`, otherwise the start of the first frame is missing and the file no longer begins on a frame boundary.

**Windows (TI tool)** `~/ti/radar_toolbox_4_00_00_05/tools/Adc_Data_Capture_Tool_DCA1000_CLI/`, using our `scripts/dca1000/cf.json` (set `fileBasePath` and `filePrefix` per run):

```
DCA1000EVM_CLI_Control.exe fpga cf.json
DCA1000EVM_CLI_Control.exe record cf.json
DCA1000EVM_CLI_Control.exe start_record cf.json
   -> send scripts/cfg/plasma_capture.cfg (Visualizer, terminal, or radar_cli.sh)
   -> measure
   -> sensorStop
DCA1000EVM_CLI_Control.exe stop_record cf.json
```

**Linux (fallback, untested on hardware)**, `scripts/dca1000/dca1000_ctl.py` (Python 3, stdlib only):

```
scripts/dca1000/dca1000_ctl.py fpga
scripts/dca1000/dca1000_ctl.py record -o data -p run01      # terminal 1, Ctrl-C stops
scripts/radar_cli.sh -f scripts/cfg/plasma_capture.cfg      # terminal 2
   -> measure
scripts/radar_cli.sh sensorStop                             # then Ctrl-C in terminal 1
```

The script writes `run01_Raw_0.bin` and `run01_Raw_LogFile.csv`. It shows lost and out-of-order packets live. Lost packets are zero-filled.

## 6. Checking the data

```
scripts/dca1000/parse_capture.py scripts/cfg/plasma_capture.cfg data/run01_Raw_0.bin
scripts/dca1000/parse_capture.py scripts/cfg/plasma_capture.cfg data/run01_Raw_0.bin --save run01.npz --no-plot
```

It prints the frame count, leftover bytes, clipped samples, zero chirps and the strongest range bin. It plots raw ADC, the range profile and range vs time. **Always pass the same cfg that was sent to the board.**

Expected: leftover bytes 0, file size = frames × 55 296 B, clipped ≈ 0, and the peak at the known target distance.

## 7. Bench dry run (do this before the measurement day)

1. Flash the current build (SOP 101), then switch to SOP 001 + NRST.
2. Put a metal plate at a known distance, around 1 m (R_max of plasma_capture.cfg is about 2.96 m).
3. Run section 5 for about 10 s, then section 6.
4. The peak must land at the plate distance. If it doesn't, the assumed byte layout is wrong. Likely culprits are the 2-lane interleaving and Rx order; compare with TI's MATLAB parser `rawDataCaptureGUI_DCA1000CLI.m`.
5. Check that the UART output still works during capture.
6. Useful backup reference: TI E2E guide "Setting up AWR2944 + DCA1000 for mmWave Studio" (the previous team's report, ref. [12]).

## 8. Per run, write down

| Field                               | Example                                      |
| ----------------------------------- | -------------------------------------------- |
| File prefix                         | `run01`                                      |
| Cfg file (copy it next to the .bin) | `plasma_capture.cfg`                         |
| Date / time                         |                                              |
| Setup                               | waveguide, cable lengths, distance to plasma |
| Plasma conditions                   | gas, pressure, power, B field                |
| Notes                               | lost packets, LEDs, anomalies                |

Also record references for calibration and subtraction: a **metal plate at a known distance** at the waveguide output, and an **empty run** (setup on, no plasma).

## 9. Troubleshooting

| Symptom                                    | Cause / fix                                                                                 |
| ------------------------------------------ | ------------------------------------------------------------------------------------------- |
| "Unable To Read FPGA Version" / no reply   | Non-gigabit adapter, wrong IP, firewall, SW2.5 not on software config                       |
| LD7 on (no LVDS data)                      | `lvdsStreamCfg` missing, sent after `sensorStart`, or `dataFmt` 0. Samtec cable not seated. |
| LD5 on (FPGA buffer overflow)              | Data rate too high for the packet delay. Lower `packetDelay_us` or reduce the chirp data    |
| LD6 on (DDR full)                          | PC not keeping up. Close other programs, use a wired gigabit link                           |
| Data only in the first frame               | `enableSW` was 1                                                                            |
| Board asserts at `sensorStart`             | `dataFmt` not 0/1/4, or `adcCfg` not `2 0`                                                  |
| Leftover bytes / frames don't line up      | Recording started after `sensorStart`, packets lost, or wrong cfg passed to the parser      |
| Rubbish after changing the chirp/LVDS rate | Reset the DCA1000 (power cycle, or `dca1000_ctl.py reset`), then `fpga` again               |
