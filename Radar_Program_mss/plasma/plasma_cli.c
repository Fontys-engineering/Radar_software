/*
 * Copyright (c) 2026,
 *      Fontys University of Applied Sciences
 *          Mete Han Keskin < m.keskin @ student . fontys . nl >
 *          Vlad Nepriakhin < v.nepriakhin @ student . fontys . nl >
 *      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdlib.h>
#include <ti/demo/awr294x/mmw/mss/mmw_mss.h>

#include <plasma/plasma_cli.h>
#include <plasma/plasma_mss.h>

/* bump UP as commands are added */
#define PLASMA_CLI_NUM_CMDS 3U

#define PLASMA_CLI_MAX_SUBFRAMES 4
#define PLASMA_CLI_MAX_PHASE_BINS 7U

extern MmwDemo_MSS_MCB gMmwMssMCB;

/* write to cli current cfgs relevant to plasma*/
static int32_t PlasmaCli_stats(int32_t argc, char *argv[]) {
  CLI_write("plasmaCfg: sf=%d gate=[%.3f %.3f]m snrThr=%.1fdB nPh=%d "
            "objDet=%d mask=0x%x\n",
            gPlasmaCfg.subFrameIdx, gPlasmaCfg.gateMinM, gPlasmaCfg.gateMaxM,
            gPlasmaCfg.snrThrDb, gPlasmaCfg.nPhaseBins, gPlasmaCfg.objDetEnable,
            gPlasmaCfg.outputMask);
  CLI_write("plasmaCalib: rangeOffset=%.4fm phaseOffset=%.4frad\n",
            gPlasmaCfg.rangeOffsetM, gPlasmaCfg.phaseOffsetRad);
  return 0;
}

/* read args and config plasma settings @ runtime */
/* parse into a local copy, only commit to gPlasmaCfg if all checks pass */
static int32_t PlasmaCli_cfg(int32_t argc, char *argv[]) {

  Plasma_Cfg cfg = gPlasmaCfg;

  int32_t subFrameIdx, nPhaseBins, objDetEnable;

  if (argc != 8) {
    CLI_write("Error: Invalid usage of the CLI command\n");
    return -1;
  }

  subFrameIdx = atoi(argv[1]);
  cfg.gateMinM = (float)atof(argv[2]);
  cfg.gateMaxM = (float)atof(argv[3]);
  cfg.snrThrDb = (float)atof(argv[4]);
  nPhaseBins = atoi(argv[5]);
  objDetEnable = atoi(argv[6]);
  cfg.outputMask = (uint32_t)strtoul(argv[7], NULL, 0);

  if ((subFrameIdx < -1) || (subFrameIdx >= PLASMA_CLI_MAX_SUBFRAMES)) {
    CLI_write("Error: subFrameIdx must be -1..%d\n",
              PLASMA_CLI_MAX_SUBFRAMES - 1);
    return -1;
  }
  if ((cfg.gateMinM < 0.0f) || (cfg.gateMinM >= cfg.gateMaxM)) {
    CLI_write("Error: need 0 <= gateMinM < gateMaxM\n");
    return -1;
  }
  if ((nPhaseBins < 1) || (nPhaseBins > (int32_t)PLASMA_CLI_MAX_PHASE_BINS) ||
      ((nPhaseBins & 1) == 0)) {
    CLI_write("Error: nPhaseBins must be odd, 1..%d\n",
              PLASMA_CLI_MAX_PHASE_BINS);
    return -1;
  }
  if ((objDetEnable != 0) && (objDetEnable != 1)) {
    CLI_write("Error: objDetEnable must be 0 or 1\n");
    return -1;
  }

  cfg.subFrameIdx = (int8_t)subFrameIdx;
  cfg.nPhaseBins = (uint8_t)nPhaseBins;
  cfg.objDetEnable = (uint8_t)objDetEnable;

  /* static fields change memory/chain layout, only allowed when stopped */
  if ((gMmwMssMCB.sensorState == MmwDemo_SensorState_STARTED) &&
      ((cfg.nPhaseBins != gPlasmaCfg.nPhaseBins) ||
       (cfg.objDetEnable != gPlasmaCfg.objDetEnable))) {
    CLI_write("Error: nPhaseBins/objDetEnable need sensorStop first\n");
    return -1;
  }

  gPlasmaCfg = cfg;
  return 0;
}

/* load plasma relevant calibration settings*/
/*rangeOffsetM compensates for signal travel time over waveguide/cables,
 * (measured_dist-true_dist) */
/* phaseOffsetRad is optional, keeps its previous value when omitted */
static int32_t PlasmaCli_calib(int32_t argc, char *argv[]) {
  if ((argc != 2) && (argc != 3)) {
    CLI_write("Error: Invalid usage of the CLI command\n");
    return -1;
  }

  gPlasmaCfg.rangeOffsetM = (float)atof(argv[1]);
  if (argc == 3) {
    gPlasmaCfg.phaseOffsetRad = (float)atof(argv[2]);
  }
  return 0;
}

int32_t PlasmaCli_register(CLI_Cfg *cfg, uint32_t *cnt) {
  /* CLI_open writes "help" after the last entry, unchecked:
   keep one slot free */
  if ((*cnt + PLASMA_CLI_NUM_CMDS) >= CLI_MAX_CMD) {
    return -1;
  }
  cfg->tableEntry[*cnt].cmd = "plasmaStats";
  cfg->tableEntry[*cnt].helpString = "No arguments";
  cfg->tableEntry[*cnt].cmdHandlerFxn = PlasmaCli_stats;
  (*cnt)++;

  cfg->tableEntry[*cnt].cmd = "plasmaCfg";
  cfg->tableEntry[*cnt].helpString =
      "<subFrameIdx> <gateMinM> <gateMaxM> <snrThrDb> <nPhaseBins> "
      "<objDetEnable> <outputMask>";
  cfg->tableEntry[*cnt].cmdHandlerFxn = PlasmaCli_cfg;
  (*cnt)++;

  cfg->tableEntry[*cnt].cmd = "plasmaCalib";
  cfg->tableEntry[*cnt].helpString = "<rangeOffsetM> [<phaseOffsetRad>]";
  cfg->tableEntry[*cnt].cmdHandlerFxn = PlasmaCli_calib;
  (*cnt)++;

  return 0;
}
