#!/usr/bin/env python3
"""
parse_capture.py - read a DCA1000 raw capture of the AWR2944 demo LVDS stream

  parse_capture.py <radar.cfg> <prefix_Raw_0.bin> [--frame N] [--save out.npz]
  parse_capture.py <radar.cfg> --synth test_Raw_0.bin --range 1.0 [--frames 20]

The frame geometry is taken from the same .cfg that was sent to the radar
(channelCfg, profileCfg, chirpCfg, frameCfg). Files _Raw_1, _Raw_2 ... next to
_Raw_0 are appended automatically.

Assumed stream layout (demo, lvdsStreamCfg -1 0 1 0, adcbufCfg non-interleaved):
  int16 little-endian, real samples
  per chirp: [Rx0 samples][Rx1 samples]...  (enabled Rx only)
  chirps in cfg order (chirpStart..chirpEnd), repeated numLoops, then frames
  -> array[frame][loop][chirp][rx][sample]
Verify this on the bench with a metal plate at a known distance (Doc/10).

--synth writes a fake capture with a tone at the given range, to test this
script without hardware.
"""

import argparse
import glob
import os
import re
import sys

import numpy as np

C = 299792458.0


def parse_cfg(path):
    cmds = {}
    for line in open(path):
        line = line.split("%", 1)[0].split()
        if line:
            cmds.setdefault(line[0], []).append(line[1:])

    rx_mask = int(cmds["channelCfg"][-1][0])
    prof = cmds["profileCfg"][-1]
    frame = cmds["frameCfg"][-1]
    adc = cmds["adcCfg"][-1]
    if int(adc[1]) != 0:
        print("warning: adcCfg output format is not real (0)", file=sys.stderr)
    lvds = cmds.get("lvdsStreamCfg")
    if not lvds or int(lvds[-1][2]) != 1:
        print("warning: cfg has no 'lvdsStreamCfg -1 0 1 0', LVDS was off?",
              file=sys.stderr)

    g = {
        "num_rx": bin(rx_mask).count("1"),
        "start_ghz": float(prof[1]),
        "idle_us": float(prof[2]),
        "adc_start_us": float(prof[3]),
        "ramp_end_us": float(prof[4]),
        "slope_mhz_us": float(prof[7]),
        "num_samples": int(prof[9]),
        "fs_ksps": float(prof[10]),
        "chirp_start": int(frame[0]),
        "chirp_end": int(frame[1]),
        "num_loops": int(frame[2]),
        # AWR294x frameCfg has numAdcSamples as 5th arg: period moves to [5]
        "frame_ms": float(frame[5] if len(frame) >= 8 else frame[4]),
    }
    g["chirps_per_loop"] = g["chirp_end"] - g["chirp_start"] + 1
    g["frame_shape"] = (g["num_loops"], g["chirps_per_loop"], g["num_rx"],
                        g["num_samples"])
    g["frame_bytes"] = int(np.prod(g["frame_shape"])) * 2
    return g


def range_axis(g, nfft):
    fs = g["fs_ksps"] * 1e3
    slope = g["slope_mhz_us"] * 1e12            # Hz/s
    freqs = np.fft.rfftfreq(nfft, 1.0 / fs)
    return freqs * C / (2 * slope)


def capture_files(first):
    m = re.match(r"(.*_Raw_)0\.bin$", first)
    if not m:
        return [first]
    files = []
    i = 0
    while os.path.exists(f"{m.group(1)}{i}.bin"):
        files.append(f"{m.group(1)}{i}.bin")
        i += 1
    return files


def load(g, first):
    files = capture_files(first)
    raw = np.concatenate([np.fromfile(f, dtype="<i2") for f in files])
    nbytes = raw.size * 2
    nframes, rest = divmod(nbytes, g["frame_bytes"])
    print(f"files      : {', '.join(os.path.basename(f) for f in files)}")
    print(f"bytes      : {nbytes}  = {nframes} frames x {g['frame_bytes']} B"
          f" + {rest} B leftover")
    if rest:
        print("warning: size is not a whole number of frames. Capture started "
              "mid-frame, packets were lost, or the cfg does not match.",
              file=sys.stderr)
    if nframes == 0:
        raise SystemExit("error: less than one frame in the capture")
    data = raw[: nframes * g["frame_bytes"] // 2].reshape((nframes,) + g["frame_shape"])
    clipped = np.count_nonzero((data == 32767) | (data == -32768))
    zeros = np.count_nonzero(np.all(data == 0, axis=-1))
    print(f"shape      : {data.shape}  [frame][loop][chirp][rx][sample]")
    print(f"clipped    : {clipped} samples ({100 * clipped / data.size:.3f} %)")
    print(f"zero chirps: {zeros} (zero-filled lost packets show up here)")
    return data


def range_profiles(g, data):
    """|FFT| per frame, power summed over loops, chirps and rx -> [frame][bin]"""
    x = data.astype(np.float32)
    x -= x.mean(axis=-1, keepdims=True)                 # remove DC per chirp
    x *= np.hanning(g["num_samples"]).astype(np.float32)
    spec = np.fft.rfft(x, axis=-1)
    pw = (np.abs(spec) ** 2).sum(axis=(1, 2, 3))
    return 10 * np.log10(pw + 1e-12)


def synth(g, path, rng_m, nframes):
    fs = g["fs_ksps"] * 1e3
    slope = g["slope_mhz_us"] * 1e12
    f_beat = 2 * rng_m * slope / C
    if f_beat >= fs / 2:
        raise SystemExit(f"error: {rng_m} m is beyond R_max "
                         f"{fs / 2 * C / (2 * slope):.2f} m")
    t = np.arange(g["num_samples"]) / fs
    tone = 8000 * np.cos(2 * np.pi * f_beat * t)
    noise = np.random.default_rng(0).normal(0, 50, (nframes,) + g["frame_shape"])
    data = np.clip(np.round(tone + noise), -32768, 32767).astype("<i2")
    data.tofile(path)
    print(f"wrote {path}: {nframes} frames, tone at {rng_m} m "
          f"(beat {f_beat / 1e3:.1f} kHz)")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cfg", help="radar .cfg sent to the board for this capture")
    ap.add_argument("bin", nargs="?", help="<prefix>_Raw_0.bin")
    ap.add_argument("--frame", type=int, default=0, help="frame to plot")
    ap.add_argument("--save", help="save array + axes to .npz")
    ap.add_argument("--no-plot", action="store_true")
    ap.add_argument("--synth", help="write a synthetic capture to this path")
    ap.add_argument("--range", type=float, default=1.0, help="synth target [m]")
    ap.add_argument("--frames", type=int, default=20, help="synth frame count")
    args = ap.parse_args()

    g = parse_cfg(args.cfg)
    r_axis = range_axis(g, g["num_samples"])
    print(f"cfg        : {g['num_rx']} Rx, {g['chirps_per_loop']} chirps/loop, "
          f"{g['num_loops']} loops, {g['num_samples']} samples, "
          f"{g['frame_ms']} ms frame")
    print(f"range      : bin {r_axis[1] * 100:.2f} cm, max {r_axis[-1]:.2f} m")

    if args.synth:
        synth(g, args.synth, args.range, args.frames)
        return
    if not args.bin:
        ap.error("bin file required (or --synth)")

    data = load(g, args.bin)
    prof = range_profiles(g, data)
    pk = int(np.argmax(prof[:, 2:].mean(axis=0))) + 2   # skip DC bins
    print(f"peak       : bin {pk}, {r_axis[pk]:.3f} m (mean over frames)")

    if args.save:
        np.savez_compressed(args.save, adc=data, range_m=r_axis,
                            range_profile_db=prof, **{k: v for k, v in g.items()
                                                       if np.isscalar(v)})
        print(f"saved      : {args.save}")

    if args.no_plot:
        return
    import matplotlib.pyplot as plt

    fr = min(args.frame, data.shape[0] - 1)
    fig, ax = plt.subplots(3, 1, figsize=(10, 11))
    for rx in range(g["num_rx"]):
        ax[0].plot(data[fr, 0, 0, rx], label=f"Rx{rx}", lw=0.8)
    ax[0].set(title=f"Raw ADC, frame {fr}, loop 0, chirp 0",
              xlabel="sample", ylabel="ADC code")
    ax[0].legend(loc="upper right")

    ax[1].plot(r_axis, prof[fr])
    ax[1].axvline(r_axis[pk], color="r", ls="--", lw=0.8)
    ax[1].set(title=f"Range profile, frame {fr} (power summed over chirps, Rx)",
              xlabel="range [m]", ylabel="dB")
    ax[1].grid(True, alpha=0.3)

    t_axis = np.arange(prof.shape[0]) * g["frame_ms"] / 1e3
    im = ax[2].imshow(prof.T, aspect="auto", origin="lower",
                      extent=[t_axis[0], t_axis[-1] + g["frame_ms"] / 1e3,
                              r_axis[0], r_axis[-1]])
    ax[2].set(title="Range vs time", xlabel="time [s]", ylabel="range [m]")
    fig.colorbar(im, ax=ax[2], label="dB")
    fig.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
