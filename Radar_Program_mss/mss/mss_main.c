/**************************************************************************
  *************************** Include Files ********************************
  **************************************************************************/
  
 /* Standard Include Files. */
 #include <stdint.h>
 #include <stdlib.h>
 #include <stddef.h>
 #include <string.h>
 #include <stdio.h>
 #include <math.h>
  
 /* MCU+SDK include files. */
 #include <drivers/uart.h>
 #include <kernel/dpl/CacheP.h>
 #include <kernel/dpl/ClockP.h>
 #include <kernel/dpl/CycleCounterP.h>
 #include <kernel/dpl/AddrTranslateP.h>
 #include <kernel/dpl/DebugP.h>
 #include "FreeRTOS.h"
 #include "task.h"
  
 /* mmWave SDK Include Files: */
 #include <ti/common/syscommon.h>
 #include <ti/common/mmwavesdk_version.h>
 #include <ti/control/mmwave/mmwave.h>
 #include <ti/control/dpm/dpm.h>
 #include <ti/utils/cli/cli.h>
 #include <ti/utils/mathutils/mathutils.h>
 #include <ti/utils/testlogger/logger.h>
  
 /* Demo Include Files */
 #include <ti/demo/awr294x/mmw/include/mmw_config.h>
 #include <ti/demo/utils/mmwdemo_rfparser.h>
 #include <ti/demo/utils/mmwdemo_adcconfig.h>
 #include <ti/demo/utils/mmwdemo_monitor.h>
 #include <ti/demo/utils/enet_stream.h>
 #ifdef MMWDEMO_TDM
 #include <ti/demo/awr294x/mmw/mmw_resTDM.h>
 #else
 #include <ti/demo/awr294x/mmw/mmw_resDDM.h>
 #endif
 #include <ti/demo/awr294x/mmw/mss/mmw_mss.h>
 #include <ti/demo/awr294x/mmw/include/mmw_output.h>
 #include <ti/board/antenna_geometry.h>
 #include <ti/demo/utils/mmwdemo_flash.h>
  
 /* Profiler Include Files */
 #include <kernel/dpl/CycleCounterP.h>
  
 /* Demo tasks should have priority higher than enet/lwip tasks */
 #ifdef ENET_STREAM
 #define MMWDEMO_CLI_TASK_PRIORITY                 7
 #define MMWDEMO_UART_EXPORT_TASK_PRIORITY         8
 #define MMWDEMO_DPC_OBJDET_DPM_TASK_PRIORITY      9
 #define MMWDEMO_MMWAVE_CTRL_TASK_PRIORITY         10
 #define MMWDEMO_MMWAVE_ENET_TASK_PRIORITY         1
 #else
 #define MMWDEMO_CLI_TASK_PRIORITY                 3
 #define MMWDEMO_UART_EXPORT_TASK_PRIORITY         4
 #define MMWDEMO_DPC_OBJDET_DPM_TASK_PRIORITY      5
 #define MMWDEMO_MMWAVE_CTRL_TASK_PRIORITY         6
 #endif
  
 #if (MMWDEMO_CLI_TASK_PRIORITY >= MMWDEMO_DPC_OBJDET_DPM_TASK_PRIORITY)
 #error CLI task priority must be < Object Detection DPM task priority
 #endif
  
 #define DPC_OBJDET_INSTANCEID       (0xFEEDFEED)
 extern SemaphoreP_Object objDataSemaphoreHandle;
  
 /* These address offsets are in bytes, when configure address offset in hardware,
    these values will be converted to number of 128bits
    Buffer at offset 0x0U is reserved by BSS, hence offset starts from 0x200
  */
 #define MMW_DEMO_CQ_SIGIMG_ADDR_OFFSET          0x200U
 #define MMW_DEMO_CQ_RXSAT_ADDR_OFFSET           0x400U
  
 /* CQ data is at 16 bytes alignment for mulitple chirps */
 #define MMW_DEMO_CQ_DATA_ALIGNMENT            16U
  
  
 #define MAX_MOD_FREQ_DIVIDER_MANTISSA         127U
  
 #ifdef ENET_STREAM
 /* Variable to store detected object data for ethernet streaming */
 MmwDemo_enetStreamObjData gEnetStreamObjData;
 #endif
  
 /* ECC AGG Macros */
 #define MMWDEMO_MSS_ECC_AGG_MSS_ECC_VECTOR_ADDR             (CSL_MSS_ECC_AGG_MSS_U_BASE + CSL_MSS_ECC_AGG_MSS_ECC_VECTOR)
 #define MMWDEMO_MSS_ECC_AGG_MSS_ERROR_STATUS1_ADDR          (CSL_MSS_ECC_AGG_MSS_U_BASE + CSL_MSS_ECC_AGG_MSS_ERROR_STATUS1)
  
 #define MMWDEMO_MSS_ECC_AGG_RAM_IDS_TOTAL_ENTRIES           4U
 /**************************************************************************
  *************************** Global Definitions ***************************
  **************************************************************************/
 /* FreeRTOS Task declarations. */
 #define MMWDEMO_INIT_TASK_PRI         (1U) // 1U > 11U
  
 #define MMWDEMO_INIT_TASK_STACK_SIZE  (2*1024U)
 #define MMWDEMO_MMWAVE_CTRL_TASK_STACK_SIZE (2*1024U)
 #define MMWDEMO_DPC_OBJDET_DPM_TASK_STACK_SIZE (2*1024U)
 #define MMWDEMO_UART_DATA_EXPORT_TASK_STACK_SIZE (2*1024U)
 #ifdef ENET_STREAM
 #define MMWDEMO_MMWAVE_ENET_TASK_STACK_SIZE (2*1024U)
 #endif
  
 /* Application task stack variables */
 StackType_t gAppMainTskStack[MMWDEMO_INIT_TASK_STACK_SIZE] __attribute__((aligned(32)));
 StackType_t gMmwCtrlTskStack[MMWDEMO_MMWAVE_CTRL_TASK_STACK_SIZE] __attribute__((aligned(32)));
 StackType_t gDpmTskStack[MMWDEMO_DPC_OBJDET_DPM_TASK_STACK_SIZE] __attribute__((aligned(32)));
 StackType_t gUartTskStack[MMWDEMO_UART_DATA_EXPORT_TASK_STACK_SIZE] __attribute__((aligned(32)));
 #ifdef ENET_STREAM
 StackType_t gMmwEnetTskStack[MMWDEMO_MMWAVE_ENET_TASK_STACK_SIZE] __attribute__((aligned(32)));
 #endif
  
 MmwDemo_MSS_MCB    gMmwMssMCB;
  
 /* RF scale factor that can be used to translate
  * RF frequency related (start frequency, frequency slope, frequency constant etc)
  * configuration expressed in user-friendly units (like GHz/MHz) into units
  * that are required for mmwavelink (and therefore MMWave) APIs related to
  * such frequency configuration. It depends on whether the device is 60 GHz
  * or 77 GHz device.*/
 #define MMWDEMO_RF_FREQ_SCALE_FACTOR              3.6f
  
 MmwDemo_HSRAM gHSRAM __attribute__((aligned(64U), section(".demoSharedMem")));
  
  
 /* Calibration Data Save/Restore defines */
 #define MMWDEMO_CALIB_FLASH_SIZE                      4096
 #define MMWDEMO_CALIB_STORE_MAGIC            (0x7CB28DF9U)
  
 /* ADC Data Dithering MACROS */
 /* 1 LSB in "RSS_CTRL::ADCBUFCFG1_EXTD_ADCBUFINTGENDLY" = 3 Clocks = 20 ns Delay (SYS_CLK = 150MHz)*/
 /* 1 LSB in "RSS_CTRL::ADCBUFCFG1_EXTD_ADCBUFINTGENDLY" = 4 Clocks = 20 ns Delay (SYS_CLK = 200MHz)*/
 #define MMWDEMO_DITHERING_MINDELAY            55U
  
 MmwDemo_calibData gCalibDataStorage __attribute__((aligned(8)));
  
 static void MmwDemo_checkEdmaErrors(void);
 static void MmwDemo_clearEccAggrs(void);
  
 /**************************************************************************
  *************************** Extern Definitions ***************************
  **************************************************************************/
  
 extern void MmwDemo_CLIInit(uint8_t taskPriority);
 #ifdef MMWDEMO_DDM
 extern MmwDemo_RFParserHwAttr MmwDemo_RFParserHwCfg;
 #endif
  
 /**************************************************************************
  ************************* Millimeter Wave Demo Functions prototype *************
  **************************************************************************/
  
 /* MMW demo functions for datapath operation */
 static int32_t MmwDemo_dataPathConfig (void);
 static void MmwDemo_dataPathStart (void);
 static void MmwDemo_dataPathStop (void);
 void MmwDemo_handleObjectDetResult(void);
 static void MmwDemo_DPC_ObjectDetection_reportFxn
 (
     DPM_Report  reportType,
     uint32_t    instanceId,
     int32_t     errCode,
     uint32_t    arg0,
     uint32_t    arg1
 );
 static void MmwDemo_transmitProcessedOutput
 (
     UART_Handle     uartHandle,
     DPC_ObjectDetection_ExecuteResult   *result,
     MmwDemo_output_message_stats        *timingInfo
 );
  
 static void MmwDemo_measurementResultOutput(void* compRxChanCfg);
 #ifdef MMWDEMO_TDM
 static int32_t MmwDemo_processPendingDynamicCfgCommands(uint8_t subFrameIndx);
 #endif
  
 static int32_t MmwDemo_DPM_ioctl_blocking
 (
     DPM_Handle handle,
     uint32_t cmd,
     void* arg,
     uint32_t argLen
 );
  
 /* Mmwave demo init functions */
 static void MmwDemo_initTask(void* args);
 static void MmwDemo_platformInit(MmwDemo_platformCfg *config);
 static bool MmwDemo_BoardInit(void);
  
 /* Mmwave control functions */
 static void MmwDemo_mmWaveCtrlTask(void* args);
 static int32_t MmwDemo_mmWaveCtrlStop (void);
 static int32_t MmwDemo_eventCallbackFxn(uint8_t devIndex, uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload);
  
 #ifdef MMWDEMO_DDM
 int32_t MmwDemo_getNumEmptySubBands(uint32_t numTxAntennas);
 #endif
  
 /* CQ config function. */
 static int32_t MmwDemo_configCQ(MmwDemo_SubFrameCfg *subFrameCfg,
                                            uint8_t numChirpsPerChirpEvent,
                                            uint8_t validProfileIdx);
  
 /* Calibration save/restore APIs */
 static int32_t MmwDemo_calibInit(void);
 static int32_t MmwDemo_calibSave(MmwDemo_calibDataHeader *ptrCalibDataHdr, MmwDemo_calibData  *ptrCalibrationData);
 static int32_t MmwDemo_calibRestore(MmwDemo_calibData  *calibrationData);
  
 volatile uint32_t transmitStartTime =0;
 /**************************************************************************
  ************************* Millimeter Wave Demo Functions **********************
  **************************************************************************/
 void _MmwDemo_debugAssert(int32_t expression, const char *file, int32_t line)
 {
     if (!expression) {
         CLI_write ("Exception: %s, line %d.\n",file,line);
     }
 }
  
 static void MmwDemo_setSubFramePendingState(MmwDemo_SubFrameCfg *subFrameCfg, uint32_t offset)
 {
     switch (offset)
     {
 #if defined(MMWDEMO_TDM)
         case MMWDEMO_GUIMONSEL_OFFSET:
             subFrameCfg->objDetDynCfg.isPrepareRangeAzimuthHeatMapPending = 1;
         break;
         case MMWDEMO_CFARCFGRANGE_OFFSET:
             subFrameCfg->objDetDynCfg.isCfarCfgRangePending = 1;
         break;
         case MMWDEMO_CFARCFGDOPPLER_OFFSET:
             subFrameCfg->objDetDynCfg.isCfarCfgDopplerPending = 1;
         break;
         case MMWDEMO_FOVRANGE_OFFSET:
             subFrameCfg->objDetDynCfg.isFovRangePending = 1;
         break;
         case MMWDEMO_FOVDOPPLER_OFFSET:
             subFrameCfg->objDetDynCfg.isFovDopplerPending = 1;
         break;
         case MMWDEMO_FOVAOA_OFFSET:
             subFrameCfg->objDetDynCfg.isFovAoaCfgPending = 1;
         break;
         case MMWDEMO_MULTIOBJBEAMFORMING_OFFSET:
             subFrameCfg->objDetDynCfg.isMultiObjBeamFormingCfgPending = 1;
         break;
         case MMWDEMO_CALIBDCRANGESIG_OFFSET:
             subFrameCfg->objDetDynCfg.isCalibDcRangeSigCfg = 1;
         break;
         case MMWDEMO_STATICCLUTTERREMOFVAL_OFFSET:
             subFrameCfg->objDetDynCfg.isStaticClutterRemovalCfgPending = 1;
         break;
         case MMWDEMO_EXTMAXVEL_OFFSET:
             subFrameCfg->objDetDynCfg.isExtMaxVelCfgPending = 1;
         break;
 #elif defined(MMWDEMO_DDM)
         case MMWDEMO_GUIMONSEL_OFFSET:
             // Do nothing
         break;
         case MMWDEMO_CFARDOPPLERCFG_OFFSET:
             subFrameCfg->datapathStaticCfg.isCfarCfgPending = 1;
         break;
         case MMWDEMO_FOVAOA_OFFSET:
             subFrameCfg->datapathStaticCfg.isFovAoaCfgPending = 1;
         break;
         case MMWDEMO_CFARCFGRANGE_OFFSET:
             subFrameCfg->datapathStaticCfg.isRangeCfarCfgPending = 1;
         break;
         case MMWDEMO_COMPRESSIONCFG_OFFSET:
             subFrameCfg->datapathStaticCfg.isCompressionCfgPending = 1;
         break;
         case MMWDEMO_INTFMITIGCFG_OFFSET:
             subFrameCfg->datapathStaticCfg.isIntfStatsdBCfgPending = 1;
         break;
         case MMWDEMO_LOCALMAXCFG_OFFSET:
             subFrameCfg->datapathStaticCfg.isLocalMaxCfgPending = 1;
         break;
 #endif
         case MMWDEMO_ADCBUFCFG_OFFSET:
             subFrameCfg->isAdcBufCfgPending = 1;
         break;
 #ifdef LVDS_STREAM
         case MMWDEMO_LVDSSTREAMCFG_OFFSET:
             subFrameCfg->isLvdsStreamCfgPending = 1;
         break;
 #endif
         default:
             MmwDemo_debugAssert(0);
         break;
     }
 }
  
 #ifdef MMWDEMO_TDM
  
 static uint8_t MmwDemo_isDynObjDetCommonCfgPendingState(MmwDemo_DPC_ObjDet_CommonCfg *cfg)
 {
     uint8_t retVal;
  
     retVal = (cfg->isCompRxChannelBiasCfgPending    == 1) &&
              (cfg->isMeasureRxChannelBiasCfgPending == 1);
  
     return(retVal);
 }
 #endif
  
 #ifdef MMWDEMO_TDM
  
 static uint8_t MmwDemo_isDynObjDetCfgPendingState(MmwDemo_DPC_ObjDet_DynCfg *cfg)
 {
     uint8_t retVal;
  
     retVal = (cfg->isCalibDcRangeSigCfg    == 1) &&
              (cfg->isCfarCfgDopplerPending == 1) &&
              (cfg->isCfarCfgRangePending   == 1) &&
              (cfg->isFovDopplerPending     == 1) &&
              (cfg->isFovRangePending       == 1) &&
              (cfg->isMultiObjBeamFormingCfgPending     == 1) &&
              (cfg->isPrepareRangeAzimuthHeatMapPending == 1) &&
              (cfg->isStaticClutterRemovalCfgPending    == 1) &&
              (cfg->isFovAoaCfgPending                  == 1) &&
              (cfg->isExtMaxVelCfgPending               == 1);
  
     return(retVal);
 }
 #endif
  
 #ifdef MMWDEMO_TDM
  
 static uint8_t MmwDemo_isDynObjDetCommonCfgInNonPendingState(MmwDemo_DPC_ObjDet_CommonCfg *cfg)
 {
     uint8_t retVal;
  
     retVal = (cfg->isCompRxChannelBiasCfgPending    == 0) &&
              (cfg->isMeasureRxChannelBiasCfgPending == 0);
  
     return(retVal);
 }
 #endif
  
 #ifdef MMWDEMO_TDM
  
 static uint8_t MmwDemo_isDynObjDetCfgInNonPendingState(MmwDemo_DPC_ObjDet_DynCfg *cfg)
 {
     uint8_t retVal;
  
     retVal = (cfg->isCalibDcRangeSigCfg    == 0) &&
              (cfg->isCfarCfgDopplerPending == 0) &&
              (cfg->isCfarCfgRangePending   == 0) &&
              (cfg->isFovDopplerPending     == 0) &&
              (cfg->isFovRangePending       == 0) &&
              (cfg->isMultiObjBeamFormingCfgPending     == 0) &&
              (cfg->isPrepareRangeAzimuthHeatMapPending == 0) &&
              (cfg->isStaticClutterRemovalCfgPending    == 0) &&
              (cfg->isFovAoaCfgPending                  == 0) &&
              (cfg->isExtMaxVelCfgPending               == 0);
  
     return(retVal);
 }
 #endif
  
 static void MmwDemo_resetDynObjDetCommonCfgPendingState(MmwDemo_DPC_ObjDet_CommonCfg *cfg)
 {
 #ifdef MMWDEMO_TDM
     cfg->isCompRxChannelBiasCfgPending = 0;
     cfg->isMeasureRxChannelBiasCfgPending = 0;
 #elif defined(MMWDEMO_DDM)
     cfg->isAntennaCalibParamCfgPending = 0;
 #endif
 }
  
 #ifdef MMWDEMO_TDM
  
 static void MmwDemo_resetDynObjDetCfgPendingState(MmwDemo_DPC_ObjDet_DynCfg *cfg)
 {
     cfg->isCalibDcRangeSigCfg    = 0;
     cfg->isCfarCfgDopplerPending = 0;
     cfg->isCfarCfgRangePending   = 0;
     cfg->isFovDopplerPending     = 0;
     cfg->isFovRangePending       = 0;
     cfg->isMultiObjBeamFormingCfgPending = 0;
     cfg->isPrepareRangeAzimuthHeatMapPending = 0;
     cfg->isStaticClutterRemovalCfgPending = 0;
     cfg->isFovAoaCfgPending = 0;
         cfg->isExtMaxVelCfgPending = 0;
 }
 #endif
  
 #ifdef MMWDEMO_TDM
  
 void MmwDemo_resetStaticCfgPendingState(void)
 {
     uint8_t indx;
  
     for(indx = 0; indx < gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.numSubFrames; indx++)
     {
         gMmwMssMCB.subFrameCfg[indx].isAdcBufCfgPending = 0;
         gMmwMssMCB.subFrameCfg[indx].isLvdsStreamCfgPending = 0;
     }
  
     gMmwMssMCB.isAnaMonCfgPending = 0;
     gMmwMssMCB.isCalibCfgPending = 0;
  
 }
  
 uint8_t MmwDemo_isAllCfgInPendingState(void)
 {
     uint8_t indx, flag = 1;
  
     for(indx = 0; indx < gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.numSubFrames; indx++)
     {
         flag = flag && MmwDemo_isDynObjDetCfgPendingState(&gMmwMssMCB.subFrameCfg[indx].objDetDynCfg);
     }
  
     flag = flag && MmwDemo_isDynObjDetCommonCfgPendingState(&gMmwMssMCB.objDetCommonCfg);
     flag = flag && (gMmwMssMCB.isCalibCfgPending == 1);
  
     return(flag);
 }
 #endif
  
 #ifdef MMWDEMO_TDM
  
 uint8_t MmwDemo_isAllCfgInNonPendingState(void)
 {
     uint8_t indx, flag = 1;
  
     for(indx = 0; indx < gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.numSubFrames; indx++)
     {
         flag = flag && MmwDemo_isDynObjDetCfgInNonPendingState(&gMmwMssMCB.subFrameCfg[indx].objDetDynCfg);
     }
  
     flag = flag && (MmwDemo_isDynObjDetCommonCfgInNonPendingState(&gMmwMssMCB.objDetCommonCfg) && flag);
     flag = flag && (gMmwMssMCB.isAnaMonCfgPending == 0);
     flag = flag && (gMmwMssMCB.isCalibCfgPending == 0);
  
     return(flag);
 }
 #endif
  
 void MmwDemo_CfgUpdate(void *srcPtr, uint32_t offset, uint32_t size, int8_t subFrameNum)
 {
     /* if subFrameNum undefined, broadcast to all sub-frames */
     if(subFrameNum == MMWDEMO_SUBFRAME_NUM_FRAME_LEVEL_CONFIG)
     {
         uint8_t  indx;
         for(indx = 0; indx < RL_MAX_SUBFRAMES; indx++)
         {
             memcpy((void *)((uint32_t) &gMmwMssMCB.subFrameCfg[indx] + offset), srcPtr, size);
             MmwDemo_setSubFramePendingState(&gMmwMssMCB.subFrameCfg[indx], offset);
         }
     }
     else
     {
         /* Apply configuration to specific subframe (or to position zero for the legacy case
            where there is no advanced frame config) */
         memcpy((void *)((uint32_t) &gMmwMssMCB.subFrameCfg[subFrameNum] + offset), srcPtr, size);
         MmwDemo_setSubFramePendingState(&gMmwMssMCB.subFrameCfg[subFrameNum], offset);
     }
 }
  
 void MmwDemo_getTemperatureReport()
 {
     /* Get Temerature report */
     gMmwMssMCB.temperatureStats.tempReportValid = rlRfGetTemperatureReport(RL_DEVICE_MAP_INTERNAL_BSS,
                         (rlRfTempData_t*)&gMmwMssMCB.temperatureStats.temperatureReport);
 }
  
  
  
 /**************************************************************************
  ******************** Millimeter Wave Demo Results Transmit Functions *************
  **************************************************************************/
  
 static void MmwDemo_measurementResultOutput(void *compRxChanCfg)
 {
 int32_t i;
 #ifdef MMWDEMO_TDM
     DPU_AoAProc_compRxChannelBiasCfg *result = (DPU_AoAProc_compRxChannelBiasCfg*)compRxChanCfg;
     CLI_write ("compRangeBiasAndRxChanPhase (Re-Im) \n");
     CLI_write (" %.7f ", result->rangeBias);
  
     /* Send the received DSS calibration info through CLI */
     for (i = 0; i < SYS_COMMON_NUM_TX_ANTENNAS*SYS_COMMON_NUM_RX_CHANNEL; i++)
     {
         CLI_write ("%.5f ", (float)result->rxChPhaseComp[i].real/32768.);
         CLI_write ("%.5f ", (float)result->rxChPhaseComp[i].imag/32768.);
     }
     CLI_write ("\n");
  
 #elif defined(MMWDEMO_DDM)
     Measure_compRxChannelBiasCfg *result = (Measure_compRxChannelBiasCfg*)compRxChanCfg;
     CLI_write ("compRxChanPhase (Im-Re) \n");
     CLI_write("range %.5f peakVal %d \n", result->targetRange, result->peakVal);
  
     /* Send the received DSS calibration info through CLI */
     for (i = 0; i < SYS_COMMON_NUM_TX_ANTENNAS*SYS_COMMON_NUM_RX_CHANNEL; i++)
     {
         CLI_write ("%.5f ", (float)result->rxChPhaseComp[i].imag/16384.);
         CLI_write ("%.5f ", (float)result->rxChPhaseComp[i].real/16384.);
     }
     CLI_write ("\n");
 #endif
 }
  
  
 static void MmwDemo_transmitProcessedOutput
 (
     UART_Handle     uartHandle,
     DPC_ObjectDetection_ExecuteResult   *result,
     MmwDemo_output_message_stats        *timingInfo
 )
 {
     MmwDemo_output_message_header header;
     MmwDemo_GuiMonSel   *pGuiMonSel;
     MmwDemo_SubFrameCfg *subFrameCfg;
     uint32_t tlvIdx = 0;
     uint32_t index;
     uint32_t numPaddingBytes;
     uint32_t packetLen;
     uint8_t padding[MMWDEMO_OUTPUT_MSG_SEGMENT_LEN];
     MmwDemo_output_message_tl   tl[MMWDEMO_OUTPUT_MSG_MAX];
     uint16_t *detMatrix = (uint16_t *)result->detMatrix.data;
     DPIF_PointCloudCartesian *objOut;
 #ifdef MMWDEMO_TDM
     cmplx16ImRe_t *azimuthStaticHeatMap;
 #endif
     DPIF_PointCloudSideInfo *objOutSideInfo;
     DPC_ObjectDetection_Stats *stats;
     UART_Transaction trans;
  
     UART_Transaction_init(&trans);
  
     /* Get subframe configuration */
     subFrameCfg = &gMmwMssMCB.subFrameCfg[result->subFrameIdx];
  
 #ifdef MMWDEMO_DDM
     uint8_t txAntMask     = gMmwMssMCB.cfg.openCfg.chCfg.txChannelEn;
     uint8_t numTxAnt      = mathUtils_countSetBits(txAntMask);
     uint16_t numDopFFTSubBins = subFrameCfg->numDopplerBins / (numTxAnt + gMmwMssMCB.numEmptySubBands);
 #endif
  
     /* Get Gui Monitor configuration */
     pGuiMonSel = &subFrameCfg->guiMonSel;
  
     /* Clear message header */
     memset((void *)&header, 0, sizeof(MmwDemo_output_message_header));
  
     /******************************************************************
        Send out data that is enabled, Since processing results are from DSP,
        address translation is needed for buffer pointers
     *******************************************************************/
     {
         detMatrix = (uint16_t *) AddrTranslateP_getLocalAddr((uint32_t)detMatrix);
  
         objOut = (DPIF_PointCloudCartesian *) AddrTranslateP_getLocalAddr((uint32_t)result->objOut);
  
         objOutSideInfo = (DPIF_PointCloudSideInfo *) AddrTranslateP_getLocalAddr((uint32_t)result->objOutSideInfo);
  
         stats = (DPC_ObjectDetection_Stats *) AddrTranslateP_getLocalAddr((uint32_t)result->stats);
  
 #ifdef MMWDEMO_TDM
         result->radarCube.data = (void *) AddrTranslateP_getLocalAddr((uint32_t)result->radarCube.data);
 #endif
  
     }
  
     /* Header: */
 #ifdef SOC_AWR2944
     header.platform =  0x2944;
 #else
     header.platform =  0x2943;
 #endif
     header.magicWord[0] = 0x0102;
     header.magicWord[1] = 0x0304;
     header.magicWord[2] = 0x0506;
     header.magicWord[3] = 0x0708;
     header.numDetectedObj = result->numObjOut;
     header.version =    MMWAVE_SDK_VERSION_BUILD |
                         (MMWAVE_SDK_VERSION_BUGFIX << 8) |
                         (MMWAVE_SDK_VERSION_MINOR << 16) |
                         (MMWAVE_SDK_VERSION_MAJOR << 24);
  
     packetLen = sizeof(MmwDemo_output_message_header);
     if (((pGuiMonSel->detectedObjects == 1) || (pGuiMonSel->detectedObjects == 2)) &&
          (result->numObjOut > 0))
     {
         tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_DETECTED_POINTS;
         tl[tlvIdx].length = sizeof(DPIF_PointCloudCartesian) * result->numObjOut;
         packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
         tlvIdx++;
     }
     /* Side info */
     if ((pGuiMonSel->detectedObjects == 1) && (result->numObjOut > 0))
     {
         tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_DETECTED_POINTS_SIDE_INFO;
         tl[tlvIdx].length = sizeof(DPIF_PointCloudSideInfo) * result->numObjOut;
         packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
         tlvIdx++;
     }
     if (pGuiMonSel->logMagRange)
     {
         tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_RANGE_PROFILE;
         tl[tlvIdx].length = sizeof(uint16_t) * subFrameCfg->numRangeBins;
         packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
         tlvIdx++;
     }
     if (pGuiMonSel->noiseProfile)
     {
         tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_NOISE_PROFILE;
         tl[tlvIdx].length = sizeof(uint16_t) * subFrameCfg->numRangeBins;
         packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
         tlvIdx++;
     }
 #ifdef MMWDEMO_TDM
     if (pGuiMonSel->rangeAzimuthHeatMap)
     {
         tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_AZIMUT_STATIC_HEAT_MAP;
         tl[tlvIdx].length = result->azimuthStaticHeatMapSize * sizeof(cmplx16ImRe_t);
         packetLen += sizeof(MmwDemo_output_message_tl) +  tl[tlvIdx].length;
         tlvIdx++;
     }
 #endif
     if (pGuiMonSel->rangeDopplerHeatMap)
     {
         tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_RANGE_DOPPLER_HEAT_MAP;
 #ifdef MMWDEMO_TDM
         tl[tlvIdx].length = subFrameCfg->numRangeBins * subFrameCfg->numDopplerBins * sizeof(uint16_t);
 #elif defined(MMWDEMO_DDM)
         tl[tlvIdx].length = subFrameCfg->numRangeBins * numDopFFTSubBins * sizeof(uint16_t);
 #endif
         packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
         tlvIdx++;
     }
     if (pGuiMonSel->statsInfo)
     {
         tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_STATS;
         tl[tlvIdx].length = sizeof(MmwDemo_output_message_stats);
         packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
         tlvIdx++;
  
         MmwDemo_getTemperatureReport();
         tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_TEMPERATURE_STATS;
         tl[tlvIdx].length = sizeof(MmwDemo_temperatureStats);
         packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
         tlvIdx++;
     }
  
     header.numTLVs = tlvIdx;
     /* Round up packet length to multiple of MMWDEMO_OUTPUT_MSG_SEGMENT_LEN */
     header.totalPacketLen = MMWDEMO_OUTPUT_MSG_SEGMENT_LEN *
             ((packetLen + (MMWDEMO_OUTPUT_MSG_SEGMENT_LEN-1))/MMWDEMO_OUTPUT_MSG_SEGMENT_LEN);
     header.timeCpuCycles = 0; //Pmu_getCount(0);
     header.frameNumber = stats->frameStartIntCounter;
     header.subFrameNumber = result->subFrameIdx;
  
     DebugP_logInfo("Platform = %d, Version = %d, NumObj = %d, numTLVs = %d", header.platform, header.version, header.numDetectedObj, header.numTLVs);
  
     CacheP_wbInv((void *)&header, sizeof(MmwDemo_output_message_header), CacheP_TYPE_ALLD);
     UART_Transaction_init(&trans);
     trans.buf   = (uint8_t*)&header;
     trans.count = sizeof(MmwDemo_output_message_header);
     UART_write(uartHandle, &trans);
  
     tlvIdx = 0;
     /* Send detected Objects */
     if (((pGuiMonSel->detectedObjects == 1) || (pGuiMonSel->detectedObjects == 2)) &&
         (result->numObjOut > 0))
     {
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)&tl[tlvIdx];
         trans.count = sizeof(MmwDemo_output_message_tl);
         UART_write(uartHandle, &trans);
  
         /*Send array of objects */
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)objOut;
         trans.count = sizeof(DPIF_PointCloudCartesian) * result->numObjOut;
         UART_write(uartHandle, &trans);
         tlvIdx++;
     }
  
 #ifdef ENET_STREAM
     if(gMmwMssMCB.enetCfg.streamEnable){
         gEnetStreamObjData.numObj = result->numObjOut;
         gEnetStreamObjData.dummy  = 0x0U;
         memcpy((void *)gEnetStreamObjData.objData, (void*)objOut, sizeof(DPIF_PointCloudCartesian) * gEnetStreamObjData.numObj);
         SemaphoreP_post(&objDataSemaphoreHandle);
     }
 #endif
  
     /* Send detected Objects Side Info */
     if ((pGuiMonSel->detectedObjects == 1) && (result->numObjOut > 0))
     {
  
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)&tl[tlvIdx];
         trans.count = sizeof(MmwDemo_output_message_tl);
         UART_write(uartHandle, &trans);
  
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)objOutSideInfo;
         trans.count = sizeof(DPIF_PointCloudSideInfo) * result->numObjOut;
         UART_write(uartHandle, &trans);
         tlvIdx++;
     }
  
     /* Send Range profile */
     if (pGuiMonSel->logMagRange)
     {
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)&tl[tlvIdx];
         trans.count = sizeof(MmwDemo_output_message_tl);
         UART_write(uartHandle, &trans);
  
         for(index = 0; index < subFrameCfg->numRangeBins; index++)
         {
             UART_Transaction_init(&trans);
 #ifdef MMWDEMO_TDM
             trans.buf   = (uint8_t*)&detMatrix[index*subFrameCfg->numDopplerBins];
 #elif defined(MMWDEMO_DDM)
             trans.buf   = (uint8_t*)&detMatrix[index * numDopFFTSubBins];
 #endif
             trans.count = sizeof(uint16_t);
             UART_write(uartHandle, &trans);
         }
         tlvIdx++;
     }
  
     /* Send noise profile */
     if (pGuiMonSel->noiseProfile)
     {
         uint32_t maxDopIdx = subFrameCfg->numDopplerBins/2 -1;
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)&tl[tlvIdx];
         trans.count = sizeof(MmwDemo_output_message_tl);
         UART_write(uartHandle, &trans);
  
         for(index = 0; index < subFrameCfg->numRangeBins; index++)
         {
             UART_Transaction_init(&trans);
             trans.buf   = (uint8_t*)&detMatrix[index*subFrameCfg->numDopplerBins + maxDopIdx];
             trans.count = sizeof(uint16_t);
             UART_write(uartHandle, &trans);
         }
         tlvIdx++;
     }
  
 #ifdef MMWDEMO_TDM
     /* Send data for static azimuth heatmap */
     if (pGuiMonSel->rangeAzimuthHeatMap)
     {
         azimuthStaticHeatMap = (cmplx16ImRe_t *) AddrTranslateP_getLocalAddr((uint32_t)result->azimuthStaticHeatMap);
  
         trans.buf   = (uint8_t*)&tl[tlvIdx];
         trans.count = sizeof(MmwDemo_output_message_tl);
         UART_write(uartHandle, &trans);
  
         trans.buf   = (uint8_t*)azimuthStaticHeatMap;
         trans.count = result->azimuthStaticHeatMapSize * sizeof(cmplx16ImRe_t);
         UART_write(uartHandle, &trans);
  
         tlvIdx++;
     }
 #endif
  
     /* Send data for range/Doppler heatmap */
     if (pGuiMonSel->rangeDopplerHeatMap == 1)
     {
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)&tl[tlvIdx];
         trans.count = sizeof(MmwDemo_output_message_tl);
         UART_write(uartHandle, &trans);
  
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)detMatrix;
         trans.count = tl[tlvIdx].length;
         UART_write(uartHandle, &trans);
         tlvIdx++;
     }
  
     /* Send stats information */
     if (pGuiMonSel->statsInfo == 1)
     {
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)&tl[tlvIdx];
         trans.count = sizeof(MmwDemo_output_message_tl);
         UART_write(uartHandle, &trans);
  
         /* Address translation is done when buffer is received*/
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)timingInfo;
         trans.count = tl[tlvIdx].length;
         UART_write(uartHandle, &trans);
         tlvIdx++;
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)&tl[tlvIdx];
         trans.count = sizeof(MmwDemo_output_message_tl);
         UART_write(uartHandle, &trans);
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)&gMmwMssMCB.temperatureStats;
         trans.count = tl[tlvIdx].length;
         UART_write(uartHandle, &trans);
         tlvIdx++;
     }
  
     /* Send padding bytes */
     numPaddingBytes = MMWDEMO_OUTPUT_MSG_SEGMENT_LEN - (packetLen & (MMWDEMO_OUTPUT_MSG_SEGMENT_LEN-1));
     if (numPaddingBytes<MMWDEMO_OUTPUT_MSG_SEGMENT_LEN)
     {
         UART_Transaction_init(&trans);
         trans.buf   = (uint8_t*)padding;
         trans.count = numPaddingBytes;
         UART_write(uartHandle, &trans);
     }
  
 }
  
 /**************************************************************************
  ******************** Millimeter Wave Demo control path Functions *****************
  **************************************************************************/
 static int32_t MmwDemo_mmWaveCtrlStop (void)
 {
     int32_t                 errCode = 0;
  
     DebugP_logInfo("App: Issuing MMWave_stop\n");
  
     /* Stop the mmWave module: */
     if (MMWave_stop (gMmwMssMCB.ctrlHandle, &errCode) < 0)
     {
         MMWave_ErrorLevel   errorLevel;
         int16_t             mmWaveErrorCode;
         int16_t             subsysErrorCode;
  
         /* Error/Warning: Unable to stop the mmWave module */
         MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);
         if (errorLevel == MMWave_ErrorLevel_ERROR)
         {
             /* Error: Display the error message: */
             test_print ("Error: mmWave Stop failed [Error code: %d Subsystem: %d]\n",
                             mmWaveErrorCode, subsysErrorCode);
  
             /* Not expected */
             MmwDemo_debugAssert(0);
         }
         else
         {
             /* Warning: This is treated as a successful stop. */
             test_print ("mmWave Stop error ignored [Error code: %d Subsystem: %d]\n",
                             mmWaveErrorCode, subsysErrorCode);
         }
     }
  
     return errCode;
 }
  
 static void MmwDemo_mmWaveCtrlTask(void* args)
 {
     int32_t errCode;
  
     while (1)
     {
         /* Execute the mmWave control module: */
         if (MMWave_execute (gMmwMssMCB.ctrlHandle, &errCode) < 0)
         {
             MmwDemo_debugAssert (0);
         }
     }
 }
  
 /**************************************************************************
  ******************** Millimeter Wave Demo data path Functions *******************
  **************************************************************************/
  
 static int32_t MmwDemo_DPM_ioctl_blocking
 (
     DPM_Handle handle,
     uint32_t cmd,
     void* arg,
     uint32_t argLen
 )
 {
     int32_t retVal = 0;
  
     retVal = DPM_ioctl(handle,
                      cmd,
                      arg,
                      argLen);
  
     if(retVal == 0)
     {
         /* Wait until ioctl completed */
         SemaphoreP_pend(&gMmwMssMCB.DPMioctlSemHandle, SystemP_WAIT_FOREVER);
     }
  
     return(retVal);
 }
  
 static void MmwDemo_dataPathOpen(void)
 {
     gMmwMssMCB.adcBufHandle = MmwDemo_ADCBufOpen();
     if(gMmwMssMCB.adcBufHandle == NULL)
     {
         MmwDemo_debugAssert(0);
     }
 }
  
 static int32_t MmwDemo_configCQ(MmwDemo_SubFrameCfg *subFrameCfg,
                                 uint8_t numChirpsPerChirpEvent,
                                 uint8_t validProfileIdx)
 {
     MmwDemo_AnaMonitorCfg*      ptrAnaMonitorCfg;
     ADCBuf_CQConf               cqConfig;
     rlRxSatMonConf_t*           ptrSatMonCfg;
     rlSigImgMonConf_t*          ptrSigImgMonCfg;
     int32_t                     retVal;
     uint16_t                    cqChirpSize;
  
     /* Get analog monitor configuration */
     ptrAnaMonitorCfg = &gMmwMssMCB.anaMonCfg;
  
     /* Config mmwaveLink to enable Saturation monitor - CQ2 */
     ptrSatMonCfg = &gMmwMssMCB.cqSatMonCfg[validProfileIdx];
  
     if (ptrAnaMonitorCfg->rxSatMonEn)
     {
         if (ptrSatMonCfg->profileIndx != validProfileIdx)
         {
             test_print ("Error: Saturation monitoring (globally) enabled but not configured for profile(%d)\n",
                            validProfileIdx);
             MmwDemo_debugAssert(0);
         }
  
         retVal = mmwDemo_cfgRxSaturationMonitor(ptrSatMonCfg);
         if(retVal != 0)
         {
             test_print ("Error: rlRfRxIfSatMonConfig returns error = %d for profile(%d)\n",
                            retVal, ptrSatMonCfg->profileIndx);
             goto exit;
         }
     }
  
     /* Config mmwaveLink to enable Saturation monitor - CQ1 */
     ptrSigImgMonCfg = &gMmwMssMCB.cqSigImgMonCfg[validProfileIdx];
  
     if (ptrAnaMonitorCfg->sigImgMonEn)
     {
         if (ptrSigImgMonCfg->profileIndx != validProfileIdx)
         {
             test_print ("Error: Sig/Image monitoring (globally) enabled but not configured for profile(%d)\n",
                            validProfileIdx);
             MmwDemo_debugAssert(0);
         }
  
         retVal = mmwDemo_cfgRxSigImgMonitor(ptrSigImgMonCfg);
         if(retVal != 0)
         {
             test_print ("Error: rlRfRxSigImgMonConfig returns error = %d for profile(%d)\n",
                            retVal, ptrSigImgMonCfg->profileIndx);
             goto exit;
         }
     }
  
     retVal = mmwDemo_cfgAnalogMonitor(ptrAnaMonitorCfg);
     if (retVal != 0)
     {
         test_print ("Error: rlRfAnaMonConfig returns error = %d\n", retVal);
         goto exit;
     }
  
     if(ptrAnaMonitorCfg->rxSatMonEn || ptrAnaMonitorCfg->sigImgMonEn)
     {
         /* CQ driver config */
         memset((void *)&cqConfig, 0, sizeof(ADCBuf_CQConf));
         cqConfig.cqDataWidth = 0; /* 16bit for mmw demo */
         cqConfig.cq1AddrOffset = MMW_DEMO_CQ_SIGIMG_ADDR_OFFSET; /* CQ1 starts from the beginning of the buffer */
         cqConfig.cq2AddrOffset = MMW_DEMO_CQ_RXSAT_ADDR_OFFSET;  /* Address should be 16 bytes aligned */
  
         retVal = ADCBuf_control(gMmwMssMCB.adcBufHandle, ADCBufMMWave_CMD_CONF_CQ, (void *)&cqConfig);
         if (retVal < 0)
         {
             test_print ("Error: MMWDemoDSS Unable to configure the CQ\n");
             MmwDemo_debugAssert(0);
         }
     }
  
     if (ptrAnaMonitorCfg->sigImgMonEn)
     {
         /* This is for 16bit format in mmw demo, signal/image band data has 2 bytes/slice
            For other format, please check DFP interface document
          */
         cqChirpSize = (ptrSigImgMonCfg->numSlices + 1) * sizeof(uint16_t);
         cqChirpSize = MATHUTILS_ROUND_UP_UNSIGNED(cqChirpSize, MMW_DEMO_CQ_DATA_ALIGNMENT);
         subFrameCfg->sigImgMonTotalSize = cqChirpSize * numChirpsPerChirpEvent;
     }
  
     if (ptrAnaMonitorCfg->rxSatMonEn)
     {
         /* This is for 16bit format in mmw demo, saturation data has one byte/slice
            For other format, please check DFP interface document
          */
         cqChirpSize = (ptrSatMonCfg->numSlices + 1) * sizeof(uint8_t);
         cqChirpSize = MATHUTILS_ROUND_UP_UNSIGNED(cqChirpSize, MMW_DEMO_CQ_DATA_ALIGNMENT);
         subFrameCfg->satMonTotalSize = cqChirpSize * numChirpsPerChirpEvent;
     }
  
 exit:
     return(retVal);
 }
  
 #ifdef MMWDEMO_TDM
  
 static uint16_t MmwDemo_convertCfarToLinear(uint16_t codedCfarVal, uint8_t numVirtualAntennas)
 {
     uint16_t linearVal;
     float    dbVal, linVal;
  
     /* dbVal is a float value from 0-100dB. It needs to
     be converted to linear scale..
     First, recover float dbVal that was encoded in CLI. */
     dbVal = (float)(codedCfarVal / MMWDEMO_CFAR_THRESHOLD_ENCODING_FACTOR);
  
     /* Now convert it to linear value according to the following:
     linear_value = dB_value * (256 / 6) * (numVirtualAntennas / (2^ ceil(log2(numVirtualAntennas)))) .
     */
     linVal = dbVal * (256.0 / 6.0) * ((float)numVirtualAntennas / (float)(1 << mathUtils_ceilLog2(numVirtualAntennas)));
  
     linearVal = (uint16_t) linVal;
     return (linearVal);
 }
 #endif
  
 #ifdef MMWDEMO_DDM
  
 static uint16_t MmwDemo_convertRangeCfarToThresh(uint16_t codedCfarVal, uint8_t numBands)
 {
     uint16_t linearVal;
     float    dbVal, linVal;
     uint32_t defaultScaling = 1 << 11;
     float additionalScaling =  numBands / (float)(1 << mathUtils_ceilLog2(numBands));
  
     /* dbVal is a float value from 0-100dB. It needs to
     be converted to linear scale..
     First, recover float dbVal that was encoded in CLI. */
     dbVal = (float)(codedCfarVal / MMWDEMO_CFAR_THRESHOLD_ENCODING_FACTOR);
  
     /* Now convert it to linear value */
     linVal = (uint32_t)(log2f(pow(10, (float)dbVal/20.0)) * additionalScaling * defaultScaling + 0.5);
  
     linearVal = (uint16_t) linVal;
     return (linearVal);
 }
  
 #define CONST_LOG2_10  (3.3219)
 static uint16_t MmwDemo_convertDopplerCfarToThresh(uint16_t codedCfarVal)
 {
     uint16_t linearVal;
     float    dbVal, linVal;
  
     /* dbVal is a float value from 0-100dB. It needs to
     be converted to linear scale..
     First, recover float dbVal that was encoded in CLI. */
     dbVal = (float)(codedCfarVal / MMWDEMO_CFAR_THRESHOLD_ENCODING_FACTOR);
  
     /* Now convert it to linear value */
     linVal = (uint32_t)(dbVal/20.0 * CONST_LOG2_10 * 2048.0);
  
     linearVal = (uint16_t) linVal;
     return (linearVal);
 }
 #endif
  
  
 static int32_t MmwDemo_dataPathConfig (void)
 {
     int32_t                         errCode;
     MMWave_CtrlCfg                  *ptrCtrlCfg;
     MmwDemo_DPC_ObjDet_CommonCfg *objDetCommonCfg;
     MmwDemo_SubFrameCfg             *subFrameCfg;
     int8_t                          subFrameIndx;
     MmwDemo_RFParserOutParams       RFparserOutParams;
     DPC_ObjectDetection_PreStartCfg  objDetPreStartCfg;
     DPC_ObjectDetection_StaticCfg   *staticCfg;
 #ifdef MMWDEMO_DDM
     bool procChain = 1;
 #elif  defined(MMWDEMO_TDM)
     bool procChain = 0;
 #endif
     /* Get data path object and control configuration */
     ptrCtrlCfg = &gMmwMssMCB.cfg.ctrlCfg;
  
     objDetCommonCfg = &gMmwMssMCB.objDetCommonCfg;
     staticCfg = &objDetPreStartCfg.staticCfg;
  
 #ifdef MMWDEMO_DDM
     {
         /* Calculating the maximum ADC Samples across all frames, useful for common array allocations */
         gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.maxAdcSamples = 0U;
         for(subFrameIndx = 0; subFrameIndx < gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.numSubFrames;
             subFrameIndx++)
         {
             subFrameCfg  = &gMmwMssMCB.subFrameCfg[subFrameIndx];
             if(subFrameCfg->numAdcSamples > gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.maxAdcSamples)
             {
                 gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.maxAdcSamples = subFrameCfg->numAdcSamples;
             }
         }
     }
 #endif
  
     gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.numSubFrames =
         MmwDemo_RFParser_getNumSubFrames(ptrCtrlCfg);
  
     DebugP_logInfo("App: Issuing Pre-start Common Config IOCTL\n");
  
     /* Get RF frequency scale factor */
     gMmwMssMCB.rfFreqScaleFactor = MMWDEMO_RF_FREQ_SCALE_FACTOR;
  
     /* DPC pre-start common config */
     errCode = MmwDemo_DPM_ioctl_blocking (gMmwMssMCB.objDetDpmHandle,
                          DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG,
                          &objDetCommonCfg->preStartCommonCfg,
                          sizeof (DPC_ObjectDetection_PreStartCommonCfg));
  
     if (errCode < 0)
     {
         test_print ("Error: Unable to send DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG [Error:%d]\n", errCode);
         goto exit;
     }
  
     MmwDemo_resetDynObjDetCommonCfgPendingState(&gMmwMssMCB.objDetCommonCfg);
  
     /* Reason for reverse loop is that when sensor is started, the first sub-frame
      * will be active and the ADC configuration needs to be done for that sub-frame
      * before starting (ADC buf hardware does not have notion of sub-frame, it will
      * be reconfigured every sub-frame). This cannot be alternatively done by calling
      * the MmwDemo_ADCBufConfig function only for the first sub-frame because this is
      * a utility API that computes the rxChanOffset that is part of ADC dataProperty
      * which will be used by range DPU and therefore this computation is required for
      * all sub-frames.
      */
     for(subFrameIndx = gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.numSubFrames -1; subFrameIndx >= 0;
         subFrameIndx--)
     {
         subFrameCfg  = &gMmwMssMCB.subFrameCfg[subFrameIndx];
  
         /*****************************************************************************
          * Data path :: Algorithm Configuration
          *****************************************************************************/
  
         /* Parse the profile and chirp configs and get the valid number of TX Antennas */
         errCode = MmwDemo_RFParser_parseConfig(&RFparserOutParams, subFrameIndx,
                                          &gMmwMssMCB.cfg.openCfg, ptrCtrlCfg,
                                          &subFrameCfg->adcBufCfg,
                                          gMmwMssMCB.rfFreqScaleFactor,
                                          false, procChain);
  
         /* if number of doppler chirps is too low, interpolate to be able to detect
          * better with CFAR tuning. E.g. a 2-pt FFT will be problematic in terms
          * of distinguishing direction of motion */
         if (RFparserOutParams.numDopplerChirps <= 4)
         {
             RFparserOutParams.dopplerStep = RFparserOutParams.dopplerStep / (8 / RFparserOutParams.numDopplerBins);
             RFparserOutParams.numDopplerBins = 8;
         }
  
         if (errCode != 0)
         {
             test_print ("Error: MmwDemo_RFParser_parseConfig [Error:%d]\n", errCode);
             goto exit;
         }
  
         subFrameCfg->numRangeBins = RFparserOutParams.numRangeBins;
         /* Workaround for range DPU limitation for FFT size 1024 and 12 virtual antennas case*/
         if ((RFparserOutParams.numVirtualAntennas == 12) && (RFparserOutParams.numRangeBins == 1024))
         {
             subFrameCfg->numRangeBins = 1022;
             RFparserOutParams.numRangeBins = 1022;
         }
  
 #ifdef MMWDEMO_DDM
         subFrameCfg->datapathStaticCfg.compressionCfg.numRxAntennaPerBlock = RFparserOutParams.numRxAntennas;
         if(subFrameCfg->datapathStaticCfg.compressionCfg.compressionMethod == 1)
         {   /* BFP Compression */
             subFrameCfg->datapathStaticCfg.compressionCfg.bfpCompExtraParamSets = 2*(RFparserOutParams.numRxAntennas -1);
         }
         else
         {
             subFrameCfg->datapathStaticCfg.compressionCfg.bfpCompExtraParamSets = 0;
         }
 #endif
         subFrameCfg->numDopplerBins = RFparserOutParams.numDopplerBins;
         subFrameCfg->numChirpsPerChirpEvent = RFparserOutParams.numChirpsPerChirpEvent;
         subFrameCfg->adcBufChanDataSize = RFparserOutParams.adcBufChanDataSize;
 #ifdef MMWDEMO_TDM
         subFrameCfg->objDetDynCfg.dynCfg.prepareRangeAzimuthHeatMap = subFrameCfg->guiMonSel.rangeAzimuthHeatMap;
 #endif
         subFrameCfg->numAdcSamples = RFparserOutParams.numAdcSamples;
         subFrameCfg->numChirpsPerSubFrame = RFparserOutParams.numChirpsPerFrame;
         subFrameCfg->numVirtualAntennas = RFparserOutParams.numVirtualAntennas;
  
         errCode = MmwDemo_ADCBufConfig(gMmwMssMCB.adcBufHandle,
                                  gMmwMssMCB.cfg.openCfg.chCfg.rxChannelEn,
                                  subFrameCfg->numChirpsPerChirpEvent,
                                  subFrameCfg->adcBufChanDataSize,
                                  &subFrameCfg->adcBufCfg,
                                  &staticCfg->ADCBufData.dataProperty.rxChanOffset[0]);
         if (errCode < 0)
         {
             test_print("Error: ADCBuf config failed with error[%d]\n", errCode);
             MmwDemo_debugAssert (0);
         }
  
         errCode = MmwDemo_configCQ(subFrameCfg, subFrameCfg->numChirpsPerChirpEvent,
                                    RFparserOutParams.validProfileIdx);
         if (errCode < 0)
         {
             test_print("Error: CQ config failed with error[%d]\n", errCode);
             MmwDemo_debugAssert(0);
         }
  
         /* DPC pre-start config */
         {
             int32_t idx;
  
             objDetPreStartCfg.subFrameNum = subFrameIndx;
  
             /* Fill static configuration */
             staticCfg->ADCBufData.data = (void *)CSL_RSS_ADCBUF_READ_U_BASE;
             staticCfg->ADCBufData.dataProperty.adcBits = ADCBUF_DATA_PROPERTY_ADCBITS_16BIT; /* 16-bit */
  
             /* only real format supported */
             MmwDemo_debugAssert(subFrameCfg->adcBufCfg.adcFmt == 1);
  
             staticCfg->ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_REAL16;
  
             if (subFrameCfg->adcBufCfg.chInterleave == 0)
             {
                 staticCfg->ADCBufData.dataProperty.interleave = DPIF_RXCHAN_INTERLEAVE_MODE;
             }
             else
             {
                 staticCfg->ADCBufData.dataProperty.interleave = DPIF_RXCHAN_NON_INTERLEAVE_MODE;
             }
             staticCfg->ADCBufData.dataProperty.numAdcSamples = RFparserOutParams.numAdcSamples;
             staticCfg->ADCBufData.dataProperty.numChirpsPerChirpEvent = RFparserOutParams.numChirpsPerChirpEvent;
             staticCfg->ADCBufData.dataProperty.numRxAntennas = RFparserOutParams.numRxAntennas;
             staticCfg->ADCBufData.dataSize = RFparserOutParams.numRxAntennas * RFparserOutParams.numAdcSamples * sizeof(cmplx16ImRe_t);
             staticCfg->dopplerStep = RFparserOutParams.dopplerStep;
             staticCfg->isValidProfileHasOneTxPerChirp = RFparserOutParams.validProfileHasOneTxPerChirp;
             staticCfg->numChirpsPerFrame = RFparserOutParams.numChirpsPerFrame;
             staticCfg->numDopplerBins = RFparserOutParams.numDopplerBins;
 #ifdef MMWDEMO_TDM
             staticCfg->numDopplerChirps = RFparserOutParams.numDopplerChirps;
 #endif
 #ifdef MMWDEMO_DDM
  
             staticCfg->ADCBufConfig.rxChannelEn = gMmwMssMCB.cfg.openCfg.chCfg.rxChannelEn;
             staticCfg->ADCBufConfig.adcBufChanDataSize = RFparserOutParams.adcBufChanDataSize;
  
             staticCfg->numChirps = RFparserOutParams.numDopplerChirps;
             /* Sum Tx must be enabled if range profile is to be sent out or
                  range CFAR is enabled */
  
             staticCfg->isSumTxEnabled = (subFrameCfg->datapathStaticCfg.rangeCfarCfg.cfg.isEnabled) ||
                                         (subFrameCfg->guiMonSel.logMagRange) ||
                                         (subFrameCfg->guiMonSel.noiseProfile) ||
                                         (subFrameCfg->guiMonSel.rangeDopplerHeatMap)||
                                         (gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.measureRxChannelBiasCfg.enabled);
  
 #endif
             staticCfg->numRangeBins = RFparserOutParams.numRangeBins;
             /* Number of range bins are half the number of FFT Bins in case of real only chirp data */
             if(RFparserOutParams.adcDataFmtIsReal){
                 staticCfg->numRangeFFTBins = (RFparserOutParams.numRangeBins)*(2);
             }
             else{
                 staticCfg->numRangeFFTBins = (RFparserOutParams.numRangeBins);
             }
             staticCfg->numTxAntennas = RFparserOutParams.numTxAntennas;
             staticCfg->numVirtualAntAzim = RFparserOutParams.numVirtualAntAzim;
             staticCfg->numVirtualAntElev = RFparserOutParams.numVirtualAntElev;
             staticCfg->numVirtualAntennas = RFparserOutParams.numVirtualAntennas;
             staticCfg->rangeStep = RFparserOutParams.rangeStep;
 #ifdef MMWDEMO_DDM
             staticCfg->numBandsTotal = staticCfg->numTxAntennas + MmwDemo_getNumEmptySubBands(staticCfg->numTxAntennas);
 #endif
  
             if(staticCfg->numRangeFFTBins  > 1024 ){
                 staticCfg->rangeFFTtuning.fftOutputDivShift = 0;
                 staticCfg->rangeFFTtuning.numLastButterflyStagesToScale = 3; /* scale only 3 stages */
             }
             else if (staticCfg->numRangeFFTBins  >= 1022)
             {
                 staticCfg->rangeFFTtuning.fftOutputDivShift = 0;
                 staticCfg->rangeFFTtuning.numLastButterflyStagesToScale = 2; /* scale only 2 stages */
             } else if (staticCfg->numRangeFFTBins  == 512)
             {
                 staticCfg->rangeFFTtuning.fftOutputDivShift = 1;
                 staticCfg->rangeFFTtuning.numLastButterflyStagesToScale = 1; /* scale last stage */
             } else
             {
                 staticCfg->rangeFFTtuning.fftOutputDivShift = 2;
                 staticCfg->rangeFFTtuning.numLastButterflyStagesToScale = 0; /* no scaling needed as ADC data is 16-bit and we have 8 bits to grow */
             }
  
             for (idx = 0; idx < RFparserOutParams.numRxAntennas; idx++)
             {
                 staticCfg->rxAntOrder[idx] = RFparserOutParams.rxAntOrder[idx];
             }
             for (idx = 0; idx < RFparserOutParams.numTxAntennas; idx++)
             {
                 staticCfg->txAntOrder[idx] = RFparserOutParams.txAntOrder[idx];
             }
  
 #ifdef MMWDEMO_TDM
             /* Convert CFAR threshold value */
             subFrameCfg->objDetDynCfg.dynCfg.cfarCfgRange.thresholdScale =
                 MmwDemo_convertCfarToLinear(subFrameCfg->objDetDynCfg.dynCfg.cfarCfgRange.thresholdScale,
                                             staticCfg->numVirtualAntennas);
  
             subFrameCfg->objDetDynCfg.dynCfg.cfarCfgDoppler.thresholdScale =
                 MmwDemo_convertCfarToLinear(subFrameCfg->objDetDynCfg.dynCfg.cfarCfgDoppler.thresholdScale,
                                             staticCfg->numVirtualAntennas);
  
             /* Fill dynamic configuration for the sub-frame */
             objDetPreStartCfg.dynCfg = subFrameCfg->objDetDynCfg.dynCfg;
 #endif
  
             DebugP_logInfo("App: Issuing Pre-start Config IOCTL (subFrameIndx = %d)\n", subFrameIndx);
  
 #ifdef MMWDEMO_DDM
  
             /* Copy out the DPC Static cfg params */
  
             staticCfg->cfarCfg.subFrameNum = subFrameIndx;
  
             subFrameCfg->datapathStaticCfg.cfarCfg.cfg.thresholdScale =
                 MmwDemo_convertDopplerCfarToThresh(subFrameCfg->datapathStaticCfg.cfarCfg.cfg.thresholdScale); //staticCfg->numVirtualAntennas);
             memcpy(&staticCfg->cfarCfg.cfg, &subFrameCfg->datapathStaticCfg.cfarCfg.cfg, sizeof(DPU_DopplerProc_CfarCfg));
             memcpy(&staticCfg->compressionCfg, &subFrameCfg->datapathStaticCfg.compressionCfg, sizeof(DPU_RangeProcHWA_CompressionCfg));
             memcpy(&staticCfg->localMaxCfg, &subFrameCfg->datapathStaticCfg.localMaxCfg, sizeof(DPU_DopplerProc_LocalMaxCfg));
             memcpy(&staticCfg->intfStatsdBCfg, &subFrameCfg->datapathStaticCfg.intfStatsdBCfg, sizeof(DPU_RangeProcHWADDMA_intfStatsdBCfg));
             memcpy(&staticCfg->aoaFovCfg, &subFrameCfg->datapathStaticCfg.aoaFovCfg, sizeof(DPC_ObjectDetection_FovAoaCfg));
  
             staticCfg->rangeCfarCfg.subFrameNum = subFrameIndx;
             subFrameCfg->datapathStaticCfg.rangeCfarCfg.cfg.thresholdScale =
                 MmwDemo_convertRangeCfarToThresh(subFrameCfg->datapathStaticCfg.rangeCfarCfg.cfg.thresholdScale, staticCfg->numBandsTotal);
             memcpy(&staticCfg->rangeCfarCfg.cfg, &subFrameCfg->datapathStaticCfg.rangeCfarCfg.cfg, sizeof(DPU_CFARProc_CfarCfg));
 #endif
             /* send pre-start config */
             errCode = MmwDemo_DPM_ioctl_blocking (gMmwMssMCB.objDetDpmHandle,
                                  DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG,
                                  &objDetPreStartCfg,
                                  sizeof (DPC_ObjectDetection_PreStartCfg));
  
 #ifdef MMWDEMO_TDM
             MmwDemo_resetDynObjDetCfgPendingState(&subFrameCfg->objDetDynCfg);
 #endif
  
             if (errCode < 0)
             {
                 test_print ("Error: Unable to send DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG [Error:%d]\n", errCode);
                 goto exit;
             }
         }
     }
 exit:
     return errCode;
 }
  
 static void MmwDemo_dataPathStart (void)
 {
     int32_t retVal;
  
     DebugP_logInfo("App: Issuing DPM_start\n");
 #ifdef LVDS_STREAM
     /* Configure HW LVDS stream for the first sub-frame that will start upon
      * start of frame */
     if (gMmwMssMCB.subFrameCfg[0].lvdsStreamCfg.dataFmt != MMW_DEMO_LVDS_STREAM_CFG_DATAFMT_DISABLED)
     {
         MmwDemo_configLVDSHwData(0);
     }
 #endif
  
     /* Start the DPM Profile: */
     if ((retVal = DPM_start(gMmwMssMCB.objDetDpmHandle)) < 0)
     {
         /* Error: Unable to start the profile */
         test_print("Error: Unable to start the DPM [Error: %d]\n", retVal);
         MmwDemo_debugAssert(0);
     }
  
     /* Wait until start completed */
     SemaphoreP_pend(&gMmwMssMCB.DPMstartSemHandle, SystemP_WAIT_FOREVER);
  
     DebugP_logInfo("App: DPM_start Done (post SemaphoreP_pend on reportFxn reporting start)\n");
 }
  
 static void MmwDemo_dataPathStop (void)
 {
     int32_t retVal;
  
     DebugP_logInfo("App: Issuing DPM_stop\n");
  
 #ifdef MMWDEMO_DDM
     DPC_ObjectDetection_ExecuteResult* result;
  
     result = (DPC_ObjectDetection_ExecuteResult *)gMmwMssMCB.ptrResult.ptrBuffer[0];
  
     /* Write the FFT clip status on CLI. */
     if(result->FFTClipCount[0]>0U)
     {
         CLI_write("Warning! FFT clipping happened for %d times in Range FFT Stage. \n", result->FFTClipCount[0]);
     }
     if(result->FFTClipCount[1]>0U)
     {
         CLI_write("Warning! FFT clipping happened for %d times in Doppler or Azimuth FFT Stage. \n", result->FFTClipCount[1]);
     }
 #endif
  
     retVal = DPM_stop (gMmwMssMCB.objDetDpmHandle);
     if (retVal < 0)
     {
         test_print ("DPM_stop failed[Error code %d]\n", retVal);
         MmwDemo_debugAssert(0);
     }
 }
  
 static int32_t MmwDemo_eventCallbackFxn(uint8_t devIndex, uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload)
 {
     uint16_t asyncSB = RL_GET_SBID_FROM_UNIQ_SBID(sbId);
  
     /* Process the received message: */
     switch (msgId)
     {
         case RL_RF_ASYNC_EVENT_MSG:
         {
             /* Received Asychronous Message: */
             switch (asyncSB)
             {
                 case RL_RF_AE_CPUFAULT_SB:
                 {
                     MmwDemo_debugAssert(0);
                     break;
                 }
                 case RL_RF_AE_ESMFAULT_SB:
                 {
                     MmwDemo_debugAssert(0);
                     break;
                 }
                 case RL_RF_AE_ANALOG_FAULT_SB:
                 {
                     MmwDemo_debugAssert(0);
                     break;
                 }
                 case RL_RF_AE_INITCALIBSTATUS_SB:
                 {
                     rlRfInitComplete_t*  ptrRFInitCompleteMessage;
                     uint32_t            calibrationStatus;
  
                     /* Get the RF-Init completion message: */
                     ptrRFInitCompleteMessage = (rlRfInitComplete_t*)payload;
                     calibrationStatus = ptrRFInitCompleteMessage->calibStatus & 0x1FFFU;
  
                     /* Display the calibration status: */
                     CLI_write ("Debug: Init Calibration Status = 0x%x\n", calibrationStatus);
                     break;
                 }
                 case RL_RF_AE_FRAME_TRIGGER_RDY_SB:
                 {
                     gMmwMssMCB.stats.frameTriggerReady++;
                     break;
                 }
                 case RL_RF_AE_MON_TIMING_FAIL_REPORT_SB:
                 {
                     gMmwMssMCB.stats.failedTimingReports++;
                     break;
                 }
                 case RL_RF_AE_RUN_TIME_CALIB_REPORT_SB:
                 {
                     gMmwMssMCB.stats.calibrationReports++;
                     break;
                 }
                 case RL_RF_AE_FRAME_END_SB:
                 {
                     gMmwMssMCB.stats.sensorStopped++;
                     DebugP_logInfo("App: BSS stop (frame end) received\n");
  
                     MmwDemo_dataPathStop();
                     break;
                 }
                 default:
                 {
                     test_print ("Error: Asynchronous Event SB Id %d not handled\n", asyncSB);
                     break;
                 }
             }
             break;
         }
         /* Async Event from MMWL */
         case RL_MMWL_ASYNC_EVENT_MSG:
         {
             switch (asyncSB)
             {
                 case RL_MMWL_AE_MISMATCH_REPORT:
                 {
                     /* link reports protocol error in the async report from BSS */
                     MmwDemo_debugAssert(0);
                     break;
                 }
                 case RL_MMWL_AE_INTERNALERR_REPORT:
                 {
                     /* link reports internal error during BSS communication */
                     MmwDemo_debugAssert(0);
                     break;
                 }
             }
             break;
         }
         /* Async Event from MSS */
         case RL_DEV_ASYNC_EVENT_MSG:
         {
             switch (asyncSB)
             {
                 case RL_DEV_AE_MSSPOWERUPDONE_SB:
                 {
                     test_print("Received RL_DEV_AE_MSSPOWERUPDONE_SB\n");
  
                 }
                 break;
                 default:
                 {
                     test_print("Unhandled Async Event msgId: 0x%x, asyncSB:0x%x  \n\n", msgId, asyncSB);
                     break;
                 }
             }
             break;
  
         }
         default:
         {
             test_print ("Error: Asynchronous message %d is NOT handled\n", msgId);
             break;
         }
     }
     return 0;
 }
  
 static void MmwDemo_DPC_ObjectDetection_reportFxn
 (
     DPM_Report  reportType,
     uint32_t    instanceId,
     int32_t     errCode,
     uint32_t    arg0,
     uint32_t    arg1
 )
 {
     /* Only errors are logged on the console: */
     if ((errCode != 0) || (instanceId != DPC_OBJDET_INSTANCEID))
     {
         /* Error: Detected log on the console and die all errors are FATAL currently. */
         test_print ("Error: DPM Report %d received with error:%d arg0:0x%x arg1:0x%x\n",
                         reportType, errCode, arg0, arg1);
         DebugP_assert (0);
     }
  
     /* Processing further is based on the reports received: This is the control of the profile
      * state machine: */
     switch (reportType)
     {
         case DPM_Report_IOCTL:
         {
  
             /*****************************************************************
              * DPC has been configured without an error:
              * - This is an indication that the profile configuration commands
              *   went through without any issues.
              *****************************************************************/
             DebugP_logInfo("App: DPM Report IOCTL, command = %d\n", arg0);
  
             if (arg0 == DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG)
             {
                 DPC_ObjectDetection_PreStartCfg *cfg;
                 DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage *memUsage;
  
                 cfg = (DPC_ObjectDetection_PreStartCfg*)arg1;
  
                 memUsage = &cfg->memUsage;
  
                 test_print("============ Heap Memory Stats ============\n");
                 test_print("%20s %12s %12s %12s %12s\n", " ", "Size", "Used", "Free", "DPCUsed");
 #ifdef MMWDEMO_TDM
                 test_print("%20s %12d %12d %12d %12d\n", "System Heap(L2)",
                               memUsage->SystemHeapTotal, memUsage->SystemHeapUsed,
                               memUsage->SystemHeapTotal - memUsage->SystemHeapUsed,
                               memUsage->SystemHeapDPCUsed);
 #endif
  
                 test_print("%20s %12d %12d %12d\n", "L3",
                               memUsage->L3RamTotal,
                               memUsage->L3RamUsage,
                               memUsage->L3RamTotal - memUsage->L3RamUsage);
  
                 test_print("%20s %12d %12d %12d\n", "localRam(L2)",
                               memUsage->CoreLocalRamTotal,
                               memUsage->CoreLocalRamUsage,
                               memUsage->CoreLocalRamTotal - memUsage->CoreLocalRamUsage);
             }
  
             switch(arg0)
             {
                 /* The following ioctls take longer time to finish. It causes DPM to queue IOCTL requests on DSS before
                  * they are handled. However DPM has limited pipe queues, hence adding sync points in demo to avoid
                  * sending too many such ioctls to DSS at a time.
                  * The semaphore blocks CLI task to wait for the response from DSS before sending the next ioctl.
                  */
                 case DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG:
                 case DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG:
                     SemaphoreP_post(&gMmwMssMCB.DPMioctlSemHandle);
                     break;
                 default:
                     break;
             }
             break;
         }
         case DPM_Report_DPC_STARTED:
         {
             /*****************************************************************
              * DPC has been started without an error:
              * - notify sensor management task that DPC is started.
              *****************************************************************/
             DebugP_logInfo("App: DPM Report DPC Started\n");
             SemaphoreP_post(&gMmwMssMCB.DPMstartSemHandle);
             break;
         }
         case DPM_Report_NOTIFY_DPC_RESULT:
         {
             /*****************************************************************
              * datapath has finished frame processing, results are reported
              *****************************************************************/
  
             DPC_ObjectDetection_ExecuteResultExportedInfo exportInfo;
             int32_t retVal=0;
  
             if(gMmwMssMCB.stats.isLastFrameDataProcessed)
             {
                 /* reset Frame data processed flag, set after full obj data is actually streamed out */
                 gMmwMssMCB.stats.isLastFrameDataProcessed = false;
                 memcpy((void*)(&gMmwMssMCB.ptrResult), (void*)arg0, sizeof(DPM_Buffer));
                 exportInfo.subFrameIdx = ((DPC_ObjectDetection_ExecuteResult *)gMmwMssMCB.ptrResult.ptrBuffer[0])->subFrameIdx;
                 /* Send notification to data path after results are handled */
                 retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                                     DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED,
                                     &exportInfo,
                                     sizeof (DPC_ObjectDetection_ExecuteResultExportedInfo));
                 if (retVal < 0) {
                     test_print ("Error: DPM DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED failed [Error code %d]\n",
                                 retVal);
                     MmwDemo_debugAssert(0);
                 }
                 /* signal the UART task to transmit the data */
                 SemaphoreP_post(&gMmwMssMCB.UartExportSemHandle);
             }
             else{
                 volatile uint32_t transmitTime = (CycleCounterP_getCount32() - transmitStartTime)/(SOC_getSelfCpuClk()/1000000U);
                 test_print ("UART processing not completed: numObjOut %d Time %d\n", ((DPC_ObjectDetection_ExecuteResult *)gMmwMssMCB.ptrResult.ptrBuffer[0])->numObjOut, transmitTime);
                 MmwDemo_debugAssert(0);
             }
             break;
         }
         case DPM_Report_DPC_ASSERT:
         {
             DPM_DPCAssert*  ptrAssert;
  
             /*****************************************************************
              * DPC Fault has been detected:
              * - This implies that the DPC has crashed.
              * - The argument0 points to the DPC assertion information
              *****************************************************************/
             ptrAssert = (DPM_DPCAssert*)arg0;
             CLI_write("Obj Det DPC Exception: %s, line %d.\n", ptrAssert->fileName,
                        ptrAssert->lineNum);
             break;
         }
         case DPM_Report_DPC_STOPPED:
         {
             /*****************************************************************
              * DPC has been stopped without an error:
              * - This implies that the DPC can either be reconfigured or
              *   restarted.
              *****************************************************************/
             DebugP_logInfo("App: DPM Report DPC Stopped\n");
             SemaphoreP_post(&gMmwMssMCB.DPMstopSemHandle);
             break;
         }
         case DPM_Report_DPC_INFO:
         case DPM_Report_NOTIFY_DPC_RESULT_ACKED:
         {
             /* Currently objDetHwa does not use this feature. */
             break;
         }
         default:
         {
             DebugP_assert (0);
             break;
         }
     }
     return;
 }
  
 static uint8_t MmwDemo_getNextSubFrameIndx(uint8_t currentIndx, uint8_t numSubFrames)
 {
     uint8_t nextIndx;
  
     if (currentIndx == (numSubFrames - 1))
     {
         nextIndx = 0;
     }
     else
     {
         nextIndx = currentIndx + 1;
     }
     return(nextIndx);
 }
  
 static uint8_t MmwDemo_getPrevSubFrameIndx(uint8_t currentIndx, uint8_t numSubFrames)
 {
     uint8_t prevIndx;
  
     if (currentIndx == 0)
     {
         prevIndx = numSubFrames - 1;
     }
     else
     {
         prevIndx = currentIndx - 1;
     }
     return(prevIndx);
 }
  
 #ifdef MMWDEMO_TDM
  
 static int32_t MmwDemo_processPendingDynamicCfgCommands(uint8_t subFrameIndx)
 {
     int32_t retVal =0;
  
     MmwDemo_DPC_ObjDet_CommonCfg *commonCfg = &gMmwMssMCB.objDetCommonCfg;
     MmwDemo_DPC_ObjDet_DynCfg *subFrameCfg = &gMmwMssMCB.subFrameCfg[subFrameIndx].objDetDynCfg;
     uint8_t numVirtualAntennas = gMmwMssMCB.subFrameCfg[subFrameIndx].numVirtualAntennas;
  
     /* perform globals ones if first sub-frame */
     if (subFrameIndx == 0)
     {
         if (commonCfg->isMeasureRxChannelBiasCfgPending == 1)
         {
             retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                                  DPC_OBJDET_IOCTL__DYNAMIC_MEASURE_RANGE_BIAS_AND_RX_CHAN_PHASE,
                                  &commonCfg->preStartCommonCfg.measureRxChannelBiasCfg,
                                  sizeof (DPC_ObjectDetection_MeasureRxChannelBiasCfg));
             if (retVal != 0)
             {
                 goto exit;
             }
             commonCfg->isMeasureRxChannelBiasCfgPending = 0;
         }
         if (commonCfg->isCompRxChannelBiasCfgPending == 1)
         {
             retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                                  DPC_OBJDET_IOCTL__DYNAMIC_COMP_RANGE_BIAS_AND_RX_CHAN_PHASE,
                                  &commonCfg->preStartCommonCfg.compRxChanCfg,
                                  sizeof (DPU_AoAProc_compRxChannelBiasCfg));
             if (retVal != 0)
             {
                 goto exit;
             }
             commonCfg->isCompRxChannelBiasCfgPending = 0;
         }
     }
  
     /* Perform sub-frame specific ones */
     if (subFrameCfg->isCalibDcRangeSigCfg == 1)
     {
         DPC_ObjectDetection_CalibDcRangeSigCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         cfg.cfg = subFrameCfg->dynCfg.calibDcRangeSigCfg;
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                              DPC_OBJDET_IOCTL__DYNAMIC_CALIB_DC_RANGE_SIG_CFG,
                              &cfg,
                              sizeof (DPC_ObjectDetection_CalibDcRangeSigCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isCalibDcRangeSigCfg = 0;
     }
     if (subFrameCfg->isCfarCfgDopplerPending == 1)
     {
         DPC_ObjectDetection_CfarCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         /* Update with correct threshold value based on number of virtual antennas */
         subFrameCfg->dynCfg.cfarCfgDoppler.thresholdScale =
             MmwDemo_convertCfarToLinear(subFrameCfg->dynCfg.cfarCfgDoppler.thresholdScale,
                                         numVirtualAntennas);
  
         cfg.cfg = subFrameCfg->dynCfg.cfarCfgDoppler;
  
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                              DPC_OBJDET_IOCTL__DYNAMIC_CFAR_DOPPLER_CFG,
                              &cfg,
                              sizeof (DPC_ObjectDetection_CfarCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isCfarCfgDopplerPending = 0;
     }
     if (subFrameCfg->isCfarCfgRangePending == 1)
     {
         DPC_ObjectDetection_CfarCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         /* Update with correct threshold value based on number of virtual antennas */
         subFrameCfg->dynCfg.cfarCfgRange.thresholdScale =
             MmwDemo_convertCfarToLinear(subFrameCfg->dynCfg.cfarCfgRange.thresholdScale,
                                         numVirtualAntennas);
  
         cfg.cfg = subFrameCfg->dynCfg.cfarCfgRange;
  
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                              DPC_OBJDET_IOCTL__DYNAMIC_CFAR_RANGE_CFG,
                              &cfg,
                              sizeof (DPC_ObjectDetection_CfarCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isCfarCfgRangePending = 0;
     }
     if (subFrameCfg->isFovDopplerPending == 1)
     {
         DPC_ObjectDetection_fovDopplerCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         cfg.cfg = subFrameCfg->dynCfg.fovDoppler;
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                              DPC_OBJDET_IOCTL__DYNAMIC_FOV_DOPPLER,
                              &cfg,
                              sizeof (DPC_ObjectDetection_fovDopplerCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isFovDopplerPending = 0;
     }
     if (subFrameCfg->isFovRangePending == 1)
     {
         DPC_ObjectDetection_fovRangeCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         cfg.cfg = subFrameCfg->dynCfg.fovRange;
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                              DPC_OBJDET_IOCTL__DYNAMIC_FOV_RANGE,
                              &cfg,
                              sizeof (DPC_ObjectDetection_fovRangeCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isFovRangePending = 0;
     }
     if (subFrameCfg->isMultiObjBeamFormingCfgPending == 1)
     {
         DPC_ObjectDetection_MultiObjBeamFormingCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         cfg.cfg = subFrameCfg->dynCfg.multiObjBeamFormingCfg;
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                              DPC_OBJDET_IOCTL__DYNAMIC_MULTI_OBJ_BEAM_FORM_CFG,
                              &cfg,
                              sizeof (DPC_ObjectDetection_MultiObjBeamFormingCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isMultiObjBeamFormingCfgPending = 0;
     }
     if (subFrameCfg->isPrepareRangeAzimuthHeatMapPending == 1)
     {
         DPC_ObjectDetection_RangeAzimuthHeatMapCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         cfg.prepareRangeAzimuthHeatMap = subFrameCfg->dynCfg.prepareRangeAzimuthHeatMap;
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                              DPC_OBJDET_IOCTL__DYNAMIC_RANGE_AZIMUTH_HEAT_MAP,
                              &cfg,
                              sizeof (DPC_ObjectDetection_RangeAzimuthHeatMapCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isPrepareRangeAzimuthHeatMapPending = 0;
     }
     if (subFrameCfg->isStaticClutterRemovalCfgPending == 1)
     {
         DPC_ObjectDetection_StaticClutterRemovalCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         cfg.cfg = subFrameCfg->dynCfg.staticClutterRemovalCfg;
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                              DPC_OBJDET_IOCTL__DYNAMIC_STATICCLUTTER_REMOVAL_CFG,
                              &cfg,
                              sizeof (DPC_ObjectDetection_StaticClutterRemovalCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isStaticClutterRemovalCfgPending = 0;
     }
     if (subFrameCfg->isFovAoaCfgPending == 1)
     {
         DPC_ObjectDetection_fovAoaCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         cfg.cfg = subFrameCfg->dynCfg.fovAoaCfg;
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                              DPC_OBJDET_IOCTL__DYNAMIC_FOV_AOA,
                              &cfg,
                              sizeof (DPC_ObjectDetection_fovAoaCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isFovAoaCfgPending = 0;
     }
     if (subFrameCfg->isExtMaxVelCfgPending == 1)
     {
         DPC_ObjectDetection_extMaxVelCfg cfg;
  
         cfg.subFrameNum = subFrameIndx;
         cfg.cfg = subFrameCfg->dynCfg.extMaxVelCfg;
         retVal = DPM_ioctl (gMmwMssMCB.objDetDpmHandle,
                             DPC_OBJDET_IOCTL__DYNAMIC_EXT_MAX_VELOCITY,
                             &cfg,
                             sizeof (DPC_ObjectDetection_extMaxVelCfg));
         if (retVal != 0)
         {
             goto exit;
         }
         subFrameCfg->isExtMaxVelCfgPending = 0;
     }
  
 exit:
     return(retVal);
 }
 #endif
  
 /* Workaround for Errata ANA#46: Spurs caused due to data transfer activity */
 static void MmwDPC_chirpAvailISR(void* arg)
 {
     uint32_t delay = 0;
     CSL_rss_ctrlRegs *ptr_rss_ctrl_regs = (CSL_rss_ctrlRegs*)CSL_RSS_CTRL_U_BASE;
  
     if(gMmwMssMCB.adcDataDithDelayCfg.isDitherEn == 1U)
     {
         /* configure the variable amount of delay */
         delay = rand() % gMmwMssMCB.adcDataDithDelayCfg.ditherVal;
  
         ptr_rss_ctrl_regs->ADCBUFCFG1_EXTD =  delay + MMWDEMO_DITHERING_MINDELAY;
     }
 }
  
 /* Workaround for Errata ANA#46: Spurs caused due to data transfer activity */
 static int32_t MmwDemo_registerChirpStartInterrupt(void)
 {
     int32_t retVal = 0;
     int32_t status = SystemP_SUCCESS;
     HwiP_Params hwiPrms;
  
     /* Register interrupt */
     HwiP_Params_init(&hwiPrms);
     hwiPrms.intNum = CSL_MSS_INTR_RSS_ADC_CAPTURE_COMPLETE_DITH;
     hwiPrms.callback = &MmwDPC_chirpAvailISR;
     hwiPrms.priority = 2;
     status = HwiP_construct(&gMmwMssMCB.adcDataDithDelayCfg.chirpAvailHwiObject, &hwiPrms);
     if (SystemP_SUCCESS != status)
     {
         retVal = SystemP_FAILURE;
     }
     else
     {
         /* Keep the interrupt disable: Interrupt should be enabled once dithering enable request is received from CLI */
         HwiP_disableInt((uint32_t)CSL_MSS_INTR_RSS_ADC_CAPTURE_COMPLETE_DITH);
     }
     return retVal;
 }
  
 void MmwDemo_handleObjectDetResult(void)
 {
     DPC_ObjectDetection_ExecuteResult        *dpcResults;
     MmwDemo_output_message_stats            *frameStats;
     volatile uint32_t                        startTime;
     uint8_t                                  nextSubFrameIdx;
     uint8_t                                  numSubFrames;
     uint8_t                                  currSubFrameIdx;
     uint8_t                                  prevSubFrameIdx;
     MmwDemo_SubFrameStats                    *currSubFrameStats;
     MmwDemo_SubFrameStats                    *prevSubFrameStats;
 #ifdef MMWDEMO_TDM
     int32_t retVal;
 #endif
     /*****************************************************************
      * datapath has finished frame processing, results are reported
      *****************************************************************/
  
     /* Validate DPC results buffer */
     DebugP_assert (gMmwMssMCB.ptrResult.size[0] == sizeof(DPC_ObjectDetection_ExecuteResult));
  
     /* Translate the address: */
     dpcResults = (DPC_ObjectDetection_ExecuteResult *) AddrTranslateP_getLocalAddr((uint32_t)gMmwMssMCB.ptrResult.ptrBuffer[0]);
  
     /* Validate timing Info buffer */
     DebugP_assert (gMmwMssMCB.ptrResult.size[1] == sizeof(MmwDemo_output_message_stats));
  
     numSubFrames = gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.numSubFrames;
     currSubFrameIdx = dpcResults->subFrameIdx;
     prevSubFrameIdx = MmwDemo_getPrevSubFrameIndx(currSubFrameIdx, numSubFrames);
     currSubFrameStats = &gMmwMssMCB.subFrameStats[currSubFrameIdx];
     prevSubFrameStats = &gMmwMssMCB.subFrameStats[prevSubFrameIdx];
  
     /*****************************************************************
      * Transmit results
      *****************************************************************/
     startTime = CycleCounterP_getCount32();
  
  
     /* Send out of CLI the range bias and phase config measurement if it was enabled. */
     if (gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.measureRxChannelBiasCfg.enabled == 1)
     {
         if(dpcResults->compRxChanBiasMeasurement != NULL)
         {
 #ifdef MMWDEMO_TDM
             dpcResults->compRxChanBiasMeasurement = (DPU_AoAProc_compRxChannelBiasCfg *) AddrTranslateP_getLocalAddr((uint32_t)dpcResults->compRxChanBiasMeasurement);
 #elif defined(MMWDEMO_DDM)
             dpcResults->compRxChanBiasMeasurement = (Measure_compRxChannelBiasCfg *) AddrTranslateP_getLocalAddr((uint32_t)dpcResults->compRxChanBiasMeasurement);
 #endif
             MmwDemo_measurementResultOutput((void*)dpcResults->compRxChanBiasMeasurement);
         }
         else
         {
             /* DPC is not ready to ship the measurement results */
         }
     }
  
     /* Translate the address: */
     frameStats = (MmwDemo_output_message_stats *) AddrTranslateP_getLocalAddr((uint32_t)gMmwMssMCB.ptrResult.ptrBuffer[1]);
  
     /* Update current frame stats */
     currSubFrameStats->outputStats.interFrameCPULoad = frameStats->interFrameCPULoad;
     currSubFrameStats->outputStats.activeFrameCPULoad= frameStats->activeFrameCPULoad;
     currSubFrameStats->outputStats.interChirpProcessingMargin = frameStats->interChirpProcessingMargin;
     currSubFrameStats->outputStats.interFrameProcessingTime = frameStats->interFrameProcessingTime;
     prevSubFrameStats->outputStats.interFrameProcessingMargin = frameStats->interFrameProcessingMargin;
     currSubFrameStats->outputStats.interFrameProcessingMargin = currSubFrameStats->outputStats.interFrameProcessingMargin -
                                                          (currSubFrameStats->pendingConfigProcTime + currSubFrameStats->subFramePreparationTime);
 #ifdef LVDS_STREAM
     if (gMmwMssMCB.subFrameCfg[currSubFrameIdx].lvdsStreamCfg.dataFmt !=
              MMW_DEMO_LVDS_STREAM_CFG_DATAFMT_DISABLED)
     {
         /* check Edma errors (which are considered fatal) for the current sub-frame's
          * h/w session that is expected to be completed by now */
         MmwDemo_checkEdmaErrors();
  
         /* Pend for completion of h/w session, generally this will not wait
          * because of time spent doing inter-frame processing is expected to
          * be bigger than the transmission of the h/w session */
         SemaphoreP_pend(&gMmwMssMCB.lvdsStream.hwFrameDoneSemHandle, SystemP_WAIT_FOREVER);
     }
 #endif
  
     /* Transmit processing results for the frame */
     transmitStartTime = CycleCounterP_getCount32();
     MmwDemo_transmitProcessedOutput(gMmwMssMCB.loggingUartHandle,
                                     dpcResults,
                                     &currSubFrameStats->outputStats);
  
     /* Update current frame transmit time */
     currSubFrameStats->outputStats.transmitOutputTime = (CycleCounterP_getCount32() - startTime)/(SOC_getSelfCpuClk()/1000000U); /* In micro seconds */
  
     /*****************************************************************
      * Handle dynamic pending configuration
      * For non-advanced frame case:
      *   process all pending dynamic config commands.
      * For advanced-frame case:
      *  Process next sub-frame related pending dynamic config commands.
      *  If the next sub-frame was the first sub-frame of the frame,
      *  then process common (sub-frame independent) pending dynamic config
      *  commands.
      *****************************************************************/
     startTime = CycleCounterP_getCount32();
  
     nextSubFrameIdx = MmwDemo_getNextSubFrameIndx(currSubFrameIdx,   numSubFrames);
 #ifdef MMWDEMO_TDM
     retVal = MmwDemo_processPendingDynamicCfgCommands(nextSubFrameIdx);
     if (retVal != 0)
     {
         test_print ("Error: Executing Pending Dynamic Configuration Commands [Error code %d]\n",
                        retVal);
         MmwDemo_debugAssert(0);
     }
 #endif
     currSubFrameStats->pendingConfigProcTime = (CycleCounterP_getCount32() - startTime)/(SOC_getSelfCpuClk()/1000000U);
  
 #ifdef MMWDEMO_TDM
     /*****************************************************************
      * Prepare for subFrame switch
      *****************************************************************/
     if(numSubFrames > 1)
     {
         MmwDemo_SubFrameCfg  *nextSubFrameCfg;
         uint16_t dummyRxChanOffset[SYS_COMMON_NUM_RX_CHANNEL];
  
         startTime = CycleCounterP_getCount32();
  
         nextSubFrameCfg = &gMmwMssMCB.subFrameCfg[nextSubFrameIdx];
  
         /* Configure ADC for next sub-frame */
         retVal = MmwDemo_ADCBufConfig(gMmwMssMCB.adcBufHandle,
                                  gMmwMssMCB.cfg.openCfg.chCfg.rxChannelEn,
                                  nextSubFrameCfg->numChirpsPerChirpEvent,
                                  nextSubFrameCfg->adcBufChanDataSize,
                                  &nextSubFrameCfg->adcBufCfg,
                                  &dummyRxChanOffset[0]);
         if(retVal < 0)
         {
             test_print("Error: ADCBuf config failed with error[%d]\n", retVal);
             MmwDemo_debugAssert(0);
         }
         currSubFrameStats->subFramePreparationTime = (CycleCounterP_getCount32() - startTime)/(SOC_getSelfCpuClk()/1000000U);
  
 #ifdef LVDS_STREAM
         /* Configure HW LVDS stream for this subframe? */
         if(nextSubFrameCfg->lvdsStreamCfg.dataFmt != MMW_DEMO_LVDS_STREAM_CFG_DATAFMT_DISABLED)
         {
             /* check Edma errors (which are considered fatal) for any previous session, even
              * though we have checked for a previous s/w session, if s/w session weren't
              * enabled, then this will check for previous h/w session related Edma errors */
             MmwDemo_checkEdmaErrors();
  
             MmwDemo_configLVDSHwData(nextSubFrameIdx);
         }
  
         currSubFrameStats->subFramePreparationTime = (CycleCounterP_getCount32() - startTime)/(SOC_getSelfCpuClk()/1000000U);
 #endif
  
     }
     else
     {
         currSubFrameStats->subFramePreparationTime = 0;
     }
 #elif MMWDEMO_DDM
     /*****************************************************************
      * Prepare for subFrame switch
      *****************************************************************/
     if(numSubFrames > 1)
     {
         MmwDemo_SubFrameCfg  *nextSubFrameCfg;
  
         startTime = CycleCounterP_getCount32();
  
         nextSubFrameCfg = &gMmwMssMCB.subFrameCfg[nextSubFrameIdx];
  
         currSubFrameStats->subFramePreparationTime = (CycleCounterP_getCount32() - startTime)/(SOC_getSelfCpuClk()/1000000U);
  
 #ifdef LVDS_STREAM
         /* Configure HW LVDS stream for this subframe? */
         if(nextSubFrameCfg->lvdsStreamCfg.dataFmt != MMW_DEMO_LVDS_STREAM_CFG_DATAFMT_DISABLED)
         {
             /* check Edma errors (which are considered fatal) for any previous session, even
              * though we have checked for a previous s/w session, if s/w session weren't
              * enabled, then this will check for previous h/w session related Edma errors */
             MmwDemo_checkEdmaErrors();
  
             MmwDemo_configLVDSHwData(nextSubFrameIdx);
         }
         currSubFrameStats->subFramePreparationTime = (CycleCounterP_getCount32() - startTime)/(SOC_getSelfCpuClk()/1000000U);
 #endif
     }
     else
     {
         currSubFrameStats->subFramePreparationTime = 0;
     }
  
 #endif
  
     /* set the Frame data processed flag to indicate that obj data is out successfully */
     gMmwMssMCB.stats.isLastFrameDataProcessed = true;
 }
  
 static void mmwDemo_mssDPMTask(void* args)
 {
     int32_t     errCode;
     DPM_Buffer  result;
  
     while (1)
     {
         /* Execute the DPM module: */
         errCode = DPM_execute (gMmwMssMCB.objDetDpmHandle, &result);
         if (errCode < 0)
         {
             test_print ("Error: DPM execution failed [Error code %d]\n", errCode);
         }
     }
 }
  
 void mmwDemo_mssUartDataExportTask(void* args)
 {
     /* Enable save/restore of Floating Point Registers */
     portTASK_USES_FLOATING_POINT();
  
     while (1)
     {
         /* Export the Data: */
         SemaphoreP_pend(&gMmwMssMCB.UartExportSemHandle, SystemP_WAIT_FOREVER);
         if ((gMmwMssMCB.ptrResult.size[0]) == sizeof(DPC_ObjectDetection_ExecuteResult)){
             MmwDemo_handleObjectDetResult();
         }
     }
 }
  
 #ifdef ENET_STREAM
  
 int32_t MmwDemo_mssEnetCfgDone(void)
 {
  
     /* Post EnetCfgDone Semaphore to signal that the IP
     has been configured and connection can now be made */
     SemaphoreP_post(&gMmwMssMCB.enetCfg.EnetCfgDoneSemHandle);
  
     return 0;
 }
 #endif
  
 /**************************************************************************
  ******************** Millimeter Wave Demo sensor management Functions **********
  **************************************************************************/
  
 int32_t MmwDemo_openSensor(bool isFirstTimeOpen)
 {
     int32_t             errCode;
     MMWave_ErrorLevel   errorLevel;
     int16_t             mmWaveErrorCode;
     int16_t             subsysErrorCode;
     int32_t             retVal;
     MMWave_CalibrationData     calibrationDataCfg;
     MMWave_CalibrationData     *ptrCalibrationDataCfg;
  
     /*  Open mmWave module, this is only done once */
     if (isFirstTimeOpen == true)
     {
  
         /**********************************************************
          **********************************************************/
  
         /* Open mmWave module, this is only done once */
         /* Setup the calibration frequency:*/
         gMmwMssMCB.cfg.openCfg.freqLimitLow  = 760U;
         gMmwMssMCB.cfg.openCfg.freqLimitHigh = 810U;
  
         /* start/stop async events */
         gMmwMssMCB.cfg.openCfg.disableFrameStartAsyncEvent = false;
         gMmwMssMCB.cfg.openCfg.disableFrameStopAsyncEvent  = false;
  
         /* No custom calibration: */
         gMmwMssMCB.cfg.openCfg.useCustomCalibration        = false;
         gMmwMssMCB.cfg.openCfg.customCalibrationEnableMask = 0x0;
  
         /* calibration monitoring base time unit
          * setting it to one frame duration as the demo doesnt support any
          * monitoring related functionality
          */
         gMmwMssMCB.cfg.openCfg.calibMonTimeUnit            = 1;
  
         if( (gMmwMssMCB.calibCfg.saveEnable != 0) &&
                 (gMmwMssMCB.calibCfg.restoreEnable != 0) )
         {
             /* Error: only one can be enabled at at time */
             test_print ("Error: MmwDemo failed with both save and restore enabled.\n");
             return -1;
         }
  
         if(gMmwMssMCB.calibCfg.restoreEnable != 0)
         {
             if(MmwDemo_calibRestore(&gCalibDataStorage) < 0)
             {
                 test_print ("Error: MmwDemo failed restoring calibration data from flash.\n");
                 return -1;
             }
  
             /*  Boot calibration during restore: Disable calibration for:
                  - Rx gain,
                  - Rx IQMM,
                  - Tx phase shifer,
                  - Tx Power
  
                  The above calibration data will be restored from flash. Since they are calibrated in a control
                  way to avoid interfaerence and spec violations.
                  In this demo, other bit fields(except the above) are enabled as indicated in customCalibrationEnableMask to perform boot time
                  calibration. The boot time calibration will overwrite the restored calibration data from flash.
                  However other bit fields can be disabled and calibration data can be restored from flash as well.
  
                  Note: In this demo, calibration masks are enabled for all bit fields when "saving" the data.
             */
             gMmwMssMCB.cfg.openCfg.useCustomCalibration        = true;
             gMmwMssMCB.cfg.openCfg.customCalibrationEnableMask = 0x1F0U;
  
             calibrationDataCfg.ptrCalibData = &gCalibDataStorage.calibData;
             calibrationDataCfg.ptrPhaseShiftCalibData = &gCalibDataStorage.phaseShiftCalibData;
             ptrCalibrationDataCfg = &calibrationDataCfg;
         }
         else
         {
             ptrCalibrationDataCfg = NULL;
         }
  
  
         /* Open the mmWave module: */
         if (MMWave_open (gMmwMssMCB.ctrlHandle, &gMmwMssMCB.cfg.openCfg, ptrCalibrationDataCfg, &errCode) < 0)
         {
             /* Error: decode and Report the error */
             MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);
             test_print ("Error: mmWave Open failed [Error code: %d Subsystem: %d]\n",
                             mmWaveErrorCode, subsysErrorCode);
             return -1;
         }
  
         /* Save calibration data in flash */
         if(gMmwMssMCB.calibCfg.saveEnable != 0)
         {
             retVal = rlRfCalibDataStore(RL_DEVICE_MAP_INTERNAL_BSS, &gCalibDataStorage.calibData);
             if(retVal != RL_RET_CODE_OK)
             {
                 /* Error: Calibration data restore failed */
                  test_print("MSS demo failed rlRfCalibDataStore with Error[%d]\n", retVal);
                 return -1;
             }
  
             /* update txIndex in all chunks to get data from all Tx.
             This should be done regardless of num TX channels enabled in MMWave_OpenCfg_t::chCfg or number of Tx
             application is interested in. Data for all existing Tx channels should be retrieved
             from RadarSS and in the order as shown below.
             RadarSS will return non-zero phase shift values for all the channels enabled via
             MMWave_OpenCfg_t::chCfg and zero phase shift values for channels disabled in MMWave_OpenCfg_t::chCfg */
             gCalibDataStorage.phaseShiftCalibData.PhShiftcalibChunk[0].txIndex = 0;
             gCalibDataStorage.phaseShiftCalibData.PhShiftcalibChunk[1].txIndex = 1;
             gCalibDataStorage.phaseShiftCalibData.PhShiftcalibChunk[2].txIndex = 2;
  
             /* Basic validation passed: Restore the phase shift calibration data */
             retVal = rlRfPhShiftCalibDataStore(RL_DEVICE_MAP_INTERNAL_BSS, &(gCalibDataStorage.phaseShiftCalibData));
             if (retVal != RL_RET_CODE_OK)
             {
                 /* Error: Phase shift Calibration data restore failed */
                 test_print("MSS demo failed rlRfPhShiftCalibDataStore with Error[%d]\n", retVal);
                 return retVal;
             }
  
             /* Save data in flash */
             retVal = MmwDemo_calibSave(&gMmwMssMCB.calibCfg.calibDataHdr, &gCalibDataStorage);
             if(retVal < 0)
             {
                 return retVal;
             }
         }
  
         /* Open the datapath modules that runs on MSS */
         MmwDemo_dataPathOpen();
     }
     return 0;
 }
  
 #ifdef MMWDEMO_DDM
  
 int32_t MmwDemo_configPhaseShifterChirps(void){
  
     int32_t     errCode = 0;
     uint16_t    chirpIdx, chirpStartIdx, chirpEndIdx, numTxAntAzim, numTxTotalDivisor,
                 numTxAntElev, txAntMask, txOrderIdx, activeTxCnt, chirpPhaseMultiplier;
     /* xth value of this array corresponds to the phase shift multiplier for the xth Tx antenna */
     uint16_t       phaseShiftMultiplier[SYS_COMMON_NUM_TX_ANTENNAS];
     rlRfPhaseShiftCfg_t phaseShiftCfg;
  
     memset ((void *)&phaseShiftCfg, 0, sizeof(phaseShiftCfg));
  
     txAntMask     = gMmwMssMCB.cfg.openCfg.chCfg.txChannelEn;
     numTxAntAzim = mathUtils_countSetBits(txAntMask & MmwDemo_RFParserHwCfg.azimTxAntMask);
     numTxAntElev = mathUtils_countSetBits(txAntMask & MmwDemo_RFParserHwCfg.elevTxAntMask);
  
     gMmwMssMCB.numEmptySubBands = MmwDemo_getNumEmptySubBands(numTxAntAzim + numTxAntElev);
     numTxTotalDivisor =  numTxAntAzim + numTxAntElev + gMmwMssMCB.numEmptySubBands;
  
     activeTxCnt = 0;
     /* Get the phase multiplier factor for each Tx antenna */
     /* Loop over Tx Antenna Phase order index */
     for(txOrderIdx = 0; txOrderIdx < SYS_COMMON_NUM_TX_ANTENNAS; txOrderIdx++){
         /* Check if the Tx antenna corresponding to the xth index in the phase order is enabled */
         if(1 << gMmwMssMCB.ddmPhaseShiftOrder[txOrderIdx] & txAntMask){
             /* Antenna is enabled, hence compute the phase shift value */
             phaseShiftMultiplier[gMmwMssMCB.ddmPhaseShiftOrder[txOrderIdx]] = activeTxCnt;
             activeTxCnt++;
         }
         else{
             /* Antenna is disabled */
             phaseShiftMultiplier[gMmwMssMCB.ddmPhaseShiftOrder[txOrderIdx]] = 0;
         }
     }
  
     /* Configure Phase Shifter Chirps */
     if(gMmwMssMCB.cfg.ctrlCfg.dfeDataOutputMode == MMWave_DFEDataOutputMode_FRAME){
  
         chirpStartIdx = gMmwMssMCB.cfg.ctrlCfg.u.frameCfg[0].frameCfg.chirpStartIdx;
         chirpEndIdx   = gMmwMssMCB.cfg.ctrlCfg.u.frameCfg[0].frameCfg.chirpEndIdx;
  
         /* Phase multipliers have been computed; phase shift for xth chirp = (x-1) * phase multipler */
         for (chirpIdx = chirpStartIdx; chirpIdx <= chirpEndIdx; chirpIdx++){
  
             chirpPhaseMultiplier = (chirpIdx - chirpStartIdx);
  
             /* Populate the chirp configuration: */
             phaseShiftCfg.chirpStartIdx   = (chirpEndIdx + 1 - chirpPhaseMultiplier) % numTxTotalDivisor;
             phaseShiftCfg.chirpEndIdx     = (chirpEndIdx + 1 - chirpPhaseMultiplier) % numTxTotalDivisor;
  
             /* 1 LSB of phaseShiftCfg.txPhaseShift = 360/2^6 = 5.625 degrees Valid range: 0 to 63 */
             phaseShiftCfg.tx0PhaseShift    =
                         ((uint32_t) MATHUTILS_ROUND_FLOAT(((float) ((chirpPhaseMultiplier * phaseShiftMultiplier[0]) % numTxTotalDivisor) / numTxTotalDivisor) * (1U << 6))) << 2;
  
             /* 1 LSB of phaseShiftCfg.txPhaseShift = 360/2^6 = 5.625 degrees Valid range: 0 to 63 */
             phaseShiftCfg.tx1PhaseShift    =
                         ((uint32_t) MATHUTILS_ROUND_FLOAT(((float) ((chirpPhaseMultiplier * phaseShiftMultiplier[1]) % numTxTotalDivisor) / numTxTotalDivisor) * (1U << 6))) << 2;
  
             /* 1 LSB of phaseShiftCfg.txPhaseShift = 360/2^6 = 5.625 degrees Valid range: 0 to 63 */
             phaseShiftCfg.tx2PhaseShift    =
                         ((uint32_t) MATHUTILS_ROUND_FLOAT(((float) ((chirpPhaseMultiplier * phaseShiftMultiplier[2]) % numTxTotalDivisor) / numTxTotalDivisor) * (1U << 6))) << 2;
  
 #ifdef SOC_AWR2944
             /* 1 LSB of phaseShiftCfg.txPhaseShift = 360/2^6 = 5.625 degrees Valid range: 0 to 63 */
             phaseShiftCfg.tx3PhaseShift    =
                         ((uint32_t) MATHUTILS_ROUND_FLOAT(((float) ((chirpPhaseMultiplier * phaseShiftMultiplier[3]) % numTxTotalDivisor) / numTxTotalDivisor) * (1U << 6))) << 2;
 #endif
  
             /* Add the chirp to the profile */
             if (MMWave_addPhaseShiftChirp (gMmwMssMCB.ctrlHandle, &phaseShiftCfg, &errCode) == NULL)
             {
                 /* Error: Unable to add the phase shifter chirp. Return the error code. */
                 CLI_write ("Error: Unable to add the phase shifter chirp.\n");
                 return errCode;
             }
         }
     }
  
     else if(gMmwMssMCB.cfg.ctrlCfg.dfeDataOutputMode == MMWave_DFEDataOutputMode_ADVANCED_FRAME){
         uint8_t numOfSubFrames = gMmwMssMCB.cfg.ctrlCfg.u.advancedFrameCfg[0].frameCfg.frameSeq.numOfSubFrames;
         uint8_t subFrameIdx = 0;
         for(subFrameIdx =0; subFrameIdx < numOfSubFrames; subFrameIdx++ ){
             chirpStartIdx = gMmwMssMCB.cfg.ctrlCfg.u.advancedFrameCfg[0].frameCfg.frameSeq.subFrameCfg[subFrameIdx].chirpStartIdx;
             chirpEndIdx   = gMmwMssMCB.cfg.ctrlCfg.u.advancedFrameCfg[0].frameCfg.frameSeq.subFrameCfg[subFrameIdx].numOfChirps + chirpStartIdx - 1;
  
             /* Phase multipliers have been computed; phase shift for xth chirp = (x-1) * phase multipler */
             for (chirpIdx = chirpStartIdx; chirpIdx <= chirpEndIdx; chirpIdx++){
  
                 // printf("Chirp %d\n", chirpIdx);
                 chirpPhaseMultiplier = (chirpIdx - chirpStartIdx);
  
                 /* Populate the chirp configuration: */
                 phaseShiftCfg.chirpStartIdx   = (chirpEndIdx + 1 - chirpPhaseMultiplier) % numTxTotalDivisor + chirpStartIdx;
                 phaseShiftCfg.chirpEndIdx     = (chirpEndIdx + 1 - chirpPhaseMultiplier) % numTxTotalDivisor + chirpStartIdx;
  
                 // printf("phaseShiftCfg.chirpEndIdx: %d\n", phaseShiftCfg.chirpEndIdx);
  
                 /* 1 LSB of phaseShiftCfg.txPhaseShift = 360/2^6 = 5.625 degrees Valid range: 0 to 63 */
                 phaseShiftCfg.tx0PhaseShift   =
                             ((uint32_t) MATHUTILS_ROUND_FLOAT(((float) ((chirpPhaseMultiplier * phaseShiftMultiplier[0]) % numTxTotalDivisor) / numTxTotalDivisor) * (1U << 6))) << 2;
  
                 /* 1 LSB of phaseShiftCfg.txPhaseShift = 360/2^6 = 5.625 degrees Valid range: 0 to 63 */
                 phaseShiftCfg.tx1PhaseShift    =
                             ((uint32_t) MATHUTILS_ROUND_FLOAT(((float) ((chirpPhaseMultiplier * phaseShiftMultiplier[1]) % numTxTotalDivisor) / numTxTotalDivisor) * (1U << 6))) << 2;
  
                 /* 1 LSB of phaseShiftCfg.txPhaseShift = 360/2^6 = 5.625 degrees Valid range: 0 to 63 */
                 phaseShiftCfg.tx2PhaseShift    =
                             ((uint32_t) MATHUTILS_ROUND_FLOAT(((float) ((chirpPhaseMultiplier * phaseShiftMultiplier[2]) % numTxTotalDivisor) / numTxTotalDivisor) * (1U << 6))) << 2;
  
 #ifdef SOC_AWR2944
                 /* 1 LSB of phaseShiftCfg.txPhaseShift = 360/2^6 = 5.625 degrees Valid range: 0 to 63 */
                 phaseShiftCfg.tx3PhaseShift    =
                             ((uint32_t) MATHUTILS_ROUND_FLOAT(((float) ((chirpPhaseMultiplier * phaseShiftMultiplier[3]) % numTxTotalDivisor) / numTxTotalDivisor) * (1U << 6))) << 2;
 #endif
  
                 /* Add the chirp to the profile */
                 if (MMWave_addPhaseShiftChirp (gMmwMssMCB.ctrlHandle, &phaseShiftCfg, &errCode) == NULL)
                 {
                 /* Error: Unable to add the phase shifter chirp. Return the error code. */
                     CLI_write ("Error: Unable to add the phase shifter chirp.\n");
                     return errCode;
                 }
                 // printf("Phase shift values- chirp %d\n", chirpIdx);
                 // printf("%d\n%d\n%d\n%d\n\n", phaseShiftCfg.tx0PhaseShift/4, phaseShiftCfg.tx1PhaseShift/4, phaseShiftCfg.tx2PhaseShift/4, phaseShiftCfg.tx3PhaseShift/4);
             }
         }
     }
  
     return errCode;
 }
  
 int32_t MmwDemo_getNumEmptySubBands(uint32_t numTxAntennas){
  
     int32_t numBandsEmpty;
     /* Empty subbands */
     switch (numTxAntennas)
     {
         case 2:
             numBandsEmpty = 1;
             break;
         case 3:
             numBandsEmpty = 1;
             break;
         case 4:
             numBandsEmpty = 2;
             break;
         default:
             numBandsEmpty = -1;
             goto exit;
     }
  
 exit:
     return numBandsEmpty;
 }
 #endif
  
 int32_t MmwDemo_configSensor(void)
 {
     int32_t     errCode = 0;
  
 #ifdef MMWDEMO_DDM
     errCode = MmwDemo_configPhaseShifterChirps();
     if(errCode != 0){
         goto exit;
     }
 #endif
  
     /* Configure the mmWave module: */
     if (MMWave_config (gMmwMssMCB.ctrlHandle, &gMmwMssMCB.cfg.ctrlCfg, &errCode) < 0)
     {
         MMWave_ErrorLevel   errorLevel;
         int16_t             mmWaveErrorCode;
         int16_t             subsysErrorCode;
  
         /* Error: Report the error */
         MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);
         test_print ("Error: mmWave Config failed [Error code: %d Subsystem: %d]\n",
                         mmWaveErrorCode, subsysErrorCode);
         goto exit;
     }
     else
     {
         errCode = MmwDemo_dataPathConfig();
     }
  
 exit:
     return errCode;
 }
  
 int32_t MmwDemo_startSensor(void)
 {
     int32_t     errCode;
     MMWave_CalibrationCfg   calibrationCfg;
  
     /*****************************************************************************
      * Data path :: start data path first - this will pend for DPC to ack
      *****************************************************************************/
     MmwDemo_dataPathStart();
  
     /*****************************************************************************
      * RF :: now start the RF and the real time ticking
      *****************************************************************************/
     /* Initialize the calibration configuration: */
     memset ((void *)&calibrationCfg, 0, sizeof(MMWave_CalibrationCfg));
     /* Populate the calibration configuration: */
     calibrationCfg.dfeDataOutputMode = gMmwMssMCB.cfg.ctrlCfg.dfeDataOutputMode;
     calibrationCfg.u.chirpCalibrationCfg.enableCalibration    = true;
     calibrationCfg.u.chirpCalibrationCfg.enablePeriodicity    = true;
     calibrationCfg.u.chirpCalibrationCfg.periodicTimeInFrames = 10U;
     calibrationCfg.u.chirpCalibrationCfg.reportEn             = 1;
  
     DebugP_logInfo("App: MMWave_start Issued\n");
  
     test_print("Starting Sensor (issuing MMWave_start)\n");
  
     /* Start the mmWave module: The configuration has been applied successfully. */
     if (MMWave_start(gMmwMssMCB.ctrlHandle, &calibrationCfg, &errCode) < 0)
     {
         MMWave_ErrorLevel   errorLevel;
         int16_t             mmWaveErrorCode;
         int16_t             subsysErrorCode;
  
         /* Error/Warning: Unable to start the mmWave module */
         MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);
         test_print ("Error: mmWave Start failed [mmWave Error: %d Subsys: %d]\n", mmWaveErrorCode, subsysErrorCode);
         /* datapath has already been moved to start state; so either we initiate a cleanup of start sequence or
            assert here and re-start from the beginning. For now, choosing the latter path */
         MmwDemo_debugAssert(0);
         return -1;
     }
  
     gMmwMssMCB.sensorStartCount++;
     return 0;
 }
  
 static void MmwDemo_sensorStopEpilog(void)
 {
     /* Note data path has completely stopped due to
      * end of frame, so we can do non-real time processing like prints on
      * console */
     test_print("Data Path Stopped (last frame processing done)\n");
 }
  
  
 void MmwDemo_stopSensor(void)
 {
     int32_t errCode;
  
     /* Stop sensor RF, data path will be stopped after RF stop is completed */
     MmwDemo_mmWaveCtrlStop();
  
     /* Wait until DPM_stop is completed */
     SemaphoreP_pend(&gMmwMssMCB.DPMstopSemHandle, SystemP_WAIT_FOREVER);
  
 #ifdef LVDS_STREAM
     /* Delete any active streaming session */
     if(gMmwMssMCB.lvdsStream.hwSessionHandle != NULL)
     {
         /* Evaluate need to deactivate h/w session:
          * One sub-frame case:
          *   if h/w only enabled, deactivation never happened, hence need to deactivate
          *   if h/w and s/w both enabled, then s/w would leave h/w activated when it is done
          *   so need to deactivate
          *   (only s/w enabled cannot be the case here because we are checking for non-null h/w session)
          * Multi sub-frame case:
          *   Given stop, we must have re-configured the next sub-frame by now which is next of the
          *   last sub-frame i.e we must have re-configured sub-frame 0. So if sub-frame 0 had
          *   h/w enabled, then it is left in active state and need to deactivate. For all
          *   other cases, h/w was already deactivated when done.
          */
         if ((gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.numSubFrames == 1) ||
             ((gMmwMssMCB.objDetCommonCfg.preStartCommonCfg.numSubFrames > 1) &&
              (gMmwMssMCB.subFrameCfg[0].lvdsStreamCfg.dataFmt != MMW_DEMO_LVDS_STREAM_CFG_DATAFMT_DISABLED))
            )
         {
             if (CBUFF_deactivateSession(gMmwMssMCB.lvdsStream.hwSessionHandle, &errCode) < 0)
             {
                 test_print("CBUFF_deactivateSession failed with errorCode = %d\n", errCode);
                 MmwDemo_debugAssert(0);
             }
         }
         MmwDemo_LVDSStreamDeleteHwSession();
     }
  
     /* Delete s/w session if it exists. S/w session never needs to be deactivated in stop because
      * it always (unconditionally) deactivates itself upon completion.
      */
     if(gMmwMssMCB.lvdsStream.swSessionHandle != NULL)
     {
         MmwDemo_LVDSStreamDeleteSwSession();
     }
 #endif
  
     /* Print epilog */
     MmwDemo_sensorStopEpilog();
  
     gMmwMssMCB.sensorStopCount++;
  
     /* print for user */
     test_print("Sensor has been stopped: startCount: %d stopCount %d\n",
                   gMmwMssMCB.sensorStartCount,gMmwMssMCB.sensorStopCount);
 }
  
 /**************************************************************************
  ******************** Millimeter Wave Demo init Functions ************************
  **************************************************************************/
  
 static void MmwDemo_platformInit(MmwDemo_platformCfg *config)
 {
     /* Initialize the DEMO configuration: */
     config->sysClockFrequency   = MSS_SYS_VCLK;
     config->loggingBaudRate     = 892857;
     config->commandBaudRate     = 115200;
  
 }
  
 #ifdef LVDS_STREAM
  
 static void MmwDemo_checkEdmaErrors(void)
 {
     bool        isAnyError = false;
     uint32_t    baseAddr = 0U;
  
     baseAddr = EDMA_getBaseAddr(gMmwMssMCB.edmaHandle);
     DebugP_assert(baseAddr != 0);
  
     isAnyError = ((EDMA_getErrIntrStatus(baseAddr) != 0U) ||
                    (EDMA_errIntrHighStatusGet(baseAddr) != 0U));
  
     if (isAnyError == true)
     {
         test_print("EDMA channel controller has errors, see gMmwMssMCB.EDMA_errorInfo\n");
         MmwDemo_debugAssert(0);
     }
 }
 #endif
  
 static int32_t MmwDemo_calibInit(void)
 {
     int32_t        retVal = 0;
     rlVersion_t    verArgs;
  
     /* Initialize verArgs */
     memset((void *)&verArgs, 0, sizeof(rlVersion_t));
  
     /* Calibration save/restore init */
     gMmwMssMCB.calibCfg.sizeOfCalibDataStorage = sizeof(MmwDemo_calibData);
     gMmwMssMCB.calibCfg.calibDataHdr.magic = MMWDEMO_CALIB_STORE_MAGIC;
     memcpy((void *)& gMmwMssMCB.calibCfg.calibDataHdr.linkVer, (void *)&verArgs.mmWaveLink, sizeof(rlSwVersionParam_t));
     memcpy((void *)& gMmwMssMCB.calibCfg.calibDataHdr.radarSSVer, (void *)&verArgs.rf, sizeof(rlFwVersionParam_t));
  
     /* Check if Calibration data is over the Reserved storage */
     if(gMmwMssMCB.calibCfg.sizeOfCalibDataStorage   <= MMWDEMO_CALIB_FLASH_SIZE)
     {
         gMmwMssMCB.calibCfg.calibDataHdr.hdrLen = sizeof(MmwDemo_calibDataHeader);
         gMmwMssMCB.calibCfg.calibDataHdr.dataLen= sizeof(MmwDemo_calibData) - sizeof(MmwDemo_calibDataHeader);
  
         /* Resets calibration data */
         memset((void *)&gCalibDataStorage, 0, sizeof(MmwDemo_calibData));
  
         retVal = mmwDemo_flashInit();
     }
     else
     {
         test_print ("Error: Calibration data size is bigger than reserved size\n");
         retVal = -1;
     }
  
     return retVal;
  
 }
  
 static int32_t MmwDemo_calibSave(MmwDemo_calibDataHeader *ptrCalibDataHdr, MmwDemo_calibData  *ptrCalibrationData)
 {
     uint32_t         flashOffset;
     int32_t          retVal = 0;
  
     /* Calculate the read size in bytes */
     flashOffset = gMmwMssMCB.calibCfg.flashOffset;
  
     /* Copy header  */
     memcpy((void *)&(ptrCalibrationData->calibDataHdr), ptrCalibDataHdr, sizeof(MmwDemo_calibDataHeader));
  
     /* Flash calibration data */
     retVal = mmwDemo_flashWrite(flashOffset, (uint8_t *)ptrCalibrationData, sizeof(MmwDemo_calibData));
     if(retVal < 0)
     {
         /* Flash Header failed */
         test_print ("Error: MmwDemo failed flashing calibration data with error[%d].\n", retVal);
     }
     return(retVal);
 }
  
  
 static int32_t MmwDemo_calibRestore(MmwDemo_calibData  *ptrCalibData)
 {
     MmwDemo_calibDataHeader    *pDataHdr;
     int32_t                     retVal = 0;
     uint32_t                    flashOffset;
  
     pDataHdr = &(ptrCalibData->calibDataHdr);
  
     /* Calculate the read size in bytes */
     flashOffset = gMmwMssMCB.calibCfg.flashOffset;
  
     /* Read calibration data header */
     if(mmwDemo_flashRead(flashOffset, (uint8_t *)pDataHdr, sizeof(MmwDemo_calibData)) < 0)
     {
         /* Error: only one can be enable at at time */
         test_print ("Error: MmwDemo failed when reading calibration data from flash.\n");
         return -1;
     }
  
     /* Validate data header */
     if( (pDataHdr->magic != MMWDEMO_CALIB_STORE_MAGIC) ||
          (pDataHdr->hdrLen != gMmwMssMCB.calibCfg.calibDataHdr.hdrLen) ||
          (pDataHdr->dataLen != gMmwMssMCB.calibCfg.calibDataHdr.dataLen) )
     {
         /* Header validation failed */
         test_print ("Error: MmwDemo calibration data header validation failed.\n");
         retVal = -1;
     }
     /* Matching mmwLink version:
          In this demo, we would like to save/restore with the matching mmwLink and RF FW version.
          However, this logic can be changed to use data saved from previous mmwLink and FW releases,
          as long as the data format of the calibration data matches.
      */
     else if(memcmp((void *)&pDataHdr->linkVer, (void *)&gMmwMssMCB.calibCfg.calibDataHdr.linkVer, sizeof(rlSwVersionParam_t)) != 0)
     {
         test_print ("Error: MmwDemo failed mmwLink version validation when restoring calibration data.\n");
         retVal = -1;
     }
     else if(memcmp((void *)&pDataHdr->radarSSVer, (void *)&gMmwMssMCB.calibCfg.calibDataHdr.radarSSVer, sizeof(rlFwVersionParam_t)) != 0)
     {
         test_print ("Error: MmwDemo failed RF FW version validation when restoring calibration data.\n");
         retVal = -1;
     }
     return(retVal);
 }
  
 void MmwDemo_mssSECCallBack(void)
 {
     CSL_mss_ecc_agg_mssRegs* ptrMssEccAggregs = (CSL_mss_ecc_agg_mssRegs*)CSL_MSS_ECC_AGG_MSS_U_BASE;
  
     /* Check if error address falls in affected memory banks
             MSS L2 Bank 0
             MSS_L2 Bank 1
             MSS MAILBOX
             MSS RETRAM
         Interrupts are enabled for only above listed memories
      */
     if ((ptrMssEccAggregs->SEC_STATUS_REG0 | 0xF) != 0U)
     {
         /*
          * A single bit memory correction event is detected on effected memories.
          * Treat this error as critical as per Errata
          */
         MmwDemo_debugAssert(0);
     }
 }
  
  
 static void MmwDemo_initTask(void* args)
 {
     int32_t             errCode;
     MMWave_InitCfg      initCfg;
     DPM_InitCfg         dpmInitCfg;
     DPC_ObjectDetection_InitParams      objDetInitParams;
     int32_t             i;
     MMWave_ErrorLevel       errorLevel;
     int16_t                 mmWaveErrorCode;
     int16_t                 subsysErrorCode;
  
     Drivers_open();
     Board_driversOpen();
  
     /* Clear all status registers of MSS AGGRA */
     MmwDemo_clearEccAggrs();
  
     /* Enable ECC and interrupts */
     HW_WR_REG32((CSL_MSS_ECC_AGG_MSS_U_BASE + CSL_MSS_ECC_AGGA_SEC_ENABLE_SET_REG0), 0xF);
  
     MmwDemo_BoardInit();
  
     /* Debug Message: */
     test_print ("**********************************************\n");
     test_print ("Debug: Launching the MMW Demo on MSS\n");
     test_print ("**********************************************\n");
  
     /* Debug Message: */
     test_print("Debug: Launched the Initialization Task\n");
  
     /*****************************************************************************
      * Initialize the mmWave SDK components:
      *****************************************************************************/
  
     /* Initialize Last Frame data Set flag for first frame. */
     gMmwMssMCB.stats.isLastFrameDataProcessed = true;
  
 #ifdef LVDS_STREAM
     gMmwMssMCB.edmaHandle = gEdmaHandle[CONFIG_EDMA0];
  
     /* Initialize LVDS streaming components */
     if ((errCode = MmwDemo_LVDSStreamInit()) < 0 )
     {
         test_print ("Error: MMWDemoDSS LVDS stream init failed with Error[%d]\n",errCode);
         return;
     }
  
     /* Configure Pad registers for LVDS. */
     HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_LVDS_PAD_CTRL0 , 0x0);
     HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_LVDS_PAD_CTRL1 , 0x02000000);
  
     /*The delay below is needed only if the DCA1000EVM is being used to capture the data traces.
       This is needed because the DCA1000EVM FPGA needs the delay to lock to the
       bit clock before they can start capturing the data correctly. */
     ClockP_usleep(12 * 1000);
 #endif
  
     /* initialize cq configs to invalid profile index to be able to detect
      * unconfigured state of these when monitors for them are enabled.
      */
     for(i = 0; i < RL_MAX_PROFILES_CNT; i++)
     {
         gMmwMssMCB.cqSatMonCfg[i].profileIndx    = (RL_MAX_PROFILES_CNT + 1);
         gMmwMssMCB.cqSigImgMonCfg[i].profileIndx = (RL_MAX_PROFILES_CNT + 1);
     }
  
     /* Platform specific configuration */
     MmwDemo_platformInit(&gMmwMssMCB.cfg.platformCfg);
  
     /* Workaround for Errata ANA#46: Spurs caused due to data transfer activity */
     if (MmwDemo_registerChirpStartInterrupt() != 0)
     {
         CLI_write("Error: Failed to register chirp start interrupts\r\n");
         DebugP_assert(0);
     }
  
     /* Open the UART Instance */
     gMmwMssMCB.commandUartHandle = gUartHandle[CONFIG_UART0];
     if (gMmwMssMCB.commandUartHandle == NULL)
     {
         MmwDemo_debugAssert (0);
         return;
     }
  
     /* Open the Logging UART Instance: */
     gMmwMssMCB.loggingUartHandle = gUartHandle[CONFIG_UART1];
     if (gMmwMssMCB.loggingUartHandle == NULL)
     {
         test_print("Error: Unable to open the Logging UART Instance\n");
         MmwDemo_debugAssert (0);
         return;
     }
  
     DebugP_logInfo("Both UART instances opened");
  
     /* Create binary semaphores which is used to signal DPM_start/DPM_stop/DPM_ioctl is done
      * to the sensor management task. The signalling (SemaphoreP_post) will be done
      * from DPM registered report function (which will execute in the DPM execute task context). */
     SemaphoreP_constructBinary(&gMmwMssMCB.DPMstartSemHandle, 0);
     SemaphoreP_constructBinary(&gMmwMssMCB.DPMstopSemHandle, 0);
     SemaphoreP_constructBinary(&gMmwMssMCB.DPMioctlSemHandle, 0);
     SemaphoreP_constructBinary(&gMmwMssMCB.UartExportSemHandle, 0);
  
     /* Create binary semaphore to pend Main task, */
     SemaphoreP_constructBinary(&gMmwMssMCB.demoInitTaskCompleteSemHandle, 0);
  
     /*****************************************************************************
      * mmWave: Initialization of the high level module
      *****************************************************************************/
  
     /* Initialize the mmWave control init configuration */
     memset ((void*)&initCfg, 0 , sizeof(MMWave_InitCfg));
  
     /* Populate the init configuration: */
     initCfg.domain                  = MMWave_Domain_MSS;
     initCfg.eventFxn                = MmwDemo_eventCallbackFxn;
     initCfg.linkCRCCfg.crcBaseAddr  = (uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR);
     initCfg.linkCRCCfg.useCRCDriver = 1U;
     initCfg.linkCRCCfg.crcChannel   = CRC_CHANNEL_1;
     initCfg.cfgMode                 = MMWave_ConfigurationMode_FULL;
  
     /* Initialize and setup the mmWave Control module */
     gMmwMssMCB.ctrlHandle = MMWave_init (&initCfg, &errCode);
     if (gMmwMssMCB.ctrlHandle == NULL)
     {
          /* Error: Unable to initialize the mmWave control module */
         MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);
  
         /* Error: Unable to initialize the mmWave control module */
         test_print ("Error: mmWave Control Initialization failed [Error code %d]\n", errCode);
         MmwDemo_debugAssert (0);
         return;
     }
     test_print ("Debug: mmWave Control Initialization was successful\n");
  
     /* Synchronization: This will synchronize the execution of the control module
      * between the domains. This is a prerequiste and always needs to be invoked. */
     if (MMWave_sync(gMmwMssMCB.ctrlHandle, &errCode) < 0)
     {
         /* Error: Unable to synchronize the mmWave control module */
         test_print ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);
         MmwDemo_debugAssert (0);
         return;
     }
     test_print ("Debug: mmWave Control Synchronization was successful\n");
  
     /*****************************************************************************
      * Launch the mmWave control execution task
      * - This should have a higher priroity than any other task which uses the
      *   mmWave control API
      *****************************************************************************/
     gMmwMssMCB.taskHandles.mmwCtrlTask = xTaskCreateStatic( MmwDemo_mmWaveCtrlTask,
                                       "mmwdemo_ctrl_task",
                                       MMWDEMO_MMWAVE_CTRL_TASK_STACK_SIZE,
                                       NULL,
                                       MMWDEMO_MMWAVE_CTRL_TASK_PRIORITY,
                                       gMmwCtrlTskStack,
                                       &gMmwMssMCB.taskHandles.mmwCtrlTaskObj );
  
     configASSERT(gMmwMssMCB.taskHandles.mmwCtrlTask != NULL);
     DebugP_log("CtrTask Done\n");
  

  
     /*****************************************************************************
      * Initialization of the DPM Module:
      *****************************************************************************/
     memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));
  
     /* Setup the configuration: */
     dpmInitCfg.ptrProcChainCfg  = NULL;
     dpmInitCfg.instanceId       = DPC_OBJDET_INSTANCEID;
     dpmInitCfg.domain           = DPM_Domain_REMOTE;
     dpmInitCfg.reportFxn        = MmwDemo_DPC_ObjectDetection_reportFxn;
     dpmInitCfg.arg              = &objDetInitParams;
     dpmInitCfg.argSize          = sizeof(DPC_ObjectDetection_InitParams);
  
     /* Initialize the DPM Module: */
     gMmwMssMCB.objDetDpmHandle = DPM_init (&dpmInitCfg, &errCode);
     if (gMmwMssMCB.objDetDpmHandle == NULL)
     {
         test_print ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
         MmwDemo_debugAssert (0);
         return;
     }
  
     /* Synchronization: This will synchronize the execution of the datapath module
      * between the domains. This is a prerequiste and always needs to be invoked. */
     while (1)
     {
         int32_t syncStatus;
  
         /* Get the synchronization status: */
         syncStatus = DPM_synch (gMmwMssMCB.objDetDpmHandle, &errCode);
         if (syncStatus < 0)
         {
             /* Error: Unable to synchronize the framework */
             test_print ("Error: DPM Synchronization failed [Error code %d]\n", errCode);
             MmwDemo_debugAssert (0);
             return;
         }
         if (syncStatus == 1)
         {
             /* Synchronization acheived: */
             break;
         }
         /* Sleep and poll again: */
         ClockP_usleep(1 * 1000U);
     }
  
     /* Launch the DPM Task */
     gMmwMssMCB.taskHandles.mmwObjDetDpmTask = xTaskCreateStatic( mmwDemo_mssDPMTask,
                                            "mmwdemo_dpm_task",
                                            MMWDEMO_DPC_OBJDET_DPM_TASK_STACK_SIZE,
                                            NULL,
                                            MMWDEMO_DPC_OBJDET_DPM_TASK_PRIORITY,
                                            gDpmTskStack,
                                            &gMmwMssMCB.taskHandles.mmwObjDetDpmTaskObj );
  
     configASSERT(gMmwMssMCB.taskHandles.mmwObjDetDpmTask != NULL);
    DebugP_log("DPMTask Done\n");
     /* Calibration save/restore initialization */
     if(MmwDemo_calibInit()<0)
     {
         test_print("Error: Calibration data initialization failed \n");
         MmwDemo_debugAssert (0);
     }
  
     /* Launch the UART Data Export Task */
     gMmwMssMCB.taskHandles.uartDataExportTask = xTaskCreateStatic( mmwDemo_mssUartDataExportTask,
                                            "mmwdemo_uart_task",
                                            MMWDEMO_UART_DATA_EXPORT_TASK_STACK_SIZE,
                                            NULL,
                                            MMWDEMO_UART_EXPORT_TASK_PRIORITY,
                                            gUartTskStack,
                                            &gMmwMssMCB.taskHandles.uartDataExportTaskObj );
  
     configASSERT(gMmwMssMCB.taskHandles.uartDataExportTask != NULL);
    DebugP_log("UARTTask Done\n");
     /*****************************************************************************
      * Initialize the Profiler
      *****************************************************************************/
     CycleCounterP_reset();
  
     /*****************************************************************************
      * Initialize the CLI Module:
      *****************************************************************************/
     DebugP_log("Before CLI Init\n");
     MmwDemo_CLIInit(MMWDEMO_CLI_TASK_PRIORITY);
     DebugP_log("After CLI Init\n");
    DebugP_log("UART0 handle=%p (CLI), UART1 handle=%p (DATA)\n",
        gUartHandle[CONFIG_UART0],
        gUartHandle[CONFIG_UART1]);
  
     /* Never return for this task. */
     SemaphoreP_pend(&gMmwMssMCB.demoInitTaskCompleteSemHandle, SystemP_WAIT_FOREVER);
  
     /* The following line should never be reached. */
     DebugP_assertNoLog(0);
 }

  #ifdef ENET_STREAM
    /*****************************************************************************
      * Launch the mmWave enet task
      *****************************************************************************/
     /* Create Enet configuration done semaphore */
     SemaphoreP_constructBinary(&gMmwMssMCB.enetCfg.EnetCfgDoneSemHandle, 0);
  
     gMmwMssMCB.taskHandles.enetTask = xTaskCreateStatic( enetTask,
                                       "enet_task",
                                       MMWDEMO_MMWAVE_ENET_TASK_STACK_SIZE,
                                       NULL,
                                       MMWDEMO_MMWAVE_ENET_TASK_PRIORITY,
                                       gMmwEnetTskStack,
                                       &gMmwMssMCB.taskHandles.enetTaskObj );
  
     configASSERT(gMmwMssMCB.taskHandles.enetTask != NULL);
     DebugP_log("EnetTask Done\n");
 #endif
  
 static bool MmwDemo_BoardInit(void)
 {
     /* Configure HSI Clock. */
     HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_HSI_CLK_SRC_SEL, 0x333);
  
     return true;
 }
  
 static void MmwDemo_clearEccAggrs(void)
 {
     uint8_t idx = 0U;
  
     /* Clear all status registers of MSS ECC AGGR.*/
     for(idx = 0; idx <= MMWDEMO_MSS_ECC_AGG_RAM_IDS_TOTAL_ENTRIES; idx++)
     {
         /* Write the RAM ID in to Vector register*/
         CSL_REG32_FINS(MMWDEMO_MSS_ECC_AGG_MSS_ECC_VECTOR_ADDR, MSS_ECC_AGG_MSS_ECC_VECTOR_ECC_VECTOR, idx);
         /* Clear pending interrupts.*/
         CSL_REG32_WR(MMWDEMO_MSS_ECC_AGG_MSS_ERROR_STATUS1_ADDR, 0xF00);
  
         HW_WR_REG32((CSL_MSS_ECC_AGG_MSS_U_BASE + CSL_MSS_ECC_AGG_MSS_CONTROL), 0x7);
         ClockP_usleep(100);
     }
 }
  
 static uint32_t computeSscFactCtrlVal(const uint32_t refClk, const uint16_t dpllM, MmwDemo_spreadSpectrumConfig *ptrdpllCfg)
 {
     float modRateSel = 0.0f;
     float deltaMStep = 0.0f;
     uint32_t delatMStepInt = 0U;
     float deltaMStepFrac = 0.0f;
     uint32_t deltaMStepFracInt = 0U;
     uint32_t modFreqDivExponent = 0U;
     uint32_t modFreqDivMantissa = 0U;
  
     modRateSel = (refClk * 1000.0f) / (4.0f * ptrdpllCfg->modRate);
  
     modFreqDivExponent = (uint32_t) (floor(modRateSel/MAX_MOD_FREQ_DIVIDER_MANTISSA));
  
     modFreqDivMantissa = (uint32_t) (floor(modRateSel / pow(2.0f, modFreqDivExponent)));
  
     ptrdpllCfg->modRate = refClk * 1000 / (4 * modFreqDivMantissa * (pow(2, modFreqDivExponent)));
  
     if(modFreqDivExponent <= 3U)
     {
         deltaMStep = (ptrdpllCfg->modDepth * dpllM) / (100.0f * modFreqDivMantissa * pow(2.0f, modFreqDivExponent));
     }
     else
     {
         deltaMStep = (ptrdpllCfg->modDepth * dpllM) / (100.0f * modFreqDivMantissa * 8.0f);
     }
  
     delatMStepInt = (uint32_t) (deltaMStep + 0.5f);
  
     deltaMStepFrac = deltaMStep - delatMStepInt;
  
     deltaMStepFracInt = (uint32_t) ceilf(deltaMStepFrac * (1U << 18U));
  
     ptrdpllCfg->modDepth = 100 * ( (deltaMStepFrac/ (1 << 18) ) * modFreqDivMantissa * ( 1U << modFreqDivExponent) / dpllM);
  
     return (deltaMStepFracInt + (delatMStepInt * (1U << 18U)) +
             (modFreqDivMantissa * (1U << 21U)) + (modFreqDivExponent * (1U << 28U)) +
             (ptrdpllCfg->downSpread * (1U << 31U)));
 }
  
  
 void MMWDemo_configSSC(void)
 {
     uint16_t dpllM = 0U;
     uint16_t dpllN = 0U;
     uint32_t finp = 40U; /* XTAL */
     uint32_t refClk = 0U;
  
     CSL_mss_toprcmRegs *ptrMssTopRcmRegs = (CSL_mss_toprcmRegs *)CSL_MSS_TOPRCM_U_BASE;
  
     if(gMmwMssMCB.coreAdpllSscCfg.isEnable)
     {
         dpllM = CSL_FEXT(ptrMssTopRcmRegs->PLL_CORE_MN2DIV,
                          MSS_TOPRCM_PLL_CORE_MN2DIV_PLL_CORE_MN2DIV_M);
  
         dpllN = CSL_FEXT(ptrMssTopRcmRegs->PLL_CORE_M2NDIV,
                          MSS_TOPRCM_PLL_CORE_M2NDIV_PLL_CORE_M2NDIV_N);
  
         refClk = finp/(dpllN + 1);
  
         ptrMssTopRcmRegs->PLL_CORE_FRACCTRL = computeSscFactCtrlVal(refClk, dpllM, &gMmwMssMCB.coreAdpllSscCfg);
  
         CSL_FINS(ptrMssTopRcmRegs->PLL_CORE_CLKCTRL, MSS_TOPRCM_PLL_CORE_CLKCTRL_PLL_CORE_CLKCTRL_ENSSC, 1U);
  
     }
  
     if(gMmwMssMCB.dspAdpllSscCfg.isEnable)
     {
         dpllM = CSL_FEXT(ptrMssTopRcmRegs->PLL_DSP_MN2DIV,
                          MSS_TOPRCM_PLL_DSP_MN2DIV_PLL_DSP_MN2DIV_M);
  
         dpllN = CSL_FEXT(ptrMssTopRcmRegs->PLL_DSP_M2NDIV,
                          MSS_TOPRCM_PLL_DSP_M2NDIV_PLL_DSP_M2NDIV_N);
  
         refClk = finp/(dpllN + 1);
  
         ptrMssTopRcmRegs->PLL_DSP_FRACCTRL = computeSscFactCtrlVal(refClk, dpllM, &gMmwMssMCB.dspAdpllSscCfg);
  
         CSL_FINS(ptrMssTopRcmRegs->PLL_DSP_CLKCTRL, MSS_TOPRCM_PLL_DSP_CLKCTRL_PLL_DSP_CLKCTRL_ENSSC, 1U);
  
     }
  
     if(gMmwMssMCB.perAdpllSscCfg.isEnable)
     {
         dpllM = CSL_FEXT(ptrMssTopRcmRegs->PLL_PER_MN2DIV,
                          MSS_TOPRCM_PLL_PER_MN2DIV_PLL_PER_MN2DIV_M);
  
         dpllN = CSL_FEXT(ptrMssTopRcmRegs->PLL_PER_M2NDIV,
                          MSS_TOPRCM_PLL_PER_M2NDIV_PLL_PER_M2NDIV_N);
  
         refClk = finp/(dpllN + 1);
  
         ptrMssTopRcmRegs->PLL_PER_FRACCTRL = computeSscFactCtrlVal(refClk, dpllM, &gMmwMssMCB.perAdpllSscCfg);
  
         CSL_FINS(ptrMssTopRcmRegs->PLL_PER_CLKCTRL, MSS_TOPRCM_PLL_PER_CLKCTRL_PLL_PER_CLKCTRL_ENSSC, 1U);
     }
  
     return;
 }
  
 int32_t main (void)
 {
     /* init SOC specific modules */
     System_init();
     Board_init();
  
     gMmwMssMCB.taskHandles.initTask = xTaskCreateStatic( MmwDemo_initTask,
                                   "mmwdemo_init_task",
                                   MMWDEMO_INIT_TASK_STACK_SIZE,
                                   NULL,
                                   MMWDEMO_INIT_TASK_PRI,
                                   gAppMainTskStack,
                                   &gMmwMssMCB.taskHandles.initTaskObj );
     configASSERT(gMmwMssMCB.taskHandles.initTask != NULL);
  
     /* Start the scheduler to start the tasks executing. */
     vTaskStartScheduler();
  
     /* The following line should never be reached because vTaskStartScheduler()
     will only return if there was not enough FreeRTOS heap memory available to
     create the Idle and (if configured) Timer tasks.  Heap management, and
     techniques for trapping heap exhaustion, are described in the book text. */
     DebugP_assertNoLog(0);
 }