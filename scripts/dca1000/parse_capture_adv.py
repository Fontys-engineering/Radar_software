#!/usr/bin/env python3
"""
parse_capture_adv.py - detailed analysis of a DCA1000 raw capture over time

  parse_capture_adv.py <radar.cfg> <prefix_Raw_0.bin> [options]

Same input and layout as parse_capture.py (Doc/10). Processes the capture in
chunks, so long recordings fit in memory. Three figure windows:

  1 Over time   range-time map with tracked peak, peak range, peak power /
                noise floor / SNR, displacement from phase, velocity
  2 Doppler     velocity spectrogram at the target bin over time, and the
                range-Doppler map of one frame
  3 Health      ADC RMS and DC per Rx over time, clipped samples per frame,
                power per Tx-Rx channel, mean range profile per Rx

Methods (see Doc/05):
  range       per frame, gated argmax of the power profile + parabolic
              interpolation on dB
  SNR         peak power over the median power inside the gate
  phase       complex range-FFT value at one fixed bin (--bin, default the
              strongest bin of the mean profile). Each Tx-Rx channel is
              rotated by its own mean phase, then all channels are summed
              coherently. Unwrapped over all loops -> d = lambda*phi/(4*pi).
              Valid while the target moves < lambda/4 (~1 mm) between
              samples; across the gap between frames this is the limit.
  velocity    pulse-pair: mean phase step between consecutive loops of one
              frame, v = lambda*dphi/(4*pi*T_loop)
  Doppler     FFT over the loops of one frame (zero-padded), power summed
              over channels. No static clutter removal: v = 0 is the
              strongest line.

Options:
  --gate MIN MAX   range gate in m for peak search (default: skip DC bins)
  --bin K          fix the phase/Doppler bin instead of the strongest one
  --frame N        frame for the range-Doppler map (default: middle)
  --t0/--t1 S      analyse only this time window (seconds from capture start)
  --csv FILE       per-frame results as CSV
  --png DIR        save the figures as PNG instead of showing them
  --no-plot        print the summary only
"""

import argparse
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from parse_capture import C, capture_files, parse_cfg, range_axis  # noqa: E402


class RawReader:
    """Frame reader over one or more _Raw_N.bin files (frames may span files)."""

    def __init__(self, first, frame_bytes):
        self.maps = [np.memmap(f, dtype="<i2", mode="r") for f in capture_files(first)]
        self.files = capture_files(first)
        self.sizes = [m.size for m in self.maps]
        total = sum(self.sizes) * 2
        self.frame_words = frame_bytes // 2
        self.num_frames, self.leftover = divmod(total, frame_bytes)

    def frames(self, f0, f1):
        start, stop = f0 * self.frame_words, f1 * self.frame_words
        parts, base = [], 0
        for m, n in zip(self.maps, self.sizes):
            lo, hi = max(start, base), min(stop, base + n)
            if lo < hi:
                parts.append(np.asarray(m[lo - base:hi - base]))
            base += n
        return np.concatenate(parts) if len(parts) > 1 else parts[0]


def chirp_tx_labels(cfg_path, g):
    """'TXn' label per chirp in the loop, from the chirpCfg tx masks."""
    masks = {}
    for line in open(cfg_path):
        w = line.split("%", 1)[0].split()
        if w and w[0] == "chirpCfg":
            for i in range(int(w[1]), int(w[2]) + 1):
                masks[i] = int(w[8])
    labels = []
    for i in range(g["chirp_start"], g["chirp_end"] + 1):
        m = masks.get(i, 0)
        labels.append("+".join(f"TX{b}" for b in range(4) if m >> b & 1) or "?")
    return labels


def parabolic(y, k):
    """sub-bin offset of a peak at k from its neighbours"""
    if k <= 0 or k >= len(y) - 1:
        return 0.0
    a, b, c = y[k - 1], y[k], y[k + 1]
    den = a - 2 * b + c
    return 0.0 if den == 0 else 0.5 * (a - c) / den


def range_fft(raw, g):
    """int16 frames -> complex range spectrum [n][loop][chirp][rx][bin]"""
    x = raw.reshape((-1,) + g["frame_shape"]).astype(np.float32)
    x -= x.mean(axis=-1, keepdims=True)
    x *= np.hanning(g["num_samples"]).astype(np.float32)
    return x, np.fft.rfft(x, axis=-1).astype(np.complex64)


def analyse(args):
    g = parse_cfg(args.cfg)
    rd = RawReader(args.bin, g["frame_bytes"])
    if rd.num_frames == 0:
        raise SystemExit("error: less than one frame in the capture")

    nb = g["num_samples"] // 2 + 1
    r_axis = range_axis(g, g["num_samples"])
    bin_m = r_axis[1]
    tc = (g["idle_us"] + g["ramp_end_us"]) * 1e-6
    t_loop = g["chirps_per_loop"] * tc
    t_frame = g["frame_ms"] * 1e-3
    f_center = (g["start_ghz"] * 1e9 + g["slope_mhz_us"] * 1e12 *
                (g["adc_start_us"] * 1e-6 + g["num_samples"] / (2 * g["fs_ksps"] * 1e3)))
    lam = C / f_center
    tx_labels = chirp_tx_labels(args.cfg, g)

    f0 = int(args.t0 / t_frame) if args.t0 else 0
    f1 = min(rd.num_frames, int(np.ceil(args.t1 / t_frame))) if args.t1 else rd.num_frames
    if f0 >= f1:
        raise SystemExit("error: empty time window")
    nf = f1 - f0
    chunk = max(1, args.chunk)

    if args.gate:
        g0 = max(2, int(np.floor(args.gate[0] / bin_m)))
        g1 = min(nb - 1, int(np.ceil(args.gate[1] / bin_m)))
    else:
        g0, g1 = 2, nb - 1
    if g1 - g0 < 3:
        raise SystemExit("error: gate too small")

    L, Cn, R, S = g["frame_shape"]

    # ---- pass 1: power profiles, ADC statistics ----
    prof = np.empty((nf, nb))                 # power summed over loops/chirps/rx
    prof_rx = np.zeros((R, nb))               # mean power per Rx
    rms = np.empty((nf, R))
    dc = np.empty((nf, R))
    clipped = np.empty(nf, dtype=np.int64)
    zero_chirps = np.empty(nf, dtype=np.int64)
    for a in range(f0, f1, chunk):
        b = min(f1, a + chunk)
        raw = rd.frames(a, b).reshape((-1,) + g["frame_shape"])
        i, j = a - f0, b - f0
        clipped[i:j] = np.count_nonzero((raw == 32767) | (raw == -32768), axis=(1, 2, 3, 4))
        zero_chirps[i:j] = np.count_nonzero(np.all(raw == 0, axis=-1), axis=(1, 2, 3))
        xf = raw.astype(np.float32)
        dc[i:j] = xf.mean(axis=(1, 2, 4))
        rms[i:j] = xf.std(axis=(1, 2, 4))
        _, X = range_fft(raw, g)
        p = np.abs(X) ** 2
        prof[i:j] = p.sum(axis=(1, 2, 3))
        prof_rx += p.sum(axis=(0, 1, 2))
    prof_db = 10 * np.log10(prof + 1e-12)
    prof_rx_db = 10 * np.log10(prof_rx / (nf * L * Cn) + 1e-12)

    # per-frame peak tracking inside the gate
    seg = prof_db[:, g0:g1 + 1]
    kk = np.argmax(seg, axis=1)
    peak_bin = np.array([g0 + k + parabolic(row, k) for row, k in zip(seg, kk)])
    peak_rng = peak_bin * bin_m
    peak_db = seg[np.arange(nf), kk]
    noise_db = np.median(seg, axis=1)
    snr_db = peak_db - noise_db

    k_ph = args.bin_k if args.bin_k is not None else g0 + int(np.argmax(prof_db[:, g0:g1 + 1].mean(axis=0)))

    # ---- pass 2: phase, Doppler at k_ph, channel powers, RD map ----
    npad = max(64, 1 << int(np.ceil(np.log2(L * 4))))
    win_l = np.hanning(L).astype(np.float32)[:, None, None]
    z = np.empty((nf, L, Cn, R), dtype=np.complex64)
    dop = np.empty((nf, npad))
    ch_pow = np.zeros((Cn, R))
    rd_frame = min(max(args.frame if args.frame is not None else f0 + nf // 2, f0), f1 - 1)
    rd_map = None
    for a in range(f0, f1, chunk):
        b = min(f1, a + chunk)
        _, X = range_fft(rd.frames(a, b), g)
        i, j = a - f0, b - f0
        zk = X[..., k_ph]                              # [n][loop][chirp][rx]
        z[i:j] = zk
        ch_pow += (np.abs(zk) ** 2).sum(axis=(0, 1))
        D = np.fft.fftshift(np.fft.fft(zk * win_l, n=npad, axis=1), axes=1)
        dop[i:j] = (np.abs(D) ** 2).sum(axis=(2, 3))
        if a <= rd_frame < b:
            Xf = X[rd_frame - a] * np.hanning(L).astype(np.float32)[:, None, None, None]
            RD = np.fft.fftshift(np.fft.fft(Xf, n=npad, axis=0), axes=0)
            rd_map = 10 * np.log10((np.abs(RD) ** 2).sum(axis=(1, 2)) + 1e-12)
    ch_pow_db = 10 * np.log10(ch_pow / (nf * L) + 1e-12)
    dop_db = 10 * np.log10(dop + 1e-12)
    v_axis = np.fft.fftshift(np.fft.fftfreq(npad, t_loop)) * lam / 2

    # coherent channel combination and phase
    ref = z.mean(axis=(0, 1))
    zc = (z * np.exp(-1j * np.angle(ref))).sum(axis=(2, 3))        # [n][loop]
    phi = np.unwrap(np.angle(zc.ravel()))
    disp_mm = (phi - phi[0]) * lam / (4 * np.pi) * 1e3
    t_loops = (f0 * t_frame + np.arange(nf)[:, None] * t_frame
               + np.arange(L)[None, :] * t_loop).ravel()
    dphi = np.angle((zc[:, 1:] * np.conj(zc[:, :-1])).sum(axis=1))
    vel_pp = lam * dphi / (4 * np.pi * t_loop)
    disp_frame = disp_mm.reshape(nf, L).mean(axis=1)

    t = (f0 + np.arange(nf)) * t_frame
    return dict(g=g, rd=rd, r_axis=r_axis, bin_m=bin_m, lam=lam, f_center=f_center,
                t_loop=t_loop, t_frame=t_frame, tx_labels=tx_labels,
                f0=f0, f1=f1, nf=nf, g0=g0, g1=g1, t=t, k_ph=k_ph,
                prof_db=prof_db, prof_rx_db=prof_rx_db, rms=rms, dc=dc,
                clipped=clipped, zero_chirps=zero_chirps,
                peak_rng=peak_rng, peak_db=peak_db, noise_db=noise_db, snr_db=snr_db,
                disp_mm=disp_mm, t_loops=t_loops, disp_frame=disp_frame, vel_pp=vel_pp,
                dop_db=dop_db, v_axis=v_axis, rd_frame=rd_frame, rd_map=rd_map,
                ch_pow_db=ch_pow_db)


def summary(res):
    g, rd = res["g"], res["rd"]
    p = lambda k, v: print(f"{k:<22}: {v}")                       # noqa: E731
    print("== capture ==")
    p("files", ", ".join(os.path.basename(f) for f in rd.files))
    p("frames in file", f"{rd.num_frames}  (+{rd.leftover} B leftover)")
    p("analysed", f"frames {res['f0']}..{res['f1'] - 1}  = {res['nf']} frames, "
      f"{res['nf'] * res['t_frame']:.2f} s")
    p("layout", f"{g['num_loops']} loops x {g['chirps_per_loop']} chirps "
      f"({', '.join(res['tx_labels'])}) x {g['num_rx']} Rx x {g['num_samples']} samples")
    p("timing", f"T_chirp {res['t_loop'] / g['chirps_per_loop'] * 1e6:.1f} us, "
      f"T_loop {res['t_loop'] * 1e6:.1f} us, T_frame {res['t_frame'] * 1e3:.3f} ms")
    p("RF", f"f_center {res['f_center'] / 1e9:.3f} GHz, lambda {res['lam'] * 1e3:.3f} mm")
    p("range axis", f"bin {res['bin_m'] * 100:.2f} cm, max {res['r_axis'][-1]:.2f} m, "
      f"gate {res['r_axis'][res['g0']]:.2f}..{res['r_axis'][res['g1']]:.2f} m")
    p("velocity axis", f"+-{abs(res['v_axis'][0]):.2f} m/s unambiguous")
    print("== data quality ==")
    p("clipped samples", f"{int(res['clipped'].sum())} total, max {int(res['clipped'].max())}/frame")
    p("zero chirps (lost)", int(res["zero_chirps"].sum()))
    for r in range(g["num_rx"]):
        p(f"Rx{r} rms / dc", f"{res['rms'][:, r].mean():8.1f} / {res['dc'][:, r].mean():7.1f} "
          f"(rms std over time {res['rms'][:, r].std():.1f})")
    ch = res["ch_pow_db"] - res["ch_pow_db"].max()
    print("  power at target bin per channel [dB rel. strongest]  (rows Tx, cols Rx)")
    for c, lab in enumerate(res["tx_labels"]):
        print(f"    {lab:<6}" + "".join(f"{v:7.1f}" for v in ch[c]))
    print("== target ==")
    p("phase/Doppler bin", f"{res['k_ph']}  ({res['k_ph'] * res['bin_m']:.3f} m)")
    p("peak range", f"mean {res['peak_rng'].mean():.4f} m, std {res['peak_rng'].std() * 1e3:.2f} mm, "
      f"min {res['peak_rng'].min():.4f}, max {res['peak_rng'].max():.4f}")
    p("SNR", f"mean {res['snr_db'].mean():.1f} dB, min {res['snr_db'].min():.1f} dB")
    p("displacement (phase)", f"p-p {np.ptp(res['disp_mm']):.3f} mm, "
      f"drift {res['disp_mm'][-1] - res['disp_mm'][0]:.3f} mm")
    p("velocity (pulse-pair)", f"mean {res['vel_pp'].mean() * 1e3:.2f} mm/s, "
      f"std {res['vel_pp'].std() * 1e3:.2f} mm/s")


def write_csv(res, path):
    g = res["g"]
    cols = ["time_s", "frame", "peak_range_m", "peak_db", "noise_db", "snr_db",
            "disp_mm", "vel_pp_mps", "clipped", "zero_chirps"]
    cols += [f"rms_rx{r}" for r in range(g["num_rx"])] + [f"dc_rx{r}" for r in range(g["num_rx"])]
    data = np.column_stack([res["t"], np.arange(res["f0"], res["f1"]), res["peak_rng"],
                            res["peak_db"], res["noise_db"], res["snr_db"], res["disp_frame"],
                            res["vel_pp"], res["clipped"], res["zero_chirps"],
                            res["rms"], res["dc"]])
    np.savetxt(path, data, delimiter=",", header=",".join(cols), comments="", fmt="%.6g")
    print(f"csv written: {path}")


def plot(res, png_dir):
    import matplotlib
    if png_dir:
        matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    g, t, r = res["g"], res["t"], res["r_axis"]
    tf = res["t_frame"]
    ext_t = [t[0], t[-1] + tf]
    figs = []

    # ---- 1: over time ----
    fig, ax = plt.subplots(5, 1, figsize=(12, 13), sharex=True,
                           gridspec_kw={"height_ratios": [2.2, 1, 1, 1, 1]})
    fig.suptitle("Over time")
    im = ax[0].imshow(res["prof_db"].T, aspect="auto", origin="lower",
                      extent=ext_t + [r[0], r[-1]], interpolation="nearest")
    ax[0].plot(t + tf / 2, res["peak_rng"], "w.", ms=2, label="tracked peak")
    for k in (res["g0"], res["g1"]):
        ax[0].axhline(r[k], color="w", ls=":", lw=0.8)
    ax[0].set(ylabel="range [m]", title="Range profile (power over chirps, Rx)")
    ax[0].legend(loc="upper right", fontsize=8)
    fig.colorbar(im, ax=ax[0], pad=0.01, label="dB")
    ax[1].plot(t, res["peak_rng"], lw=0.8)
    ax[1].set(ylabel="peak range [m]")
    ax[2].plot(t, res["peak_db"], lw=0.8, label="peak")
    ax[2].plot(t, res["noise_db"], lw=0.8, label="noise (gate median)")
    a2 = ax[2].twinx()
    a2.plot(t, res["snr_db"], "k", lw=0.6, alpha=0.6)
    a2.set_ylabel("SNR [dB]")
    ax[2].set(ylabel="power [dB]")
    ax[2].legend(loc="upper left", fontsize=8)
    ax[3].plot(res["t_loops"], res["disp_mm"], lw=0.6)
    ax[3].set(ylabel="displacement [mm]",
              title=f"Phase at bin {res['k_ph']} ({res['k_ph'] * res['bin_m']:.3f} m), "
                    f"channels combined")
    ax[4].plot(t, res["vel_pp"] * 1e3, lw=0.6)
    ax[4].set(ylabel="velocity [mm/s]", xlabel="time [s]")
    for a in ax[1:]:
        a.grid(True, alpha=0.3)
    fig.tight_layout()
    figs.append(("1_over_time", fig))

    # ---- 2: Doppler ----
    fig, ax = plt.subplots(2, 1, figsize=(12, 9))
    fig.suptitle("Doppler")
    v = res["v_axis"]
    im = ax[0].imshow(res["dop_db"].T, aspect="auto", origin="lower",
                      extent=ext_t + [v[0], v[-1]], interpolation="nearest")
    ax[0].set(xlabel="time [s]", ylabel="velocity [m/s]",
              title=f"Velocity spectrum at bin {res['k_ph']} per frame")
    fig.colorbar(im, ax=ax[0], pad=0.01, label="dB")
    if res["rd_map"] is not None:
        im = ax[1].imshow(res["rd_map"], aspect="auto", origin="lower",
                          extent=[r[0], r[-1], v[0], v[-1]], interpolation="nearest")
        ax[1].axvline(res["k_ph"] * res["bin_m"], color="w", ls=":", lw=0.8)
        ax[1].set(xlabel="range [m]", ylabel="velocity [m/s]",
                  title=f"Range-Doppler map, frame {res['rd_frame']}")
        fig.colorbar(im, ax=ax[1], pad=0.01, label="dB")
    fig.tight_layout()
    figs.append(("2_doppler", fig))

    # ---- 3: health ----
    fig = plt.figure(figsize=(12, 10))
    fig.suptitle("Data health")
    gs = fig.add_gridspec(3, 2)
    a = fig.add_subplot(gs[0, :])
    for rx in range(g["num_rx"]):
        a.plot(t, res["rms"][:, rx], lw=0.7, label=f"Rx{rx}")
    a.set(ylabel="ADC rms", title="ADC RMS per Rx")
    a.legend(fontsize=8, ncol=4)
    a.grid(True, alpha=0.3)
    a = fig.add_subplot(gs[1, 0])
    for rx in range(g["num_rx"]):
        a.plot(t, res["dc"][:, rx], lw=0.7, label=f"Rx{rx}")
    a.set(xlabel="time [s]", ylabel="ADC mean", title="DC offset per Rx")
    a.grid(True, alpha=0.3)
    a = fig.add_subplot(gs[1, 1])
    a.plot(t, res["clipped"], lw=0.7, label="clipped samples")
    a.plot(t, res["zero_chirps"], lw=0.7, label="zero chirps (lost)")
    a.set(xlabel="time [s]", ylabel="count / frame", title="Clipping and loss")
    a.legend(fontsize=8)
    a.grid(True, alpha=0.3)
    a = fig.add_subplot(gs[2, 0])
    ch = res["ch_pow_db"] - res["ch_pow_db"].max()
    im = a.imshow(ch, cmap="viridis", aspect="auto", vmin=min(ch.min(), -1.0), vmax=0)
    for (i, j), val in np.ndenumerate(ch):
        a.text(j, i, f"{val:.1f}", ha="center", va="center", color="w", fontsize=8)
    a.set(xticks=range(g["num_rx"]), xticklabels=[f"Rx{i}" for i in range(g["num_rx"])],
          yticks=range(len(res["tx_labels"])), yticklabels=res["tx_labels"],
          title=f"Power at bin {res['k_ph']} per channel [dB rel.]")
    fig.colorbar(im, ax=a)
    a = fig.add_subplot(gs[2, 1])
    for rx in range(g["num_rx"]):
        a.plot(r, res["prof_rx_db"][rx], lw=0.8, label=f"Rx{rx}")
    a.set(xlabel="range [m]", ylabel="dB", title="Mean range profile per Rx")
    a.legend(fontsize=8)
    a.grid(True, alpha=0.3)
    fig.tight_layout()
    figs.append(("3_health", fig))

    if png_dir:
        os.makedirs(png_dir, exist_ok=True)
        for name, f in figs:
            path = os.path.join(png_dir, f"{name}.png")
            f.savefig(path, dpi=120)
            print(f"png written: {path}")
    else:
        plt.show()


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cfg", help="radar .cfg sent to the board for this capture")
    ap.add_argument("bin", help="<prefix>_Raw_0.bin")
    ap.add_argument("--gate", type=float, nargs=2, metavar=("MIN", "MAX"))
    ap.add_argument("--bin", dest="bin_k", type=int, help="phase/Doppler range bin")
    ap.add_argument("--frame", type=int, help="frame for the range-Doppler map")
    ap.add_argument("--t0", type=float, help="start time [s]")
    ap.add_argument("--t1", type=float, help="end time [s]")
    ap.add_argument("--chunk", type=int, default=200, help="frames per chunk")
    ap.add_argument("--csv", help="write per-frame results to this CSV")
    ap.add_argument("--png", help="save figures as PNG into this directory")
    ap.add_argument("--no-plot", action="store_true")
    args = ap.parse_args()

    res = analyse(args)
    summary(res)
    if args.csv:
        write_csv(res, args.csv)
    if not args.no_plot:
        plot(res, args.png)


if __name__ == "__main__":
    main()
