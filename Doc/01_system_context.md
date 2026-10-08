# 01 - System Context

## Glossary/terms

Glossary can be found in [here](01_1_glossary.md).

## Goal

The client Fontys DSS, together with DIFFER want a plasma boundary reflectometer for the ITER Tokamak system. We're using an automotive radar AWR2944 EVM modified to work as a **plasma boundary reflectometer**. The RF signal is/will be coupled through SMA connectors into a waveguide, and the reflection from the plasma is processed into:

- **Boundary position**: the distance from the wall or waveguide aperture to the reflecting layer.
- **Boundary velocity**: Doppler shift, or phase rate.
- **Density-related information**: the cutoff density of the layer, and/or a density profile (see [05](05_measurement_algorithms.md)).

The EVM is configured to transmit live data over LVDS, but LVDS needs a DCA1000 capture board. 

As part of client's request, we're developing the live output to be through Ethernet.

## Team split (embedded)

| Owner  | Scope                                                                                                   |
| ------ | ------------------------------------------------------------------------------------------------------- |
| Mete   | RF front-end config, processing chain (DSS), measurement extraction, producing frame records on the MSS |
| Vlad   | Ethernet streaming on the MSS: lwIP/CPSW, wire protocol, host receiver. Replaces LVDS.                  |
| Shared | Handoff contract ([07](07_data_handoff_contract.md)) init order ([08](08_init_and_config.md))           |

## Intro physics primer

https://en.wikipedia.org/wiki/Electromagnetic_electron_wave
This is self researched, not given by client specifically but relevant to how we're going to measure.

- **O-mode cutoff.** A wave at frequency f reflects where the plasma frequency equals f:
  `f_pe ≈ 8.98·√n_e` Hz, so the cutoff density is `n_c = 1.24e-2 · f²` m⁻³.

  * **n_e** is electron density, thus how many free electrons there are per cubic meter of plasma

  * **n_c** is cutoff/critical density. the electron density at which a wave of a given frequency can no longer pass through and is reflected instead. It depends only on frequency: n_c ≈ 1.24×10⁻² · f².

  For our hardware bottom,mid and top values are:

  | f      | n_c          |
  | ------ | ------------ |
  | 76 GHz | 7.2 e19 m⁻³  |
  | 77 GHz | 7.35 e19 m⁻³ |
  | 81 GHz | 8.1 e19 m⁻³  |

  The radar sees a reflecting layer only if the plasma reaches about `7e19 m⁻³` to  `8e19 m⁻³`. Otherwise the beam passes through and reflects off whatever is behind it.

- **X-mode** cutoffs depend on the magnetic field B. Not considered unless the client asks.

- **FMCW.** The beat frequency is `f_b = S·τ_g`, where S is the slope and τ_g the group delay, so the apparent range is `R = c·τ_g/2`. In a plasma, τ_g includes dispersive propagation up to the cutoff layer.

- **Phase.** `φ = 4πR/λ`. Over chirps or frames, Δφ gives displacement `Δd = λΔφ/4π`, which is about 0.31 mm/rad at 77 GHz. The rate of change gives velocity.

- **Profile.** Over a sweep, τ_g(f) maps out reflection depth vs cutoff density. Abel-type inversion turns that into an n_e profile, but only for the narrow 7.2–8.1e19 window.

- **Underdense plasma.** A plasma with n_e ≪ n_c adds a phase shift `Δφ ≈ 2.2e-17·∫n_e dl` rad against a back reflector, which makes interferometry possible. See [05](05_measurement_algorithms.md) option D3.

## AWR2944 limits relevant to us

Table below focuses mainly on the RF frontend limit. But we also know that Ethernet data format should be raw ADC samples.  

| Parameter    | Limit                                                                                    | Consequence                                               |
| ------------ | ---------------------------------------------------------------------------------------- | --------------------------------------------------------- |
| RF band      | 76–81 GHz. One sweep must fit within 76–80.5 or 76.5–81 GHz.                             | B_max ≈ 4.5 GHz, so ΔR = c/2B ≈ 3.3 cm                    |
| Slope        | ≤ 266 MHz/µs, with restricted codes                                                      | Limits R_max at a given IF                                |
| ADC          | **Real only**, ≤ 37.5–45 Msps, IF BW 15–20 MHz depending on grade **[VERIFY datasheet]** | `R_max = F_IF·c/(2S)`                                     |
| ADC buffer   | 16 KB ping/pong: ≤ 2048 real samples at 4 Rx channels used                               | Range FFT ≤ 1024 bins (HWA DPU limit)                     |
| Chirps       | 512 chirp-RAM entries, `numLoops` 1–255, 4 profiles                                      | Frequency-stepping across chirps or subframes is possible |
| Subframes    | Up to 4 per advanced frame, each with its own profile                                    | Basis for sub-band sweeps (D2)                            |
| λ at 77 GHz  | 3.89 mm                                                                                  | Phase sensitivity is about 0.31 mm/rad                    |
| λ at 81 GHz  | 3.7 mm                                                                                   | Same as above                                             |
| Frame timing | Active chirp time ≤ 50 % of the frame period (demo rule)                                 | Leaves inter-frame time for DSS processing                |

## Requirements status

This refers to the PoA made by our group. 

| Item                                   | Source                      | Value                                                                          |
| -------------------------------------- | --------------------------- | ------------------------------------------------------------------------------ |
| Measured quantities                    | PoA §2                      | Boundary distance, "thickness", velocity, "other parameters"                   |
| Transport                              | PoA                         | TCP (different from prev doc using UDP)                                        |
| Range-profile streaming rate           | EthernetPlasma.pdf (target) | 512–2048 bins × 16 bit at 100–1000 Hz, ≥ 60 FPS without drops                  |
| Update rate / accuracy / range window  |                             | Best possible. Unknown as of now.                                              |
| Expected plasma density                |                             | Research from EAST toakmak for ITER development provides somewhat info.        |
| Calibration of the SMA/waveguide delay | PoA §2.2                    | Required. Handled by a reference/offset in [05](05_measurement_algorithms.md). |
