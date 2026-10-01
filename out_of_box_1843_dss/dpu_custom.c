/**
 *  \file   dpu_custom.c
 *  \brief  Custom DPU - Retain Furthest, Closest, and Middle Objects Post-AoA Processing
 */

#include "dpu_custom.h"
#include <string.h>
#include <ti/drivers/osal/MemoryP.h>
#include <c6x.h>

/**
 * @brief Candidate point helper structure
 */
typedef struct Candidate_t
{
    uint32_t originalIdx;
    float    distSq;
} Candidate;

typedef struct DPU_Custom_Obj_t
{
    DPU_Custom_Config cfg;
    bool              isConfigured;
    uint32_t          frameCount;

    /* Candidate buffer stored in handle heap to avoid stack overflow */
    Candidate         candidates[DPU_CUSTOM_MAX_CANDIDATES];
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
    /* C89 Requirement: Declarations at the top of the function block */
    DPU_Custom_Obj           *dpuObj;
    uint32_t                  startTime;
    uint32_t                  numInputPoints;
    DPIF_PointCloudCartesian *pInPoints;
    DPIF_PointCloudSideInfo  *pInSide;
    DPIF_PointCloudCartesian *pOutPoints;
    DPIF_PointCloudSideInfo  *pOutSide;
    float                     maxLat;
    uint32_t                  numCandidates;
    uint32_t                  i;
    int32_t                   j;
    Candidate                 key;
    float                     x, y, z, distSq;
    Candidate                *candidates;
    uint32_t                  closestIdx, middleIdx, furthestIdx;

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
    candidates     = dpuObj->candidates;
    numCandidates  = 0;

    if ((numInputPoints == 0) || (pInPoints == NULL) || (pOutPoints == NULL))
    {
        outParams->numOutputElements    = 0;
        outParams->processingTimeCycles = TSCL - startTime;
        return 0;
    }

    /* Step 1: Collect all valid candidates within lateral boundaries */
    for (i = 0; i < numInputPoints; i++)
    {
        x = pInPoints[i].x;

        if ((x >= -maxLat) && (x <= maxLat))
        {
            y = pInPoints[i].y;
            z = pInPoints[i].z;
            distSq = (x * x) + (y * y) + (z * z);

            candidates[numCandidates].originalIdx = i;
            candidates[numCandidates].distSq      = distSq;
            numCandidates++;

            if (numCandidates >= DPU_CUSTOM_MAX_CANDIDATES)
            {
                break;
            }
        }
    }

    /* Step 2: Select 0, 1, 2, or 3 objects based on candidates found */
    if (numCandidates == 0)
    {
        outParams->numOutputElements = 0;
    }
    else if (numCandidates == 1)
    {
        /* Only 1 point found */
        closestIdx = candidates[0].originalIdx;

        pOutPoints[0] = pInPoints[closestIdx];
        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            pOutSide[0] = pInSide[closestIdx];
        }

        outParams->numOutputElements = 1;
    }
    else if (numCandidates == 2)
    {
        /* 2 points found: assign closest and furthest */
        if (candidates[0].distSq <= candidates[1].distSq)
        {
            closestIdx  = candidates[0].originalIdx;
            furthestIdx = candidates[1].originalIdx;
        }
        else
        {
            closestIdx  = candidates[1].originalIdx;
            furthestIdx = candidates[0].originalIdx;
        }

        pOutPoints[0] = pInPoints[closestIdx];
        pOutPoints[1] = pInPoints[furthestIdx];

        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            pOutSide[0] = pInSide[closestIdx];
            pOutSide[1] = pInSide[furthestIdx];
        }

        outParams->numOutputElements = 2;
    }
    else
    {
        /* 3 or more points found: Sort candidates ascending by distance squared */
        for (i = 1; i < numCandidates; i++)
        {
            key = candidates[i];
            j = (int32_t)i - 1;

            while ((j >= 0) && (candidates[j].distSq > key.distSq))
            {
                candidates[j + 1] = candidates[j];
                j--;
            }
            candidates[j + 1] = key;
        }

        /* Extract Closest, Middle (Median), and Furthest indices */
        closestIdx  = candidates[0].originalIdx;
        middleIdx   = candidates[numCandidates / 2].originalIdx;
        furthestIdx = candidates[numCandidates - 1].originalIdx;

        /* Assign output array entries */
        pOutPoints[0] = pInPoints[closestIdx];   /* Index 0: Closest  */
        pOutPoints[1] = pInPoints[middleIdx];    /* Index 1: Middle   */
        pOutPoints[2] = pInPoints[furthestIdx];  /* Index 2: Furthest */

        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            pOutSide[0] = pInSide[closestIdx];
            pOutSide[1] = pInSide[middleIdx];
            pOutSide[2] = pInSide[furthestIdx];
        }

        outParams->numOutputElements = 3;
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
