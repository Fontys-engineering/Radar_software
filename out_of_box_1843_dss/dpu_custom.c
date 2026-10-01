/**
 *  \file   dpu_custom.c
 *  \brief  Custom DPU - Retain Furthest Object within Lateral Bound Post-AoA Processing
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
    /* C89 Requirement: All variable declarations must precede executable code */
    DPU_Custom_Obj           *dpuObj;
    uint32_t                  startTime;
    uint32_t                  numInputPoints;
    DPIF_PointCloudCartesian *pInPoints;
    DPIF_PointCloudSideInfo  *pInSide;
    DPIF_PointCloudCartesian *pOutPoints;
    DPIF_PointCloudSideInfo  *pOutSide;
    float                     maxDistSq;
    int32_t                   maxIdx;
    uint32_t                  i;
    float                     x, y, z, distSq;
    float                     maxLat;

    dpuObj = (DPU_Custom_Obj *)handle;

    if ((dpuObj == NULL) || (outParams == NULL))
    {
        return DPU_CUSTOM_EINVAL;
    }

    if (!dpuObj->isConfigured)
    {
        return DPU_CUSTOM_ENOTINITIALIZED;
    }

    startTime = TSCL;

    numInputPoints = dpuObj->cfg.numInputPoints;
    pInPoints      = dpuObj->cfg.pInPointCloud;
    pInSide        = dpuObj->cfg.pInSideInfo;

    pOutPoints     = dpuObj->cfg.pOutPointCloud;
    pOutSide       = dpuObj->cfg.pOutSideInfo;

    maxLat         = dpuObj->cfg.maxLateralDist;

    if ((numInputPoints == 0) || (pInPoints == NULL) || (pOutPoints == NULL))
    {
        outParams->numOutputElements    = 0;
        outParams->processingTimeCycles = TSCL - startTime;
        return 0;
    }

    maxDistSq = -1.0f;
    maxIdx    = -1;

    for (i = 0; i < numInputPoints; i++)
    {
        x = pInPoints[i].x;
        y = pInPoints[i].y;
        z = pInPoints[i].z;

        /* Filter: Only evaluate candidates within [-maxLat, +maxLat] meters */
        if ((x >= -maxLat) && (x <= maxLat))
        {
            distSq = (x * x) + (y * y) + (z * z);

            if (distSq > maxDistSq)
            {
                maxDistSq = distSq;
                maxIdx    = (int32_t)i;
            }
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
        case DPU_Custom_Cmd_SetMaxLateralDist:
            if ((arg != NULL) && (argLen == sizeof(float)))
            {
                dpuObj->cfg.maxLateralDist = *(float *)arg;
            }
            else
            {
                return DPU_CUSTOM_EINVAL;
            }
            break;

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
