/**
 *  \file   dpu_custom.c
 *  \brief  Optimized Custom DPU - Fast Post-AoA Selection (C674x)
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
} DPU_Custom_Obj;

/**
 * @brief O(N) Quickselect to partition array and locate median element
 */
static void quickselect_partition(Candidate *arr, uint32_t n)
{
    uint32_t left;
    uint32_t right;
    uint32_t k;
    uint32_t pivotIdx, i, j;
    float pivotVal;
    Candidate tmp;

    left = 0;
    right = n - 1;
    k = n / 2;

    while (left < right)
    {
        pivotIdx = left + (right - left) / 2;
        pivotVal = arr[pivotIdx].distSq;
        i = left;
        j = right;

        while (i <= j)
        {
            while (arr[i].distSq < pivotVal)
            {
                i++;
            }
            while (arr[j].distSq > pivotVal)
            {
                if (j == 0) break;
                j--;
            }
            if (i <= j)
            {
                tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
                i++;
                if (j > 0)
                {
                    j--;
                }
            }
        }

        if (k <= j)
        {
            right = j;
        }
        else if (k >= i)
        {
            left = i;
        }
        else
        {
            break;
        }
    }
}

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
    float                     minDepth;
    float                     maxDepth;
    uint32_t                  numCandidates;
    uint32_t                  i;
    float                     x, y, z, distSq;
    float                     minDistSq, maxDistSq;
    uint32_t                  closestIdx, middleIdx, furthestIdx;

    /* Stack-allocated candidate buffer: 100 * 8 bytes = 800 bytes (L1D Cache fast RAM) */
    Candidate                 candidates[DPU_CUSTOM_MAX_CANDIDATES];

    /* Temporary variables to prevent in-place buffer overwrite corruption */
    DPIF_PointCloudCartesian  tempPoint[3];
    DPIF_PointCloudSideInfo   tempSide[3];

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
    minDepth       = dpuObj->cfg.minDepthDist;
    maxDepth       = dpuObj->cfg.maxDepthDist;
    numCandidates  = 0;

    if ((numInputPoints == 0) || (pInPoints == NULL) || (pOutPoints == NULL))
    {
        outParams->numOutputElements    = 0;
        outParams->processingTimeCycles = TSCL - startTime;
        return 0;
    }

    minDistSq   = 1e9f;
    maxDistSq   = -1.0f;
    closestIdx  = 0;
    furthestIdx = 0;

    /* Single-pass lateral and depth filtering + Min/Max distance tracking */
    for (i = 0; i < numInputPoints; i++)
    {
        x = pInPoints[i].x;
        y = pInPoints[i].y;

        /* Filter by both Lateral (-maxLat <= x <= maxLat) and Depth (minDepth <= y <= maxDepth) */
        if ((x >= -maxLat) && (x <= maxLat) && (y >= minDepth) && (y <= maxDepth))
        {
            z = pInPoints[i].z;
            distSq = (x * x) + (y * y) + (z * z);

            candidates[numCandidates].originalIdx = i;
            candidates[numCandidates].distSq      = distSq;

            if (distSq < minDistSq)
            {
                minDistSq  = distSq;
                closestIdx = i;
            }

            if (distSq > maxDistSq)
            {
                maxDistSq  = distSq;
                furthestIdx = i;
            }

            numCandidates++;
            if (numCandidates >= DPU_CUSTOM_MAX_CANDIDATES)
            {
                break;
            }
        }
    }

    /* Output Selection (Performed AFTER filtering) */
    if (numCandidates == 0)
    {
        outParams->numOutputElements = 0;
    }
    else if (numCandidates == 1)
    {
        tempPoint[0] = pInPoints[closestIdx];
        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            tempSide[0] = pInSide[closestIdx];
        }

        pOutPoints[0] = tempPoint[0];
        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            pOutSide[0] = tempSide[0];
        }

        outParams->numOutputElements = 1;
    }
    else if (numCandidates == 2)
    {
        tempPoint[0] = pInPoints[closestIdx];
        tempPoint[1] = pInPoints[furthestIdx];

        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            tempSide[0] = pInSide[closestIdx];
            tempSide[1] = pInSide[furthestIdx];
        }

        pOutPoints[0] = tempPoint[0];
        pOutPoints[1] = tempPoint[1];
        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            pOutSide[0] = tempSide[0];
            pOutSide[1] = tempSide[1];
        }

        outParams->numOutputElements = 2;
    }
    else
    {
        /* Quickselect to isolate median element among filtered candidates */
        quickselect_partition(candidates, numCandidates);
        middleIdx = candidates[numCandidates / 2].originalIdx;

        tempPoint[0] = pInPoints[closestIdx];
        tempPoint[1] = pInPoints[middleIdx];
        tempPoint[2] = pInPoints[furthestIdx];

        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            tempSide[0] = pInSide[closestIdx];
            tempSide[1] = pInSide[middleIdx];
            tempSide[2] = pInSide[furthestIdx];
        }

        pOutPoints[0] = tempPoint[0];
        pOutPoints[1] = tempPoint[1];
        pOutPoints[2] = tempPoint[2];

        if ((pInSide != NULL) && (pOutSide != NULL))
        {
            pOutSide[0] = tempSide[0];
            pOutSide[1] = tempSide[1];
            pOutSide[2] = tempSide[2];
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

        case DPU_Custom_Cmd_SetMinDepthDist:
            if ((arg != NULL) && (argLen == sizeof(float)))
            {
                dpuObj->cfg.minDepthDist = *(float *)arg;
            }
            else
            {
                return DPU_CUSTOM_EINVAL;
            }
            break;

        case DPU_Custom_Cmd_SetMaxDepthDist:
            if ((arg != NULL) && (argLen == sizeof(float)))
            {
                dpuObj->cfg.maxDepthDist = *(float *)arg;
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
