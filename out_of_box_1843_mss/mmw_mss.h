/**
 *   @file  mmw_mss.h
 *
 *   @brief
 *      This is the main header file for the Millimeter Wave Demo
 */
#ifndef MMW_MSS_H
#define MMW_MSS_H

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/common/mmwave_error.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/gpio/gpio.h>
#include <ti/drivers/mailbox/mailbox.h>

#include <ti/demo/utils/mmwdemo_adcconfig.h>
#include <ti/demo/utils/mmwdemo_monitor.h>
#include <ti/demo/xwr18xx/mmw/include/mmw_output.h>
#include "../out_of_box_1843_dss/objectdetection.h"

#include "mmw_config.h"
#include <ti/demo/xwr18xx/mmw/mss/mmw_lvds_stream.h>

/* Shared Custom DPU Header */
#include "dpu_custom.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MMWDEMO_SUBFRAME_NUM_FRAME_LEVEL_CONFIG (-1)
#define MMWDEMO_CFAR_THRESHOLD_ENCODING_FACTOR (100.0)

#define MMWDEMO_GUIMONSEL_OFFSET                 (offsetof(MmwDemo_SubFrameCfg, guiMonSel))
#define MMWDEMO_ADCBUFCFG_OFFSET                 (offsetof(MmwDemo_SubFrameCfg, adcBufCfg))
#define MMWDEMO_LVDSSTREAMCFG_OFFSET             (offsetof(MmwDemo_SubFrameCfg, lvdsStreamCfg))

#define MMWDEMO_SUBFRAME_DYNCFG_OFFSET           (offsetof(MmwDemo_SubFrameCfg, objDetDynCfg) + \
                                                  offsetof(MmwDemo_DPC_ObjDet_DynCfg, dynCfg))

#define MMWDEMO_CFARCFGRANGE_OFFSET              (MMWDEMO_SUBFRAME_DYNCFG_OFFSET + \
                                                  offsetof(DPC_ObjectDetection_DynCfg, cfarCfgRange))

#define MMWDEMO_CFARCFGDOPPLER_OFFSET            (MMWDEMO_SUBFRAME_DYNCFG_OFFSET + \
                                                  offsetof(DPC_ObjectDetection_DynCfg, cfarCfgDoppler))

#define MMWDEMO_FOVRANGE_OFFSET                  (MMWDEMO_SUBFRAME_DYNCFG_OFFSET + \
                                                  offsetof(DPC_ObjectDetection_DynCfg, fovRange))

#define MMWDEMO_FOVDOPPLER_OFFSET                (MMWDEMO_SUBFRAME_DYNCFG_OFFSET + \
                                                  offsetof(DPC_ObjectDetection_DynCfg, fovDoppler))

#define MMWDEMO_FOVAOA_OFFSET                    (MMWDEMO_SUBFRAME_DYNCFG_OFFSET + \
                                                  offsetof(DPC_ObjectDetection_DynCfg, fovAoaCfg))

#define MMWDEMO_EXTMAXVEL_OFFSET                 (MMWDEMO_SUBFRAME_DYNCFG_OFFSET + \
                                                  offsetof(DPC_ObjectDetection_DynCfg, extMaxVelCfg))

#define MMWDEMO_MULTIOBJBEAMFORMING_OFFSET       (MMWDEMO_SUBFRAME_DYNCFG_OFFSET + \
                                                  offsetof(DPC_ObjectDetection_DynCfg, multiObjBeamFormingCfg))

#define MMWDEMO_CALIBDCRANGESIG_OFFSET           (MMWDEMO_SUBFRAME_DYNCFG_OFFSET + \
                                                  offsetof(DPC_ObjectDetection_DynCfg, calibDcRangeSigCfg))

#define MMWDEMO_STATICCLUTTERREMOFVAL_OFFSET     (MMWDEMO_SUBFRAME_DYNCFG_OFFSET + \
                                                  offsetof(DPC_ObjectDetection_DynCfg, staticClutterRemovalCfg))

#define MMWDEMO_DPUCUSTOMCFG_OFFSET              (offsetof(MmwDemo_SubFrameCfg, dpuCustomCfg))

typedef enum MmwDemo_SensorState_e
{
    MmwDemo_SensorState_INIT = 0,
    MmwDemo_SensorState_OPENED,
    MmwDemo_SensorState_STARTED,
    MmwDemo_SensorState_STOPPED
}MmwDemo_SensorState;

typedef struct MmwDemo_MSS_Stats_t
{
    uint64_t     frameTriggerReady;
    uint32_t     failedTimingReports;
    uint32_t     calibrationReports;
    uint32_t     sensorStopped;
}MmwDemo_MSS_Stats;

typedef struct MmwDemo_SubFrameCfg_t
{
    MmwDemo_ADCBufCfg adcBufCfg;
    uint8_t isAdcBufCfgPending : 1;

    MmwDemo_LvdsStreamCfg lvdsStreamCfg;
    uint8_t isLvdsStreamCfgPending : 1;

    MmwDemo_GuiMonSel guiMonSel;

    MmwDemo_DPC_ObjDet_DynCfg objDetDynCfg;

    /* Decoupled Custom DPU Dynamic Configuration */
    DPC_ObjectDetection_DpuCustomCfg dpuCustomCfg;
    uint8_t isDpuCustomCfgPending : 1;

    uint16_t    numRangeBins;
    uint16_t    numDopplerBins;
    uint8_t     numChirpsPerChirpEvent;
    uint32_t    adcBufChanDataSize;
    uint32_t    sigImgMonTotalSize;
    uint32_t    satMonTotalSize;
    uint16_t    numAdcSamples;
    uint16_t    numChirpsPerSubFrame;
    uint8_t     numVirtualAntennas; 
} MmwDemo_SubFrameCfg;

typedef struct MmwDemo_SubFrameStats_t
{
    MmwDemo_output_message_stats    outputStats;
    uint32_t                        pendingConfigProcTime;
    uint32_t                        subFramePreparationTime;
} MmwDemo_SubFrameStats;

typedef struct MmwDemo_TaskHandles_t
{
    Task_Handle mmwaveCtrl;
    Task_Handle objDetDpmTask;
    Task_Handle initTask;
} MmwDemo_taskHandles;

typedef struct MmwDemo_temperatureStats_t
{
    int32_t        tempReportValid;
    rlRfTempData_t temperatureReport;
} MmwDemo_temperatureStats;

typedef struct MmwDemo_calibDataHeader_t
{
    uint32_t 	magic;
    uint32_t 	hdrLen;
    rlSwVersionParam_t 	linkVer;
    rlFwVersionParam_t 	radarSSVer;
    uint32_t 	dataLen;
    uint32_t      padding;
} MmwDemo_calibDataHeader;

typedef struct MmwDemo_calibCfg_t
{
    MmwDemo_calibDataHeader    calibDataHdr;
    uint32_t 		sizeOfCalibDataStorage;
    uint32_t 		saveEnable;
    uint32_t 		restoreEnable;
    uint32_t 		flashOffset;
} MmwDemo_calibCfg;

typedef struct MmwDemo_calibData_t
{
    MmwDemo_calibDataHeader    calibDataHdr;
    rlCalibrationData_t               calibData;
    rlPhShiftCalibrationData_t     phaseShiftCalibData;
} MmwDemo_calibData;

typedef struct MmwDemo_MSS_MCB_t
{
    MmwDemo_Cfg                 cfg;
    SOC_Handle                  socHandle;
    UART_Handle                 loggingUartHandle;
    UART_Handle                 commandUartHandle;
    MMWave_Handle             ctrlHandle;
    ADCBuf_Handle               adcBufHandle;
    EDMA_Handle                  edmaHandle;
    uint8_t                     numEdmaEventQueues;
    bool                        isPollEdmaError;
    bool                        isPollEdmaTransferControllerError;
    EDMA_errorInfo_t            EDMA_errorInfo;
    EDMA_transferControllerErrorInfo_t EDMA_transferControllerErrorInfo;
    DPM_Handle                  objDetDpmHandle;
    MmwDemo_DPC_ObjDet_CommonCfg objDetCommonCfg;
    MmwDemo_SubFrameCfg         subFrameCfg[RL_MAX_SUBFRAMES];
    MmwDemo_SubFrameStats       subFrameStats[RL_MAX_SUBFRAMES];
    MmwDemo_MSS_Stats           stats;
    MmwDemo_taskHandles         taskHandles;
    double                      rfFreqScaleFactor;
    Semaphore_Handle            DPMstartSemHandle;
    Semaphore_Handle            DPMstopSemHandle;
    Semaphore_Handle            DPMioctlSemHandle;
    MmwDemo_SensorState         sensorState;
    uint32_t                    sensorStartCount;
    uint32_t                    sensorStopCount;
    rlSigImgMonConf_t           cqSigImgMonCfg[RL_MAX_PROFILES_CNT];
    rlRxSatMonConf_t            cqSatMonCfg[RL_MAX_PROFILES_CNT];
    MmwDemo_AnaMonitorCfg       anaMonCfg;
    MmwDemo_LVDSStream_MCB_t    lvdsStream;
    MmwDemo_temperatureStats  temperatureStats;
    MmwDemo_calibCfg                calibCfg;
    uint8_t isAnaMonCfgPending : 1;
    uint8_t isCalibCfgPending : 1;
} MmwDemo_MSS_MCB;

extern int32_t MmwDemo_openSensor(bool isFirstTimeOpen);
extern int32_t MmwDemo_configSensor(void);
extern int32_t MmwDemo_startSensor(void);
extern void MmwDemo_stopSensor(void);

extern uint8_t MmwDemo_isAllCfgInPendingState(void);
extern uint8_t MmwDemo_isAllCfgInNonPendingState(void);
extern void MmwDemo_resetStaticCfgPendingState(void);
extern void MmwDemo_CfgUpdate(void *srcPtr, uint32_t offset, uint32_t size, int8_t subFrameNum);

extern void _MmwDemo_debugAssert(int32_t expression, const char *file, int32_t line);
#define MmwDemo_debugAssert(expression) {                                      \
                                         _MmwDemo_debugAssert(expression,      \
                                                  __FILE__, __LINE__);         \
                                         DebugP_assert(expression);             \
                                        }

#ifdef __cplusplus
}
#endif

#endif /* MMW_MSS_H */
