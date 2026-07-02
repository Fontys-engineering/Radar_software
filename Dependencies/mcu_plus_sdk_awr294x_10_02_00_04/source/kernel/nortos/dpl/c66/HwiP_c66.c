/*
 *  Copyright (C) 2021-2025 Texas Instruments Incorporated
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

#include <kernel/nortos/dpl/c66/HwiP_c66.h>
#include <drivers/hw_include/csl_types.h>
#include <c6x.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Number of available direct hardware interrupts (INT4 through INT11) */
#define HwiP_INTC_NUM_DIRECT_INTR         (8U)

/** \brief Enable Direct Interrupt Mapping */
#define HwiP_DIRECT_INTR_MAPPED           (1U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct HwiP_Struct_s
{
    uint32_t intNum;
    uint8_t  dspIntNum;
    uint8_t  isDirectMap;
} HwiP_Struct;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

unsigned int _disable_interrupts(void);
unsigned int _enable_interrupts(void);
void _restore_interrupts (unsigned int key);
static inline void HwiP_intcMapEventVector(HwiP_IntcRegsOvly pIntcRegs,
                                           uint32_t eventId,
                                           uint32_t vectId);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

HwiP_Ctrl       gHwiCtrl;
HwiP_IntcVect   gHwiIntcIntrTable;
uint8_t         gDirectMap[HwiP_INTC_NUM_DIRECT_INTR] = {0};

HwiP_raisePrivilegeFxnPtr gHwiRaisePrivilegeHook = NULL;
HwiP_restorPrivilegeFxnPtr gHwiRestorePrivilegeHook = NULL;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void HwiP_init(void)
{
    uint32_t            i, key;
    uint32_t            vectId;
    HwiP_IntcRegsOvly   pIntcRegs;
    int32_t             currentState;

    key = _disable_interrupts();

    /* initalize data structure */
    gHwiCtrl.pIntcRegs = (HwiP_IntcRegsOvly) HwiP_INTC_BASE_ADDR;
    for(i = 0U; i < HwiP_MAX_EVENTS; i++)
    {
        gHwiCtrl.isr[i] = NULL;
        gHwiCtrl.isrArgs[i] = NULL;
    }
    gHwiCtrl.nmiHandler = NULL;
    gHwiCtrl.nmiArgs = NULL;

    /* Assign dispatcher - gets set based on No-RTOS/FreeRTOS */
    HwiP_assignIntrHandlers();
    HwiP_intcIvpSet();

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        currentState = gHwiRaisePrivilegeHook();
    }

    /*
     * Disable and clear all ECM events
     */
    pIntcRegs = gHwiCtrl.pIntcRegs;
    for(i = 0U; i < HwiP_NUM_ECM; i++)
    {
        pIntcRegs->EVTMASK[i] = 0xFFFFFFFFU;
        pIntcRegs->EVTCLR[i]  = 0xFFFFFFFFU;
    }

    /*
     * Route ECM events to CPU interrupts and enable the CPU interrupts
     */
    for(i = 0U; i < HwiP_NUM_ECM; i++)
    {
        vectId = HwiP_VECTID_ECM_START + i;
        /* Note: Event ID 0-3 are reserved for ECM!! */
        HwiP_intcMapEventVector(pIntcRegs, i, vectId);
        HwiP_intcInterruptClear(vectId);
        (void) HwiP_intcInterruptEnable(vectId);
    }

    /*
     * Enable Global interrupts - NMIE and GIE to enable maskable interrupts
     */
    /* Set NMIE bit in the interrupt enable register (IER) */
    (void) HwiP_intcGlobalNmiEnable();
    /* Set global interrupt enable bit (GIE) bit in the control status register (CSR) */
    (void) HwiP_intcGlobalEnable(NULL);

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        gHwiRestorePrivilegeHook(currentState);
    }

    (void) _restore_interrupts(key);

    return;
}

int32_t HwiP_construct(HwiP_Object *handle, HwiP_Params *params)
{
    uint32_t            key;
    uint32_t            ecmId, eventId;
    HwiP_Struct        *obj;
    HwiP_IntcRegsOvly   pIntcRegs;
    int32_t             currentState;

    obj = (HwiP_Struct *)handle;
    pIntcRegs = gHwiCtrl.pIntcRegs;
    DebugP_assertNoLog(sizeof(HwiP_Struct) <= sizeof(HwiP_Object));
    DebugP_assertNoLog(params->callback != NULL);
    DebugP_assertNoLog(params->intNum < HwiP_MAX_EVENTS);
    /* Check for reserved event used by ECM - 0, 2, 3 */
    DebugP_assertNoLog(params->intNum >= HwiP_INTC_NUM_RESV_INTR);

    key = _disable_interrupts();

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        currentState = gHwiRaisePrivilegeHook();
    }

    ecmId = (params->intNum >> 5U);
    eventId = (params->intNum & 0x1FU);

    if(HwiP_DIRECT_INTR_MAPPED == params->isDirectMap)
    {
        /* Check if enabled through ECM */
        DebugP_assertNoLog((pIntcRegs->EVTMASK[ecmId] & ((uint32_t) 1U << eventId)) != 0U);
        gDirectMap[(params->dspIntNum - 4U)] = (uint8_t) params->intNum;

        /* Check for Direct interrupt number is between 4 and 11 */
        DebugP_assertNoLog(params->dspIntNum >= HwiP_INTC_NUM_RESV_INTR);
        DebugP_assertNoLog(params->dspIntNum < HwiP_VECTID_ECM_START);

        /* Configure interrupt callback for DSP interrupt */
        gHwiCtrl.isr[params->intNum] = params->callback;
        gHwiCtrl.isrArgs[params->intNum] = params->args;

        /* Map Event to CPU interrupt number */
        /*
         * pIntcRegs            -
         * params->intNum       -   Event ID
         * params->dspIntNum    -   CPU INT [4 - 11]
         */
        HwiP_intcMapEventVector(pIntcRegs, params->intNum, params->dspIntNum);
        HwiP_intcInterruptClear(params->dspIntNum);
        (void) HwiP_intcInterruptEnable(params->dspIntNum);
    }
    else
    {
        for(uint32_t idx = 0U; idx < HwiP_INTC_NUM_DIRECT_INTR; idx++)
        {
            /* Check if interrupt was registered as Direct intr */
            DebugP_assertNoLog(params->intNum != gDirectMap[idx]);
        }
        gHwiCtrl.isr[params->intNum] = params->callback;
        gHwiCtrl.isrArgs[params->intNum] = params->args;

        /* Enable the event through ECM */
        pIntcRegs->EVTMASK[ecmId] &= ~((uint32_t) 1U << eventId);
    }

    /* Populate HwiP object parameters */
    obj->intNum = params->intNum;
    obj->isDirectMap = params->isDirectMap;
    obj->dspIntNum = params->dspIntNum;

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        gHwiRestorePrivilegeHook(currentState);
    }

   (void) _restore_interrupts(key);

    return SystemP_SUCCESS;
}

void HwiP_destruct(HwiP_Object *handle)
{
    uint32_t            key;
    uint32_t            ecmId, eventId;
    HwiP_Struct        *obj;
    HwiP_IntcRegsOvly   pIntcRegs;
    int32_t             currentState;

    obj = (HwiP_Struct *) handle;
    pIntcRegs = gHwiCtrl.pIntcRegs;
    DebugP_assertNoLog(obj->intNum < HwiP_MAX_EVENTS);
    /* Check for reserved event used by ECM - 0 to 3 */
    DebugP_assertNoLog(obj->intNum >= HwiP_INTC_NUM_RESV_INTR);

    key = _disable_interrupts();

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        currentState = gHwiRaisePrivilegeHook();
    }

    /* Handle direct interrupts */
    if(HwiP_DIRECT_INTR_MAPPED == obj->isDirectMap)
    {
        (void)HwiP_intcInterruptDisable(obj->dspIntNum);
        gDirectMap[obj->dspIntNum - 4U] = 0;
    }
    else
    {
        /* Disable and clear event through ECM */
        ecmId = (obj->intNum >> 5U);
        eventId = (obj->intNum & 0x1FU);
        pIntcRegs->EVTMASK[ecmId] |= ((uint32_t) 1U << eventId);
        pIntcRegs->EVTCLR[ecmId]   = ((uint32_t) 1U << eventId);
    }

    /* clear interrupt data structure */
    gHwiCtrl.isr[obj->intNum] = NULL;
    gHwiCtrl.isrArgs[obj->intNum] = NULL;

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        gHwiRestorePrivilegeHook(currentState);
    }

    (void) _restore_interrupts(key);

    return;
}

int32_t HwiP_registerNmiHandler(HwiP_FxnCallback nmiHandler, void *args)
{
    int32_t retVal = SystemP_SUCCESS;
    if (gHwiCtrl.nmiHandler != NULL)
    {
        retVal = SystemP_FAILURE;
    }
    else
    {
        gHwiCtrl.nmiHandler = nmiHandler;
        gHwiCtrl.nmiArgs = args;
    }
    return retVal;
}

int32_t HwiP_unregisterNmiHandler(void)
{
    int32_t retVal = SystemP_SUCCESS;
    if (gHwiCtrl.nmiHandler == NULL)
    {
        retVal = SystemP_FAILURE;
    }
    else
    {
        gHwiCtrl.nmiHandler = NULL;
        gHwiCtrl.nmiArgs = NULL;
    }
    return retVal;
}

int32_t HwiP_setArgs(HwiP_Object *handle, void *args)
{
    HwiP_Struct *obj = (HwiP_Struct *)handle;

    DebugP_assertNoLog(obj->intNum < HwiP_MAX_EVENTS);

    gHwiCtrl.isrArgs[obj->intNum] = args;

    return SystemP_SUCCESS;
}

void HwiP_enableInt(HwiP_Object *handle)
{
    uint32_t            key;
    uint32_t            ecmId, eventId;
    HwiP_IntcRegsOvly   pIntcRegs;
    int32_t             currentState;
    HwiP_Struct        *obj = (HwiP_Struct *)handle;

    pIntcRegs = gHwiCtrl.pIntcRegs;
    DebugP_assertNoLog(obj->intNum < HwiP_MAX_EVENTS);
    /* Check for reserved event used by ECM - 0 to 3 */
    DebugP_assertNoLog(obj->intNum >= HwiP_INTC_NUM_RESV_INTR);

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        currentState = gHwiRaisePrivilegeHook();
    }

    key = _disable_interrupts();
    if(HwiP_DIRECT_INTR_MAPPED == obj->isDirectMap)
    {
        (void)HwiP_intcInterruptEnable(obj->dspIntNum);
    }
    else
    {
        ecmId = (obj->intNum >> 5U);
        eventId = (obj->intNum & 0x1FU);
        pIntcRegs->EVTMASK[ecmId] &= ~((uint32_t) 1U << eventId);
    }
    (void) _restore_interrupts(key);

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        gHwiRestorePrivilegeHook(currentState);
    }

    return;
}

uint32_t HwiP_disableInt(HwiP_Object *handle)
{
    uint32_t            key;
    uint32_t            ecmId, eventId;
    HwiP_IntcRegsOvly   pIntcRegs;
    uint32_t            isEnable = 0;
    int32_t             currentState;
    HwiP_Struct        *obj = (HwiP_Struct *)handle;

    pIntcRegs = gHwiCtrl.pIntcRegs;
    DebugP_assertNoLog(obj->intNum < HwiP_MAX_EVENTS);
    /* Check for reserved event used by ECM - 0 to 3 */
    DebugP_assertNoLog(obj->intNum >= HwiP_INTC_NUM_RESV_INTR);

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        currentState = gHwiRaisePrivilegeHook();
    }

    key = _disable_interrupts();
    if(HwiP_DIRECT_INTR_MAPPED == obj->isDirectMap)
    {
        /* Disable directly mapped interrupt if already enabled */
        if((IER & ((uint32_t)1U << obj->dspIntNum)) != 0U)
        {
            isEnable = 1U;
            (void)HwiP_intcInterruptDisable(obj->dspIntNum);
        }
    }
    else
    {
        /* Disable event through ECM if already enabled */
        ecmId = (obj->intNum >> 5U);
        eventId = (obj->intNum & 0x1FU);
        if ((pIntcRegs->EVTMASK[ecmId] & ((uint32_t) 1U << eventId)) == 0U)
        {
            isEnable = 1U;
            pIntcRegs->EVTMASK[ecmId] |= ((uint32_t) 1U << eventId);
        }
    }
    (void) _restore_interrupts(key);

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        gHwiRestorePrivilegeHook(currentState);
    }

    return (isEnable);
}

void HwiP_restoreInt(HwiP_Object *handle, uint32_t oldIntState)
{
    if(oldIntState!=0U)
    {
        HwiP_enableInt(handle);
    }
    else
    {
       (void) HwiP_disableInt(handle);
    }

    return;
}

void HwiP_clearInt(HwiP_Object *handle)
{
    uint32_t            ecmId, eventId;
    HwiP_IntcRegsOvly   pIntcRegs;
    int32_t             currentState;
    HwiP_Struct        *obj = (HwiP_Struct *)handle;

    pIntcRegs = gHwiCtrl.pIntcRegs;
    DebugP_assertNoLog(obj->intNum < HwiP_MAX_EVENTS);
    /* Check for reserved event used by ECM - 0 to 3 */
    DebugP_assertNoLog(obj->intNum >= HwiP_INTC_NUM_RESV_INTR);

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        currentState = gHwiRaisePrivilegeHook();
    }

    ecmId = (obj->intNum >> 5U);
    eventId = (obj->intNum & 0x1FU);
    pIntcRegs->EVTCLR[ecmId] = ((uint32_t) 1U << eventId);

    if(HwiP_DIRECT_INTR_MAPPED == obj->isDirectMap)
    {
        HwiP_intcInterruptClear(obj->dspIntNum);
    }

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        gHwiRestorePrivilegeHook(currentState);
    }

    return;
}

void HwiP_setPri(uint32_t intNum, uint32_t priority)
{
    /* C66x doesn't support programmable priority - it is implicit based on vect ID */
    return;
}

void HwiP_post(HwiP_Object *handle)
{
    uint32_t            ecmId, eventId;
    HwiP_IntcRegsOvly   pIntcRegs;
    int32_t             currentState;

    HwiP_Struct        *obj = (HwiP_Struct *)handle;

    pIntcRegs = gHwiCtrl.pIntcRegs;
    DebugP_assertNoLog(obj->intNum < HwiP_MAX_EVENTS);
    /* Check for reserved event used by ECM - 0 to 3 */
    DebugP_assertNoLog(obj->intNum >= HwiP_INTC_NUM_RESV_INTR);

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        currentState = gHwiRaisePrivilegeHook();
    }

    ecmId = (obj->intNum >> 5U);
    eventId = (obj->intNum & 0x1FU);
    pIntcRegs->EVTSET[ecmId] = ((uint32_t) 1U << eventId);

    if(HwiP_DIRECT_INTR_MAPPED == obj->isDirectMap)
    {
        HwiP_intcInterruptSet(obj->dspIntNum);
    }

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        gHwiRestorePrivilegeHook(currentState);
    }

    return;
}

uintptr_t HwiP_disable(void)
{
    return (uintptr_t)(_disable_interrupts());
}

void HwiP_enable(void)
{
	(void) _enable_interrupts();
    return;
}

void HwiP_restore(uintptr_t oldIntState)
{
    (void)_restore_interrupts(oldIntState);
    return;
}

void HwiP_Params_init(HwiP_Params *params)
{
    params->intNum = 0;
    params->callback = NULL;
    params->args = NULL;
    params->dspIntNum = 0U;    /* NOT USED */
    params->priority = 0;   /* NOT USED */
    params->isFIQ = 0;      /* NOT USED */
    params->isPulse = 0;    /* NOT USED */
    params->isDirectMap = 0U;  /* Default is disabled */

    return;
}

void HwiP_intcEcmDispatcher(uint32_t ecmId)
{
    uint32_t            isrIdx, isrStartIdx;
    HwiP_IntcRegsOvly   pIntcRegs = gHwiCtrl.pIntcRegs;
    uint32_t            i, evtMask;
    volatile uint32_t   mevtFlag;
    uint32_t flag = 0U;
    uint32_t loop = 1U;
    int32_t             currentState;

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        currentState = gHwiRaisePrivilegeHook();
    }

    isrStartIdx = HwiP_EVENTS_PER_ECM * ecmId;
    while(loop != 0U)
    {
        /* Get current pending ECM interrupts */
        mevtFlag = pIntcRegs->MEVTFLAG[ecmId];
        if(mevtFlag == 0U)
        {
            /* No pending interrupts */
            break;
        }
        /* Clear current pending ECM interrupts */
        pIntcRegs->EVTCLR[ecmId] = mevtFlag;

        /* Check and process pending interrupts */
        for(i = 0U; i < HwiP_EVENTS_PER_ECM; i++)
        {
            evtMask = ((uint32_t) 1U << i);
            flag = (mevtFlag & evtMask);
            isrIdx = isrStartIdx + i;
            if((gHwiCtrl.isr[isrIdx] != NULL) &&
               (flag != 0U))
            {
                /* Call user callback */
                gHwiCtrl.isr[isrIdx](gHwiCtrl.isrArgs[isrIdx]);
            }
            mevtFlag &= ~evtMask;
            if(mevtFlag == 0U)
            {
                break;
            }
        }
    }

    if((gHwiRaisePrivilegeHook != NULL) && (gHwiRestorePrivilegeHook != NULL))
    {
        gHwiRestorePrivilegeHook(currentState);
    }

    return;
}

void HwiP_intcDispatcherCore(uint32_t intrId)
{
    uint32_t evtId = 0U;
    uint32_t ecmId = 0U;
    uint32_t eventId = 0U;

    HwiP_IntcRegsOvly   pIntcRegs = gHwiCtrl.pIntcRegs;

    /* Get the event IF registered with corresponding DSP INT */
    if (intrId < 8U)
    {
        evtId = CSL_FEXTR(pIntcRegs->INTMUX1,
                            (((intrId - 4U) * 8U) + 6U), ((intrId - 4U) * 8U));
    }
    else if (intrId < 12U)
    {
        evtId = CSL_FEXTR(pIntcRegs->INTMUX2,
                            (((intrId - 8U) * 8U) + 6U), ((intrId - 8U) * 8U));
    }
    else
    {
        /*MISRAC*/
    }

    /* Check and Clear Event flag */
    ecmId = (evtId >> 5U);
    eventId = (evtId & 0x1FU);

    if(((pIntcRegs->EVTFLAG[ecmId]) & ( (uint32_t)1U << eventId)) != 0U )
    {
        pIntcRegs->EVTCLR[ecmId]   = ((uint32_t)1U << eventId);

        if(gHwiCtrl.isr[evtId] != NULL)
        {
            /* Call user callback */
            gHwiCtrl.isr[evtId](gHwiCtrl.isrArgs[evtId]);
        }
    }
    return;
}

static inline void HwiP_intcMapEventVector(HwiP_IntcRegsOvly pIntcRegs,
                                           uint32_t eventId,
                                           uint32_t vectId)
{
    uint32_t    bitLow;

    if(vectId < 8U)
    {
        bitLow = (vectId - 4U) * 8U;
        CSL_FINSR(pIntcRegs->INTMUX1, bitLow + 6U, bitLow, eventId);
    }
    else if(vectId < 12U)
    {
        bitLow = (vectId - 8U) * 8U;
        CSL_FINSR(pIntcRegs->INTMUX2, bitLow + 6U, bitLow, eventId);
    }
    else
    {
        bitLow = (vectId - 12U) * 8U;
        CSL_FINSR(pIntcRegs->INTMUX3, bitLow + 6U, bitLow, eventId);
    }

    return;
}

/* Register Raise Privilege Access hook. */
void HwiP_registerRaisePrivilegeHandlerHook(HwiP_raisePrivilegeFxnPtr hookFxnPtr)
{
    gHwiRaisePrivilegeHook = hookFxnPtr;
}

/* Register Raise Privilege Access hook. */
void HwiP_registerRestorePrivilegeHandlerHook(HwiP_restorPrivilegeFxnPtr hookFxnPtr)
{
    gHwiRestorePrivilegeHook = hookFxnPtr;
}

