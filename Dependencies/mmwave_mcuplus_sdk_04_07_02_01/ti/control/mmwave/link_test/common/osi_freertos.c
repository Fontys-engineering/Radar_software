/**
 *   @file  osi_freertos.c
 *
 *   @brief
 *      The file implements the mmWave link operating interface layer.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016-2020 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* mmWave SDK Include Files */
#include <ti/common/syscommon.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwave/link_test/common/mmwavelink_test_common.h>

/* OS Porting driver files */
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/CycleCounterP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/SemaphoreP.h>
#include "FreeRTOS.h"
#include "task.h"

/**************************************************************************
 ************************* Ctrl OSAL Functions ****************************
 **************************************************************************/


typedef struct MMWave_osalSem_t
{
    /**
     * @brief   Semahore object
     */
    SemaphoreP_Object           object;

    /**
     * @brief   Flag to check if semaphore is alread allocated.
     */
    uint32_t                    isUsed;
}MMWave_osalSem;


/**
 * @brief   Mutex object structure.
 */
MMWave_osalSem              osalMutex;

/**
 * @brief   Semaphore object structure.
 */
MMWave_osalSem              osalSem;

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered OSAL function which is used
 *      to create a mutex.
 *
 *  @param[out]  mutexHandle
 *      Handle to the mutex object which is to be created
 *  @param[in]   name
 *      Name of the mutex object
 *
 *  \ingroup MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -  <0
 */
rlInt32_t Osal_mutexCreate(rlOsiMutexHdl_t* mutexHandle, rlInt8_t* name)
{
    int32_t  retVal = MINUS_ONE;

    retVal = SemaphoreP_constructMutex(&osalMutex.object);

    if(retVal == SystemP_FAILURE)
    {
       /* Error: Unable to create the semaphore */
       retVal       = MINUS_ONE;
       *mutexHandle = NULL;
    }
    else
    {
       /* Successfully created the semaphore */
       retVal    = 0;
       *mutexHandle = (rlOsiMutexHdl_t*)&osalMutex.object;
    }

    return (rlInt32_t)retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered OSAL function which is used
 *      to lock the mutex
 *
 *  @param[in]  mutexHandle
 *      Handle to the mutex object
 *  @param[in]  timeout
 *      Maximum timeout to wait for the mutex
 *
 *  \ingroup MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -  <0
 */
rlInt32_t Osal_mutexLock(rlOsiMutexHdl_t* mutexHandle, rlOsiTime_t timeout)
{
    uint32_t   semTimeout;

    /* Translate the timeout from mmWave link format to the semaphore OSAL module */
    if (timeout == RL_OSI_WAIT_FOREVER)
    {
        /* Semaphore timeout is set to wait forever */
        semTimeout = SystemP_WAIT_FOREVER;
    }
    else
    {
        /* Set the semaphore timeout. */
        semTimeout = timeout;
    }

    /* Pend on the semaphore: */
    SemaphoreP_pend((SemaphoreP_Object*)(*mutexHandle), semTimeout);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered OSAL function which is used
 *      to unlock the mutex
 *
 *  @param[in]  mutexHandle
 *      Handle to the mutex object
 *
 *  \ingroup MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -  <0
 */
rlInt32_t Osal_mutexUnlock(rlOsiMutexHdl_t* mutexHandle)
{
    /* Post the semaphore */
    SemaphoreP_post((SemaphoreP_Object *)(*mutexHandle));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered OSAL function which is used
 *      to delete the mutex
 *
 *  @param[in]  mutexHandle
 *      Handle to the mutex object
 *
 *  \ingroup MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -  <0
 */
rlInt32_t Osal_mutexDelete(rlOsiMutexHdl_t* mutexHandle)
{
    /* Delete the semaphore: */
    SemaphoreP_destruct((SemaphoreP_Object*)(*mutexHandle));

    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered OSAL function which is used
 *      to create a semaphore
 *
 *  @param[out]  semHandle
 *      Handle to the semaphore object which is to be created
 *  @param[in]   name
 *      Name of the semaphore object
 *
 *  \ingroup MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -  <0
 */
rlInt32_t Osal_semCreate(rlOsiSemHdl_t* semHandle, rlInt8_t* name)
{
    int32_t              retVal = MINUS_ONE;

    retVal = SemaphoreP_constructBinary(&osalSem.object, 0);

    if(retVal == SystemP_FAILURE)
    {
        /* Error: Unable to create the semaphore */
        retVal       = MINUS_ONE;
        *semHandle = NULL;
    }
    else
    {
        /* Successfully created the semaphore */
        retVal    = 0;
        *semHandle = (rlOsiSemHdl_t*)&osalSem.object;
    }

    return (rlInt32_t)retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered OSAL function which is used
 *      to wait on the semaphore object.
 *
 *  @param[in]  semHandle
 *      Handle to the semaphore
 *  @param[in]  timeout
 *      Maximum timeout to wait for the semaphore
 *
 *  \ingroup MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -  <0
 */
rlInt32_t Osal_semWait(rlOsiSemHdl_t* semHandle, rlOsiTime_t timeout)
{
    uint32_t   semTimeout;

    /* Translate the timeout from mmWave link format to the semaphore OSAL module */
    if (timeout == RL_OSI_WAIT_FOREVER)
    {
        /* Semaphore timeout is set to wait forever */
        semTimeout = SystemP_WAIT_FOREVER;
    }
    else
    {
        /* Set the semaphore timeout. */
        semTimeout = timeout;
    }

    /* Pend on the semaphore: */
    return(SemaphoreP_pend((SemaphoreP_Object*)(*semHandle), semTimeout));

    
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered OSAL function which is used
 *      to signal the semaphore object
 *
 *  @param[in]  semHandle
 *      Handle to the semaphore
 *
 *  \ingroup MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -  <0
 */
rlInt32_t Osal_semSignal(rlOsiSemHdl_t* semHandle)
{
    /* Post the semaphore */
    SemaphoreP_post((SemaphoreP_Object*)(*semHandle));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the mmWave link registered OSAL function which is used
 *      to delete the semaphore object
 *
 *  @param[in]  semHandle
 *      Handle to the semaphore
 *
 *  \ingroup MMWAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -  <0
 */
rlInt32_t Osal_semDelete(rlOsiSemHdl_t* semHandle)
{
    /* Delete the semaphore: */
    SemaphoreP_destruct((SemaphoreP_Object*)(*semHandle));

    return 0;
}

