#!/usr/bin/env python3
"""
live_plot.py - minimal live viewer for the demo's UART TLV output
(stand-in for the TI mmWave Demo Visualizer)

  scripts/live_plot.py [-p /dev/ttyACM1] [-c scripts/cfg/plasma.cfg]

Reads the 2nd XDS110 port ("Auxiliary Data Port", 3.125 Mbaud) and shows:
  range profile (current frame, + noise profile if enabled), range-time
  waterfall, detected points (x/y), and frame / rate / stats text.
Configure the board as usual with radar_cli.sh, before or after starting this.

What is sent is chosen by guiMonitor in the cfg:
  guiMonitor <sf> <detectedObjects> <logMagRange> <noiseProfile> <azHeat> <rdHeat> <stats>
  e.g. guiMonitor -1 1 1 1 0 0 1  -> points + side info, range + noise profile, stats

Packet (RadarSetup.c MmwDemo_transmitProcessedOutput, include/mmw_output.h):
  header 40 B: magic 02 01 04 03 06 05 08 07, u32 version, totalPacketLen,
               platform, frameNumber, timeCpuCycles, numDetectedObj, numTLVs,
               subFrameNumber
  TLVs: u32 type, u32 length (payload bytes), payload. Packet padded to 32 B.
  1 points     float x, y, z, v  per object
  2 range      u16 per range bin, sum of log2 |X| over Rx, Q9 (zero Doppler)
  3 noise      same format, at the max Doppler bin
  6 stats      u32 interFrameProcTime, transmitOutputTime, interFrameMargin,
               interChirpMargin, activeFrameCPULoad, interFrameCPULoad
  7 side info  i16 snr, i16 noise per object, in 0.1 dB
"""

import argparse
import collections
import os
import struct
import sys
import threading
import time

import numpy as np

MAGIC = bytes([0x02, 0x01, 0x04, 0x03, 0x06, 0x05, 0x08, 0x07])
HDR = struct.Struct("<8s8I")              # 40 bytes
Q9_TO_DB = 20 * np.log10(2) / 512         # log2 magnitude in Q9 -> dB

TLV_POINTS, TLV_RANGE, TLV_NOISE, TLV_STATS, TLV_SIDE = 1, 2, 3, 6, 7


def parse_packet(pkt):
    (_, version, total, platform, frame, _, nobj, ntlv, subframe) = HDR.unpack_from(pkt)
    out = {"frame": frame, "subframe": subframe, "num_obj": nobj, "platform": platform}
    off = HDR.size
    for _ in range(ntlv):
        if off + 8 > len(pkt):
            break
        t, n = struct.unpack_from("<II", pkt, off)
        body = pkt[off + 8:off + 8 + n]
        off += 8 + n
        if t == TLV_POINTS:
            out["points"] = np.frombuffer(body, "<f4").reshape(-1, 4)
        elif t == TLV_RANGE:
            out["range"] = np.frombuffer(body, "<u2") * Q9_TO_DB
        elif t == TLV_NOISE:
            out["noise"] = np.frombuffer(body, "<u2") * Q9_TO_DB
        elif t == TLV_STATS:
            out["stats"] = struct.unpack_from("<6I", body)
        elif t == TLV_SIDE:
            out["side"] = np.frombuffer(body, "<i2").reshape(-1, 2) / 10.0
    return out


class Reader(threading.Thread):
    """Reads the serial port, resyncs on the magic word, keeps the latest frame."""

    def __init__(self, port, baud):
        super().__init__(daemon=True)
        import serial
        self.ser = serial.Serial(port, baud, timeout=0.1)
        self.lock = threading.Lock()
        self.latest = None
        self.packets = 0
        self.bad = 0
        self.missed = 0
        self.times = collections.deque(maxlen=50)
        self._last_frame = None

    def run(self):
        buf = bytearray()
        while True:
            buf += self.ser.read(self.ser.in_waiting or 1)
            while True:
                i = buf.find(MAGIC)
                if i < 0:
                    del buf[:-7]                       # keep a possible partial magic
                    break
                if i > 0:
                    del buf[:i]
                if len(buf) < HDR.size:
                    break
                total = HDR.unpack_from(buf)[2]
                if total < HDR.size or total > 1 << 20:
                    self.bad += 1
                    del buf[:len(MAGIC)]
                    continue
                if len(buf) < total:
                    break
                pkt = bytes(buf[:total])
                del buf[:total]
                try:
                    data = parse_packet(pkt)
                except (struct.error, ValueError):
                    self.bad += 1
                    continue
                if self._last_frame is not None and data["frame"] > self._last_frame + 1:
                    self.missed += data["frame"] - self._last_frame - 1
                self._last_frame = data["frame"]
                with self.lock:
                    self.latest = data
                    self.packets += 1
                    self.times.append(time.monotonic())

    def get(self):
        with self.lock:
            d, self.latest = self.latest, None
            t = list(self.times)
        rate = (len(t) - 1) / (t[-1] - t[0]) if len(t) > 1 and t[-1] > t[0] else 0.0
        return d, rate


def range_step_from_cfg(path):
    """metres per range bin, same formula as mmwdemo_rfparser.c"""
    prof = None
    for line in open(path):
        w = line.split("%", 1)[0].split()
        if w and w[0] == "profileCfg":
            prof = w
    if prof is None:
        return None
    slope = float(prof[8]) * 1e12
    n = int(prof[10])
    fs = float(prof[11]) * 1e3
    nfft = 1 << (n - 1).bit_length()
    return 299792458.0 * fs / (2 * slope * nfft)


def main():
    here = os.path.dirname(os.path.abspath(__file__))
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-p", "--port", default=os.environ.get("RADAR_DATA_PORT", "/dev/ttyACM1"))
    ap.add_argument("-b", "--baud", type=int, default=3125000)
    ap.add_argument("-c", "--cfg", default=os.path.join(here, "cfg", "plasma.cfg"),
                    help="cfg sent to the board, for the range axis in metres")
    ap.add_argument("-n", "--history", type=int, default=300, help="waterfall frames")
    ap.add_argument("--snapshot", help="save one PNG after a few seconds and exit (test)")
    args = ap.parse_args()

    step = range_step_from_cfg(args.cfg) if args.cfg and os.path.exists(args.cfg) else None
    reader = Reader(args.port, args.baud)
    reader.start()

    import matplotlib
    if args.snapshot:
        matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    from matplotlib.animation import FuncAnimation

    fig = plt.figure(figsize=(13, 8))
    fig.canvas.manager.set_window_title(f"live_plot {args.port}")
    gs = fig.add_gridspec(2, 3)
    ax_r = fig.add_subplot(gs[0, :2])
    ax_w = fig.add_subplot(gs[1, :2])
    ax_p = fig.add_subplot(gs[0, 2])
    ax_t = fig.add_subplot(gs[1, 2])
    ax_t.axis("off")
    unit = "range [m]" if step else "range bin"

    (l_range,) = ax_r.plot([], [], lw=1, label="range profile")
    (l_noise,) = ax_r.plot([], [], lw=0.8, alpha=0.7, label="noise profile")
    (l_peak,) = ax_r.plot([], [], "rv")
    ax_r.set(xlabel=unit, ylabel="dB", title="Range profile (zero Doppler)")
    ax_r.grid(True, alpha=0.3)
    ax_r.legend(loc="upper right", fontsize=8)
    sc = ax_p.scatter([], [], c=[], s=15, cmap="coolwarm", vmin=-1, vmax=1)
    ax_p.set(xlabel="x [m]", ylabel="y [m]", title="Detected points (colour = v)")
    ax_p.grid(True, alpha=0.3)
    txt = ax_t.text(0, 1, "waiting for data ...", va="top", family="monospace", fontsize=9)

    state = {"water": None, "img": None, "x": None, "start": time.monotonic()}

    def update(_):
        d, rate = reader.get()
        if d is None:
            return ()
        if "range" in d:
            y = d["range"]
            if state["x"] is None or len(state["x"]) != len(y):
                state["x"] = np.arange(len(y)) * (step or 1)
                state["water"] = np.full((args.history, len(y)), np.nan)
                ax_r.set_xlim(state["x"][0], state["x"][-1])
                ax_w.clear()
                state["img"] = ax_w.imshow(state["water"].T, aspect="auto", origin="lower",
                                           extent=[-args.history, 0, state["x"][0], state["x"][-1]],
                                           interpolation="nearest")
                ax_w.set(xlabel="frames ago", ylabel=unit, title="Range profile over time")
            x = state["x"]
            l_range.set_data(x, y)
            k = 2 + int(np.argmax(y[2:]))                 # skip DC bins
            l_peak.set_data([x[k]], [y[k] + 2])
            if "noise" in d:
                l_noise.set_data(x, d["noise"])
            lo = min(y.min(), d["noise"].min()) if "noise" in d else y.min()
            ax_r.set_ylim(lo - 3, y.max() + 8)
            w = state["water"]
            w[:-1] = w[1:]
            w[-1] = y
            state["img"].set_data(w.T)
            state["img"].set_clim(np.nanpercentile(w, 5), np.nanmax(w))
            peak = f"{x[k]:.3f} m" if step else f"bin {k}"
        else:
            peak = "-"
        pts = d.get("points")
        if pts is not None and len(pts):
            sc.set_offsets(pts[:, :2])
            sc.set_array(pts[:, 3])
            lim = max(1.0, np.abs(pts[:, :2]).max() * 1.1)
            ax_p.set_xlim(-lim, lim)
            ax_p.set_ylim(0, lim)
        else:
            sc.set_offsets(np.empty((0, 2)))
        lines = [f"port      {args.port}",
                 f"frame     {d['frame']}  (subframe {d['subframe']})",
                 f"rate      {rate:5.1f} frames/s",
                 f"packets   {reader.packets}",
                 f"missed    {reader.missed} frames",
                 f"bad       {reader.bad} packets",
                 f"objects   {d['num_obj']}",
                 f"peak      {peak}"]
        if "side" in d and len(d["side"]):
            lines.append(f"max SNR   {d['side'][:, 0].max():.1f} dB")
        if "stats" in d:
            s = d["stats"]
            lines += ["", "stats (previous frame)",
                      f"proc time {s[0]} us",
                      f"tx time   {s[1]} us",
                      f"margin    {s[2]} us",
                      f"CPU load  {s[4]} % active, {s[5]} % inter"]
        txt.set_text("\n".join(lines))
        return ()

    anim = FuncAnimation(fig, update, interval=50, cache_frame_data=False)  # noqa: F841
    fig.tight_layout()
    if args.snapshot:
        t_end = time.monotonic() + 3
        while time.monotonic() < t_end:
            update(None)
            time.sleep(0.05)
        fig.savefig(args.snapshot, dpi=100)
        print(f"snapshot: {args.snapshot}  packets {reader.packets}, missed {reader.missed}, "
              f"bad {reader.bad}")
        return
    plt.show()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        sys.exit(0)
