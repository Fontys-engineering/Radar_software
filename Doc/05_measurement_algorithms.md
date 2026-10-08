# 05 - Measurement Algorithms

## Suitability matrix

the (a) (b) (c) (d) are further explained below the table.

| Quantity                        | Algorithm                                                                                      | Building blocks                                                           | Stock chain gives it?                                           | Where                                 |
| ------------------------------- | ---------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------- |
| (a) Boundary range              | Range FFT -> gated peak search -> parabolic interpolation -> calibration offset                | rangeprochwa, `DSPF_sp_maxidx` / `multiPeakSearch`, `DPC_ObjDet_quadFit`  | Partly. The range FFT is in the cube; the peak is not exported. | DSS                                   |
| (b) Doppler velocity            | Slow-time FFT at the boundary bin (coarse) **and** mean chirp-to-chirp phase difference (fine) | dopplerprochwa (detMatrix), cube taps, `atan2sp_v`                        | Partly. detMatrix only, as point cloud via CFAR/AoA.            | DSS                                   |
| (c) Displacement / fluctuations | Unwrapped phase of the boundary bin across chirps and frames, `Δd = λΔφ/4π`                    | cube taps or HWA slow-DFT / `mmwavelib_dftSingleBin`, `atan2sp_v`, unwrap | No                                                              | DSS (per chirp) + MSS (across frames) |
| (d) Density information         | See options D1–D3                                                                              | (a) + (c) per sub-band                                                    | No                                                              | DSS (extract) + host (invert)         |
| CFAR / AoA point cloud          | Stock                                                                                          | cfarproc, aoaproc                                                         | Yes                                                             | Optional. Off by default.             |

## (a) Boundary range spec

As plasma's electron density gradulally increases towards the core, the earliest boundary detected will be captured. To find it we process the range FFT performed by demo code further to take the closes peak.

1. **Input:** radar cube for the current frame (L3), shape `[1 Tx][Nc chirps][Nrx][Nr bins]`.
2. **Coherent range profile:** `P[r] = Σ_chirps Σ_rx |X[c,rx,r]|²`, accumulated on the DSP. The detMatrix row at Doppler bin 0 is a cheaper alternative, but it is log2 and sums incoherently over Doppler bins.
3. **Gate:** only searched in `[gateMin, gateMax]` bins (CLI `plasmaCfg`). This excludes the SMA/waveguide internal reflections and the DC leakage near bin 0.
4. **Peak:** `k = argmax P[r]` in the gate. Require `SNR = P[k]/noise > thr`, where noise is the median of the gate. Otherwise set the `VALID` flag to 0.
5. **Interpolate:** `δ = quadFit(P[k-1], P[k], P[k+1])`, `R = (k+δ)·rangeStep − R_cal`.
   - `rangeStep` comes from the RF parser (`mmwdemo_rfparser.c:896`).
   - `R_cal` is the measured path length of the SMA cable + waveguide, from the calibration command.
6. **Output:** `rangeM`, `peakBin`, `snrDb`, `flags`.

## (b) Velocity spec

- **Coarse:** in the detMatrix row `k`, find the Doppler peak `d`, then `v = (d − Nd/2)·dopplerStep`. Use `dopplerStep` from the parser.

  - Example: 64 chirps at Tc ≈ 57 µs (full-band profile below) gives v_max ≈ λ/(4Tc) ≈ 17 m/s and Δv ≈ 0.53 m/s.

- **Fine:** at bin k, combine the Rx channels coherently (after Rx phase calibration) into `z[c]`. Then:

  ```
  φ̄ = arg Σ_c z[c+1]·conj(z[c])
  v = λ·φ̄ / (4π·Tc)
  ```

  This is the pulse-pair estimator. Its resolution is limited by SNR

- **Static clutter  may need to be left off**.  The boundary can be quasi-static, and the mean subtraction would erase it. This will need to be verified during testing.

## (c) Phase / displacement spec

- **Per frame (DSS):**
  - Extract `z[c]` for `nPhaseBins` bins centred on k (default 3). Use cube taps; `cmplx16ReIm` is read directly from L3.
  - Store **raw complex samples** in the frame record. This lets the host re-process data.
  - Compute `φ[c] = atan2sp_v(Im, Re)` for the peak bin and unwrap it within the frame.
- **Across frames (MSS):** unwrap the frame-mean phase against the previous frame. Track the cumulative displacement `d = λ·φ_unwrapped/4π`.
  - The phase change between consecutive measurements must stay below π (λ/4 ≈ 1 mm). This sets the minimum chirp rate for a given boundary speed: `v_max,unambiguous = λ/(4Tc)`.

## (d) Density information options

| Option                                    | Principle                                                                                                                                                                                       | Requires                                                                                                           | On-target work                                                               | Applicability                                           |
| ----------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------- | ------------------------------------------------------- |
| **D1 Cutoff-layer position**              | The layer at `n_c(f)` reflects. Range from (a) = position of the 7.2–8.1e19 m⁻³ layer.                                                                                                          | Nothing extra                                                                                                      | (a)                                                                          | Plasma reaches n_c. Gives one density point per band.   |
| **D2 Sub-band group delay (recommended)** | Split 76–81 GHz into K sub-bands (K = 4). Measure the group delay `τ_g(f_k) = 2R_k/c` and phase per band. τ_g vs f gives the cutoff-layer position vs density, which feeds a profile inversion. | Advanced frame with **K subframes**, each with its own `profileCfg` start frequency and about 1.1 GHz of bandwidth | (a) + (c) per subframe. Results are tagged with `subFrameIdx` and `fCenter`. | Plasma reaches n_c. Inversion runs on the **host**.     |
| D2b Sub-band via raw-ADC STFT             | One full-band chirp; split ADC samples into windows; run an FFT per window                                                                                                                      | Raw-ADC capture into L3 (custom EDMA/HWA param-sets)                                                               | High: custom rangeproc                                                       | Same as D2, finer f-sampling. Only if D2 is too coarse. |
| D3 Dispersive interferometry              | Underdense plasma (n_e ≪ n_c) between the antenna and a metal back-reflector. Phase shift `≈ 2.2e-17·∫n_e dl` rad, with 1/f² dispersion across the band.                                        | Back-reflector, vacuum reference shot                                                                              | (c) per sub-band + reference subtraction                                     | Low-density plasma. Gives line-integrated density only. |

## Starter RF configurations to try

| Param                        | Full-band mode (a/b/c)                                                  | D2 mode (per subframe, K = 4)                        |
| ---------------------------- | ----------------------------------------------------------------------- | ---------------------------------------------------- |
| Start freq                   | 76.5 GHz                                                                | 76.0 / 77.1 / 78.2 / 79.3 GHz                        |
| Slope                        | 80 MHz/µs                                                               | 50 MHz/µs                                            |
| ADC samples / rate           | 512 @ 12.8 Msps                                                         | 256 @ 12 Msps                                        |
| Swept bandwidth (ADC window) | 3.2 GHz → ΔR ≈ 4.7 cm (full ramp ≈ 76.5→80.3 GHz incl. adcStart/excess) | ≈1.07 GHz → ΔR ≈ 14 cm (interpolation gives sub-bin) |
| R_max = Fs·c/(4S)            | 12 m                                                                    | 18 m                                                 |
| Chirps per frame             | 64 (1 Tx)                                                               | 32 per subframe                                      |
| Frame period                 | 10 ms → 100 Hz                                                          | 4 subframes in 10 ms                                 |
| Static clutter removal       | off                                                                     | off                                                  |
| CFAR / AoA                   | off (`plasmaCfg` flag)                                                  | off                                                  |

These values are examples. Validate them with TI's mmWave Sensing Estimator and with the frame-timing rule (active ≤ 50 % of the period). **[ TODO TODO TODO VERIFY]**

## Calibration

- **Range offset `R_cal`:** measure a metal plate at a known distance at the waveguide output. Store the result through the `plasmaCal` CLI command; it can optionally be persisted with the existing flash calib mechanism (`mmwdemo_flash.c`).
- **Rx phase/gain:** use the existing `measureRangeBiasAndRxChanPhase` / `compRangeBiasAndRxChanPhase` path. It is already in the DPC.
- **Vacuum reference (D3):** store the per-sub-band phase with no plasma present. Subtract it on the host.
