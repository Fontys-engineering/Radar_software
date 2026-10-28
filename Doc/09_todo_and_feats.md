# 09 - Todo & Features

In order. Each phase should build and run on the board before the next starts.

## 1. Plasma CLI (MSS only)

- [x] Plasma source files created, Makefile picks up `plasma/*.c` + `Radar_common`
- [x] MSS CCS include path `${PROJECT_LOC}/../Radar_common`
- [x] DSS CCS include path `${PROJECT_LOC}/../Radar_common`
- [x] Fix `plasma_mss.h` include: `<Plasma_common/plasma_types.h>` -> `<plasma_types.h>`
- [x] `plasma_types.h`: `Plasma_Cfg` + `outputMask` bits
- [x] `plasma_mss.c`: `gPlasmaCfg` + `PlasmaMss_init()` defaults
- [ ] `plasma_cli.c`: `plasmaCfg`, `plasmaCal`, `plasmaStats` handlers + `PlasmaCli_register()` (keep 1 slot free for `help`)
- [ ] Hook: `PlasmaCli_register(&cliCfg, &cnt)` before `CLI_open` in `mmw_cli.c`
- [ ] Hook: `PlasmaMss_init()` after `initCtrlTask()` in `mss_main.c`, remove hello-world debug prints
- [ ] `make mss` + CCS build clean
- [ ] On board: `help` lists plasma cmds, `plasmaCfg`/`plasmaStats` round-trip, full TI cfg + `sensorStart` still works

## 2. Record ring (handoff contract, Doc/07)

- [ ] `Plasma_Record` header + section types in `plasma_types.h`
- [ ] `plasma_stream.c`: SPSC ring, `acquireWrite/commit/acquireRead/release/getStats`
- [ ] `PlasmaStream_init()` from `PlasmaMss_init()`
- [ ] `plasmaStats` prints ring stats
- [ ] Agree API + record layout with Vlad

## 3. Config to DSS

- [ ] Check DPM ioctl arg size limit (`dpm.h`)
- [ ] `PLASMA_IOCTL_CFG` define + `case` in `objectdetection.c` ioctl switch
- [ ] `PlasmaMss_sendCfgToDss()` at end of `MmwDemo_dataPathConfig` (RS:1355)
- [ ] Verify DSS receives cfg (log/breakpoint)

## 4. DSS plasma stage

- [ ] `PlasmaDsp_process` hook after Doppler (OD:1053), scratch alloc in `preStartConfig`
- [ ] Skip CFAR/AoA when `objDetEnable == 0`
- [ ] Range profile -> peak + quadFit -> `rangeM`
- [ ] Doppler peak -> `velCoarse`
- [ ] IQ taps -> phase + unwrap -> `velFine`
- [ ] Copy `Plasma_FrameResult` into HSRAM, `ptrBuffer[2]` (`dss_main.c:607`)
- [ ] Check DSP cycles fit inside the frame period

## 5. MSS producer

- [ ] `PlasmaMss_onFrameResult` in `MmwDemo_handleObjectDetResult` (RS:2380)
- [ ] Build record: header, timestamp, cross-frame unwrap, commit to ring
- [ ] Remove stock ENET producer block (RS:853-873, fixes D3)

## 6. End to end

- [ ] Vlad's consumer streams records, host receiver parses them
- [ ] 100 Hz without drops, `dropCount` stays 0
- [ ] Range calibration (`plasmaCal`) against a known reflector
- [ ] Reference cfg file with `plasma*` lines, update Doc/08

## Features / later

- [ ] D2 sub-band sweep (4 subframes)
- [ ] Density profile inversion on the host
- [ ] Dynamic cfg changes while running
- [ ] Custom Visualizer page with plasma controls
