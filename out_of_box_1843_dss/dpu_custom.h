/**
 *  \file   dpu_custom.h
 *  \brief  Custom DPU Interface Header - Retain 3 Representative Objects
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

#define DPU_CUSTOM_MAX_CANDIDATES       (100U)

typedef void* DPU_Custom_Handle;

typedef struct DPU_Custom_InitParams_t
{
    uint8_t reserved;
} DPU_Custom_InitParams;

typedef struct DPU_Custom_Config_t
{
    DPIF_PointCloudCartesian *pInPointCloud;
    DPIF_PointCloudSideInfo  *pInSideInfo;
    uint32_t                 numInputPoints;
    float                    maxLateralDist;
    float                    minDepthDist;
    float                    maxDepthDist;

    DPIF_PointCloudCartesian *pOutPointCloud;
    DPIF_PointCloudSideInfo  *pOutSideInfo;

} DPU_Custom_Config;

typedef struct DPU_Custom_OutParams_t
{
    uint32_t numOutputElements;
    uint32_t processingTimeCycles;
} DPU_Custom_OutParams;

typedef enum DPU_Custom_Cmd_e
{
    DPU_Custom_Cmd_SetMaxLateralDist = 0,
    DPU_Custom_Cmd_SetMinDepthDist,
    DPU_Custom_Cmd_SetMaxDepthDist,
    DPU_Custom_Cmd_ResetStats
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
