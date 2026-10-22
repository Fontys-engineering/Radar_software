/*
 *  Copyright (C) 2024-25 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <kernel/nortos/dpl/c66/Context_c66.h>
#include <kernel/dpl/CacheP.h>
#include <drivers/hw_include/cslr_soc.h>
#include <kernel/nortos/dpl/c66/HwiP_c66.h>

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Function Declarations                             */
/* ========================================================================== */

void Context_triggerSleep(void);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

Context_data contextSave = {0};

extern void Power_Down(void);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Context_save(void)
{
    /* Save control Regs */
    contextSave.ctrlIcfgRegs.IER = IER;
    contextSave.ctrlIcfgRegs.TSR = TSR;

    /* Save ICFG Registers */
    contextSave.ctrlIcfgRegs.EVTMASK0 = gHwiCtrl.pIntcRegs->EVTMASK[0];
    contextSave.ctrlIcfgRegs.EVTMASK1 = gHwiCtrl.pIntcRegs->EVTMASK[1];
    contextSave.ctrlIcfgRegs.EVTMASK2 = gHwiCtrl.pIntcRegs->EVTMASK[2];
    contextSave.ctrlIcfgRegs.EVTMASK3 = gHwiCtrl.pIntcRegs->EVTMASK[3];
    contextSave.ctrlIcfgRegs.EXPMASK0 = gHwiCtrl.pIntcRegs->EXPMASK[0];
    contextSave.ctrlIcfgRegs.EXPMASK1 = gHwiCtrl.pIntcRegs->EXPMASK[1];
    contextSave.ctrlIcfgRegs.EXPMASK2 = gHwiCtrl.pIntcRegs->EXPMASK[2];
    contextSave.ctrlIcfgRegs.EXPMASK3 = gHwiCtrl.pIntcRegs->EXPMASK[3];
    contextSave.ctrlIcfgRegs.INTMUX1  = gHwiCtrl.pIntcRegs->INTMUX1;
    contextSave.ctrlIcfgRegs.INTMUX2  = gHwiCtrl.pIntcRegs->INTMUX2;
    contextSave.ctrlIcfgRegs.INTMUX3  = gHwiCtrl.pIntcRegs->INTMUX3;
    contextSave.ctrlIcfgRegs.INTDMASK = gHwiCtrl.pIntcRegs->INTDMASK;

    /* asm function which executes the power down sequence */
    Power_Down();

    return;

}

void Context_restore(void)
{
    HwiP_IntcRegsOvly   pIntcRegs;

    pIntcRegs = gHwiCtrl.pIntcRegs;

    /* Restore control Regs */
    IER = contextSave.ctrlIcfgRegs.IER;
    TSR = contextSave.ctrlIcfgRegs.TSR;

    /* Restore ICFG Registers */
    pIntcRegs->EVTMASK[0] = contextSave.ctrlIcfgRegs.EVTMASK0;
    pIntcRegs->EVTMASK[1] = contextSave.ctrlIcfgRegs.EVTMASK1;
    pIntcRegs->EVTMASK[2] = contextSave.ctrlIcfgRegs.EVTMASK2;
    pIntcRegs->EVTMASK[3] = contextSave.ctrlIcfgRegs.EVTMASK3;
    pIntcRegs->EXPMASK[0] = contextSave.ctrlIcfgRegs.EXPMASK0;
    pIntcRegs->EXPMASK[1] = contextSave.ctrlIcfgRegs.EXPMASK1;
    pIntcRegs->EXPMASK[2] = contextSave.ctrlIcfgRegs.EXPMASK2;
    pIntcRegs->EXPMASK[3] = contextSave.ctrlIcfgRegs.EXPMASK3;
    pIntcRegs->INTMUX1    = contextSave.ctrlIcfgRegs.INTMUX1;
    pIntcRegs->INTMUX2    = contextSave.ctrlIcfgRegs.INTMUX2;
    pIntcRegs->INTMUX3    = contextSave.ctrlIcfgRegs.INTMUX3;
    pIntcRegs->INTDMASK   = contextSave.ctrlIcfgRegs.INTDMASK;

    return;

}

/* Called in asm function */
void Context_triggerSleep(void)
{
    /* Write back cache before powering down */
    CacheP_wbAll(CacheP_TYPE_ALL);

    /* To ensure all memory operations are complete */
    _mfence();
    _mfence();

    CSL_dss_rcmRegs *DssRcmRegs = (CSL_dss_rcmRegs *)contextSave.ptrDssRcmRegs;

    /* Clear all status registers */
    DssRcmRegs->DSP_PD_WAKEUP_STATUS0_CLR = 0xFFFFFFFFU;
    DssRcmRegs->DSP_PD_WAKEUP_STATUS1_CLR = 0xFFFFFFFFU;
    DssRcmRegs->DSP_PD_WAKEUP_STATUS2_CLR = 0xFFFFFFFFU;

    if(contextSave.wakeupSrc < 32U)
    {
        DssRcmRegs->DSP_PD_WAKEUP_MASK0 &= (~(0x1U << contextSave.wakeupSrc));
    }
    else if(contextSave.wakeupSrc < 64U)
    {
        contextSave.wakeupSrc = contextSave.wakeupSrc - 32U;
        DssRcmRegs->DSP_PD_WAKEUP_MASK1 &= (~(0x1U << contextSave.wakeupSrc));
    }
    else
    {
        contextSave.wakeupSrc = contextSave.wakeupSrc - 64U;
        DssRcmRegs->DSP_PD_WAKEUP_MASK2 &= (~(0x1U << contextSave.wakeupSrc));
    }
    DssRcmRegs->DSP_PD_TRIGGER_SLEEP |= 0x00000001U;
}

void Context_sleepHook(void)
{
    if(contextSave.altPc != 0U)
    {
        /* When altPc flag is non zero, Power Down entire C66x core */
       if (contextSave.PreSaveFxn != NULL) {
        (*(contextSave.PreSaveFxn))();
        }
        Context_save();
        /* DSP resumes execution here after power_up asm function is executed */
        Context_restore();
        if (contextSave.PostRestoreFxn != NULL) {
            (*(contextSave.PostRestoreFxn))();
        }
    }
    else
    {
        /* When altPc flag is 0, execute IDLE instruction */
        asm("    IDLE");
    }
}
