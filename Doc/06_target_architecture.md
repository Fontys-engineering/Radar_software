# 06 - Target Architecture (planned)

This is where the show starts

## Principles and rules

1. **Keep TI's frame machinery.** 
   This covers mmWave control, DPM, the rangeproc/dopplerproc DPUs, and the HSRAM result path. We only insert stages and do not rewrite them.
2. **Heavy maths on the DSS, light maths on the MSS.**
   Per-chirp work stays in DSS. The MSS does cross-frame tracking, timestamping and packaging.
3. **Our code lives in our own files.**
   Edits to TI files are limited to single-line hook calls. Nothing in `Dependencies/` is edited.
4. **One-way, non-blocking handoff to Ethernet.** 
   The measurement path must never wait on the network. Frames are dropped with a counter rather than stalling the chain, because stalling makes the DPC assert (OD:511).
5. **The DSS -> MSS -> Ethernet path is one frame record per (sub)frame.**
   UART TLV output is kept as a debug path and is optional.

## Component view diags

```plantuml
@startuml system-overview
' High-level overview of the plasma reflectometer firmware (AWR2944).
' Companion to target-architecture.dot - simplified for presentations.
' Render:  plantuml -tpng system-overview.puml

title Plasma Reflectometer Firmware - System Overview

skinparam backgroundColor white
skinparam shadowing false
skinparam defaultFontName "Helvetica"
skinparam defaultFontSize 14
skinparam ArrowColor #555555
skinparam ArrowFontSize 12
skinparam ArrowFontColor #333333
skinparam ArrowThickness 1.4
skinparam roundCorner 14
skinparam rectangle {
  BorderThickness 1.5
}
skinparam package {
  BorderThickness 2
  FontSize 15
}
skinparam note {
  BackgroundColor #FFFFFF
  BorderColor #999999
  FontSize 12
}
skinparam legend {
  BackgroundColor #FFFFFF
  BorderColor #BBBBBB
  FontSize 12
}

top to bottom direction

' ---------- colours (by ownership) ----------
!$REUSED = "#E8E8E8"
!$OURS   = "#CFE3FF"
!$NET    = "#D4F2D2"
!$BUF    = "#FFF6B3"

' ================= 1. RF =================
package "Radar subsystem  (TI firmware)" as BSS #F7F7F7 {
  rectangle "**RF front end**\n76-81 GHz FMCW chirps\nTx / Rx via SMA + waveguide" as RF $REUSED
}

' ================= 2. DSP =================
package "DSP core  -  C66x  (FreeRTOS)" as DSS #F4F8FF {
  rectangle "**Processing task**\nRange FFT  ->  Doppler FFT  ->  **Plasma stage**\n<size:12>distance · velocity · phase · range profile</size>" as PROC $OURS
}

' ================= 3. R5F =================
package "Control core  -  R5F  (FreeRTOS)" as MSS #F6FBF4 {
  rectangle "measurement data" as COLDATA #F6FBF4;line.dashed;line:AAAAAA;text:888888 {
    rectangle "**Result task**\n<size:12>priority 8 · one record per frame</size>" as RES $OURS
    database "**Record buffer**\n<size:12>16 frames · never blocks</size>" as RING $BUF
    rectangle "**Ethernet task**\n<size:12>priority <= 6 · lwIP UDP</size>" as ETH $NET
  }
  rectangle "configuration" as COLCFG #F6FBF4;line.dashed;line:AAAAAA;text:888888 {
    rectangle "**Radar control task**\n<size:12>priority 10 · drives the RF chip</size>" as CTRL $REUSED
    rectangle "**CLI task**\n<size:12>priority 7 · config commands</size>" as CLI $REUSED
  }
}

' ================= 4. Host =================
package "Host PC" as PC #FFFFFF {
  rectangle "**Config terminal**\n<size:12>sends CLI commands</size>" as TERM #FFFFFF
  rectangle "**Data receiver**\n<size:12>live plots · density analysis</size>" as RECV #FFFFFF
}

' ---------- measurement data path (right column, flows DOWN) ----------
RF   -down-> PROC : raw ADC samples\n<size:11>int16, per chirp</size>
PROC -down-> RES  : frame result\n<size:11>via shared memory + IPC</size>
RES  -down-> RING : Plasma_Record
note on link
  **Plasma_Record**
  ----
  header (48 B)
  · sequence nr, timestamp
  · frame nr, drop count
  sections
  · boundary: range, velocity, phase
  · range profile
  · raw IQ samples
end note
RING -down-> ETH  : next record
ETH  -down-> RECV : **UDP**  ~1.4 KB / frame\n<size:11>100 frames/s ≈ 1.1 Mbit/s</size>

' ---------- configuration path (left column, flows UP) ----------
TERM .up.> CLI  : config\n<size:11>UART</size>
CLI  .up.> CTRL : start / stop
CTRL .up.> RF   : chirp settings

' ---------- force the two-column layout ----------
TERM -[hidden]right-> RECV

' ---------- friendly presenter ----------
actor " " as HI #FFFFFF
note right of HI : Hi!
HI -[hidden]right-> BSS


legend bottom
  |= Colour |= Description |
  |<back:#E8E8E8>   TI demo code   </back>| reused, unchanged |
  |<back:#CFE3FF>   Custom implementation code   </back>| plasma measurement |
  |<back:#D4F2D2>   Ethernet   </back>| Vlad's streaming code |
  |<back:#FFF6B3>   Buffer   </back>| handoff between the two |
  | solid arrow | measurement data |
  | dotted arrow | configuration data|
endlegend

@enduml
```

```plantuml
@startuml target-architecture-plantuml
' Detailed target architecture of the plasma reflectometer firmware (AWR2944).
' PlantUML twin of target-architecture.dot. Source of truth: ../06_target_architecture.md, ../07_data_handoff_contract.md
' Render:  plantuml -tpng target-architecture-plantuml.puml
' FreeRTOS priorities: HIGHER number = MORE important (0 = idle).

title Plasma Reflectometer Firmware - Target Architecture (AWR2944)\n<size:12>per-frame data path: RF -> DSS processing -> MSS handoff -> Ethernet -> host</size>

skinparam backgroundColor white
skinparam shadowing false
skinparam defaultFontName "Helvetica"
skinparam defaultFontSize 13
skinparam ArrowColor #555555
skinparam ArrowFontSize 11
skinparam ArrowFontColor #333333
skinparam ArrowThickness 1.3
skinparam roundCorner 12
skinparam rectangle {
  BorderThickness 1.4
}
skinparam package {
  BorderThickness 2
  FontSize 14
}
skinparam database {
  BorderThickness 1.4
}
skinparam note {
  BackgroundColor #FFFFFF
  BorderColor #999999
  FontSize 11
}
skinparam legend {
  BackgroundColor #FFFFFF
  BorderColor #BBBBBB
  FontSize 11
}

top to bottom direction

' ---------- colours (by ownership) ----------
!$TI     = "#E8E8E8"
!$HOOK   = "#FFE2C2"
!$OURS   = "#CFE3FF"
!$NET    = "#D4F2D2"
!$MEM    = "#FFF6B3"
!$OFF    = "#F4F4F4;line.dashed;text:777777"

' ======================= BSS =======================
package "BSS - Radar subsystem  (TI RF firmware, via mmWaveLink)" as BSS #F7F7F7 {
  rectangle "**RF front end**\n<size:11>chirps 76-81 GHz · 1 Tx / 1-4 Rx · real ADC</size>" as RF $TI
  database "**ADC buffer**\n<size:11>16 KB ping/pong</size>" as ADCBUF $MEM
}

' ======================= DSS =======================
package "DSS - C66x DSP @ 360 MHz  (FreeRTOS · DPM task, prio 5)" as DSS #F4F8FF {
  rectangle "objectdetection.c - DPC  (local TI copy, hook edits)" as DPC #FFFAF3;line.dashed;line:D9822B {
    rectangle "**Range FFT**\n<size:11>rangeprocHWA · during chirps</size>" as RANGE $TI
    rectangle "**Doppler FFT**\n<size:11>dopplerprocHWA · inter-frame</size>" as DOPPLER $TI
    rectangle "**PlasmaDsp_process**   <color:#2E6FD8>NEW STAGE</color>\n<size:11>range profile · gated peak + quadFit · R - R_cal</size>\n<size:11>coarse/fine velocity · IQ taps · atan2 + unwrap</size>" as PLASMADSP $OURS;line:2E6FD8;line.bold
    rectangle "**CFAR + AoA**\n<size:11>optional · objDetEnable = 0 by default</size>" as CFAR $OFF
  }
  rectangle "L3 buffers" as L3 #F4F8FF;line.dashed;line:AAAAAA;text:888888 {
    database "**Detection matrix**\n<size:11>uint16 log2 [range][doppler]</size>" as DETMAT $MEM
    database "**Radar cube**\n<size:11>cmplx16 [Tx][chirp][Rx][bin]</size>" as CUBE $MEM
  }
  rectangle "**dss_main.c**\n<size:11>copy results -> HSRAM · set ptrBuffer[2]</size>" as DSSMAIN $HOOK
  database "**HSRAM result buffer**  (32 KB, L3)\n<size:11>TI result + stats · <color:#2E6FD8>**Plasma_FrameResult**</color></size>" as HSRAM $MEM
}

' ======================= MSS =======================
package "MSS - Cortex-R5F  (FreeRTOS · higher prio number = more important)" as MSS #F6FBF4 {
  rectangle "measurement data" as COLDATA #F6FBF4;line.dashed;line:AAAAAA;text:888888 {
    rectangle "**DPM task**  [prio 9]\n<size:11>reportFxn: receive result -> ack</size>" as DPM $TI
    rectangle "UART export task  [prio 8]  -  RadarSetup.c (hook edit)" as UARTTASK $HOOK {
      rectangle "**PlasmaMss_onFrameResult**\n<size:11>addr translate · copy from HSRAM</size>\n<size:11>unwrap across frames · timestamp · build Plasma_Record</size>" as PLASMAMSS $OURS;line:2E6FD8;line.bold
      rectangle "**TI UART TLV output**\n<size:11>debug, optional</size>" as TLV $OFF
    }
    database "**PlasmaStream ring**   <color:#2E6FD8>HANDOFF CONTRACT</color>\n<size:11>16 x 4 KB slots · MSS L2 · SPSC</size>\n<size:11>producer never blocks (drop-newest)</size>" as RING $MEM;line:2E6FD8;line.bold
    rectangle "**Ethernet consumer task**  [prio <= 6]" as ETH $NET
    rectangle "**lwIP TCP/IP + CPSW driver**\n<size:11>RGMII</size>" as LWIP $NET
  }
  rectangle "configuration" as COLCFG #F6FBF4;line.dashed;line:AAAAAA;text:888888 {
    rectangle "**mmWave control task**  [prio 10]\n<size:11>drives the BSS</size>" as CTRL $TI
    rectangle "**CLI task**  [prio 7]\n<size:11>profileCfg · frameCfg · sensorStart/Stop</size>\n<size:11><color:#2E6FD8>+ plasma_cli.c: plasmaCfg · plasmaCal · plasmaStats</color></size>" as CLI $HOOK
  }
}

' ======================= Host =======================
package "Host PC" as PC #FFFFFF {
  rectangle "**Data receiver**\n<size:11>live plots · density inversion (Python)</size>" as RECV #FFFFFF
  rectangle "**Config terminal**\n<size:11>UART CLI, 115200 baud</size>" as TERM #FFFFFF
}

' ---------- RF -> DSS ----------
RF      -down->  ADCBUF  : ADC samples
ADCBUF  -down->  RANGE   : per chirp (EDMA)

' ---------- DSS processing chain ----------
RANGE   -down->  DOPPLER
DOPPLER -down->  PLASMADSP
PLASMADSP -down-> CFAR
RANGE   .> CUBE    : writes
CUBE    .>       DOPPLER : reads
CUBE    .>       PLASMADSP : reads taps
DOPPLER .right.> DETMAT : writes
DETMAT  .>       PLASMADSP : Doppler row k
PLASMADSP -[#2E6FD8,bold]-> DSSMAIN : Plasma_FrameResult
DSSMAIN -down->  HSRAM

' ---------- DSS -> MSS ----------
HSRAM   -[bold]down-> DPM : DPM_sendResult (IPC)
DPM     .[#B03A2E].>  DSSMAIN : <color:#B03A2E>ack RESULT_EXPORTED</color>\n<color:#B03A2E><size:10>must arrive before next frame</size></color>

' ---------- MSS data path ----------
DPM       -down-> PLASMAMSS : UartExportSem
PLASMAMSS -[#2E6FD8,bold]down-> RING : acquireWrite / commit
note on link
  **Plasma_Record**  (~1.4 KB)
  ----
  header 48 B: seq · frame nr · timestamp · drop count
  sections: boundary · range profile · IQ taps · diag
end note
PLASMAMSS .right.> TLV
RING      -[#3A9A3A,bold]down-> ETH : acquireRead / release\n<size:10>counting semaphore</size>
ETH       -down-> LWIP
LWIP      -[#3A9A3A,bold]down-> RECV : **UDP** Plasma_Record\n<size:10>~1.4 KB @ 100 Hz</size>
TLV       .down.> RECV : TLVs @ 3.125 Mbaud

' ---------- configuration path (flows up) ----------
TERM    .up.> CLI  : config commands
CLI     .up.> CTRL : MMWave_config / start
CTRL    .up.> RF   : profile / chirp / frame\n<size:10>mmWaveLink</size>
CLI     .[#2E6FD8].> PLASMADSP : <color:#2E6FD8>DPM ioctl PLASMA_IOCTL_CFG</color>

' ---------- layout hints ----------
RECV -[hidden]right-> TERM

legend bottom
  |= Colour / style |= Meaning |
  |<back:#E8E8E8>   TI, unchanged   </back>| reused from the TI demo |
  |<back:#FFE2C2>   TI + hook edit   </back>| small call into our code |
  |<back:#CFE3FF>   Ours (plasma)   </back>| new measurement code |
  |<back:#D4F2D2>   Ethernet   </back>| Vlad's streaming code |
  |<back:#FFF6B3>   Shared memory   </back>| buffers between stages / cores |
  | dashed box | optional or switched off |
  | solid arrow | data / control flow |
  | dotted arrow | memory access or configuration |
  | <color:#B03A2E>red dotted</color> | frame-deadline acknowledgement |
endlegend

@enduml
```

## New expected files

| File                                           | Core |
| ---------------------------------------------- | ---- |
| `Radar_common/plasma/plasma_types.h`           | both |
| `Radar_Program_dss/plasma/plasma_dsp.{h,c}`    | DSS  |
| `Radar_Program_mss/plasma/plasma_mss.{h,c}`    | MSS  |
| `Radar_Program_mss/plasma/plasma_cli.{h,c}`    | MSS  |
| `Radar_Program_mss/plasma/plasma_stream.{h,c}` | MSS  |

## Edits to TI files

| File                                                            | Edit                                                                                                                                                                                                                                                                                                                                                                                  | Why              |
| --------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------- |
| `Radar_Program_dss/objectdetection.c`                           | (1) After `DPU_DopplerProcHWA_process` (OD:1053) call `PlasmaDsp_process(radarCube, detMatrix, &plasmaResult)`. <br/>(2) Skip CFAR/AoA (OD:1079–1118) when `cfg.objDetEnable == 0`, setting `numObjOut = 0`. <br/>(3) Add `case PLASMA_IOCTL_CFG:` to the ioctl switch (near OD:3176). <br/> (4) In `DPC_ObjDet_preStartConfig` (≈OD:2789), allocate plasma scratch from the L2 heap. | Insert the stage |
| `Radar_Program_dss/dss/dss_main.c`                              | After `MmwDemo_copyResultToHSRAM` (`dss_main.c:607`), copy `Plasma_FrameResult` into the remaining HSRAM payload. The function returns the remaining bytes, so the offset is `MMWDEMO_HSRAM_PAYLOAD_SIZE − ret`. Set `resultBuffer.ptrBuffer[2]`/`size[2]`. `DPM_MAX_BUFFER = 3`, and slot 2 is free.                                                                                 | Transport to MSS |
| `Radar_Program_mss/utils/RadarSetup.c`                          | (1) In `MmwDemo_handleObjectDetResult` (RS:2380), call `PlasmaMss_onFrameResult(&gMmwMssMCB.ptrResult)` before `MmwDemo_transmitProcessedOutput`. (2) At the end of `MmwDemo_dataPathConfig` (RS:1355), call `PlasmaMss_sendCfgToDss()`. (3) Remove the stock ENET producer block (RS:853–873), which fixes D3.                                                                       | Hooks            |
| `Radar_Program_mss/mss/mmw_cli.c`                               | In `MmwDemo_CLIInit`, call `PlasmaCli_register(&cliCfg, &cnt)` before `CLI_open`. Watch `CLI_MAX_CMD = 40`.                                                                                                                                                                                                                                                                           | CLI              |
| `Radar_Program_mss/mss/mss_main.c`                              | Call `PlasmaMss_init()` right after `initCtrlTask()` (`mss_main.c:137`). Then the colleague's `Net_init()` goes in place of `initEnetTask` (line 139). The order is in [08](08_init_and_config.md).                                                                                                                                                                                   | Init             |
| `Radar_Program_dss/c66x_linker.cmd` (+ MSS `r5f_linker_BU.cmd`) | Split L3 so the cores do not overlap (fixes D1, see [09](09_memory_budget.md))                                                                                                                                                                                                                                                                                                        | Memory           |

## DSS stage: `PlasmaDsp_process`

**Inputs:**

- `DPIF_RadarCube` (FMT1, L3)
- `DPIF_DetMatrix` (L3)
- `Plasma_Cfg`
- derived parameters from `staticCfg`: `numRangeBins`, `numDopplerChirps`, `numRxAntennas`, `rangeStep`, `dopplerStep`

**Steps** (all from [05](05_measurement_algorithms.md)):

1. Coherent range profile over the gate. Output is `float` power, decimated to `uint16` dB×100 for transport.
2. Peak, SNR and quadFit, minus `R_cal`, giving `rangeM`.
3. Doppler peak in detMatrix row k, giving `velCoarse`.
4. Cube taps: `nPhaseBins` × `numDopplerChirps` complex samples, Rx-combined. Then `atan2sp_v`, intra-frame unwrap, and pulse-pair `velFine`.
5. Fill `Plasma_FrameResult` (layout in [07](07_data_handoff_contract.md) §Payload), with `subFrameIdx` and `fCenterHz` taken from the profile.



## MSS stage: `PlasmaMss_onFrameResult`

Runs in the **UART export task** (priority 8) after the DPM report.

1. `AddrTranslateP_getLocalAddr(ptrBuffer[2])` gives the pointer to `Plasma_FrameResult`.
2. `PlasmaStream_acquireWrite()` returns a slot, or NULL if the ring is full. On NULL, count a drop and skip.
3. Fill the record header: magic, version, seq, `timestampUs` from the R5F cycle counter, `frameNumber` from the result stats, `subFrameIdx`, flags.
4. `memcpy` the payload from HSRAM into the slot. That is ≤ 8 KB, roughly tens of µs.
5. Cross-frame processing: unwrap the frame phase against the previous value of the same subframe; accumulate displacement; apply an optional EMA on range.
6. `PlasmaStream_commit()`. This publishes the record and notifies the consumer.
