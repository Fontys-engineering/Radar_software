# TI mmWave AWR1843-BOOST Custom DPU Demo

## Project Summary
This repository contains a modified Texas Instruments (TI) mmWave Out-of-the-Box (OOB) demo for the **AWR1843BOOST** evaluation board. It integrates a custom Data Processing Unit (DPU) into the processing chain that filters point cloud detections within a 3D bounding box defined by three parameters: lateral distance, minimum depth, and maximum depth. From the remaining points within this spatial area, the DPU isolates exactly three key targets: the closest, furthest, and middle (median) objects.

For detailed architectural context on Data Processing Modules (DPM), Data Processing Chains (DPC), and custom DPU integration, refer to the included [mmWave Datapath Manual](./Docs/DPM,%20DPC,%20and%20DPU%20manual.pdf).

---

## Usage

### Custom DPU Command (`dpuCustomCfg`)

The custom DPU filtering parameters can be updated dynamically via CLI before or between active radar frames.

```cmd
dpuCustomCfg <subFrameIdx> <enabled> <maxLateralDist> <minDepthDist> <maxDepthDist>
```

| Parameter | Type | Description |
| --- | --- | --- |
| `<subFrameIdx>` | `int` | Subframe index (0-based; set to `-1` to apply configuration to all subframes, or `0` when advanced frame mode is disabled). |
| `<enabled>` | `int` | DPU processing toggle (`1` = Enable spatial filtering & target isolation, `0` = Disable/bypass filtering). |
| `<maxLateralDist>` | `float` | Maximum lateral distance threshold in meters. |
| `<minDepthDist>` | `float` | Minimum depth distance threshold in meters. |
| `<maxDepthDist>` | `float` | Maximum depth distance threshold in meters. |

---

## Prerequisites

* **TI mmWave SDK:** `3.06.00.00-LTS`
* **TI UniFlash:** `v9.6.0` (expected installation path: `C:\ti\uniflash_9.6.0`)
* **Code Composer Studio (CCS):** `v20.4.1`
* **Hardware:** TI AWR1843BOOST Evaluation Module (EVM)

> **Note:** If you install new components, make sure to reload them inside Code Composer Studio.  
> File -> Preferences -> Code Composer Studio Settings... -> General -> Compilers  
> File -> Preferences -> Code Composer Studio Settings... -> General -> Products  
> Refresh both.

---

## Utilities

### Automated Flasher (`uniflash.bat`)

An automated batch script is provided to flash firmware binaries to the board via UART using UniFlash's `dslite.bat` tool[cite: 1].

* **Binary Flashed:** `.\out_of_box_1843_mss\isk\out_of_box_1843_isk.bin`
* **Target Configuration:** `.\AWR1843_Serial.ccxml`
* **Default Port:** `COM5` (used if no parameter is provided)

#### Usage

```cmd
uniflash.bat [COM_PORT]
```

> **Note:** Set the board SOP jumpers to **Flashing Mode (SOP2)** before running the script. Switch back to **Functional Mode (SOP0)** and power-cycle the board after flashing completes.

---

## Debugging Guide

To debug and reload firmware (MSS and DSS) using Code Composer Studio:

1. **Flash CCS Debug Binary:**
Use the **UniFlash GUI** to flash the SDK debug binary to the board:
```text
mmwave_sdk_03_06_00_00-LTS\packages\ti\utils\ccsdebug\xwr18xx_ccsdebug.bin
```

2. **Hardware Setup:**
Ensure board jumpers are set to **Functional/Debug Mode (SOP0)** and power-cycle the target.
3. **Launch CCS Target Configuration:**
In CCS, launch the included target configuration: `out_of_box_1843_combined`. The target configuration automatically loads the firmware binaries onto the cores.
4. **Start Core Execution:**
Connect to the Cortex-R4F (MSS) and C674x (DSS) cores and start/resume execution.
5. **On-the-Fly Code Reloading:**
During an active debug session, you can rebuild and reload code (primarily DSS) directly without resetting the board, provided the sensor is stopped first (via the `sensorStop` CLI command or through the mmWave Demo Visualizer).

---

## Configuration & Visualization

You can configure sensor parameters and visualize the custom DPU output using the TI mmWave Demo Visualizer:

1. Connect the EVM board to your PC via USB.
2. Launch the **[TI mmWave Demo Visualizer v3.6.0](https://dev.ti.com/gallery/view/mmwave/mmWave_Demo_Visualizer/ver/3.6.0/)**.
3. Select **xWR18xx** as your target device and connect the matching User UART and Data serial COM ports.
4. Send your CLI configuration profile (`.cfg`) to initialize the sensor and inspect real-time point cloud data along with custom spatial object outputs.