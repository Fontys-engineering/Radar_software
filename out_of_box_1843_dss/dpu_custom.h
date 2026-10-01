/**
 *  \file   dpu_custom.h
 *  \brief  Custom Max-Distance DPU Interface Header (Post-AoA Filtering)
 */

#ifndef DPU_CUSTOM_H
#define DPU_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <ti/common/sys_common.h>
#include <ti/datapath/dpif/dpif_pointcloud.h>

#define DPU_CUSTOM_ERROR_CODE_BASE       (-20000)
#define DPU_CUSTOM_EINVAL               (DPU_CUSTOM_ERROR_CODE_BASE - 1)
#define DPU_CUSTOM_ENOMEM               (DPU_CUSTOM_ERROR_CODE_BASE - 2)
#define DPU_CUSTOM_ENOTINITIALIZED      (DPU_CUSTOM_ERROR_CODE_BASE - 3)

typedef void* DPU_Custom_Handle;

typedef struct DPU_Custom_InitParams_t
{
    uint8_t reserved;
} DPU_Custom_InitParams;

/**
 * @brief Post-AoA Filter Configuration Parameters
 */
typedef struct DPU_Custom_Config_t
{
    /*! \brief Pointer to input point cloud array from AoAProc */
    DPIF_PointCloudCartesian *pInPointCloud;

    /*! \brief Pointer to input side info (SNR/noise) array from AoAProc */
    DPIF_PointCloudSideInfo  *pInSideInfo;

    /*! \brief Number of detected points passed from AoAProc */
    uint32_t                 numInputPoints;

    /*! \brief Pointer to output point cloud buffer (allocates for filtered point) */
    DPIF_PointCloudCartesian *pOutPointCloud;

    /*! \brief Pointer to output side info buffer */
    DPIF_PointCloudSideInfo  *pOutSideInfo;

} DPU_Custom_Config;

typedef struct DPU_Custom_OutParams_t
{
    /*! \brief Number of output objects retained (0 or 1) */
    uint32_t numOutputElements;

    /*! \brief DSP cycle count for processing */
    uint32_t processingTimeCycles;
} DPU_Custom_OutParams;

typedef enum DPU_Custom_Cmd_e
{
    DPU_Custom_Cmd_ResetStats = 0
} DPU_Custom_Cmd;

/* Function Prototypes */
DPU_Custom_Handle DPU_Custom_init(DPU_Custom_InitParams *initParams, int32_t *errCode);
int32_t DPU_Custom_config(DPU_Custom_Handle handle, DPU_Custom_Config *dpuCfg);
int32_t DPU_Custom_process(DPU_Custom_Handle handle, DPU_Custom_OutParams *outParams);
int32_t DPU_Custom_control(DPU_Custom_Handle handle, DPU_Custom_Cmd cmd, void *arg, uint32_t argLen);
int32_t DPU_Custom_deinit(DPU_Custom_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* DPU_CUSTOM_H */
