/**
 *  \file   dpu_custom.c
 *  \brief  Custom DPU - Retain Furthest Object Post-AoA Processing
 */

#include "dpu_custom.h"
#include <string.h>
#include <ti/drivers/osal/MemoryP.h>
#include <c6x.h>

typedef struct DPU_Custom_Obj_t
{
    DPU_Custom_Config cfg;
    bool              isConfigured;
    uint32_t          frameCount;
} DPU_Custom_Obj;

DPU_Custom_Handle DPU_Custom_init(
    DPU_Custom_InitParams *initParams,
    int32_t               *errCode
)
{
    DPU_Custom_Obj *dpuObj = NULL;
    *errCode = 0;

    dpuObj = (DPU_Custom_Obj *)MemoryP_ctrlAlloc(sizeof(DPU_Custom_Obj), 0);

    if (dpuObj == NULL)
    {
        *errCode = DPU_CUSTOM_ENOMEM;
        return NULL;
    }

    memset(dpuObj, 0, sizeof(DPU_Custom_Obj));
    dpuObj->isConfigured = false;

    return ((DPU_Custom_Handle)dpuObj);
}

int32_t DPU_Custom_config(
    DPU_Custom_Handle  handle,
    DPU_Custom_Config *dpuCfg
)
{
    DPU_Custom_Obj *dpuObj = (DPU_Custom_Obj *)handle;

    if ((dpuObj == NULL) || (dpuCfg == NULL))
    {
        return DPU_CUSTOM_EINVAL;
    }

    memcpy(&dpuObj->cfg, dpuCfg, sizeof(DPU_Custom_Config));
    dpuObj->isConfigured = true;

    return 0;
}

int32_t DPU_Custom_process(
    DPU_Custom_Handle     handle,
    DPU_Custom_OutParams *outParams
)
{
    DPU_Custom_Obj *dpuObj = (DPU_Custom_Obj *)handle;
    uint32_t startTime;

    if ((dpuObj == NULL) || (outParams == NULL))
    {
        return DPU_CUSTOM_EINVAL;
    }

    if (!dpuObj->isConfigured)
    {
        return DPU_CUSTOM_ENOTINITIALIZED;
    }

    startTime = TSCL;

    uint32_t numInputPoints             = dpuObj->cfg.numInputPoints;
    DPIF_PointCloudCartesian *pInPoints = dpuObj->cfg.pInPointCloud;
    DPIF_PointCloudSideInfo  *pInSide   = dpuObj->cfg.pInSideInfo;

    DPIF_PointCloudCartesian *pOutPoints = dpuObj->cfg.pOutPointCloud;
    DPIF_PointCloudSideInfo  *pOutSide   = dpuObj->cfg.pOutSideInfo;

    if ((numInputPoints == 0) || (pInPoints == NULL) || (pOutPoints == NULL))
    {
        outParams->numOutputElements    = 0;
        outParams->processingTimeCycles = TSCL - startTime;
        return 0;
    }

    float maxDistSq = -1.0f;
    int32_t maxIdx  = -1;

    for (uint32_t i = 0; i < numInputPoints; i++)
    {
        float x = pInPoints[i].x;
        float y = pInPoints[i].y;
        float z = pInPoints[i].z;

        float distSq = (x * x) + (y * y) + (z * z);

        if (distSq > maxDistSq)
        {
            maxDistSq = distSq;
            maxIdx    = (int32_t)i;
        }
    }

    if (maxIdx >= 0)
    {
        pOutPoints[0] = pInPoints[maxIdx];

        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            pOutSide[0] = pInSide[maxIdx];
        }

        outParams->numOutputElements = 1;
    }
    else
    {
        outParams->numOutputElements = 0;
    }

    dpuObj->frameCount++;
    outParams->processingTimeCycles = TSCL - startTime;

    return 0;
}

int32_t DPU_Custom_control(
    DPU_Custom_Handle handle,
    DPU_Custom_Cmd    cmd,
    void             *arg,
    uint32_t          argLen
)
{
    DPU_Custom_Obj *dpuObj = (DPU_Custom_Obj *)handle;

    if (dpuObj == NULL)
    {
        return DPU_CUSTOM_EINVAL;
    }

    switch (cmd)
    {
        case DPU_Custom_Cmd_ResetStats:
            dpuObj->frameCount = 0;
            break;

        default:
            return DPU_CUSTOM_EINVAL;
    }

    return 0;
}

int32_t DPU_Custom_deinit(DPU_Custom_Handle handle)
{
    DPU_Custom_Obj *dpuObj = (DPU_Custom_Obj *)handle;

    if (dpuObj == NULL)
    {
        return DPU_CUSTOM_EINVAL;
    }

    MemoryP_ctrlFree(dpuObj, sizeof(DPU_Custom_Obj));
    return 0;
}