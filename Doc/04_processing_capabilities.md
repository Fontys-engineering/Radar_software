# 04 - Processing Capabilities (SoC/SDK supported)

## DPUs available for AWR294x (`SDK/ti/datapath/dpu/`)

All of these are hardware accelerator based and driven by the DSP. Libraries exist for `.ae66` (DSS) and `.aer5f` (MSS).

The "cubes" mentioned below is the main data buffer of the processing chain. Holds every chirp on all antennas in one frame.

| DPU                                    | In → Out                                                                   | Key features                                                                                                                                                                     | Currently linked? |
| -------------------------------------- | -------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------- |
| `rangeproc` (rangeprochwa)             | ADCBuf (REAL16, 1/2/4 Rx, 64–2048 samples) -> radar cube (FMT1/FMT2) in L3 | Windowed real→complex FFT per chirp during chirping. Ping/pong, so chirps/Tx must be even. Optional DC range-signature calibration. 1024 bins + FMT1 + 1 or 3 Tx is unsupported. | yes               |
| `dopplerproc` (dopplerprochwa)         | radar cube --> detMatrix (log2\|·\| summed over antennas)                  | Static clutter removal (mean subtraction), Doppler window, butterfly scaling                                                                                                     | yes               |
| `cfarproc` (cfarprochwa)               | detMatrix -> CFAR list (range idx, Doppler idx, SNR, noise)                | CA / CAGO / CASO, cyclic, peak grouping, FOV filter                                                                                                                              | yes               |
| `aoaproc` (aoaprochwa)                 | cube + CFAR list -> point cloud (x, y, z, v), side info, azimuth heatmap   | Azimuth/elevation FFT, Rx phase compensation, multi-object beamforming, extended v_max                                                                                           | yes               |
| `rangeprocDDMA`                        | ADCBuf -> compressed cube                                                  | DC subtraction, interference mitigation, EGE/BFP compression                                                                                                                     | No - DDM only     |
| `dopplerprocDDMA`, `rangecfarprocDDMA` | compressed cube → detMatrix + list                                         | DDMA demodulation, local max                                                                                                                                                     | No - DDM only     |

Not present for AWR294x: `aoa2dproc`, `rangeprocdsp`, a standalone static-clutter DPU, `rangeprocReal2x` (AWR2544 only).

## HWA (`MCUSDK/source/drivers/hwa/v0/hwa.h`)

(HW-accelerator)

- **Resources:** 64 param-sets, 8 × 16 KB memory banks (128 KB), 8 KB window RAM, up to 4095 loops, 12-channel combining.
- **FFT:**
  - 2–2048 points (2ⁿ); radix-3 for 3·2ⁿ, n ≤ 9
  - 2-D FFT
  - 4K/8K via stitching
  - real or complex input, 16 or 32 bit
  - per-stage scaling
- **Post-FFT:**
  - magnitude, magnitude + log2
  - max/sum statistics, histogram
- **Pre-processing:**
  - DC estimation and subtraction
  - interference localisation and mitigation
  - zero insertion, BPM
  - **complex multiply**: frequency shifter, **slow DFT**, scalar, vector, recursive window, LUT de-rotate, magnitude²
- **CFAR:** CA / CAGO / CASO / **OS**, on log or linear input.
- **Other modes:** EGE/BFP compression; 2-D local max.
- **No phase / atan2 output.** Phase must be computed on the DSP or R5F.

## Algorithm libraries

<!-- send help pls -->

| Library            | Path                                | Useful functions                                                                                                                                                                               | Linked               |
| ------------------ | ----------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------- |
| mmwavelib (C66x)   | `SDK/ti/alg/mmwavelib`              | `mmwavelib_dftSingleBin(WithWindow)` (cheap single-bin phase), `multiPeakSearch`, `secondPeakSearch`, `cfarCa*`, `cfarOS`, `dcRemovalFloat`, `windowCoef_gen`, `log2Abs*`, `power*`, `vecmul*` | no (add to DSS link) |
| DSPLIB 3.4         | `Dependencies/dsplib_c66x_3_4_0_0`  | `DSPF_sp_fftSPxSP` / `ifft`, `DSP_fft16x16/32x32`, `DSPF_sp_dotp_cplx`, `DSPF_sp_maxidx`, `DSPF_sp_autocor`, `DSPF_sp_fir_*`, `DSPF_sp_qrd*` / `lud*` / `svd*` / `cholesky*`                   | yes                  |
| MATHLIB 3.1        | `Dependencies/mathlib_c66x_3_1_2_1` | `atan2sp`, `atan2sp_v` (vector), `sqrtsp`, `log10sp`, `divsp`, `recipsp`                                                                                                                       | yes                  |
| dml, music, gtrack | `SDK/ti/alg/`                       | DoA / tracking: not relevant                                                                                                                                                                   | no                   |

Reusable helpers inside the DPC:

- `DPC_ObjDet_quadFit` (OD:543): 3-point parabolic peak interpolation
- `DPC_ObjDet_rangeBiasRxChPhaseMeasure`: reference-target calibration

Must be written ourselves: phase unwrap, and the sub-band group-delay fit.

## Networking / streaming

| Path             | Where                                  | Notes                                                                                                                        |
| ---------------- | -------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| ENET CPSW + lwIP | `MCUSDK/source/networking/{enet,lwip}` | **R5F only**. Prebuilt `enet-cpsw.awr294x.r5f` and `lwipif-cpsw-freertos` libraries. 100 Mbit RGMII on the EVM **[VERIFY]**. |
| TSN / gPTP       | `MCUSDK/source/networking/tsn`         | Optional time sync                                                                                                           |
| CBUFF/LVDS       | `MCUSDK/source/drivers/cbuff/v0`       | Raw ADC to DCA1000. Being replaced.                                                                                          |

## 
