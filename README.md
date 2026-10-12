# AWR2944 development MEP7_EE02 SEPT26

This repository contains the source code of the AWR2944 EVM board for the MEP7_EE02 Plasma boundary detector system design and integration project.

Previous work was done by Petri Miettinen. Original README.md moved to [Docs](Doc/README.md).

Current work is for september 2026 S7 project. This branch based on Petri's work and is being developed towards measurement of magnetized plasma beams.

Main project contact :

* Chris Lee - c.lee@fontys.nl

Current maintainers/developers for this branch:

* Vlad Nepriakhin - v.nepriakhin@student.fontys.nl
* Mete Han Keskin - m.keskin@student.fontys.nl

Prev maintainers/developers:

* Petri Miettinen - p.miettinen@student.fontys.nl

## Getting started (WIP)

Relevant documentation can be found in Doc directory. This repository will be developed with Windows and Linux systems, therefore some path's may differ. Please open an issue for those.

## Building

Requirements: CCS 20.3 with its default compilers (TI Arm Clang 4.0.3, C6000 8.5.0) and the SysConfig it ships with. All SDKs are in `Dependencies/`; nothing else needs to be installed.

**Command line (Windows and Linux):**

```sh
make            # -> out/Radar_Program.appimage
make mss        # or: make dss / make syscfg / make clean
make V=1        # show full compiler command lines
```

On Windows use the gmake that comes with CCS (`C:/ti/ccs2030/ccs/utils/bin/gmake`). `host.mk` detects the OS and expects CCS in `~/ti/ccs2030` or `C:/ti/ccs2030`. If yours is somewhere else, create a `local.mk` (git-ignored) with e.g. `CCS_PATH = /opt/ti/ccs2030/ccs`.

**CCS:** import `Radar_Program_dss` and `Radar_Program_mss` and build `Radar_Program_mss` (it builds the DSS first). The image ends up in `Radar_Program_mss/Debug/Radar_Program_Debug.appimage`.

The project files and the Makefile use the same flags; if you change one (defines, include paths, libraries), change the other too. `Dependencies/sysconfig_1.28.0` is a Windows-only copy and is no longer used by either build.

---

# Architecture Docs

These docs explain how the TI derived radar firmware in `Radar_Software` works on commit `e920574aaa4f350b3e5566a57cdbadaebd04d2a5` of `RPS_Fix_Compilation` branch and what they're being developed into.

Beware that some error's in the structure may exist. Open an issue if spotted or contact devs.

Relevant technical documentation can be found in the [00_index.md](Doc/00_index.md) file. Pick a nice markdown reader, I recommend using MarkText, its pretty neat.


