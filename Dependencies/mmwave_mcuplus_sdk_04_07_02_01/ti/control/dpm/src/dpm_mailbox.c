/**
 *   @file  dpm_mailbox.c
 *
 *   @brief
 *      DPM Remote/Distributed Domain Support through the Mailbox
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017-2025 Texas Instruments, Inc.
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
#include <stdint.h>
#include <string.h>

#include <kernel/dpl/DebugP.h>

#include <ti/common/syscommon.h>
#include <ti/control/dpm/dpm.h>
#include <ti/control/dpm/include/dpm_internal.h>

#define RPMSG_LOCAL_END_POINT    (RPMESSAGE_MAX_LOCAL_ENDPT - 1U)
#define RPMSG_REMOTE_END_POINT   (RPMESSAGE_MAX_LOCAL_ENDPT - 2U)

/**************************************************************************
 **************************** Local Functions *****************************
 **************************************************************************/
static void DPM_mboxRemoteMailboxCallbackFxn (RPMessage_Object *obj,
            void *arg, void *data, uint16_t dataLen, int32_t crcStatus,
            uint16_t remoteCoreId, uint16_t remoteEndPt);
static int32_t DPM_mboxInit (DPM_MCB* ptrDPM, int32_t* errCode);
static int32_t DPM_mboxSynch (DPM_MCB* ptrDPM, int32_t* errCode);
static int32_t DPM_mboxMsgSend (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode);
static int32_t DPM_mboxSendResult(DPM_MCB* ptrDPM, bool isAckNeeded, bool isRelayResult, DPM_Buffer* ptrResult, int32_t* errCode);
static int32_t DPM_mboxDeinit (DPM_MCB* ptrDPM, int32_t* errCode);

/**************************************************************************
 ********************** DPM Mailbox Domain Globals ************************
 **************************************************************************/

/**
 * @brief   Global DPM Remote/Distributed Domain Function Table
 */
DPM_DomainTable gDPMRemoteTable =
{
    &DPM_mboxInit,
    &DPM_mboxSynch,
    &DPM_mboxMsgSend,
    &DPM_mboxSendResult,
    &DPM_mboxDeinit
};

/**
 * @brief   Global DPM MCB: We need to use this since the
 * Mailbox does not allow an optional argument to be passed to the
 * callback function.
 */
DPM_MCB*  gDPM = NULL;

/**************************************************************************
 ********************** DPM Mailbox Domain Functions **********************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      The function is used to get the base address to the MSS CTRL register space
 *
 *  @retval
 *      pointer to the MSS CTRL Register space.
 */
CSL_mss_ctrlRegs* CSL_MSS_CTRL_getBaseAddress (void)
{
    return (CSL_mss_ctrlRegs*) CSL_MSS_CTRL_U_BASE;
}


/**
 *  @b Description
 *  @n
 *      This function is used to set the MSS Operational status of a
 *      module executing on the MSS.
 *
 *  @param[in] state
 *      Operational State (1 is operational and 0 is not operational)
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPM_setMSSLinkState(uint8_t state)
{
    CSL_mss_ctrlRegs *mssCtrl  = CSL_MSS_CTRL_getBaseAddress();
    int32_t             retVal = 0;

    if (mssCtrl != NULL)
    {
        CSL_FINSR (mssCtrl->MSS_BOOT_INFO_REG0, 0U, 0U, (uint32_t)state);
    }
    else
    {
        retVal = MINUS_ONE;
    }

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This function is used to check the MSS Operational status for the
 *      module.
 *
 *
 *  @retval
 *      Synchronized    -   1
 *  @retval
 *      Unsynchronized  -   0
 *  @retval
 *      Error           -   <0
 */
int32_t DPM_isMSSOperational(void)
{
    CSL_mss_ctrlRegs *mssCtrl  = CSL_MSS_CTRL_getBaseAddress();
    int32_t             retVal = MINUS_ONE;
    uint32_t            status;

    if (mssCtrl != NULL)
    {
        /* Get the operational status for the MSS */
        status = CSL_FEXTR (mssCtrl->MSS_BOOT_INFO_REG0, 0U, 0U);
        retVal = (int32_t) status;
    }
    return retVal;

}

/**
 *  @b Description
 *  @n
 *      This function is used to set the DSS Operational status for the
 *      module.
 *
 *  @param[in] state
 *      Operational State (1 is operational and 0 is not operational)
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPM_setDSSLinkState(uint8_t state)
{
    CSL_mss_ctrlRegs *mssCtrl  = CSL_MSS_CTRL_getBaseAddress();
    int32_t             retVal = 0;

    if (mssCtrl != NULL)
    {
        CSL_FINSR (mssCtrl->MSS_BOOT_INFO_REG0, 1U, 1U, (uint32_t)state);
    }
    else
    {
        retVal = MINUS_ONE;
    }

    return retVal;

}

/**
 *  @b Description
 *  @n
 *      This function is used to check the DSS Operational status for the
 *      module.
 *
 *  @retval
 *      Synchronized    -   1
 *  @retval
 *      Unsynchronized  -   0
 *  @retval
 *      Error           -   <0
 */
int32_t DPM_isDSSOperational(void)
{
    CSL_mss_ctrlRegs *mssCtrl  = CSL_MSS_CTRL_getBaseAddress();
    int32_t             retVal = MINUS_ONE;
    uint32_t            status;

    if (mssCtrl != NULL)
    {
        /* Get the operational status for the MSS */
        status = CSL_FEXTR (mssCtrl->MSS_BOOT_INFO_REG0, 1U, 1U);
        retVal = (int32_t) status;
    }
    return retVal;
}


/**
 *  @b Description
 *  @n
 *      This is the callback function which is registered with the mailbox
 *      and is invoked when a message is received from the remote peer.
 *
 *  @param[in]  obj
 *      RPMessage end point object
 *  @param[in]  arg
 *      Arguments specified by user
 *  @param[in]  data
 *      Pointer to message
 *  @param[in]  dataLen
 *      Length of message
 *  @param[in]  crcStatus
 *      CRC Check status. SystemP_SUCCESS on success, else SystemP_FAILURE.
 *  @param[in]  remoteCoreId
 *      Core ID of sender
 *  @param[in]  remoteEndPt
 *      End point of sender
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
static void DPM_mboxRemoteMailboxCallbackFxn (RPMessage_Object *obj,
            void *arg, void *data, uint16_t dataLen, int32_t crcStatus,
            uint16_t remoteCoreId, uint16_t remoteEndPt)
{
    int32_t     errCode;
    DPM_Msg     message;

    memcpy ((void*)&message.header, (void*)data, dataLen);

    /* Place the message into the pipe: This message will then be picked up and processed in the
     * context of the DPM Task. */
    (void)DPM_pipeSend (gDPM, &message, &errCode);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to send a message to the remote IPC Module
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[in] ptrMessage
 *      Pointer to the DPM message to be sent
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success   - 0
 *  @retval
 *      Error     - <0
 */
static int32_t DPM_mboxMsgSend (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode)
{
    int32_t     retVal = MINUS_ONE;
    volatile uint32_t    sizeMessage;

    /* Size of the message being sent out:
     * - Header + Payload (if any) */
    sizeMessage = sizeof(DPM_HeaderMsg) + ptrMessage->header.payloadLength;

    /* Send out the message: */
    retVal = RPMessage_send((void *)ptrMessage, (uint16_t)sizeMessage, ptrDPM->remoteCoreID, ptrDPM->remoteEndPoint,
                            RPMessage_getLocalEndPt(&ptrDPM->ipcMailboxObj),
                            SystemP_WAIT_FOREVER);

    if (retVal != SystemP_SUCCESS)
    {
        /* Error: Unable to send the message. */
        *errCode = retVal;
        goto exit;
    }

    /* Message was sent successfully. */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Device specific function which is used to send the processing
 *      chain results to the remote DPM domain
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[in] isAckNeeded
 *      Flag which indicates if the results need to be acknowledged
 *  @param[in] isRelayResult
 *      Flag which indicates if the results are being relayed or not
 *  @param[in] ptrResult
 *      Results which need to be sent to the remote domain
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
static int32_t DPM_mboxSendResult
(
    DPM_MCB*    ptrDPM,
    bool        isAckNeeded,
    bool        isRelayResult,
    DPM_Buffer* ptrResult,
    int32_t*    errCode
)
{
    DPM_Msg     message;

    /* Send out the result message to the peer domain */
    DPM_msgInitHeader (ptrDPM, &message, DPM_MessageId_RESULT, sizeof(DPM_ResultInfo));

    /* Is this a relayed result? */
    if (isRelayResult == true)
    {
        /* YES: Ovewrite the default message type */
        message.header.type = DPM_MessageType_RELAY;
    }

    message.u.resultInfoMsg.isAckNeeded    = isAckNeeded;
    memcpy ((void*)&message.u.resultInfoMsg.result, (void*)ptrResult, sizeof(DPM_Buffer));
    return DPM_mboxMsgSend (ptrDPM, &message, errCode);
}

/**
 *  @b Description
 *  @n
 *      The function is used to synchronize the DPM Module on the device.
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Synchronized    -   1
 *  @retval
 *      Unsynchronized  -   0
 *  @retval
 *      Error           -   <0
 */
static int32_t DPM_mboxSynch (DPM_MCB* ptrDPM, int32_t* errCode)
{

#ifdef SUBSYS_DSS
    return DPM_isMSSOperational();
#else
    return DPM_isDSSOperational();
#endif
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the DPM Remote IPC module.
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t DPM_mboxInit (DPM_MCB* ptrDPM, int32_t* errCode)
{
    int32_t         retVal = MINUS_ONE;
    RPMessage_CreateParams createParams;

    /* Setup the IPC Parameters */
    RPMessage_CreateParams_init(&createParams);

     /* Setup the remote endpoint: */
#ifdef SUBSYS_MSS
    createParams.localEndPt = RPMSG_LOCAL_END_POINT;
    ptrDPM->remoteEndPoint = RPMSG_REMOTE_END_POINT;
    ptrDPM->remoteCoreID = CSL_CORE_ID_C66SS0;
#else
    createParams.localEndPt = RPMSG_REMOTE_END_POINT;
    ptrDPM->remoteEndPoint = RPMSG_LOCAL_END_POINT;
    ptrDPM->remoteCoreID = CSL_CORE_ID_R5FSS0_0;
#endif

    /* Setup IPC callback configuration: */
    createParams.recvCallback         = DPM_mboxRemoteMailboxCallbackFxn;

    /* Open the Instance */
    retVal = RPMessage_construct(&ptrDPM->ipcMailboxObj, &createParams);
    if (retVal != SystemP_SUCCESS)
    {
        /* Error: Unable to open the mailbox */
        goto exit;
    }

    /* Setup the global: This is needed for the mailbox semaphore handle */
    gDPM = ptrDPM;

#ifdef SUBSYS_DSS
    (void)DPM_setDSSLinkState (1U);
#else
    (void)DPM_setMSSLinkState (1U);
#endif

    /* Setup the return value: The IPC module has been initialized successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the DPM Remote IPC module.
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t DPM_mboxDeinit (DPM_MCB* ptrDPM, int32_t* errCode)
{
    /* Close the mailbox: */
    RPMessage_destruct(&ptrDPM->ipcMailboxObj);

    /* Reset the global handle: */
    gDPM = NULL;

#ifdef SUBSYS_DSS
    return DPM_setDSSLinkState (0U);
#else
    return DPM_setMSSLinkState (0U);
#endif

}
