#!/usr/bin/env python3
"""
parse_capture_adcstep.py - stitch a stepped-ADC-start DCA1000 capture

  parse_capture_adcstep.py <radar.cfg> <prefix_Raw_0.bin> [options]
  parse_capture_adcstep.py <radar.cfg> --synth test_Raw_0.bin [--range 0.8 0.87]

For cfgs like differ_test_cfgs/differ_shortrange_adcstep_1.cfg: the Tx
sequence is repeated inside one loop, each repeat with a different chirpCfg
adcStartTimeVar, so each repeat samples another part of the same ramp. This
script puts the windows of one Tx/Rx channel back on the ramp time axis and
treats them as one long chirp (3 x 16 us -> 48 us, 1.34 -> 4.03 GHz).
Same input layout as parse_capture.py (Doc/10).

Three figure windows (or PNGs with --png):
  1 Stitching   ADC trace of one channel on the ramp time axis with the seams,
                range profiles of each window vs stitched, window-to-window
                phase / gain / coherence over time, DC and RMS per window
  2 Over time   range-time map (stitched), peak range stitched vs each
                window alone, power / SNR, displacement, velocity
  3 Doppler     velocity spectrum at the target bin, range-Doppler map,
     + health   mean profile per Rx, power per Tx-Rx channel, ADC RMS

Methods:
  geometry    chirps are grouped by (Tx mask, adcStartTimeVar). Every Tx needs
              a chirp at every ADC offset. Window k starts t_k = offset_k after
              window 0 and lands at sample m_k = round(t_k * Fs) of the
              stitched record. Overlapping samples are averaged, gaps stay 0.
  stitching   the beat tone of a target depends only on the time since the
              ramp start, so the windows of one channel form one longer beat
              signal. The spectrum is the sum of the per-window FFTs, each
              referenced to its own start time (exact, also for fractional
              offsets):  Y(f) = sum_k c_k FFT(w_k x_k)(f) exp(-j 2 pi f t_k)
              w = one Hann window over the whole stitched length.
  coherence   the windows of one Tx come from different chirps, 4 T_chirp
              apart. Target motion between them, or a fixed phase difference
              between chirps in the chip, makes them disagree: the stitched
              peak gets wider, splits or grows sidelobes.
              window phase: f = peak of the summed per-window power in the gate,
              refined on the DTFT (it is measured inside the windows, so window
              phase offsets cannot shift it). At f, each window's DTFT on the
              ramp time axis  Z_k = sum_n hann(n) x_k(n) exp(-j 2 pi f (t_k + n/Fs))
              dphi_k = angle(sum_ch Z_k conj(Z_0)) per loop. 0 for a static
              target and an ideal chip; a move dR between the chirps gives
              4 pi dR / lambda_k. Only meaningful when ONE target dominates
              a window resolution cell (~16 cm): two targets closer than that
              beat inside each window, which looks exactly like a window
              phase step (and it is the information stitching resolves them
              with). The summary warns when the windows disagree on the
              peak range, which is the sign of this.
              coherence eta at f = |sum_k Zg_k|^2 / (sum_k |Zg_k|)^2 over
              channels, Zg with the stitched window segments: 1 = windows add
              in phase.
  --align     none   plain stitching. Right for a static scene and ideal chip.
              calib  removes the phase and gain per window stored with
                     --calib-out from a capture of ONE reflector (metal plate,
                     corner reflector) in the same cfg: chip offsets, without
                     touching the scene. The recommended way to correct.
              fixed  removes one constant phase per window (the circular mean
                     of dphi_k over this capture). Single-target scenes only.
              peak   removes dphi_k per frame and loop: also motion between
                     the chirps. Locks onto the strongest target in the gate.
              Motion between the windows and a range offset both give a phase
              step per window, so no mode can tell them apart. Check the
              dphi plot: a flat line is a chip offset (calib/fixed), a moving
              one is motion (peak). The dotted lines there are the window
              phase the pulse-pair velocity predicts (4 pi v dt_k / lambda_k);
              when they follow the measured phase, it is motion.
  --equalize  also divides each window by its mean amplitude ratio |Z_k|/|Z_0|
              (fixed/peak; calib always applies the stored gains).
  --calib-out writes the circular-mean dphi_k and mean gain of this capture
              to an .npz for --align calib --calib FILE.
  DC          removed per stitched record (default). --dc window removes it
              per window: kills seam steps from DC differences between
              chirps, but also removes part of a slow beat tone.
  range etc.  as parse_capture_adv.py, on the stitched spectrum. Profiles are
              tone-normalised (a tone has the same peak level in a window and
              stitched), so the stitched noise floor sits lower.
              lambda for phase -> displacement at the RF centre of the
              stitched span.

Options:
  --gate MIN MAX   range gate in m (default: plasmaCfg gate from the cfg)
  --rmax M         max range shown and stored (default: from the gate)
  --align MODE     none | calib | fixed | peak (default none)
  --calib FILE     window calibration for --align calib
  --calib-out FILE save this capture's window phase/gain as a calibration
  --equalize       equalise window amplitudes (with fixed/peak)
  --dc MODE        stitched | window (default stitched)
  --bin K          fix the phase/Doppler bin (index on the zero-padded axis)
  --frame N        frame for the ADC trace and range-Doppler map
  --nfft N         range FFT size (default 4 x pow2(stitched length))
  --t0/--t1 S      analyse only this time window
  --csv FILE       per-frame results as CSV
  --save FILE.npz  profiles, axes and window phases
  --png DIR        save the figures as PNG instead of showing them
  --no-plot        print the summary only

--synth writes a fake capture for the cfg: targets at --range (several
allowed), optional --vib-mm/--vib-hz motion and --win-phase offsets per window
in degrees, to test the stitching and the align modes without hardware.
"""

import argparse
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from parse_capture import C, parse_cfg  # noqa: E402
from parse_capture_adv import RawReader, parabolic  # noqa: E402

try:
    from scipy import fft as _fft

    def rfft(x, n):
        return _fft.rfft(x, n=n, axis=-1, workers=-1)
except ImportError:
    def rfft(x, n):
        return np.fft.rfft(x, n=n, axis=-1)


def cfg_commands(path):
    cmds = {}
    for line in open(path):
        w = line.split("%", 1)[0].split()
        if w:
            cmds.setdefault(w[0], []).append(w[1:])
    return cmds


def step_geometry(cfg_path, g):
    """ADC windows per Tx from the chirpCfg adcStartTimeVar values."""
    cmds = cfg_commands(cfg_path)
    chirps = {}
    for w in cmds.get("chirpCfg", []):
        sf, sl, idle, adc = (float(v) for v in w[3:7])
        for i in range(int(w[0]), int(w[1]) + 1):
            # firmware quantises the variation to 10 ns
            chirps[i] = dict(sf=sf, sl=sl, idle=idle, adc=round(adc * 100) / 100, tx=int(w[7]))
    order = list(range(g["chirp_start"], g["chirp_end"] + 1))
    missing = [i for i in order if i not in chirps]
    if missing:
        raise SystemExit(f"error: frameCfg uses chirps {missing} that have no chirpCfg")
    for i in order:
        if chirps[i]["sf"] or chirps[i]["sl"]:
            raise SystemExit(f"error: chirp {i} sets startFreqVar/freqSlopeVar, only "
                             "adcStartTimeVar steps are supported")

    txs = []
    for i in order:
        if chirps[i]["tx"] not in txs:
            txs.append(chirps[i]["tx"])
    offs = sorted({chirps[i]["adc"] for i in order})
    K, T = len(offs), len(txs)
    idx = np.full((K, T), -1)
    for p, i in enumerate(order):
        k, t = offs.index(chirps[i]["adc"]), txs.index(chirps[i]["tx"])
        if idx[k, t] >= 0:
            raise SystemExit(f"error: two chirps with Tx mask {chirps[i]['tx']} at "
                             f"ADC offset {chirps[i]['adc']} us")
        idx[k, t] = p
    if (idx < 0).any():
        raise SystemExit("error: every Tx needs one chirp at every ADC offset")
    if K < 2:
        print("warning: all chirps have the same ADC start, nothing to stitch",
              file=sys.stderr)

    fs = g["fs_ksps"] * 1e3
    slope = g["slope_mhz_us"] * 1e12
    f0 = g["start_ghz"] * 1e9
    adc0 = g["adc_start_us"] * 1e-6
    N = g["num_samples"]
    t_k = np.array(offs) * 1e-6
    m_exact = t_k * fs
    m_k = np.round(m_exact).astype(int)
    frac = np.abs(m_exact - m_k).max()
    if frac > 0.01:
        print(f"warning: ADC offsets are not whole samples (up to {frac:.2f} sample). "
              "Spectra use the exact times, the time trace is rounded.", file=sys.stderr)
    n_tot = int(m_k[-1] + N)
    cover = np.zeros(n_tot)
    for m in m_k:
        cover[m:m + N] += 1
    w_glob = np.hanning(n_tot)
    wseg = np.stack([w_glob[m:m + N] / cover[m:m + N] for m in m_k]).astype(np.float32)
    hann = np.hanning(N).astype(np.float32)

    ramp_end = g["ramp_end_us"] * 1e-6
    late = [k for k in range(K) if adc0 + t_k[k] + N / fs > ramp_end + 1e-12]
    if late:
        print(f"warning: windows {late} run past rampEnd {g['ramp_end_us']} us",
              file=sys.stderr)

    # chirp start times inside one loop
    tc = np.array([g["idle_us"] + chirps[i]["idle"] + g["ramp_end_us"] for i in order]) * 1e-6
    t_chirp = np.concatenate([[0.0], np.cumsum(tc)[:-1]])
    dt_win = np.array([np.mean(t_chirp[idx[k]] - t_chirp[idx[0]]) for k in range(K)])

    f_lo = f0 + slope * (adc0 + t_k)
    f_hi = f_lo + slope * N / fs
    f_center = f0 + slope * (adc0 + n_tot / (2 * fs))

    gate = None
    if "plasmaCfg" in cmds:
        p = cmds["plasmaCfg"][-1]
        gate = (float(p[1]), float(p[2]))

    return dict(
        K=K, T=T, N=N, n_tot=n_tot, idx=idx, tx_masks=txs,
        tx_labels=["+".join(f"TX{b}" for b in range(4) if m >> b & 1) for m in txs],
        offs_us=np.array(offs), t_k=t_k, m_k=m_k, cover=cover, wseg=wseg,
        cg=float(wseg.sum()), hann=hann, cg_win=float(hann.sum()),
        fs=fs, slope=slope, f0=f0, adc0=adc0,
        f_lo=f_lo, f_hi=f_hi, lam_win=C / ((f_lo + f_hi) / 2),
        f_center=f_center, lam=C / f_center,
        t_chirp=t_chirp, t_loop=float(tc.sum()), dt_win=dt_win,
        gaps=int(np.count_nonzero(cover == 0)), overlaps=int(np.count_nonzero(cover > 1)),
        gate_cfg=gate,
    )


def split_windows(raw, g, sg):
    """int16 frames -> float32 [n][loop][window][tx][rx][sample]"""
    x = raw.reshape((-1,) + g["frame_shape"]).astype(np.float32)
    return x[:, :, sg["idx"]]


def remove_dc(x, mode):
    if mode == "window":
        return x - x.mean(axis=-1, keepdims=True)
    return x - x.mean(axis=(2, 5), keepdims=True)


def contributions(x, sg, nfft, nbd):
    """per-window parts of the stitched spectrum, on the ramp time axis of window 0
    -> complex64 [n][loop][window][tx][rx][bin]"""
    Y = rfft(x * sg["wseg"][:, None, None, :], nfft)[..., :nbd]
    f = np.arange(nbd) * sg["fs"] / nfft
    ramp = np.exp(-2j * np.pi * f[None, :] * sg["t_k"][:, None]).astype(np.complex64)
    return Y * ramp[:, None, None, :]


def window_dtft(x, sg, f, w):
    """DTFT of every window at frequency f[n] (one per frame) on the ramp time
    axis, with per-window weights w [window][sample] -> [n][loop][window][tx][rx]"""
    tn = sg["t_k"][None, :, None] + np.arange(sg["N"])[None, None, :] / sg["fs"]
    E = w[None] * np.exp(-2j * np.pi * f[:, None, None] * tn)
    return np.einsum("nlktrs,nks->nlktr", x, E.astype(np.complex64))


def circ_mean(a, axis=None):
    return np.angle(np.mean(np.exp(1j * a), axis=axis))


def circ_std(a, axis=None):
    r = np.abs(np.mean(np.exp(1j * a), axis=axis))
    return np.sqrt(-2 * np.log(np.clip(r, 1e-12, 1)))


def peak_in(prof_db, g0, g1):
    seg = prof_db[g0:g1 + 1]
    k = int(np.argmax(seg))
    return g0 + k + parabolic(seg, k), seg[k]


def width_3db(prof_db, k):
    """-3 dB width around integer bin k in bins, linear interpolation"""
    lvl = prof_db[k] - 3.0
    i = k
    while i > 0 and prof_db[i - 1] > lvl:
        i -= 1
    left = i - (prof_db[i] - lvl) / (prof_db[i] - prof_db[i - 1]) if i > 0 else i
    j = k
    while j < len(prof_db) - 1 and prof_db[j + 1] > lvl:
        j += 1
    right = j + (prof_db[j] - lvl) / (prof_db[j] - prof_db[j + 1]) if j < len(prof_db) - 1 else j
    return right - left


def analyse(args):
    g = parse_cfg(args.cfg)
    sg = step_geometry(args.cfg, g)
    rd = RawReader(args.bin, g["frame_bytes"])
    if rd.num_frames == 0:
        raise SystemExit("error: less than one frame in the capture")

    K, T, N = sg["K"], sg["T"], sg["N"]
    L, R = g["num_loops"], g["num_rx"]
    fs, slope = sg["fs"], sg["slope"]
    nfft = args.nfft or 4 * (1 << int(np.ceil(np.log2(sg["n_tot"]))))
    bin_m = fs / nfft * C / (2 * slope)
    r_max = fs / 2 * C / (2 * slope)

    gate = args.gate or sg["gate_cfg"]
    native = C / (2 * slope * sg["n_tot"] / fs)          # stitched resolution
    if gate is None:
        gate = (2 * native, 0.9 * r_max)
    r_disp = args.rmax or min(r_max, max(1.5 * gate[1], gate[1] + 0.5))
    nbd = min(nfft // 2 + 1, int(np.ceil(r_disp / bin_m)) + 2)
    r_axis = np.arange(nbd) * bin_m
    g0 = max(1, int(np.floor(gate[0] / bin_m)))
    g1 = min(nbd - 2, int(np.ceil(gate[1] / bin_m)))
    if g1 - g0 < 3:
        raise SystemExit("error: gate too small")

    t_frame = g["frame_ms"] * 1e-3
    f0 = int(args.t0 / t_frame) if args.t0 else 0
    f1 = min(rd.num_frames, int(np.ceil(args.t1 / t_frame))) if args.t1 else rd.num_frames
    if f0 >= f1:
        raise SystemExit("error: empty time window")
    nf = f1 - f0
    chunk = max(1, args.chunk)
    cg2, cgw2 = sg["cg"] ** 2, sg["cg_win"] ** 2

    # ---- pass 1: plain stitched + per-window profiles, window phases ----
    prof_none = np.empty((nf, nbd))
    prof_win = np.empty((nf, K, nbd))
    dphi = np.zeros((nf, L, K))
    amp = np.ones((nf, K))
    coh = np.empty(nf)
    f_pk = np.empty(nf)
    rms = np.empty((nf, R))
    dc = np.empty((nf, R))
    dc_win = np.empty((nf, K, R))
    rms_win = np.empty((nf, K, R))
    clipped = np.empty(nf, dtype=np.int64)
    zero_chirps = np.empty(nf, dtype=np.int64)
    for a in range(f0, f1, chunk):
        b = min(f1, a + chunk)
        i, j = a - f0, b - f0
        raw = rd.frames(a, b).reshape((-1,) + g["frame_shape"])
        clipped[i:j] = np.count_nonzero((raw == 32767) | (raw == -32768), axis=(1, 2, 3, 4))
        zero_chirps[i:j] = np.count_nonzero(np.all(raw == 0, axis=-1), axis=(1, 2, 3))
        xf = split_windows(raw, g, sg)
        dc[i:j] = xf.mean(axis=(1, 2, 3, 5))
        rms[i:j] = xf.std(axis=(1, 2, 3, 5))
        dc_win[i:j] = xf.mean(axis=(1, 3, 5))
        rms_win[i:j] = xf.std(axis=(1, 3, 5))
        x = remove_dc(xf, args.dc)

        Y = contributions(x, sg, nfft, nbd)
        prof_none[i:j] = (np.abs(Y.sum(axis=2)) ** 2).mean(axis=(1, 2, 3)) / cg2
        Pw = np.abs(rfft(x * sg["hann"], nfft)[..., :nbd]) ** 2
        prof_win[i:j] = Pw.mean(axis=(1, 3, 4)) / cgw2

        # target frequency from inside the windows (incoherent sum), so that
        # window phase offsets cannot pull it: a frequency error df would show
        # up as a fake phase step 2 pi df t_k per window
        hw = np.broadcast_to(sg["hann"], (K, N))
        pdb = 10 * np.log10(prof_win[i:j].sum(axis=1) + 1e-12)
        f = np.array([peak_in(row, g0, g1)[0] for row in pdb]) * fs / nfft
        df = fs / nfft / 2
        pw3 = np.stack([(np.abs(window_dtft(x, sg, f + s * df, hw)) ** 2).sum(axis=(1, 2, 3, 4))
                        for s in (-1, 0, 1)], axis=1)
        f_pk[i:j] = f + df * np.array([parabolic(np.log(row + 1e-30), 1) for row in pw3])
        Z = window_dtft(x, sg, f_pk[i:j], hw)
        cross = (Z * np.conj(Z[:, :, :1])).sum(axis=(3, 4))          # [n][loop][k]
        dphi[i:j] = np.angle(cross)
        p = (np.abs(Z) ** 2).sum(axis=(1, 3, 4))                     # [n][k]
        amp[i:j] = np.sqrt(p / (p[:, :1] + 1e-30))
        Zg = window_dtft(x, sg, f_pk[i:j], sg["wseg"])
        num = (np.abs(Zg.sum(axis=2)) ** 2).sum(axis=(1, 2, 3))
        den = (np.abs(Zg).sum(axis=2) ** 2).sum(axis=(1, 2, 3))
        coh[i:j] = num / (den + 1e-30)

    fixed = circ_mean(dphi.reshape(-1, K), axis=0)
    gain_meas = np.exp(np.log(amp + 1e-12).mean(axis=0))
    gain = gain_meas if args.equalize else np.ones(K)
    calib = None
    if args.align == "calib":
        if not args.calib:
            raise SystemExit("error: --align calib needs --calib FILE")
        cz = np.load(args.calib)
        if not np.allclose(cz["offsets_us"], sg["offs_us"]):
            raise SystemExit(f"error: calib file is for ADC offsets {cz['offsets_us']} us, "
                             f"this cfg has {sg['offs_us']} us")
        calib = cz["dphi_rad"]
        gain = cz["gain"]
    prof_none_db = 10 * np.log10(prof_none + 1e-12)
    if args.bin_k is not None:
        k_ph = args.bin_k
    else:
        k_ph = g0 + int(np.argmax(prof_none_db[:, g0:g1 + 1].mean(axis=0)))
    if not g0 <= k_ph <= g1:
        print(f"warning: phase bin {k_ph} is outside the gate", file=sys.stderr)

    def phases(mode, i, j):
        if mode == "peak":
            return dphi[i:j]
        if mode == "fixed":
            return np.broadcast_to(fixed, (j - i, L, K))
        if mode == "calib":
            return np.broadcast_to(calib, (j - i, L, K))
        return np.zeros((j - i, L, K))

    # ---- pass 2: aligned stitched spectrum ----
    variants = ("none", "fixed", "peak") + (("calib",) if calib is not None else ())
    prof_var = {v: np.zeros(nbd) for v in variants}
    prof = np.empty((nf, nbd))
    prof_rx = np.zeros((R, nbd))
    z = np.empty((nf, L, T, R), dtype=np.complex64)
    ch_pow = np.zeros((T, R))
    npad = max(64, 1 << int(np.ceil(np.log2(L * 4))))
    win_l = np.hanning(L + 2)[1:-1].astype(np.float32)          # no zero end points
    dop = np.empty((nf, npad))
    rd_frame = min(max(args.frame if args.frame is not None else f0 + nf // 2, f0), f1 - 1)
    rd_map = trace = None
    for a in range(f0, f1, chunk):
        b = min(f1, a + chunk)
        i, j = a - f0, b - f0
        raw = rd.frames(a, b).reshape((-1,) + g["frame_shape"])
        x = remove_dc(split_windows(raw, g, sg), args.dc)
        Y = contributions(x, sg, nfft, nbd) / gain[:, None, None, None].astype(np.float32)
        for v in variants:
            c = np.exp(-1j * phases(v, i, j)).astype(np.complex64)
            Ya = np.einsum("nlktrb,nlk->nltrb", Y, c)
            pw = np.abs(Ya) ** 2
            prof_var[v] += pw.mean(axis=(1, 2, 3)).sum(axis=0) / cg2
            if v != args.align:
                continue
            prof[i:j] = pw.mean(axis=(1, 2, 3)) / cg2
            prof_rx += pw.sum(axis=(0, 1, 2)) / cg2
            zk = Ya[..., k_ph]
            z[i:j] = zk
            ch_pow += (np.abs(zk) ** 2).sum(axis=(0, 1)) / cg2
            D = np.fft.fftshift(np.fft.fft(zk * win_l[:, None, None], n=npad, axis=1), axes=1)
            dop[i:j] = (np.abs(D) ** 2).sum(axis=(2, 3))
            if a <= rd_frame < b:
                Xf = Ya[rd_frame - a] * win_l[:, None, None, None]
                RD = np.fft.fftshift(np.fft.fft(Xf, n=npad, axis=0), axes=0)
                rd_map = 10 * np.log10((np.abs(RD) ** 2).sum(axis=(1, 2)) / cg2 + 1e-12)
        if a <= rd_frame < b:
            trace = x[rd_frame - a, :, :, 0, 0]                      # [loop][window][sample]
    for v in variants:
        prof_var[v] = 10 * np.log10(prof_var[v] / nf + 1e-12)
    prof_db = 10 * np.log10(prof + 1e-12)
    prof_win_db = 10 * np.log10(prof_win + 1e-12)
    prof_rx_db = 10 * np.log10(prof_rx / (nf * L * T) + 1e-12)
    ch_pow_db = 10 * np.log10(ch_pow / (nf * L) + 1e-12)
    dop_db = 10 * np.log10(dop + 1e-12)
    lam = sg["lam"]
    v_axis = np.fft.fftshift(np.fft.fftfreq(npad, sg["t_loop"])) * lam / 2

    # per-frame peaks: stitched and each window alone
    pk = np.array([peak_in(row, g0, g1) for row in prof_db])
    peak_rng, peak_db = pk[:, 0] * bin_m, pk[:, 1]
    noise_db = np.median(prof_db[:, g0:g1 + 1], axis=1)
    snr_db = peak_db - noise_db
    peak_rng_win = np.array([[peak_in(prof_win_db[n, k], g0, g1)[0] * bin_m
                              for k in range(K)] for n in range(nf)])

    # -3 dB widths of the mean profiles at the gate peak
    widths = {}
    for name, p in [("window 0", 10 * np.log10(prof_win[:, 0].mean(axis=0) + 1e-12)),
                    ("incoherent", 10 * np.log10(prof_win.mean(axis=(0, 1)) + 1e-12))] + \
                   [(f"stitched {v}", prof_var[v]) for v in variants]:
        k = g0 + int(np.argmax(p[g0:g1 + 1]))
        widths[name] = width_3db(p, k) * bin_m

    # phase -> displacement and pulse-pair velocity, channels combined
    ref = z.mean(axis=(0, 1))
    zc = (z * np.exp(-1j * np.angle(ref))).sum(axis=(2, 3))         # [n][loop]
    phi = np.unwrap(np.angle(zc.ravel()))
    disp_mm = (phi - phi[0]) * lam / (4 * np.pi) * 1e3
    t_loops = (f0 * t_frame + np.arange(nf)[:, None] * t_frame
               + np.arange(L)[None, :] * sg["t_loop"]).ravel()
    disp_frame = disp_mm.reshape(nf, L).mean(axis=1)
    if L > 1:
        dpp = np.angle((zc[:, 1:] * np.conj(zc[:, :-1])).sum(axis=1))
        vel_pp = lam * dpp / (4 * np.pi * sg["t_loop"])
    else:
        vel_pp = np.full(nf, np.nan)
    # window phase that the pulse-pair velocity predicts: 4 pi v dt_k / lambda_k
    dphi_motion = np.angle(np.exp(4j * np.pi * np.nan_to_num(vel_pp)[:, None]
                                  * sg["dt_win"][None, :] / sg["lam_win"][None, :]))

    t = (f0 + np.arange(nf)) * t_frame
    return dict(g=g, sg=sg, rd=rd, args=args, nfft=nfft, bin_m=bin_m, r_max=r_max,
                r_axis=r_axis, gate=gate, g0=g0, g1=g1, native=native,
                t_frame=t_frame, f0=f0, f1=f1, nf=nf, t=t, k_ph=k_ph,
                prof_db=prof_db, prof_none_db=prof_none_db, prof_win_db=prof_win_db,
                prof_var=prof_var, prof_rx_db=prof_rx_db, ch_pow_db=ch_pow_db,
                dphi=dphi, fixed=fixed, amp=amp, gain=gain, gain_meas=gain_meas,
                coh=coh, f_pk=f_pk,
                rms=rms, dc=dc, dc_win=dc_win, rms_win=rms_win,
                clipped=clipped, zero_chirps=zero_chirps,
                peak_rng=peak_rng, peak_db=peak_db, noise_db=noise_db, snr_db=snr_db,
                peak_rng_win=peak_rng_win, widths=widths,
                disp_mm=disp_mm, t_loops=t_loops, disp_frame=disp_frame, vel_pp=vel_pp,
                dphi_motion=dphi_motion,
                dop_db=dop_db, v_axis=v_axis, rd_frame=rd_frame, rd_map=rd_map,
                trace=trace, lam=lam)


def summary(res):
    g, sg, rd, args = res["g"], res["sg"], res["rd"], res["args"]
    K = sg["K"]
    p = lambda k, v: print(f"{k:<24}: {v}")                       # noqa: E731
    print("== capture ==")
    p("files", ", ".join(os.path.basename(f) for f in rd.files))
    p("frames in file", f"{rd.num_frames}  (+{rd.leftover} B leftover)")
    p("analysed", f"frames {res['f0']}..{res['f1'] - 1}  = {res['nf']} frames, "
      f"{res['nf'] * res['t_frame']:.2f} s")
    p("layout", f"{g['num_loops']} loops x {K} ADC offsets x {sg['T']} Tx "
      f"({', '.join(sg['tx_labels'])}) x {g['num_rx']} Rx x {sg['N']} samples")
    print("== ADC windows ==")
    for k in range(K):
        t0 = (sg["adc0"] + sg["t_k"][k]) * 1e6
        print(f"  window {k}: +{sg['offs_us'][k]:5.2f} us  ADC {t0:6.2f}..{t0 + sg['N'] / sg['fs'] * 1e6:6.2f} us"
              f"  RF {sg['f_lo'][k] / 1e9:.3f}..{sg['f_hi'][k] / 1e9:.3f} GHz"
              f"  chirp dt {sg['dt_win'][k] * 1e6:6.1f} us")
    b_win = sg["slope"] * sg["N"] / sg["fs"]
    b_st = sg["slope"] * sg["n_tot"] / sg["fs"]
    p("stitched record", f"{sg['n_tot']} samples ({sg['n_tot'] / sg['fs'] * 1e6:.2f} us), "
      f"gaps {sg['gaps']}, overlapping {sg['overlaps']} samples")
    p("bandwidth", f"window {b_win / 1e9:.3f} GHz ({C / (2 * b_win) * 100:.2f} cm), "
      f"stitched {b_st / 1e9:.3f} GHz ({C / (2 * b_st) * 100:.2f} cm)")
    p("RF centre", f"{sg['f_center'] / 1e9:.3f} GHz, lambda {res['lam'] * 1e3:.3f} mm")
    p("range axis", f"nfft {res['nfft']}, bin {res['bin_m'] * 100:.2f} cm, R_max {res['r_max']:.2f} m, "
      f"gate {res['gate'][0]:.2f}..{res['gate'][1]:.2f} m")
    p("timing", f"T_loop {sg['t_loop'] * 1e6:.1f} us, T_frame {res['t_frame'] * 1e3:.3f} ms, "
      f"velocity +-{abs(res['v_axis'][0]):.2f} m/s")
    print("== data quality ==")
    p("clipped samples", f"{int(res['clipped'].sum())} total, max {int(res['clipped'].max())}/frame")
    p("zero chirps (lost)", int(res["zero_chirps"].sum()))
    for r in range(g["num_rx"]):
        dcs = " ".join(f"{v:7.1f}" for v in res["dc_win"][:, :, r].mean(axis=0))
        p(f"Rx{r} rms / dc per window", f"{res['rms'][:, r].mean():7.1f} / {dcs}")
    print("== stitching ==")
    for k in range(1, K):
        d = res["dphi"][:, :, k]
        a = 20 * np.log10(res["amp"][:, k].mean())
        dmm = np.degrees(circ_mean(d))
        print(f"  window {k} vs 0: phase {dmm:7.1f} deg (std {np.degrees(circ_std(d)):5.1f}),"
              f" gain {a:+5.2f} dB, = {np.radians(dmm) * sg['lam_win'][k] / (4 * np.pi) * 1e3:+.3f} mm")
    p("coherence eta", f"mean {res['coh'].mean():.3f}, min {res['coh'].min():.3f}  (1 = in phase)")
    p("align / dc / equalize", f"{args.align} / {args.dc} / {args.equalize}")
    th_w = 1.44 * C / (2 * b_win) * 100
    th_s = 1.44 * C / (2 * b_st) * 100
    print(f"  -3 dB width of the mean peak (Hann theory: window {th_w:.1f} cm, stitched {th_s:.1f} cm)")
    for name, w in res["widths"].items():
        print(f"    {name:<16}{w * 100:7.2f} cm")
    print("== target ==")
    p("phase/Doppler bin", f"{res['k_ph']}  ({res['k_ph'] * res['bin_m']:.4f} m)")
    p("peak range stitched", f"mean {res['peak_rng'].mean():.4f} m, std {res['peak_rng'].std() * 1e3:.2f} mm")
    for k in range(K):
        pr = res["peak_rng_win"][:, k]
        p(f"peak range window {k}", f"mean {pr.mean():.4f} m, std {pr.std() * 1e3:.2f} mm")
    p("SNR", f"mean {res['snr_db'].mean():.1f} dB, min {res['snr_db'].min():.1f} dB")
    p("displacement (phase)", f"p-p {np.ptp(res['disp_mm']):.3f} mm, "
      f"drift {res['disp_mm'][-1] - res['disp_mm'][0]:.3f} mm")
    p("velocity (pulse-pair)", f"mean {np.nanmean(res['vel_pp']) * 1e3:.2f} mm/s, "
      f"std {np.nanstd(res['vel_pp']) * 1e3:.2f} mm/s")

    if K > 1:
        d = res["dphi"][:, :, 1:].reshape(-1, K - 1)
        mean = np.abs(np.degrees(circ_mean(d, axis=0)))
        std = np.degrees(circ_std(d, axis=0))
        # windows alone should all see the same peak; a spread means more
        # than one target inside a window resolution cell
        win_res = C / (2 * b_win)
        spread = np.ptp(res["peak_rng_win"].mean(axis=0))
        # how much of the frame-mean window phase the measured velocity explains
        dfr = circ_mean(res["dphi"][:, :, 1:], axis=1)
        resid = np.degrees(circ_std(dfr - res["dphi_motion"][:, 1:], axis=0))
        std_fr = np.degrees(circ_std(dfr, axis=0))
        for k in range(1, K):
            print(f"  window {k} phase std over frames {std_fr[k - 1]:5.1f} deg, "
                  f"after removing the pulse-pair motion {resid[k - 1]:5.1f} deg")
        print("== hint ==")
        if spread > 0.1 * win_res:
            print(f"  the windows put the peak at different ranges (spread {spread * 100:.1f} cm): "
                  "more than one target within ~one window resolution cell. The window phase "
                  "then shows the scene, not the chip: keep --align none, or use --align calib "
                  "with a calibration from a single reflector.")
        elif (std > 20).any() and (resid < 0.5 * std_fr).all():
            print("  window phase follows the pulse-pair velocity: the target moves between the "
                  "chirps of one loop. Use --align peak.")
        elif (std > 20).any():
            print("  window phase scatters and the velocity does not explain it: low SNR, or "
                  "motion faster than the loop rate. Check the SNR; --align peak may help.")
        elif (mean > 20).any() and args.align == "none":
            print("  constant window phase offset with a single target: a chip offset. Save it "
                  "with --calib-out (on a metal plate / corner reflector capture) and use "
                  "--align calib, or --align fixed for this capture.")
        else:
            print("  windows agree: plain stitching is fine.")


def write_calib(res, path):
    np.savez(path, offsets_us=res["sg"]["offs_us"], dphi_rad=res["fixed"],
             gain=res["gain_meas"], frames=res["nf"], cfg=os.path.abspath(res["args"].cfg),
             capture=os.path.abspath(res["args"].bin))
    print(f"calib written: {path}  (phase "
          f"{np.degrees(res['fixed']).round(1)} deg, gain {res['gain_meas'].round(3)})")


def write_csv(res, path):
    sg, K = res["sg"], res["sg"]["K"]
    cols = ["time_s", "frame", "peak_range_m", "peak_db", "noise_db", "snr_db", "coherence"]
    cols += [f"peak_range_w{k}_m" for k in range(K)]
    cols += [f"dphi_w{k}_deg" for k in range(1, K)] + [f"dphi_w{k}_motion_deg" for k in range(1, K)]
    cols += [f"gain_w{k}_db" for k in range(1, K)]
    cols += ["disp_mm", "vel_pp_mps", "clipped", "zero_chirps"]
    data = np.column_stack([res["t"], np.arange(res["f0"], res["f1"]), res["peak_rng"],
                            res["peak_db"], res["noise_db"], res["snr_db"], res["coh"],
                            res["peak_rng_win"],
                            np.degrees(circ_mean(res["dphi"][:, :, 1:], axis=1)),
                            np.degrees(res["dphi_motion"][:, 1:]),
                            20 * np.log10(res["amp"][:, 1:] + 1e-12),
                            res["disp_frame"], res["vel_pp"], res["clipped"], res["zero_chirps"]])
    np.savetxt(path, data, delimiter=",", header=",".join(cols), comments="", fmt="%.6g")
    print(f"csv written: {path}")


def save_npz(res, path):
    np.savez_compressed(path, time_s=res["t"], range_m=res["r_axis"],
                        profile_db=res["prof_db"], profile_window_db=res["prof_win_db"],
                        dphi_rad=res["dphi"], amp_ratio=res["amp"], coherence=res["coh"],
                        peak_range_m=res["peak_rng"], peak_range_window_m=res["peak_rng_win"],
                        disp_mm=res["disp_mm"], t_loops=res["t_loops"],
                        offsets_us=res["sg"]["offs_us"], align=res["args"].align)
    print(f"npz written: {path}")


def plot(res, png_dir):
    import matplotlib
    if png_dir:
        matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    g, sg, args = res["g"], res["sg"], res["args"]
    K, N = sg["K"], sg["N"]
    t, r = res["t"], res["r_axis"]
    tf = res["t_frame"]
    ext_t = [t[0], t[-1] + tf]
    wc = [f"C{k}" for k in range(K)]
    gate_lo, gate_hi = r[res["g0"]], r[res["g1"]]
    figs = []

    # ---- 1: stitching ----
    fig = plt.figure(figsize=(13, 15), layout="constrained")
    fig.suptitle(f"Stitching: {K} ADC windows of {N / sg['fs'] * 1e6:.1f} us, "
                 f"align = {args.align}")
    gs = fig.add_gridspec(4, 2, height_ratios=[1.3, 0.9, 1.5, 1.1])
    ramp_t = lambda k: (sg["adc0"] + sg["t_k"][k] + np.arange(N) / sg["fs"]) * 1e6  # noqa: E731
    a = fig.add_subplot(gs[0, :])
    tr = res["trace"]
    for k in range(K):
        for lp in range(1, tr.shape[0]):
            a.plot(ramp_t(k), tr[lp, k], color=wc[k], lw=0.4, alpha=0.25)
        a.plot(ramp_t(k), tr[0, k], color=wc[k], lw=0.8,
               label=f"window {k} (+{sg['offs_us'][k]:g} us)")
    for k in range(1, K):
        a.axvline((sg["adc0"] + sg["t_k"][k]) * 1e6, color="k", ls="--", lw=0.8)
    a.set(xlabel="time since ramp start [us]", ylabel="ADC code (DC removed)",
          title=f"Raw ADC (before alignment) on the ramp time axis: frame {res['rd_frame']}, "
                f"{sg['tx_labels'][0]}, Rx0 (loop 0 bold, other loops faint)")
    sec = a.secondary_xaxis("top", functions=(
        lambda u: (sg["f0"] + sg["slope"] * u * 1e-6) / 1e9,
        lambda f: (f * 1e9 - sg["f0"]) / sg["slope"] * 1e6))
    sec.set_xlabel("RF [GHz]")
    a.legend(loc="upper right", fontsize=8, ncol=K)
    a.grid(True, alpha=0.3)

    if K > 1:
        sub = gs[1, :].subgridspec(1, K - 1)
        nz = max(8, N // 20)
        for k in range(1, K):
            a = fig.add_subplot(sub[0, k - 1])
            tk, tp = ramp_t(k), ramp_t(k - 1)
            a.plot(tp[-nz:], tr[0, k - 1, -nz:], ".-", color=wc[k - 1], lw=0.8, ms=3)
            a.plot(tk[:nz], tr[0, k, :nz], ".-", color=wc[k], lw=0.8, ms=3)
            a.axvline(tk[0], color="k", ls="--", lw=0.8)
            a.set(title=f"seam {k - 1}|{k}", xlabel="us")
            a.grid(True, alpha=0.3)

    a = fig.add_subplot(gs[2, 0])
    lo = max(0, res["g0"] - int(0.3 * (res["g1"] - res["g0"])))
    hi = min(len(r) - 1, res["g1"] + int(0.3 * (res["g1"] - res["g0"])))
    sl = slice(lo, hi + 1)
    pw_mean = res["prof_win_db"].mean(axis=0)
    w = res["widths"]
    for k in range(K):
        a.plot(r[sl], pw_mean[k, sl], color=wc[k], lw=0.7, alpha=0.8, label=f"window {k}")
    inc = 10 * np.log10((10 ** (res["prof_win_db"] / 10)).mean(axis=(0, 1)) + 1e-12)
    a.plot(r[sl], inc[sl], ":", color="0.4", lw=1.0,
           label=f"incoherent ({w['incoherent'] * 100:.1f} cm)")
    styles = {"none": ("k", "-"), "calib": ("C3", (0, (5, 1))), "fixed": ("C4", "--"),
              "peak": ("C5", "-.")}
    for v in res["prof_var"]:
        a.plot(r[sl], res["prof_var"][v][sl], color=styles[v][0], ls=styles[v][1],
               lw=1.6 if v == args.align else 0.8,
               label=f"stitched {v} ({w['stitched ' + v] * 100:.1f} cm)")
    a.axvspan(gate_lo, gate_hi, color="0.5", alpha=0.08)
    a.set(xlabel="range [m]", ylabel="dB (tone-normalised)",
          title=f"Mean range profile (-3 dB width), window 0 {w['window 0'] * 100:.1f} cm")
    a.legend(fontsize=7)
    a.grid(True, alpha=0.3)

    a = fig.add_subplot(gs[2, 1])
    for k in range(1, K):
        d = np.degrees(res["dphi"][:, :, k])
        a.plot(np.repeat(t, d.shape[1]), d.ravel(), ".", color=wc[k], ms=1.5, alpha=0.4)
        a.plot(t, np.degrees(circ_mean(res["dphi"][:, :, k], axis=1)), color=wc[k], lw=0.9,
               label=f"window {k} vs 0 (mean {np.degrees(res['fixed'][k]):.1f} deg)")
        a.plot(t, np.degrees(res["dphi_motion"][:, k]), color=wc[k], ls=":", lw=1.0,
               label=f"window {k} from pulse-pair velocity")
    a.set(xlabel="time [s]", ylabel="phase [deg]", ylim=(-180, 180), yticks=range(-180, 181, 60),
          title="Window phase at the target frequency (dots per loop, line = frame mean)")
    a.legend(fontsize=8)
    a.grid(True, alpha=0.3)

    a = fig.add_subplot(gs[3, 0])
    for k in range(1, K):
        a.plot(t, 20 * np.log10(res["amp"][:, k] + 1e-12), color=wc[k], lw=0.8,
               label=f"gain window {k} / 0")
    lo, hi = a.get_ylim()
    a.set(xlabel="time [s]", ylabel="dB", title="Window gain and coherence",
          ylim=(min(lo, -1.0), max(hi, 1.0)))
    a2 = a.twinx()
    a2.plot(t, res["coh"], "k", lw=0.7, alpha=0.7, label="coherence eta")
    a2.set(ylabel="eta", ylim=(0, 1.05))
    h1, l1 = a.get_legend_handles_labels()
    h2, l2 = a2.get_legend_handles_labels()
    a.legend(h1 + h2, l1 + l2, fontsize=8, loc="lower left")
    a.grid(True, alpha=0.3)

    a = fig.add_subplot(gs[3, 1])
    R = g["num_rx"]
    bw = 0.8 / K
    dcm = res["dc_win"].mean(axis=0)
    rmm = res["rms_win"].mean(axis=0)
    for k in range(K):
        a.bar(np.arange(R) + (k - (K - 1) / 2) * bw, dcm[k], bw, color=wc[k], label=f"DC w{k}")
    a.set(xticks=range(R), xticklabels=[f"Rx{i}" for i in range(R)], ylabel="ADC mean",
          title="DC (bars) and RMS (circles) per window")
    a2 = a.twinx()
    for k in range(K):
        a2.plot(np.arange(R) + (k - (K - 1) / 2) * bw, rmm[k], "o", color=wc[k], mfc="w")
    a2.set_ylabel("ADC rms")
    a.legend(fontsize=7, ncol=K, loc="upper center", bbox_to_anchor=(0.5, -0.08))
    a.grid(True, alpha=0.3, axis="y")
    figs.append(("1_stitching", fig))

    # ---- 2: over time ----
    fig, ax = plt.subplots(5, 1, figsize=(12, 13), sharex=True, layout="constrained",
                           gridspec_kw={"height_ratios": [2.2, 1, 1, 1, 1]})
    fig.suptitle(f"Over time (stitched, align = {args.align})")
    im = ax[0].imshow(res["prof_db"].T, aspect="auto", origin="lower",
                      extent=ext_t + [r[0], r[-1]], interpolation="nearest")
    ax[0].plot(t + tf / 2, res["peak_rng"], "w.", ms=2, label="tracked peak")
    for v in (gate_lo, gate_hi):
        ax[0].axhline(v, color="w", ls=":", lw=0.8)
    ax[0].set(ylabel="range [m]", title="Stitched range profile (power over loops, Tx, Rx)")
    ax[0].legend(loc="upper right", fontsize=8)
    fig.colorbar(im, ax=ax[0], pad=0.01, label="dB")
    for k in range(K):
        ax[1].plot(t, res["peak_rng_win"][:, k], color=wc[k], lw=0.5, alpha=0.7,
                   label=f"window {k} alone")
    ax[1].plot(t, res["peak_rng"], "k", lw=0.9, label="stitched")
    ax[1].set(ylabel="peak range [m]")
    ax[1].legend(fontsize=7, ncol=K + 1, loc="upper right")
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
                    f"channels combined, lambda {res['lam'] * 1e3:.3f} mm")
    ax[4].plot(t, res["vel_pp"] * 1e3, lw=0.6)
    ax[4].set(ylabel="velocity [mm/s]", xlabel="time [s]")
    for a in ax[1:]:
        a.grid(True, alpha=0.3)
    figs.append(("2_over_time", fig))

    # ---- 3: Doppler and health ----
    fig = plt.figure(figsize=(13, 12), layout="constrained")
    fig.suptitle("Doppler and data health (stitched)")
    gs = fig.add_gridspec(3, 2)
    v = res["v_axis"]
    a = fig.add_subplot(gs[0, :])
    im = a.imshow(res["dop_db"].T, aspect="auto", origin="lower",
                  extent=ext_t + [v[0], v[-1]], interpolation="nearest")
    a.set(xlabel="time [s]", ylabel="velocity [m/s]",
          title=f"Velocity spectrum at bin {res['k_ph']} per frame ({g['num_loops']} loops)")
    fig.colorbar(im, ax=a, pad=0.01, label="dB")
    a = fig.add_subplot(gs[1, 0])
    if res["rd_map"] is not None:
        im = a.imshow(res["rd_map"], aspect="auto", origin="lower",
                      extent=[r[0], r[-1], v[0], v[-1]], interpolation="nearest")
        a.axvline(res["k_ph"] * res["bin_m"], color="w", ls=":", lw=0.8)
        a.set(xlabel="range [m]", ylabel="velocity [m/s]",
              title=f"Range-Doppler map, frame {res['rd_frame']}")
        fig.colorbar(im, ax=a, pad=0.01, label="dB")
    a = fig.add_subplot(gs[1, 1])
    for rx in range(R):
        a.plot(r, res["prof_rx_db"][rx], lw=0.8, label=f"Rx{rx}")
    a.axvspan(gate_lo, gate_hi, color="0.5", alpha=0.08)
    a.set(xlabel="range [m]", ylabel="dB", title="Mean stitched range profile per Rx")
    a.legend(fontsize=8)
    a.grid(True, alpha=0.3)
    a = fig.add_subplot(gs[2, 0])
    ch = res["ch_pow_db"] - res["ch_pow_db"].max()
    im = a.imshow(ch, cmap="viridis", aspect="auto", vmin=min(ch.min(), -1.0), vmax=0)
    for (i, j), val in np.ndenumerate(ch):
        a.text(j, i, f"{val:.1f}", ha="center", va="center", color="w", fontsize=8)
    a.set(xticks=range(R), xticklabels=[f"Rx{i}" for i in range(R)],
          yticks=range(sg["T"]), yticklabels=sg["tx_labels"],
          title=f"Power at bin {res['k_ph']} per channel [dB rel.]")
    fig.colorbar(im, ax=a)
    a = fig.add_subplot(gs[2, 1])
    for rx in range(R):
        a.plot(t, res["rms"][:, rx], lw=0.7, label=f"Rx{rx} rms")
    a.set(xlabel="time [s]", ylabel="ADC rms", title="ADC RMS per Rx, clipping and loss")
    a2 = a.twinx()
    a2.plot(t, res["clipped"], "k", lw=0.6, alpha=0.6, label="clipped")
    a2.plot(t, res["zero_chirps"], "r", lw=0.6, alpha=0.6, label="zero chirps")
    a2.set_ylabel("count / frame")
    h1, l1 = a.get_legend_handles_labels()
    h2, l2 = a2.get_legend_handles_labels()
    a.legend(h1 + h2, l1 + l2, fontsize=7, ncol=3)
    a.grid(True, alpha=0.3)
    figs.append(("3_doppler_health", fig))

    if png_dir:
        os.makedirs(png_dir, exist_ok=True)
        for name, f in figs:
            path = os.path.join(png_dir, f"{name}.png")
            f.savefig(path, dpi=110)
            print(f"png written: {path}")
    else:
        plt.show()


def synth(args):
    """fake capture on the ramp time axis: targets, motion, window phase offsets"""
    g = parse_cfg(args.cfg)
    sg = step_geometry(args.cfg, g)
    K, T, N = sg["K"], sg["T"], sg["N"]
    L, P, R = g["num_loops"], g["chirps_per_loop"], g["num_rx"]
    fs, S, f0 = sg["fs"], sg["slope"], sg["f0"]
    if max(args.range) >= fs / 2 * C / (2 * S):
        raise SystemExit("error: target beyond R_max")
    win_ph = np.radians(np.resize(np.array(args.win_phase or [0.0], float), K))
    rng = np.random.default_rng(args.seed)
    ch_ph = rng.uniform(0, 2 * np.pi, (T, R))
    nfr = args.frames
    t_frame = g["frame_ms"] * 1e-3
    pos = {int(sg["idx"][k, t]): (k, t) for k in range(K) for t in range(T)}
    tn = np.arange(N) / fs
    data = rng.normal(0, args.noise, (nfr, L, P, R, N))
    data += rng.normal(0, 20, R)[None, None, None, :, None]          # DC per Rx
    for q, r0 in enumerate(args.range):
        amp = 2000.0 / (1 + q)
        for p in range(P):
            k, tx = pos[p]
            T_abs = (np.arange(nfr)[:, None] * t_frame + np.arange(L)[None, :] * sg["t_loop"]
                     + sg["t_chirp"][p])
            rr = r0 + args.vib_mm * 1e-3 * np.sin(2 * np.pi * args.vib_hz * T_abs)
            tau = (2 * rr / C)[:, :, None, None]
            t_r = sg["adc0"] + sg["t_k"][k] + tn
            ph = 2 * np.pi * (f0 * tau + S * tau * t_r) + ch_ph[tx][None, None, :, None] + win_ph[k]
            data[:, :, p] += amp * np.cos(ph)
    out = np.clip(np.round(data), -32768, 32767).astype("<i2")
    out.tofile(args.synth)
    print(f"wrote {args.synth}: {nfr} frames, targets {args.range} m, "
          f"vib {args.vib_mm} mm @ {args.vib_hz} Hz, window phase {np.degrees(win_ph).round(1)} deg")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cfg", help="radar .cfg sent to the board for this capture")
    ap.add_argument("bin", nargs="?", help="<prefix>_Raw_0.bin")
    ap.add_argument("--gate", type=float, nargs=2, metavar=("MIN", "MAX"))
    ap.add_argument("--rmax", type=float, help="max range shown [m]")
    ap.add_argument("--align", choices=("none", "calib", "fixed", "peak"), default="none")
    ap.add_argument("--calib", help="window calibration .npz for --align calib")
    ap.add_argument("--calib-out", help="write this capture's window phase/gain to .npz")
    ap.add_argument("--equalize", action="store_true")
    ap.add_argument("--dc", choices=("stitched", "window"), default="stitched")
    ap.add_argument("--bin", dest="bin_k", type=int, help="phase/Doppler range bin")
    ap.add_argument("--frame", type=int, help="frame for the ADC trace and RD map")
    ap.add_argument("--nfft", type=int, help="range FFT size")
    ap.add_argument("--t0", type=float, help="start time [s]")
    ap.add_argument("--t1", type=float, help="end time [s]")
    ap.add_argument("--chunk", type=int, default=10, help="frames per chunk")
    ap.add_argument("--csv", help="write per-frame results to this CSV")
    ap.add_argument("--save", help="write profiles and window phases to this .npz")
    ap.add_argument("--png", help="save figures as PNG into this directory")
    ap.add_argument("--no-plot", action="store_true")
    ap.add_argument("--synth", help="write a synthetic capture to this path")
    ap.add_argument("--range", type=float, nargs="+", default=[0.8], help="synth targets [m]")
    ap.add_argument("--frames", type=int, default=100, help="synth frame count")
    ap.add_argument("--vib-mm", type=float, default=0.0, help="synth motion amplitude [mm]")
    ap.add_argument("--vib-hz", type=float, default=5.0, help="synth motion frequency [Hz]")
    ap.add_argument("--win-phase", type=float, nargs="+", help="synth phase per window [deg]")
    ap.add_argument("--noise", type=float, default=50.0, help="synth noise rms [ADC codes]")
    ap.add_argument("--seed", type=int, default=0)
    args = ap.parse_args()

    if args.synth:
        synth(args)
        return
    if not args.bin:
        ap.error("bin file required (or --synth)")
    res = analyse(args)
    summary(res)
    if args.csv:
        write_csv(res, args.csv)
    if args.save:
        save_npz(res, args.save)
    if args.calib_out:
        write_calib(res, args.calib_out)
    if not args.no_plot:
        plot(res, args.png)


if __name__ == "__main__":
    main()
