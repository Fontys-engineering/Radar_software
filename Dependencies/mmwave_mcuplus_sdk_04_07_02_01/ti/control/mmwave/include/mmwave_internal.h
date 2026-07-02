/**
 *   @file  mmwave_internal.h
 *
 *   @brief
 *      This is the internal Header for the mmWave module. This header file
 *      should *NOT* be directly included by applications.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016-2021 Texas Instruments, Inc.
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

/** @mainpage mmWave API
 *
 *  The mmWave API allow application developers to be abstracted from the lower
 *  layer drivers and the mmWave link API.
 *
 *  The mmWave file should be included in an application as follows:
 *
 *  @code

    #include <ti/control/mmwave/mmwave.h>

    @endcode
 *
 *  ## Initializing the module #
 *
 *  The mmWave module is initialized by using the #MMWave_init API
 *
 *  ## Opening the module #
 *
 *  After the mmWave module has been synchronized; the mmWave module needs to be
 *  opened. This will initialize the mmWave link to the BSS. While operating
 *  in minimal mode applications can invoke the mmWave link API directly after
 *  this step has been initiated.
 *
 *  ## Configuration of the module #
 *
 *  Applications are responsible for populate and configuring the BSS using
 *  the configuration #MMWave_config API. The API will take the application
 *  supplied configuration and will pass this to the BSS via the mmWave link
 *  API(s). Application developers are abstracted from the calling sequence and
 *  the various synchronization events which are required.
 *
 *  Once the configuration has been completed; the application can setup the
 *  data path. After the data path has been successfully configured application
 *  developers can start the mmWave.
 *
 *
 *  ## Starting the mmWave #
 *
 *  After successful configuration the mmWave needs to be started using the
 *  #MMWave_start API. On successful execution of the API the data path
 *  is being excercised.
 *
 *  ## Executing the mmWave module #
 *
 *  The mmWave module requires an execution context which needs to be provided
 *  by the application. This is because there are asynchronous events and
 *  response messages which are received by the BSS using the mmWave Link
 *  infrastructure. Thes need to be handled and processed in the application
 *  supplied execution context.
 *
 *  Failure to provide and execution context and not invoking the
 *  #MMWave_execute API can result in the mmWave API getting stuck
 *
 *  ## Error Code #
 *  The mmWave API return an encoded error code. The encoded error code has
 *  the following information:-
 *  - Error or Informational message
 *  - mmWave error code
 *  - Subsystem error code
 *  The mmWave module is a high level control module which is basically layered
 *  over multiple modules like the mmWave Link, Mailbox etc. When an mmWave API
 *  reports a failure it could be because of a multitude of reasons. Also the
 *  mmWave Link API reports certains errors as not fatal but informational. In
 *  order to satisfy these requirements the error code returned by the mmWave API
 *  is encoded. There exists a #MMWave_decodeError which can be used to determine
 *  the exact error code and error level.
 */

#ifndef MMWAVE_INTERNAL_H
#define MMWAVE_INTERNAL_H

#include <kernel/dpl/SemaphoreP.h>
#include <ti/control/mmwave/include/mmwave_listlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup MMWAVE_INTERNAL_DEFINITIONS
 @{ */

/**
 * @brief
 *  This defines the maximum number of entries in the spawn list.
 */
#define MMWAVE_MAX_NUM_SPAWN_LIST                   4U

/**
 * @brief   Status flag to indicate the open state
 */
#define MMWAVE_STATUS_OPENED                        1U

/**
 * @brief   Status flag to indicate the configured state
 */
#define MMWAVE_STATUS_CONFIGURED                    2U


/**
 * @brief   Status flag to indicate the started state
 */
#define MMWAVE_STATUS_STARTED                       8U

/**
 * @brief   Link Status flag used to indicate that the RF was initialized successfully
 */
#define MMWAVE_RF_INITIALIZED                       1U

/**
 * @brief   Link Status flag used to indicate that the RF initialization failed.
 */
#define MMWAVE_RF_INIT_FAILED                       2U

/**
 * @brief   Link Status flag used to indicate that the RF Calibration completed
 */
#define MMWAVE_RF_CALIBRATION_DONE                  4U

/**
 * @brief   Link Status flag used to indicate that the Calibration failed
 */
#define MMWAVE_RF_CALIBRATION_FAILED                8U

/**
 * @brief   Link Status flag used to indicate that the ESM fault was detected
 */
#define MMWAVE_RF_ESM_FAULT                         16U

/**
 * @brief   Link Status flag used to indicate that the CPU fault was detected
 */
#define MMWAVE_RF_CPU_FAULT                         32U

/**
 * @brief   Link Status flag used to indicate that the Analog fault was detected
 */
#define MMWAVE_RF_ANALOG_FAULT                      64U

/**
 * @brief   Link Status flag used to indicate that the mmwavelink detected protocol 
 *          error in async report
 */
#define MMWAVE_LINK_ASYNC_EVENT_MISMATCH_ERROR      128U

/**
 * @brief   Link Status flag used to indicate that the mmwavelink detected internal 
 *          error in async report
 */
#define MMWAVE_LINK_ASYNC_EVENT_INTERNAL_ERROR      256U

/**
 * @brief   Front End MSS Status flag used to indicate that MSS power up is done
 */
#define MMWAVE_FRONT_END_MSS_POWER_UP_DONE     1U
/**
 * @brief   Front End MSS Status flag used to indicate CPU fault
 */
#define MMWAVE_FRONT_END_MSS_CPU_FAULT         2U

/**
 * @brief   Front End MSS Status flag used to indicate RF power up done
 */
#define MMWAVE_FRONT_END_MSS_RF_POWER_UP_DONE  4U

/**
 * @brief   Front End MSS Status flag used to indicate ESM fault
 */
#define MMWAVE_FRONT_END_MSS_ESM_FAULT         8U

/**
 * @brief   Front End MSS Status flag used to indicate boot error
 */
#define MMWAVE_FRONT_END_MSS_BOOT_ERROR        16U

/**
 * @brief   Status flag to indicate the synchronization state
 */
#define MMWAVE_STATUS_SYNCHRONIZED                  4U


/**
@}
*/

/** @addtogroup MMWAVE_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  mmWave Spawn function entry
 *
 * @details
 *  The structure is used to keep track of the spawn functions which
 *  need to be executed by the control module.
 */
typedef struct MMWave_SpawnFxnNode_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode         links;

    /**
     * @brief   Spawn function entry
     */
    RL_P_OSI_SPAWN_ENTRY    spawnEntry;

    /**
     * @brief   Argument passed to the spawn function
     */
    const void*             arg;
}MMWave_SpawnFxnNode;

/**
 * @brief
 *  mmWave Chirp configuration
 *
 * @details
 *  The structure is used to keep track of the chirps which
 *  have been configured in the system.
 */
typedef struct MMWave_Chirp_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode             links;

    /**
     * @brief   Chirp configuration
     */
    rlChirpCfg_t                chirpCfg;

    /**
     * @brief   Profile for which the chirp is configured
     */
    struct MMWave_Profile_t*    ptrMMWaveProfile;
}MMWave_Chirp;

/**
 * @brief
 *  mmWave Profile configuration
 *
 * @details
 *  The structure is used to keep track of the profiles
 */
typedef struct MMWave_Profile_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode         links;

    /**
     * @brief   Profile configuration
     */
    rlProfileCfg_t          profileCfg;

    /**
     * @brief   Link to the mmWave
     */
    struct MMWave_MCB_t*    ptrMMWaveMCB;

    /**
     * @brief   Number of chirps
     */
    uint32_t                numChirps;

    /**
     * @brief   List of all the chirps associated with the profile
     */
    MMWave_Chirp*           ptrChirpList;
}MMWave_Profile;

/**
 * @brief
 *  mmWave Advance chirp configuration
 *
 * @details
 *  The structure is used to keep track of the advanced chirp configurations
 */
typedef struct MMWave_AdvChirp_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode         links;

    /**
     * @brief   Advanced Chirp Configuration
     */
    rlAdvChirpCfg_t         advChirpCfg;

    /**
     * @brief   Link to the mmWave
     */
    struct MMWave_MCB_t*    ptrMMWaveMCB;

    /**
     * @brief   Array of all the lut parameters associated with the chirp parameter
     */
    rlInt32_t*              ptrLUTData;

}MMWave_AdvChirp;

/**
 * @brief
 *  mmWave BPM configuration
 *
 * @details
 *  The structure is used to keep track of the BPM configurations
 */
typedef struct MMWave_BpmChirp_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode         links;

    /**
     * @brief   Profile configuration
     */
    rlBpmChirpCfg_t         bpmChirp;

    /**
     * @brief   Link to the mmWave
     */
    struct MMWave_MCB_t*    ptrMMWaveMCB;

}MMWave_BpmChirp;

/**
 * @brief
 *  mmWave chirp level phase shifter configuration
 *
 * @details
 *  The structure is used to keep track of the chirp level phase shifter configurations
 */
typedef struct MMWave_phaseShiftChirp_t
{
    /**
     * @brief   Links to the other elements in the list
     */
    MMWave_ListNode         links;

    /**
     * @brief   chirp level phase shifter configuration
     */
    rlRfPhaseShiftCfg_t     phaseShiftChirp;

    /**
     * @brief   Link to the mmWave
     */
    struct MMWave_MCB_t*    ptrMMWaveMCB;

}MMWave_phaseShiftChirp;

/**
 * @brief
 *  mmWave debug stats
 *
 * @details
 *  The structure is used to store relevant debug stats.
 */
typedef struct MMWave_debugStats_t
{
    /**
     * @brief   Number of received IRQs
     */
    uint16_t   numIRQReceived;

    /**
     * @brief   Number of control channel writes
     */
    uint16_t   numControlChannelWrite;

    /**
     * @brief   Number of control channel reads
     */
    uint16_t   numControlChannelRead;

    /**
     * @brief   Number of Async events received
     */
    uint16_t   numAsyncEvents;

    /**
     * @brief   Number of Internal error Async events received
     */
    uint16_t   numErrAsyncEvents;
}MMWave_debugStats;

/**
 * @brief
 *  mmWave OSAL semaphore object
 *
 * @details
 *  The structure is used to store Semaphore/mutex objects used.
 */
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
 * @brief
 *  mmWave Control MCB
 *
 * @details
 *  The structure is used to store all the relevant information required
 *  to execute the mmWave control module.
 */
typedef struct MMWave_MCB_t
{
    /**
     * @brief   Initialization configuration which was used to initialize the
     * control module.
     */
    MMWave_InitCfg              initCfg;

    /**
     * @brief   Configuration which is used to open the control module.
     */
    MMWave_OpenCfg              openCfg;

    /**
     * @brief   Calibration configuration which is used to start the control module
     */
    MMWave_CalibrationCfg       calibrationCfg;

    /**
     * @brief   DFE Data output mode: This is valid only when the mmWave module is in
     * the following states:-
     *  Full Configuration    - Configured State
     *  Minimal Configuration - Start State
     */
    MMWave_DFEDataOutputMode    dfeDataOutputMode;

    /**
     * @brief   This is the semaphore handle which is used to handle the messages exchanged
     * between the MSS/DSS and BSS.
     */
    SemaphoreP_Object           linkSemHandle;

    /**
     * @brief   This is the semaphore handle which is used to protect the chirp/profile
     * configuration while operating in FULL configuration mode.
     */
    SemaphoreP_Object           cfgSemHandle;

    /**
     * @brief   This is the spawn table used by the free & active lists.
     */
    MMWave_SpawnFxnNode         spawnTable[MMWAVE_MAX_NUM_SPAWN_LIST];

    /**
     * @brief   List which tracks all the nodes in the spawn free list.
     */
    MMWave_SpawnFxnNode*        ptrSpawnFxnFreeList;

    /**
     * @brief   List which tracks all the nodes in the spawn list which are
     * to be executed.
     */
    MMWave_SpawnFxnNode*        ptrSpawnFxnActiveList;

    /**
     * @brief   List which tracks all the profiles which have been created
     */
    MMWave_Profile*             ptrProfileList;
    
    /**
     * @brief  List which tracks all the advanced chirp parameters which have been created
     */
    MMWave_AdvChirp*            ptrAdvChirpList;

    /**
     * @brief   List which tracks all the BPM configurations which have been created
     */
    MMWave_BpmChirp*            ptrBpmChirpList;

    /**
     * @brief   List which tracks all the chirp level phase shifter configurations which have been created
     */
    MMWave_phaseShiftChirp*     ptrPhaseShiftChirpList;

    /**
     * @brief   Status of the mmWave Link: This can be changed from multiple execution
     * contexts
     */
    volatile uint32_t           linkStatus[MMWAVE_RADAR_DEVICES];

    /**
     * @brief   Status of the mmWave control module:
     */
    uint32_t                    status;

    /**
     * @brief   Counter which tracks the number of times the spawn list overflowed
     */
    uint32_t                    spawnOverflow;

    /**
     * @brief   Counter which tracks the number of times the spawn was invoked
     */
    uint32_t                    spawnCounter;

    /**
     * @brief   Counter which tracks the number of times the write reset was invoked
     */
    uint32_t                    wResetCounter;

    /**
     * @brief   Version Information which is stored once the link is operational
     */
    rlVersion_t                 version;
        
    /**
    * @brief   Debug stats. 
    */
    MMWave_debugStats           debugStats;
    
    /**
     * @brief   Status of the front end MSS. Valid for external RF front ends such as AWR22xx.
     * contexts
     */
    volatile uint32_t           frontEndMssStatus[MMWAVE_RADAR_DEVICES];
    
    /**
     * @brief   mmWaveLink Event Handler callback
     */
    RL_P_EVENT_HANDLER          mmwavelinkInterruptFunc;
    
    /**
     * @brief   Bitmap of devices to send the message. Check the mmwavelink documentation.
     */
    rlUInt8_t                   deviceMap;

    /**
     * @brief   Mutex object structure.
     */
    MMWave_osalSem              osalMutex[MMWAVE_RADAR_DEVICES];

    /**
     * @brief   Semaphore object structure.
     */
    MMWave_osalSem              osalSem[MMWAVE_RADAR_DEVICES];

}MMWave_MCB;

/**
@}
*/

/* Global mmWave Control MCB: */
extern MMWave_MCB           gMMWave_MCB;

/***************************************************************************************
 * Internal Link Exported API:
 ***************************************************************************************/
extern int32_t MMWave_initLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_deinitLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_openLink (MMWave_MCB* ptrMMWaveMCB, MMWave_CalibrationData* ptrCalibrationData, int32_t* errCode);
extern int32_t MMWave_closeLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_executeLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_configLink (MMWave_MCB* ptrMMWaveMCB, MMWave_CtrlCfg* ptrControlCfg, int32_t* errCode);
extern int32_t MMWave_startLink (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern int32_t MMWave_stopLink (const MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);

/***************************************************************************************
 * Internal common APIs:
 ***************************************************************************************/
extern rlInt32_t MMWave_spawn(RL_P_OSI_SPAWN_ENTRY fxn, const void* pValue, uint32_t flags);
extern rlInt32_t MMWave_computeCRC(rlUInt8_t* ptrData, rlUInt32_t dataLen, rlUInt8_t crcType, rlUInt8_t* crc);
extern void MMWave_asyncEventHandler(uint8_t devIndex, uint16_t sbId, uint16_t sbLen, uint8_t* payload);
extern int32_t MMWave_initCRC (MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);
extern void    MMWave_internalLinkSync(const MMWave_MCB* ptrMMWaveMCB, uint32_t flag);
extern int32_t MMWave_deviceGetVersion(MMWave_MCB* ptrMMWaveMCB, int32_t* errCode);

/***************************************************************************************
 * Error Management API:
 ***************************************************************************************/
extern int32_t MMWave_encodeError (MMWave_ErrorLevel errorLevel, int32_t mmWaveError, int32_t subSysError);
extern MMWave_ErrorLevel MMWave_decodeErrorLevel (int32_t errCode);

/***************************************************************************************
 * Link OSAL Exported API:
 ***************************************************************************************/
extern rlInt32_t MMWave_osalMutexCreate(rlOsiMutexHdl_t* mutexHandle, rlInt8_t* name);
extern rlInt32_t MMWave_osalMutexLock(rlOsiMutexHdl_t* mutexHandle, rlOsiTime_t timeout);
extern rlInt32_t MMWave_osalMutexUnlock(rlOsiMutexHdl_t* mutexHandle);
extern rlInt32_t MMWave_osalMutexDelete(rlOsiMutexHdl_t* mutexHandle);
extern rlInt32_t MMWave_osalSemCreate(rlOsiSemHdl_t* semHandle, rlInt8_t* name);
extern rlInt32_t MMWave_osalSemWait(rlOsiSemHdl_t* semHandle, rlOsiTime_t timeout);
extern rlInt32_t MMWave_osalSemSignal(rlOsiSemHdl_t* semHandle);
extern rlInt32_t MMWave_osalSemDelete(rlOsiSemHdl_t* semHandle);

#ifdef __cplusplus
}
#endif

#endif /* MMWAVE_INTERNAL_H */

