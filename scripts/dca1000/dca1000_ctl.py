#!/usr/bin/env python3
"""
dca1000_ctl.py - minimal DCA1000EVM control + raw recorder (Linux fallback)

Stand-in for TI's Windows-only DCA1000EVM_CLI_Control.exe. Implements the
UDP command protocol of SPRUIJ4A sec. 5 and records the raw-mode data port
into <prefix>_Raw_0.bin, same layout as TI's tool (payload only, in order).

  UNTESTED ON HARDWARE. Try it on the bench before relying on it.

  dca1000_ctl.py fpga   [-c cf.json]       connect, read version, configure
  dca1000_ctl.py record [-c cf.json] [-t seconds] [-o dir] [-p prefix]
                                           start record, write .bin until
                                           Ctrl-C or -t, then stop record
  dca1000_ctl.py stop                      send RECORD_STOP only
  dca1000_ctl.py reset                     RESET_FPGA
  dca1000_ctl.py version                   READ_FPGA_VERSION

Host PC must be 192.168.33.30/24 on a gigabit adapter (see Doc/10).

Command packet (little-endian u16s): 0xA55A, cmd, size, data[size], 0xEEAA
Response:                            0xA55A, cmd, status, 0xEEAA (0 = ok)
Raw data packet on port 4098:        u32 seq, u48 byteCount, payload
  byteCount = bytes sent before this packet -> file offset of payload.
"""

import argparse
import json
import os
import signal
import socket
import struct
import sys
import time

HDR, FTR = 0xA55A, 0xEEAA

RESET_FPGA = 0x01
CONFIG_FPGA_GEN = 0x03
RECORD_START = 0x05
RECORD_STOP = 0x06
SYSTEM_CONNECT = 0x09
CONFIG_PACKET_DATA = 0x0B
READ_FPGA_VERSION = 0x0E

DATA_HDR_BYTES = 10
PACKET_BYTES = 1472          # max UDP payload at 1500 MTU
FPGA_CLK_PER_US = 125        # packet delay is given in 8 ns FPGA ticks


def load_cfg(path):
    with open(path) as f:
        c = json.load(f)["DCA1000Config"]
    eth = c["ethernetConfig"]
    return {
        "dca_ip": eth["DCA1000IPAddress"],
        "cfg_port": int(eth["DCA1000ConfigPort"]),
        "data_port": int(eth["DCA1000DataPort"]),
        "host_ip": c.get("ethernetConfigUpdate", {}).get("systemIPAddress", ""),
        "logging": {"raw": 1, "multi": 2}[c["dataLoggingMode"]],
        "lvds": int(c["lvdsMode"]),            # 1 = 4 lane, 2 = 2 lane
        "transfer": {"LVDSCapture": 1, "playback": 2}[c["dataTransferMode"]],
        "capture": {"SDCardStorage": 1, "ethernetStream": 2}[c["dataCaptureMode"]],
        "fmt": int(c["dataFormatMode"]),       # 1 = 12, 2 = 14, 3 = 16 bit
        "delay_us": int(c["packetDelay_us"]),
        "prefix": c["captureConfig"]["filePrefix"],
    }


class Dca1000:
    def __init__(self, cfg, timeout=2.0):
        self.cfg = cfg
        self.dca = (cfg["dca_ip"], cfg["cfg_port"])
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            self.sock.bind((cfg["host_ip"], cfg["cfg_port"]))
        except OSError as e:
            raise SystemExit(f"error: cannot bind {cfg['host_ip']}:{cfg['cfg_port']} "
                             f"({e}). Is the adapter set to 192.168.33.30/24?")
        self.sock.settimeout(timeout)

    def cmd(self, code, data=b"", name=""):
        pkt = struct.pack("<HHH", HDR, code, len(data)) + data + struct.pack("<H", FTR)
        self.sock.sendto(pkt, self.dca)
        try:
            resp, _ = self.sock.recvfrom(2048)
        except socket.timeout:
            raise SystemExit(f"error: no reply to {name or hex(code)} "
                             f"(check IP 192.168.33.30, gigabit link, SW2.5)")
        hdr, rcode, status, ftr = struct.unpack("<HHHH", resp[:8])
        if hdr != HDR or ftr != FTR or rcode != code:
            raise SystemExit(f"error: bad reply to {name}: {resp.hex()}")
        return status

    def ok(self, code, data=b"", name=""):
        status = self.cmd(code, data, name)
        print(f"  {name:<20} {'ok' if status == 0 else f'FAILED ({status})'}")
        return status == 0

    def connect(self):
        return self.ok(SYSTEM_CONNECT, name="SYSTEM_CONNECT")

    def version(self):
        v = self.cmd(READ_FPGA_VERSION, name="READ_FPGA_VERSION")
        major, minor, playback = v & 0x7F, (v >> 7) & 0x7F, (v >> 14) & 1
        print(f"  FPGA version {major}.{minor}{' (playback)' if playback else ''}")

    def configure(self):
        c = self.cfg
        gen = bytes([c["logging"], c["lvds"], c["transfer"], c["capture"],
                     c["fmt"], 30])                       # 30 = LVDS timeout
        pkt = struct.pack("<HHH", PACKET_BYTES, c["delay_us"] * FPGA_CLK_PER_US, 0)
        a = self.ok(CONFIG_FPGA_GEN, gen, "CONFIG_FPGA_GEN")
        b = self.ok(CONFIG_PACKET_DATA, pkt, "CONFIG_PACKET_DATA")
        return a and b


def record(dca, out_dir, prefix, seconds):
    cfg = dca.cfg
    os.makedirs(out_dir, exist_ok=True)
    bin_path = os.path.join(out_dir, f"{prefix}_Raw_0.bin")
    log_path = os.path.join(out_dir, f"{prefix}_Raw_LogFile.csv")

    ds = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    ds.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 16 * 1024 * 1024)
    ds.bind((cfg["host_ip"], cfg["data_port"]))
    ds.settimeout(0.5)

    stop = {"flag": False}
    signal.signal(signal.SIGINT, lambda *_: stop.update(flag=True))

    if not dca.ok(RECORD_START, name="RECORD_START"):
        raise SystemExit("error: record start refused")
    print(f"recording -> {bin_path}  (Ctrl-C to stop)")

    first_seq = None
    expect_seq = None
    packets = lost = out_of_order = 0
    end_offset = 0
    t0 = time.monotonic()
    last_print = t0
    with open(bin_path, "wb") as f:
        while not stop["flag"]:
            if seconds and time.monotonic() - t0 >= seconds:
                break
            try:
                pkt = ds.recv(65536)
            except socket.timeout:
                continue
            seq = struct.unpack_from("<I", pkt, 0)[0]
            offset = int.from_bytes(pkt[4:10], "little")
            payload = pkt[DATA_HDR_BYTES:]
            if first_seq is None:
                first_seq = expect_seq = seq
                base = offset              # file starts at first byte we saw
            if seq > expect_seq:
                lost += seq - expect_seq
            elif seq < expect_seq:
                out_of_order += 1
            expect_seq = max(expect_seq, seq + 1)
            packets += 1
            # gaps stay zero-filled because we seek past them
            f.seek(offset - base)
            f.write(payload)
            end_offset = max(end_offset, offset - base + len(payload))

            now = time.monotonic()
            if now - last_print >= 1.0:
                rate = end_offset * 8 / (now - t0) / 1e6
                print(f"\r  {end_offset / 1e6:8.2f} MB  {rate:6.1f} Mbit/s  "
                      f"lost {lost}  ooo {out_of_order}", end="", flush=True)
                last_print = now
        f.truncate(end_offset)
    print()
    dca.ok(RECORD_STOP, name="RECORD_STOP")
    ds.close()

    with open(log_path, "w") as lf:
        lf.write("packets,bytes,lost_packets,out_of_order,first_seq,duration_s\n")
        lf.write(f"{packets},{end_offset},{lost},{out_of_order},"
                 f"{first_seq},{time.monotonic() - t0:.3f}\n")
    print(f"done: {packets} packets, {end_offset} bytes, lost {lost}, "
          f"out of order {out_of_order}  (log {log_path})")
    if packets == 0:
        print("warning: no data received. LD7 on? sensorStart sent? "
              "lvdsStreamCfg in cfg?", file=sys.stderr)


def main():
    here = os.path.dirname(os.path.abspath(__file__))
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("action", choices=["fpga", "record", "stop", "reset", "version"])
    ap.add_argument("-c", "--config", default=os.path.join(here, "cf.json"))
    ap.add_argument("-o", "--out", default="data", help="output directory")
    ap.add_argument("-p", "--prefix", help="file prefix (default from json)")
    ap.add_argument("-t", "--seconds", type=float, default=0,
                    help="stop after N seconds (default: until Ctrl-C)")
    args = ap.parse_args()

    cfg = load_cfg(args.config)
    dca = Dca1000(cfg)

    if args.action == "fpga":
        dca.connect()
        dca.version()
        if not dca.configure():
            sys.exit(1)
    elif args.action == "record":
        record(dca, args.out, args.prefix or cfg["prefix"], args.seconds)
    elif args.action == "stop":
        dca.ok(RECORD_STOP, name="RECORD_STOP")
    elif args.action == "reset":
        dca.ok(RESET_FPGA, name="RESET_FPGA")
    elif args.action == "version":
        dca.version()


if __name__ == "__main__":
    main()
