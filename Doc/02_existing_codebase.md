# 02 - Existing Codebase

## Origin

This is TI's **AWR294x mmWave OOB demo** from MMWAVE-MCUPLUS-SDK 4.7. Petri imported it into CCS 20 and switched it from DDM to TDM. The changes were the build defines plus linker edits. He also moved most of `mss_main.c` into `RadarSetup.c`. The algorithmic code is unchanged TI code.

After 08-10-2026 these information may somewhat irrelevant due to implemtation of [06_target_architecture.md](06_target_architecture.md) plan. 

## Layout

```
Radar_software/
├── Radar_Program_mss/            R5F (FreeRTOS) project
│   ├── mss/mss_main.c            main(), init task (≈200 lines)
│   ├── mss/mmw_cli.c             demo CLI commands
│   ├── mss/mmw_lvds_stream.c/.h  CBUFF/LVDS streaming
│   ├── mss/mss.syscfg, mss_enet.syscfg   SysConfig (drivers, ENET, MPU)
│   ├── mss/mmw_mss.h             NOT USED (see "header trap")
│   ├── include/mmw_config.h, mmw_output.h   identical to SDK copies
│   ├── utils/RadarSetup.c        ≈3700 lines: init, tasks, DPM, sensor control, UART TLV output
│   ├── utils/mmwdemo_rfparser.c  CLI config → derived DPC params
│   ├── utils/mmwdemo_adcconfig.c ADCBuf open/config
│   ├── utils/mmwdemo_monitor.c   CQ/analog monitors
│   ├── utils/mmwdemo_flash.c     calibration save/restore in QSPI
│   ├── utils/enet_tcpclient.c    lwIP netconn TCP client (point cloud only)
│   ├── utils/MotionDetectDemo.c  intern PoC (dead code)
│   ├── utils/TaskP_freertos.c
│   ├── r5f_linker_BU.cmd         ACTIVE MSS linker script
│   └── mss/mmw_mss_linker.cmd    fully commented out
├── Radar_Program_dss/            C66x project
│   ├── dss/dss_main.c            DSS init, DPM task, HSRAM copy (unmodified TI)
│   ├── dss/data_path.c           driver/HWA open (unmodified TI)
│   ├── objectdetection.c         LOCAL copy of TDM DPC (unmodified TI, 3808 lines)
│   ├── c66x_linker.cmd + dss/mmw_dss_linker.cmd
│   └── mmw_resDDM.h
├── Dependencies/                 SDKs vendored (817 MB), several files EDITED in place
└── Doc/                          Installation (= DSS Radar Program Manual) + Customization manuals
```

## Header trap(s) (important)

The code includes MSS headers through SDK paths, e.g. `<ti/demo/awr294x/mmw/mss/mmw_mss.h>`. Those resolve to `SDK/ti/demo/awr294x/mmw/mss/`. The SDK copies have been edited in place:

- `mmw_mss.h` adds `motionTask` to `MmwDemo_taskHandles`, and `ready` is non-volatile.
- `ti/demo/utils/enet_stream.c` has a `vTaskDelay` edit.
- `lwipopts.h` has its buffer sizes raised.

The local `Radar_Program_mss/mss/mmw_mss.h` is dead.

**Rule for our implementation:** do not edit `Dependencies/`. Put new types in our own headers inside the project trees (see [06](06_target_architecture.md)).

## Build configurations

| Project / config | Defines                                                                                                                                                | Status                             |
| ---------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------------------------------- |
| MSS Debug        | `MMWDEMO_TDM`, `ENET_STREAM`, `INCLUDE_DPM`, `SOC_AWR2944`, `DRIVERS_RADAR_HWA_V2`, `APP_RESOURCE_FILE=<…mmw_resTDM.h>`, `-U MMWDEMO_DDM`, `-Oz` + LTO | **In use**                         |
| MSS Release      | `MMWDEMO_DDM`                                                                                                                                          | Untested                           |
| DSS Debug        | `MMWDEMO_TDM`, `ENET_STREAM`, `-O3`                                                                                                                    | **In use**                         |
| DSS Release      | DDM resource file, but neither TDM nor DDM defined                                                                                                     | May have build issues or not build |

- The project files hard-code Windows paths (`C:/Git/Radar_software/...`) in `.cproject` and the generated `Debug/subdir_rules.mk`. Building on Linux needs path macros fixed.

## Defects / traps

Beware: fully AI written analysis below:

| #   | Defect                                                                                                                                                                                                                                           | Location                                                                             | Impact                                                                                                                                       |
| --- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------- |
| D1  | ~~**The MSS and DSS both link data at L3 0x88000000.** MSS: `.sysmem` heap, lwIP pools, ENET packet pool. DSS: `gMmwL3`, L2 heap, `gHSRAM`. TI's ENET build moves the DSS L3 to 0x88100000 (`SDK/…/platform/awr2944/c66x_linker_enet.cmd:41`).~~ | ~~`Radar_Program_mss/r5f_linker_BU.cmd:41`, `Radar_Program_dss/c66x_linker.cmd:42`~~ | Solved by Vlad 08-10-2026                                                                                                                    |
| D2  | `initEnetTask()` is commented out, but `enetStreamCfg` (`mmw_cli.c:1944`) and each frame (RS:871) still post semaphores that were never constructed                                                                                              | `mss_main.c:139`                                                                     | Crash or undefined behaviour if `enetStreamCfg 1` is sent                                                                                    |
| D3  | ENET producer does an early `return` when the previous frame is unsent. This truncates the UART TLV packet after its header has already announced the full length.                                                                               | RS:857–860                                                                           | UART parser desync                                                                                                                           |
| D4  | The MSS acknowledges `EXECUTE_RESULT_EXPORTED` **before** reading results (RS:1990). The radar cube and detMatrix pointers point into live DSS L3.                                                                                               | RS:1974–2007                                                                         | The next frame can overwrite the cube while the MSS reads it                                                                                 |
| D5  | If the UART export takes longer than 1 frame, `MmwDemo_debugAssert(0)` fires                                                                                                                                                                     | RS:2002–2006                                                                         | Hard stop. Any MSS-side processing must fit the frame period.                                                                                |
| D6  | Stacks tagged `.bss.dll.l3` have no linker rule, so they land in MSS L2 **[VERIFY .map]**                                                                                                                                                        | `mss_main.c:85` etc.                                                                 | Misleading; L2 is fuller than expected                                                                                                       |
| D7  | `#error` priority check (RS:79) uses a macro that is undefined in ENET builds                                                                                                                                                                    | RS:67–80                                                                             | The check does nothing                                                                                                                       |
| D8  | `MotionDetectDemo.c`: `MotionDetection_detectMotion` is never called; `MOTION_TASK` is not defined                                                                                                                                               |                                                                                      | Dead code                                                                                                                                    |
| D9  | Board freezes on reconfiguration while running (Petri)                                                                                                                                                                                           |                                                                                      | Must always do `sensorStop` → `flushCfg` → full cfg → `sensorStart`. Open-config changes after the first start assert (`mmw_cli.c:327–347`). |
| D10 | lwIP `.lwip.pools` linker rules name `C:/ti/...` library paths that don't match                                                                                                                                                                  | `r5f_linker_BU.cmd:84–103`                                                           | Pools may be placed by fallback rules **[VERIFY .map]**                                                                                      |
| D11 | The CBUFF/LVDS session is always opened (`LVDS_STREAM` is hard-defined, `mmw_mss.h:87`)                                                                                                                                                          | `setupInit` RS:3423–3440                                                             | EDMA and CBUFF resources stay allocated even if unused                                                                                       |
