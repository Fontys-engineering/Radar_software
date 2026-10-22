/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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

#ifndef CONTEXT_C66_H_
#define CONTEXT_C66_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <c6x.h>
#include <stdio.h>

/**
 * \defgroup KERNEL_DPL_C66_CONTEXT_SAVE_RESTORE Structures and APIS
 * \ingroup KERNEL_DPL
 *
 * This module defines generic APIs to power down, save and restore DSP context.
 *
 * For more details and example usage, see
 * \ref KERNEL_DPL_C66_CONTEXT_SAVE_RESTORE_PAGE
 *
 *
 * @{
 */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Control and ICFG registers to be saved and restored
 * Refer:https://www.ti.com/lit/ug/sprui04f/sprui04f.pdf for DSP core intrinsics
*/
typedef struct Context_ctrlIcfgRegs_t
{
    volatile uint32_t IER; // Interrupt enable register
    volatile uint32_t TSR; // Task State Register
    volatile uint32_t EVTMASK0;
    volatile uint32_t EVTMASK1;
    volatile uint32_t EVTMASK2;
    volatile uint32_t EVTMASK3;
    volatile uint32_t EXPMASK0;
    volatile uint32_t EXPMASK1;
    volatile uint32_t EXPMASK2;
    volatile uint32_t EXPMASK3;
    volatile uint32_t INTMUX1;
    volatile uint32_t INTMUX2;
    volatile uint32_t INTMUX3;
    volatile uint32_t INTDMASK;

} Context_ctrlIcfgRegs;

/** \brief Core registers to be saved and restored when assembly function
 * is called.
 * Refer: https://www.ti.com/lit/ug/sprui04f/sprui04f.pdf section 8.6.1
 * for DSP core intrinsic details
*/
typedef struct Context_coreRegs_t
{
    volatile uint32_t B3;
    /**< Return register (address to return to) */

    volatile uint32_t B10;
    volatile uint32_t B11;
    volatile uint32_t B12;
    volatile uint32_t B13;
    volatile uint32_t B14;

    volatile uint32_t B15;
    /**< Stack Pointer */

    volatile uint32_t A10;
    volatile uint32_t A11;
    volatile uint32_t A12;
    volatile uint32_t A13;
    volatile uint32_t A14;
    volatile uint32_t A15;

} Context_coreRegs;

/** \brief Pointer to Pre-save function defined in application */
typedef void (*Context_PreSaveFxn)(void);

/** \brief Pointer to Post-restore function defined in application */
typedef void (*Context_PostRestoreFxn)(void);

/** \brief Structure to hold context save and restore details
 * Note: The order of the structure elements should not be modified.
*/
typedef struct Context_data_t
{
    uint32_t altPc;
    /**< Flag to indicate if DSP context has to be restored in
     * HwiP Entry function.*/

    Context_coreRegs coreRegs;
    /**< Structure to save the core registers before DSP Power Down */

    Context_ctrlIcfgRegs ctrlIcfgRegs;
    /**< Structure to save the ICFG registers before DSP Power Down */

    Context_PreSaveFxn PreSaveFxn;
    /**< Function executed before DSP is powered down.
     * This function halts task execution by stopping RTI Timer
     * (FreeRTOS Ticks) and disable interrupts*/

    Context_PostRestoreFxn PostRestoreFxn;
    /**< Function executed after DSP is powered up.
     * This function resumes task execution by resuming RTI Timer
     * (FreeRTOS Ticks), enable interrupts and reset altPc*/

    uint32_t minTime;
    /**< Application can set minimum ide time value below which DSP
     * is not power down */

    uint32_t wakeupSrc;
    /**< DSP event which powers up the core.
     * Refer Interrupt table of DSP in TRM. */

    void *ptrDssRcmRegs;
    /**<  Points to DSS RCM Register Base for power down register operations*/

} Context_data;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/**
 * \brief
 *  Global structure for context save and restore
 *
 * \details
 *  Contains all the information needed for DSP Power Gating with context save
 * and restore
 *
 */
extern Context_data contextSave;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief
 *  Saves necessary resgisters before DSP Power Down
 *
 */
void Context_save(void);

/**
 * \brief
 *  Restores the saved registers once DSP wakes up
 *
 */
void Context_restore(void);

/**
 * \brief
 *  Function is plugged into the vPortSuppressTicksAndSleep in case of freertos
 *  or it can be called directly by the nortos application.
 *  contextSave.altPc value has to be non zero before executing this.
 *
 */
void Context_sleepHook(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CONTEXT_C66_H_ */
