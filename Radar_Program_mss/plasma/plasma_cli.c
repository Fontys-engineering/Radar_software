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

#include <plasma/plasma_cli.h>
#include <plasma/plasma_mss.h>

/* bump UP as commands are added KEEP IN SYNC W/mmw_cli.c */
#define PLASMA_CLI_NUM_CMDS 1U

static int32_t PlasmaCli_stats(int32_t argc, char *argv[]) {
  CLI_write("plasmaCfg: sf=%d nPh=%d objDet=%d mask=0x%x\n",
            gPlasmaCfg.subFrameIdx, gPlasmaCfg.nPhaseBins,
            gPlasmaCfg.objDetEnable, gPlasmaCfg.outputMask);
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

  return 0;
}