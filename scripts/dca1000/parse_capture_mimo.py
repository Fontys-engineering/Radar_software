#!/usr/bin/env python3
"""
parse_capture_mimo.py - MIMO decode + angle (azimuth/elevation) of a DCA1000 capture

  parse_capture_mimo.py <radar.cfg> <prefix_Raw_0.bin> [options]
  parse_capture_mimo.py <radar.cfg> --synth test_Raw_0.bin [--target R AZ ...]

Works for TDM cfgs (one Tx per chirp, e.g. differ_shortrange_hires_1.cfg,
plasma_capture_4tx.cfg) and BPM-coded cfgs (all Tx in every chirp with a
+/-180 deg code from bpmCfgAdvanced, e.g. differ_shortrange_bpm_1.cfg).
Same input layout as parse_capture.py (Doc/10).

Three figure windows (or PNGs with --png):
  1 Angle       range-azimuth map (mean over the capture) in range/angle and
                in x/y, azimuth spectrum at the gate peak (beamformer and
                Capon), phase over the virtual array at the peak
  2 Over time   azimuth-time map at the peak range, peak azimuth and lateral
                offset, elevation, peak power / SNR, velocity
  3 Health      power per decoded Tx and per virtual channel, channel SNR,
                the code matrix

Methods:
  code        chirp c of a loop sends y_c = sum_tx C[c,tx] x_tx, with
              C[c,tx] = 0 if Tx is off in the chirpCfg txMask, else +1 or -1
              (bpmCfgAdvanced constBpmVal bit 2*tx+1 set). TDM: C selects one
              Tx per chirp. Decoding: x = pinv(C) y per loop, Rx and range bin.
              A Hadamard code gives every decoded channel the energy of all
              chirps: noise / N_chirps -> +10 log10(N) dB SNR vs TDM.
  virtual     antGeometryCfg (row, col) per Tx*4+Rx, x/z spacing in lambda.
              The row with the most channels is the azimuth row (stock cfg:
              TX0/TX2/TX3 -> 12 channels, 0.5 lambda); the other row is the
              elevation row (TX1, raised by zSpacing).
  clutter     --clutter mean subtracts the mean complex range spectrum per
              chirp/Rx over the whole capture (2 passes): removes everything
              static (window, flange, back wall) and keeps what changes.
  dopcomp     a target moving between the chirps of one loop adds a phase
              psi per chirp, which leaks between decoded Tx (BPM) or tilts
              the array phase (TDM). --dopcomp estimates the velocity per frame
              (pulse-pair over loops at the gate peak) and removes c * psi
              from chirp c before decoding.
  calib       --calib-out: one reflector (plate / corner reflector) at
              --calib-az (default 0 = boresight) inside the gate. Stores the
              complex gain per Tx/Rx relative to the first azimuth channel,
              after removing the ideal steering phase. --calib applies it.
              The chip/board Tx phases do not depend on the code, so a TDM
              calibration also applies to BPM captures of the same cfg
              profile (and vice versa).
  angle       Bartlett beamformer P(az) = |a(az)^H x|^2 over the azimuth
              channels on a 0.5 deg grid, a_k = exp(j 2 pi x_k sin(az)).
              --capon: MVDR P = 1 / (a^H R^-1 a), R over loops (and frames
              for the mean map) with 5 % diagonal loading.
              Elevation: phase between the elevation channels and the
              azimuth channels in the same columns, sin(el) = dphi /
              (2 pi zSpacing). Unambiguous for |el| < asin(1/(2 zSpacing)).
              The sign of az/el depends on the board orientation: check it
              with a reflector to one side (--flip-az / --flip-el).
  peak        per frame: strongest (range, az) cell inside the gate on the
              beamformer map, parabolic refinement in angle.
              lateral = R sin(az).
  SNR         channel SNR: |mean over loops|^2 at the peak bin over the
              median (over gate bins) of the variance over loops. Static
              targets cancel in the variance, so it is the noise per decoded
              virtual channel: compare a TDM and a BPM capture with it.

Options:
  --gate MIN MAX   range gate in m (default: plasmaCfg gate from the cfg)
  --fov DEG        azimuth grid +/- DEG (default 70)
  --clutter MODE   none | mean (default none)
  --dopcomp        Doppler-compensate the chirps of a loop before decoding
  --capon          also compute Capon/MVDR spectra
  --calib FILE     apply a channel calibration
  --calib-out FILE store a channel calibration from this capture
  --calib-az DEG   azimuth of the calibration reflector (default 0)
  --flip-az        mirror the azimuth axis
  --flip-el        mirror the elevation axis
  --nfft N         range FFT size (default 2 x pow2(numSamples))
  --t0/--t1 S      analyse only this time window
  --chunk N        frames per chunk (default 200)
  --csv FILE       per-frame results as CSV
  --save FILE.npz  maps and per-frame results
  --png DIR        save the figures as PNG instead of showing them
  --no-plot        print the summary only

--synth writes a fake capture for the cfg: point targets with --target
R AZ [EL] [AMP_DB] (several allowed), optional --vel m/s, per-Tx phase
offsets --tx-phase (deg, 4 values) and --noise, to test decoding, the
calibration and the angle estimation without hardware.
"""

import argparse
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from parse_capture import C, parse_cfg, range_axis  # noqa: E402
from parse_capture_adv import RawReader, chirp_tx_labels, parabolic  # noqa: E402

try:
    from scipy import fft as _fft

    def rfft(x, n):
        return _fft.rfft(x, n=n, axis=-1, workers=-1)
except ImportError:
    def rfft(x, n):
        return np.fft.rfft(x, n=n, axis=-1)

# stock AWR2944EVM geometry (row, col) for virtual channel tx*4+rx
STOCK_GEOM = ("1 0 1 1 1 2 1 3 0 2 0 3 0 4 0 5 1 4 1 5 1 6 1 7 1 8 1 9 1 10 1 11 "
              "0.5 0.8").split()


def cfg_commands(path):
    cmds = {}
    for line in open(path):
        w = line.split("%", 1)[0].split()
        if w:
            cmds.setdefault(w[0], []).append(w[1:])
    return cmds


def mimo_geometry(cfg_path, g):
    """Code matrix C[chirp, tx] and virtual array positions from the cfg."""
    cmds = cfg_commands(cfg_path)
    n_ch = g["chirps_per_loop"]
    masks, bpm = {}, {}
    for w in cmds.get("chirpCfg", []):
        for i in range(int(w[0]), int(w[1]) + 1):
            masks[i] = int(w[7])
    for w in cmds.get("bpmCfgAdvanced", []):
        for i in range(int(w[0]), int(w[1]) + 1):
            bpm[i] = int(w[2])

    chirps = range(g["chirp_start"], g["chirp_end"] + 1)
    all_mask = 0
    for i in chirps:
        all_mask |= masks.get(i, 0)
    txs = [t for t in range(4) if all_mask >> t & 1]
    code = np.zeros((n_ch, len(txs)))
    for c, i in enumerate(chirps):
        for k, t in enumerate(txs):
            if masks.get(i, 0) >> t & 1:
                code[c, k] = -1.0 if bpm.get(i, 0) >> (2 * t + 1) & 1 else 1.0
    dec = np.linalg.pinv(code)

    geo = cmds.get("antGeometryCfg", [STOCK_GEOM])[-1]
    rows = np.array([int(geo[2 * v]) for v in range(16)]).reshape(4, 4)
    cols = np.array([int(geo[2 * v + 1]) for v in range(16)]).reshape(4, 4)
    xsp, zsp = float(geo[32]), float(geo[33])
    nrx = g["num_rx"]
    vrow = rows[txs, :nrx]
    vcol = cols[txs, :nrx]
    vals, counts = np.unique(vrow, return_counts=True)
    az_row = vals[np.argmax(counts)]

    az = [(vcol[k, r], k, r) for k in range(len(txs)) for r in range(nrx)
          if vrow[k, r] == az_row]
    az.sort()
    az_idx = [(k, r) for _, k, r in az]
    az_x = np.array([c for c, _, _ in az], float) * xsp
    # elevation channels with an azimuth channel in the same column
    az_by_col = {c: (k, r) for c, k, r in az}
    el_pairs = [((k, r), az_by_col[vcol[k, r]]) for k in range(len(txs)) for r in range(nrx)
                if vrow[k, r] != az_row and vcol[k, r] in az_by_col]
    return {
        "txs": txs, "code": code, "dec": dec,
        "cond": np.linalg.cond(code),
        "rank": int(np.linalg.matrix_rank(code)),
        "is_bpm": bool(np.any(code < 0) or np.any((code != 0).sum(axis=1) > 1)),
        "rows": vrow, "cols": vcol, "xsp": xsp, "zsp": zsp, "az_row": az_row,
        "az_idx": az_idx, "az_x": az_x, "el_pairs": el_pairs,
        "tx_labels": chirp_tx_labels(cfg_path, g),
    }


def gate_from_cfg(cfg_path):
    pc = cfg_commands(cfg_path).get("plasmaCfg")
    return (float(pc[-1][1]), float(pc[-1][2])) if pc else None


def steering(az_x, az_deg):
    """a[angle, element] = exp(j 2 pi x sin(az))"""
    return np.exp(2j * np.pi * np.outer(np.sin(np.radians(az_deg)), az_x))


def capon(R, A, load=0.05):
    """MVDR spectrum for covariance R[..., n, n] and steering A[angle, n]"""
    n = R.shape[-1]
    tr = np.real(np.trace(R, axis1=-2, axis2=-1))[..., None, None]
    Ri = np.linalg.inv(R + load * tr / n * np.eye(n))
    q = np.einsum("an,...nm,am->...a", A.conj(), Ri, A)
    return 1.0 / np.maximum(np.real(q), 1e-30)


def spectra(raw, g, nfft):
    """int16 frames -> range spectrum [n][loop][chirp][rx][bin]"""
    x = raw.reshape((-1,) + g["frame_shape"]).astype(np.float32)
    x -= x.mean(axis=-1, keepdims=True)
    x *= np.hanning(g["num_samples"]).astype(np.float32)
    return rfft(x, nfft).astype(np.complex64)


def analyse(args):
    g = parse_cfg(args.cfg)
    m = mimo_geometry(args.cfg, g)
    if m["rank"] < len(m["txs"]):
        raise SystemExit(f"error: code matrix has rank {m['rank']} < {len(m['txs'])} Tx,"
                         " the Tx channels cannot be separated")
    rd = RawReader(args.bin, g["frame_bytes"])
    if rd.num_frames == 0:
        raise SystemExit("error: less than one frame in the capture")

    nfft = args.nfft or 2 * int(2 ** np.ceil(np.log2(g["num_samples"])))
    r_axis = range_axis(g, nfft)
    nb = r_axis.size
    tc = (g["idle_us"] + g["ramp_end_us"]) * 1e-6
    t_loop = g["chirps_per_loop"] * tc
    t_frame = g["frame_ms"] * 1e-3
    f_center = (g["start_ghz"] * 1e9 + g["slope_mhz_us"] * 1e12 *
                (g["adc_start_us"] * 1e-6 + g["num_samples"] / (2 * g["fs_ksps"] * 1e3)))
    lam = C / f_center

    gate = args.gate or gate_from_cfg(args.cfg) or (r_axis[3], r_axis[-1])
    gsel = np.flatnonzero((r_axis >= gate[0]) & (r_axis <= gate[1]))
    gsel = gsel[gsel >= 2]
    if gsel.size < 3:
        raise SystemExit(f"error: gate {gate} has fewer than 3 range bins")

    az_deg = np.arange(-args.fov, args.fov + 1e-9, 0.5)
    A = steering(m["az_x"], az_deg)
    AH = np.ascontiguousarray(A.conj().T).astype(np.complex64)
    # range-azimuth map only up to the plotted range (1.3 x gate end)
    nm = min(nb, int(np.searchsorted(r_axis, gate[1] * 1.3)) + 1)
    az_sign = -1.0 if args.flip_az else 1.0
    el_sign = -1.0 if args.flip_el else 1.0

    calib = None
    if args.calib:
        cz = np.load(args.calib)
        calib = cz["gain"]
        if calib.shape != (len(m["txs"]), g["num_rx"]):
            raise SystemExit(f"error: calibration shape {calib.shape} does not match "
                             f"{len(m['txs'])} Tx x {g['num_rx']} Rx")

    f0 = int(args.t0 / t_frame) if args.t0 else 0
    f1 = min(rd.num_frames, int(np.ceil(args.t1 / t_frame))) if args.t1 else rd.num_frames
    if f0 >= f1:
        raise SystemExit("error: empty time window")
    nf = f1 - f0
    chunk = max(1, args.chunk)

    clutter = None
    if args.clutter == "mean":
        acc = np.zeros(g["frame_shape"][1:3] + (nb,), np.complex128)
        for a in range(f0, f1, chunk):
            b = min(f1, a + chunk)
            acc += spectra(rd.frames(a, b), g, nfft).sum(axis=(0, 1))
        clutter = (acc / (nf * g["num_loops"])).astype(np.complex64)

    n_tx, nrx = len(m["txs"]), g["num_rx"]
    ai = tuple(np.array(m["az_idx"]).T)
    ra_pow = np.zeros((nb, az_deg.size))             # mean range-azimuth map
    R_acc = np.zeros((gsel.size, len(m["az_idx"]), len(m["az_idx"])), np.complex128)
    tx_pow = np.zeros((n_tx, nrx, nb))               # mean |x|^2 per channel
    ch_mean = np.zeros((n_tx, nrx, nb), np.complex128)
    ch_var = np.zeros((n_tx, nrx, nb))
    per = {k: np.full(nf, np.nan) for k in
           ("range", "az", "el", "lateral", "power_db", "snr_db", "vel")}
    az_time = np.zeros((nf, az_deg.size))

    for a in range(f0, f1, chunk):
        b = min(f1, a + chunk)
        Y = spectra(rd.frames(a, b), g, nfft)        # [n, loop, chirp, rx, bin]
        if clutter is not None:
            Y -= clutter
        n = Y.shape[0]
        sl = slice(a - f0, b - f0)

        # pulse-pair velocity per frame at the strongest gate bin
        pg = (np.abs(Y[..., gsel]) ** 2).sum(axis=(1, 2, 3))
        kb = gsel[np.argmax(pg, axis=1)]
        yk = Y[np.arange(n), :, :, :, kb]            # [n, loop, chirp, rx]
        pp = (yk[:, 1:] * yk[:, :-1].conj()).sum(axis=(1, 2, 3))
        dphi_loop = np.angle(pp)
        per["vel"][sl] = lam * dphi_loop / (4 * np.pi * t_loop)
        if args.dopcomp:
            psi = dphi_loop * tc / t_loop            # phase step per chirp
            c = np.arange(g["chirps_per_loop"])
            Y *= np.exp(-1j * psi[:, None] * c[None, :]).astype(np.complex64)[
                :, None, :, None, None]

        X = np.einsum("tc,nlcrb->nltrb", m["dec"].astype(np.complex64), Y)
        if calib is not None:
            X /= calib[None, None, :, :, None].astype(np.complex64)

        tx_pow += (np.abs(X) ** 2).mean(axis=1).sum(axis=0)
        mu = X.mean(axis=1)                          # [n, tx, rx, bin]
        ch_mean += mu.sum(axis=0)
        ch_var += (np.abs(X - mu[:, None]) ** 2).mean(axis=1).sum(axis=0)

        Xa = X[:, :, ai[0], ai[1], :nm]              # [n, loop, az, bin]
        B = np.moveaxis(Xa, 2, -1) @ AH              # [n, loop, bin, angle]
        P = (np.abs(B) ** 2).mean(axis=1).transpose(0, 2, 1)   # [n, angle, bin]
        ra_pow[:nm] += P.sum(axis=0).T
        Xg = Xa[..., gsel]                           # [n, loop, az, gbin]
        R_acc += np.einsum("nlkb,nljb->bkj", Xg, Xg.conj())

        Pg = P[:, :, gsel]                           # [n, angle, gbin]
        flat = Pg.reshape(n, -1).argmax(axis=1)
        ia, ig = np.unravel_index(flat, Pg.shape[1:])
        for j in range(n):
            spec = Pg[j, :, ig[j]]
            frac = parabolic(10 * np.log10(spec + 1e-30), ia[j])
            rprof = Pg[j, ia[j], :]
            rfrac = parabolic(10 * np.log10(rprof + 1e-30), ig[j])
            rng = r_axis[gsel[ig[j]]] + rfrac * r_axis[1]
            azv = az_sign * (az_deg[ia[j]] + frac * 0.5)
            per["range"][a - f0 + j] = rng
            per["az"][a - f0 + j] = azv
            per["lateral"][a - f0 + j] = rng * np.sin(np.radians(azv))
            per["power_db"][a - f0 + j] = 10 * np.log10(spec[ia[j]] + 1e-30)
            per["snr_db"][a - f0 + j] = 10 * np.log10(
                spec[ia[j]] / (np.median(Pg[j]) + 1e-30))
            az_time[a - f0 + j] = P[j, :, gsel[ig[j]]]
            if m["el_pairs"]:
                kb2 = gsel[ig[j]]
                s = 0
                for (ke, re), (kz, rz) in m["el_pairs"]:
                    s = s + (X[j, :, ke, re, kb2] * X[j, :, kz, rz, kb2].conj()).sum()
                se = np.angle(s) / (2 * np.pi * m["zsp"])
                per["el"][a - f0 + j] = el_sign * np.degrees(np.arcsin(np.clip(se, -1, 1)))
        print(f"\rframes {b - f0}/{nf}", end="", file=sys.stderr)
    print(file=sys.stderr)

    ra_pow /= nf
    tx_pow /= nf
    ch_mean /= nf
    ch_var /= nf
    mean_g = ra_pow[gsel]
    ib, ia = np.unravel_index(mean_g.argmax(), mean_g.shape)
    kpk = gsel[ib]
    az_pk = az_sign * az_deg[ia]

    snr_ch = (np.abs(ch_mean[..., kpk]) ** 2 /
              np.maximum(np.median(ch_var[..., gsel], axis=-1), 1e-30))
    res = dict(args=args, g=g, m=m, r_axis=r_axis, gsel=gsel, gate=gate,
               az_deg=az_sign * az_deg, ra_pow=ra_pow, tx_pow=tx_pow,
               kpk=kpk, az_pk=az_pk, per=per, az_time=az_time, lam=lam,
               t=(np.arange(nf) + f0) * t_frame, nf=nf,
               snr_ch_db=10 * np.log10(snr_ch + 1e-30),
               ch_phase=np.angle(ch_mean[..., kpk]))
    if args.capon:
        nsnap = nf * g["num_loops"]
        res["capon_map"] = capon(R_acc / nsnap, A).T  # [gbin, angle] -> [angle, gbin]
    if args.calib_out:
        x_pk = ch_mean[..., kpk]
        ideal = np.ones_like(x_pk)
        sa = np.sin(np.radians(az_sign * args.calib_az))
        for k, r in m["az_idx"]:
            ideal[k, r] = np.exp(2j * np.pi * m["cols"][k, r] * m["xsp"] * sa)
        for (ke, re), (kz, rz) in m["el_pairs"]:
            ideal[ke, re] = ideal[kz, rz]
        gain = x_pk / ideal
        k0, r0 = m["az_idx"][0]
        gain = gain / gain[k0, r0]
        if calib is not None:
            gain = gain * calib
        np.savez(args.calib_out, gain=gain, range_m=r_axis[kpk],
                 calib_az=args.calib_az, cfg=os.path.basename(args.cfg))
        res["calib_out"] = gain
    return res


def summary(res):
    g, m, per = res["g"], res["m"], res["per"]
    print("== cfg ==")
    print(f"chirps/loop : {g['chirps_per_loop']} "
          f"({', '.join(m['tx_labels'])}), {g['num_loops']} loops, {g['num_rx']} Rx")
    mode = "BPM / multi-Tx code" if m["is_bpm"] else "TDM"
    gain = -10 * np.log10((m["dec"] ** 2).sum(axis=1))
    print(f"mode        : {mode}, Tx {m['txs']}, cond(C) = {m['cond']:.2f}, "
          f"noise gain per decoded channel vs TDM {gain.mean():+.1f} dB")
    print("code C      : " + " | ".join(" ".join(f"{v:+.0f}" for v in row)
                                        for row in m["code"]))
    n_az = len(m["az_idx"])
    span = m["az_x"][-1] - m["az_x"][0]
    print(f"virtual     : {n_az} azimuth ch, aperture {span:.1f} lambda "
          f"(~{np.degrees(1 / (span + m['xsp'])):.1f} deg res.), "
          f"{len(m['el_pairs'])} elevation pairs at {m['zsp']} lambda")
    r = res["r_axis"]
    print(f"range       : bin {100 * r[1]:.2f} cm (zero-padded), gate "
          f"{res['gate'][0]:.2f}..{res['gate'][1]:.2f} m, lambda {1e3 * res['lam']:.3f} mm")
    print(f"frames      : {res['nf']}")
    print("== mean map ==")
    print(f"peak        : {r[res['kpk']]:.3f} m, az {res['az_pk']:+.1f} deg, "
          f"lateral {r[res['kpk']] * np.sin(np.radians(res['az_pk'])) * 100:+.1f} cm")
    tp = 10 * np.log10(res["tx_pow"][..., res["kpk"]].mean(axis=1) + 1e-30)
    print("Tx power    : " + ", ".join(f"TX{t} {v - tp.max():+.1f} dB"
                                       for t, v in zip(m["txs"], tp)))
    if m["is_bpm"] and np.min(tp - tp.max()) < -10:
        print("warning: a decoded Tx is >10 dB below the others at the peak. If"
              " the code was not applied (bpmCfgAdvanced missing/ignored), all"
              " energy ends up in the Tx of the all-'+' column.")
    sc = res["snr_ch_db"]
    print(f"channel SNR : mean {sc.mean():.1f} dB (min {sc.min():.1f}, max {sc.max():.1f})"
          " per decoded virtual channel at the peak bin")
    print("== per frame (gate) ==")
    for k, unit in (("range", "m"), ("az", "deg"), ("lateral", "m"), ("el", "deg"),
                    ("snr_db", "dB"), ("vel", "m/s")):
        v = per[k][np.isfinite(per[k])]
        if v.size:
            print(f"{k:12s}: median {np.median(v):+.3f} {unit}, "
                  f"p5..p95 {np.percentile(v, 5):+.3f}..{np.percentile(v, 95):+.3f}")
    if "calib_out" in res:
        gnz = res["calib_out"]
        print("== calibration written: " + res["args"].calib_out)
        for k, t in enumerate(m["txs"]):
            print(f"  TX{t}: " + "  ".join(
                f"{20 * np.log10(abs(gnz[k, rr]) + 1e-30):+5.1f}dB/"
                f"{np.degrees(np.angle(gnz[k, rr])):+6.1f}deg"
                for rr in range(gnz.shape[1])))


def write_csv(res, path):
    per = res["per"]
    keys = ("range", "az", "lateral", "el", "power_db", "snr_db", "vel")
    with open(path, "w") as f:
        f.write("t_s,range_m,az_deg,lateral_m,el_deg,power_db,snr_db,vel_mps\n")
        for i in range(res["nf"]):
            f.write(f"{res['t'][i]:.4f}," +
                    ",".join(f"{per[k][i]:.5f}" for k in keys) + "\n")
    print(f"csv written: {path}")


def plot(res, png_dir):
    import matplotlib
    if png_dir:
        matplotlib.use("Agg")
    import matplotlib.pyplot as plt

    g, m, per = res["g"], res["m"], res["per"]
    r, az, gsel = res["r_axis"], res["az_deg"], res["gsel"]
    rmax = min(r[-1], res["gate"][1] * 1.3)
    rs = r <= rmax
    db = 10 * np.log10(res["ra_pow"][rs] + 1e-30)
    vmax = db.max()
    figs = []

    fig, ax = plt.subplots(2, 2, figsize=(13, 9))
    a = ax[0, 0]
    im = a.pcolormesh(az, r[rs], db, vmin=vmax - 40, vmax=vmax, shading="auto")
    for y in res["gate"]:
        a.axhline(y, color="w", ls=":", lw=0.8)
    a.plot(res["az_pk"], r[res["kpk"]], "r+", ms=12)
    a.set(title="Range-azimuth (beamformer, mean)", xlabel="azimuth [deg]",
          ylabel="range [m]")
    fig.colorbar(im, ax=a, label="dB")
    a = ax[0, 1]
    th = np.radians(az)
    RR, TT = np.meshgrid(r[rs], th, indexing="ij")
    im = a.pcolormesh(RR * np.sin(TT), RR * np.cos(TT), db, vmin=vmax - 40, vmax=vmax,
                      shading="auto")
    a.set(title="Top view", xlabel="lateral x [m]", ylabel="downrange y [m]",
          aspect="equal")
    fig.colorbar(im, ax=a, label="dB")
    a = ax[1, 0]
    spec = res["ra_pow"][res["kpk"]]
    a.plot(az, 10 * np.log10(spec / spec.max() + 1e-30), label="beamformer")
    if "capon_map" in res:
        cp = res["capon_map"][:, np.flatnonzero(gsel == res["kpk"])[0]]
        a.plot(az, 10 * np.log10(cp / cp.max() + 1e-30), label="Capon")
    a.set(title=f"Azimuth spectrum at {r[res['kpk']]:.3f} m", xlabel="azimuth [deg]",
          ylabel="dB", ylim=(-40, 3))
    a.legend()
    a.grid(True, alpha=0.3)
    a = ax[1, 1]
    ph = res["ch_phase"]
    ph_az = np.unwrap([ph[k, rr] for k, rr in m["az_idx"]])
    a.plot(m["az_x"], np.degrees(ph_az), "o-", label="azimuth row")
    for (ke, re), (kz, rz) in m["el_pairs"]:
        x = m["cols"][kz, rz] * m["xsp"]
        d = np.degrees(np.angle(np.exp(1j * (ph[ke, re] - ph[kz, rz]))))
        a.plot(x, np.degrees(ph_az[m["az_idx"].index((kz, rz))]) + d, "s", color="C1")
    a.set(title="Phase over the virtual array at the peak (linear = one target)",
          xlabel="position [lambda]", ylabel="phase [deg]")
    a.grid(True, alpha=0.3)
    fig.tight_layout()
    figs.append(("1_angle", fig))

    t = res["t"]
    fig, ax = plt.subplots(5, 1, figsize=(12, 13), sharex=True)
    adb = 10 * np.log10(res["az_time"] + 1e-30)
    im = ax[0].pcolormesh(t, az, adb.T, vmin=adb.max() - 30, vmax=adb.max(), shading="auto")
    ax[0].set(title="Azimuth vs time at the peak range of each frame", ylabel="az [deg]")
    ax[1].plot(t, per["az"], ".", ms=2, label="azimuth [deg]")
    ax[1].plot(t, per["el"], ".", ms=2, label="elevation [deg]")
    ax[1].set(ylabel="deg")
    ax[1].legend(loc="upper right")
    ax[2].plot(t, 100 * per["lateral"], ".", ms=2)
    ax[2].set(ylabel="lateral [cm]")
    ax3 = ax[3].twinx()
    ax[3].plot(t, per["range"], ".", ms=2, color="C0")
    ax[3].set_ylabel("range [m]", color="C0")
    ax3.plot(t, per["snr_db"], ".", ms=2, color="C3")
    ax3.set_ylabel("SNR [dB]", color="C3")
    ax[4].plot(t, per["vel"] * 1e3, ".", ms=2)
    ax[4].set(ylabel="v [mm/s]", xlabel="time [s]")
    for a in ax[1:]:
        a.grid(True, alpha=0.3)
    fig.tight_layout()
    figs.append(("2_time", fig))

    fig, ax = plt.subplots(1, 3, figsize=(15, 4.5))
    tp = 10 * np.log10(res["tx_pow"].mean(axis=1) + 1e-30)
    for k, tx in enumerate(m["txs"]):
        ax[0].plot(r[rs], tp[k][rs], label=f"TX{tx}", lw=0.9)
    ax[0].set(title="Decoded power per Tx (mean over Rx)", xlabel="range [m]", ylabel="dB")
    ax[0].legend()
    ax[0].grid(True, alpha=0.3)
    im = ax[1].imshow(res["snr_ch_db"], aspect="auto", cmap="viridis")
    ax[1].set(title="Channel SNR at peak bin [dB]",
              yticks=range(len(m["txs"])), yticklabels=[f"TX{t}" for t in m["txs"]],
              xticks=range(g["num_rx"]), xticklabels=[f"RX{i}" for i in range(g["num_rx"])])
    fig.colorbar(im, ax=ax[1])
    im = ax[2].imshow(m["code"], cmap="bwr", vmin=-1, vmax=1)
    ax[2].set(title="Code matrix C (chirp x Tx)",
              xticks=range(len(m["txs"])), xticklabels=[f"TX{t}" for t in m["txs"]],
              ylabel="chirp")
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


def synth(args):
    g = parse_cfg(args.cfg)
    m = mimo_geometry(args.cfg, g)
    rng = np.random.default_rng(args.seed)
    fs = g["fs_ksps"] * 1e3
    slope = g["slope_mhz_us"] * 1e12
    tc = (g["idle_us"] + g["ramp_end_us"]) * 1e-6
    f_center = (g["start_ghz"] * 1e9 + slope *
                (g["adc_start_us"] * 1e-6 + g["num_samples"] / (2 * fs)))
    lam = C / f_center
    nl, nc, nrx, ns = g["frame_shape"]
    t = np.arange(ns) / fs
    tx_ph = np.radians(np.array(args.tx_phase or [0, 0, 0, 0], float))
    targets = args.target or [[1.0, 0.0]]
    data = np.zeros((args.frames,) + g["frame_shape"])
    fr_t = np.arange(args.frames)[:, None, None] * g["frame_ms"] * 1e-3
    chirp_t = fr_t + (np.arange(nl)[None, :, None] * nc + np.arange(nc)[None, None, :]) * tc
    for tg in targets:
        rr, az = tg[0], np.radians(tg[1])
        el = np.radians(tg[2]) if len(tg) > 2 else 0.0
        amp = 4000 * 10 ** ((tg[3] if len(tg) > 3 else 0.0) / 20)
        f_b = 2 * rr * slope / C
        if f_b >= fs / 2:
            raise SystemExit(f"error: {rr} m is beyond R_max {fs / 2 * C / (2 * slope):.2f} m")
        ph_r = 4 * np.pi * (rr + args.vel * chirp_t) / lam      # [frame, loop, chirp]
        for k, tx in enumerate(m["txs"]):
            for rx in range(nrx):
                x = m["cols"][k, rx] * m["xsp"]
                z = (m["az_row"] - m["rows"][k, rx]) * m["zsp"]
                ph_v = 2 * np.pi * (x * np.sin(az) * np.cos(el) + z * np.sin(el)) + tx_ph[tx]
                w = m["code"][:, k]                               # [chirp]
                data[:, :, :, rx, :] += (amp * w[None, None, :, None] * np.cos(
                    2 * np.pi * f_b * t[None, None, None, :] +
                    (ph_r + ph_v)[..., None]))
    data += rng.normal(0, args.noise, data.shape)
    out = np.clip(np.round(data), -32768, 32767).astype("<i2")
    out.tofile(args.synth)
    print(f"wrote {args.synth}: {args.frames} frames, {len(targets)} target(s), "
          f"{'BPM' if m['is_bpm'] else 'TDM'} code, Tx {m['txs']}")


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cfg", help="radar .cfg sent to the board for this capture")
    ap.add_argument("bin", nargs="?", help="<prefix>_Raw_0.bin")
    ap.add_argument("--gate", type=float, nargs=2, metavar=("MIN", "MAX"))
    ap.add_argument("--fov", type=float, default=70.0)
    ap.add_argument("--clutter", choices=("none", "mean"), default="none")
    ap.add_argument("--dopcomp", action="store_true")
    ap.add_argument("--capon", action="store_true")
    ap.add_argument("--calib")
    ap.add_argument("--calib-out")
    ap.add_argument("--calib-az", type=float, default=0.0)
    ap.add_argument("--flip-az", action="store_true")
    ap.add_argument("--flip-el", action="store_true")
    ap.add_argument("--nfft", type=int)
    ap.add_argument("--t0", type=float)
    ap.add_argument("--t1", type=float)
    ap.add_argument("--chunk", type=int, default=200)
    ap.add_argument("--csv")
    ap.add_argument("--save")
    ap.add_argument("--png")
    ap.add_argument("--no-plot", action="store_true")
    ap.add_argument("--synth", help="write a synthetic capture to this path")
    ap.add_argument("--target", type=float, nargs="+", action="append",
                    metavar="R AZ [EL] [AMP_DB]", help="synth target (repeatable)")
    ap.add_argument("--vel", type=float, default=0.0, help="synth radial velocity m/s")
    ap.add_argument("--tx-phase", type=float, nargs=4, help="synth Tx phase offsets deg")
    ap.add_argument("--noise", type=float, default=50.0, help="synth noise std (ADC codes)")
    ap.add_argument("--frames", type=int, default=60, help="synth frame count")
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
        np.savez_compressed(args.save, r_axis=res["r_axis"], az_deg=res["az_deg"],
                            ra_pow=res["ra_pow"], az_time=res["az_time"], t=res["t"],
                            **{f"per_{k}": v for k, v in res["per"].items()})
        print(f"saved      : {args.save}")
    if not args.no_plot:
        plot(res, args.png)


if __name__ == "__main__":
    main()
