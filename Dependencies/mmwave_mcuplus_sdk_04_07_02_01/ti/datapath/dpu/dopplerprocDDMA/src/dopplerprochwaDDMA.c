/**
 *   @file  dopplerprochwaDDMA.c
 *
 *   @brief
 *      Implements Data path Doppler processing Unit using HWA.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 - 2026 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#ifdef SUBSYS_M4
#include <arm_acle.h>
#endif

#ifdef SUBSYS_DSS
#include <ti/mathlib/mathlib.h>
#endif

/* MCU+SDK Include files */
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/HeapP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/CycleCounterP.h>
#include <kernel/dpl/DebugP.h>

/* HWA_SOC Include files */
#include <drivers/soc.h>

/* Utils */
#include <ti/utils/mathutils/mathutils.h>

/* Data Path Include files */
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpu/dopplerprocDDMA/dopplerprochwaDDMA.h>
#include <ti/datapath/dpu/dopplerprocDDMA/include/dopplerprochwaDDMAinternal.h>

#include <ti/control/mmwavelink/mmwavelink.h>

/******************************
* DECOMPRESSION STAGE *********
*******************************/
#define DECOMP_PING_HWA_PARAMSET_RELATIVE_IDX       0U
#define DECOMP_PONG_HWA_PARAMSET_RELATIVE_IDX       1U

#define DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PING_IN  0U
#define DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PING_OUT 1U
#define DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PONG_IN  4U
#define DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PONG_OUT 5U

#define DPU_DOPPLERHWADDMA_ADDR_DECOMP_PING_IN     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PING_IN])
#define DPU_DOPPLERHWADDMA_ADDR_DECOMP_PING_OUT    HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PING_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_DECOMP_PONG_IN     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PONG_IN])
#define DPU_DOPPLERHWADDMA_ADDR_DECOMP_PONG_OUT    HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PONG_OUT])

/******************************
* DOPPLER STAGE ***************
*******************************/
#define DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PING_IN      0U
#define DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PING_OUT     DPU_DOPPLERHWADDMA_MEM_BANK_LOGABS_PING_IN
#define DPU_DOPPLERHWADDMA_MEM_BANK_LOGABS_PING_IN          4U
#define DPU_DOPPLERHWADDMA_MEM_BANK_SUMRX_PING_OUT          DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PING_IN
#define DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PING_IN      1U
#define DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PING_OUT     0U
#define DPU_DOPPLERHWADDMA_MEM_BANK_MAXSUBBAND_PING_OUT     1U /* This will be stored at an offset of sumRXOut in M1 */
#define DPU_DOPPLERHWADDMA_MEM_BANK_SUMTX_PING_IN           DPU_DOPPLERHWADDMA_MEM_BANK_SUMRX_PING_OUT
#define DPU_DOPPLERHWADDMA_MEM_BANK_SUMTX_PING_OUT          0U /* This will actually be stored at an offset in M0 */

#define DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PONG_IN      2U
#define DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PONG_OUT     DPU_DOPPLERHWADDMA_MEM_BANK_LOGABS_PONG_IN
#define DPU_DOPPLERHWADDMA_MEM_BANK_LOGABS_PONG_IN          6U
#define DPU_DOPPLERHWADDMA_MEM_BANK_SUMRX_PONG_OUT          DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PONG_IN
#define DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PONG_IN      3U
#define DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PONG_OUT     2U
#define DPU_DOPPLERHWADDMA_MEM_BANK_MAXSUBBAND_PONG_OUT     3U /* This will be stored at an offset of sumRXOut in M3 */
#define DPU_DOPPLERHWADDMA_MEM_BANK_SUMTX_PONG_IN           DPU_DOPPLERHWADDMA_MEM_BANK_SUMRX_PONG_OUT
#define DPU_DOPPLERHWADDMA_MEM_BANK_SUMTX_PONG_OUT          2U /* This will actually be stored at an offset in M2 */

#define DPU_DOPPLERHWADDMA_ADDR_DOPPLERFFT_PING_IN      HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PING_IN])
#define DPU_DOPPLERHWADDMA_ADDR_DOPPLERFFT_PING_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PING_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_LOGABS_PING_IN          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_LOGABS_PING_IN])
#define DPU_DOPPLERHWADDMA_ADDR_SUMRX_PING_OUT          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_SUMRX_PING_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PING_IN      HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PING_IN])
#define DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PING_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PING_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_MAXSUBBAND_PING_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_MAXSUBBAND_PING_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_SUMTX_PING_IN           HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_SUMTX_PING_IN])
#define DPU_DOPPLERHWADDMA_ADDR_SUMTX_PING_OUT          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_SUMTX_PING_OUT])

#define DPU_DOPPLERHWADDMA_ADDR_DOPPLERFFT_PONG_IN      HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PONG_IN])
#define DPU_DOPPLERHWADDMA_ADDR_DOPPLERFFT_PONG_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PONG_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_LOGABS_PONG_IN          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_LOGABS_PONG_IN])
#define DPU_DOPPLERHWADDMA_ADDR_SUMRX_PONG_OUT          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_SUMRX_PONG_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PONG_IN      HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PONG_IN])
#define DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PONG_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PONG_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_MAXSUBBAND_PONG_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_MAXSUBBAND_PONG_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_SUMTX_PONG_IN           HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_SUMTX_PONG_IN])
#define DPU_DOPPLERHWADDMA_ADDR_SUMTX_PONG_OUT          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_SUMTX_PONG_OUT])

#define DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_ISREAL           (0U)
#define DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE   (sizeof(cmplx16ImRe_t))
#define DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_ISSIGNED         (1U)
#define DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_ISREAL          (0U)
#define DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE  (sizeof(cmplx32ImRe_t))
#define DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_ISSIGNED        (1U)

#define DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_ISREAL            (DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_ISREAL)
#define DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_BYTESPERSAMPLE    (DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE)
#define DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_ISSIGNED          (DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_ISSIGNED)
#define DPU_DOPPLERHWADDMA_LOGABSIO_OUTPUT_ISREAL           (1U)
#define DPU_DOPPLERHWADDMA_LOGABSIO_OUTPUT_BYTESPERSAMPLE   (sizeof(uint16_t))
#define DPU_DOPPLERHWADDMA_LOGABSIO_OUTPUT_ISSIGNED         (0U)

#define DPU_DOPPLERHWADDMA_SUMRXIO_INPUT_ISREAL             (DPU_DOPPLERHWADDMA_LOGABSIO_OUTPUT_ISREAL)
#define DPU_DOPPLERHWADDMA_SUMRXIO_INPUT_BYTESPERSAMPLE     (DPU_DOPPLERHWADDMA_LOGABSIO_OUTPUT_BYTESPERSAMPLE)
#define DPU_DOPPLERHWADDMA_SUMRXIO_INPUT_ISSIGNED           (DPU_DOPPLERHWADDMA_LOGABSIO_OUTPUT_ISSIGNED)
#define DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_ISREAL            (1U)
#define DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_BYTESPERSAMPLE    (sizeof(cmplx32ImRe_t)) // Stats Block output is always 32-bit complex
#define DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_ISSIGNED          (0U)

#define DPU_DOPPLERHWADDMA_DDMAMETRICIO_INPUT_ISREAL            (DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_ISREAL)
#define DPU_DOPPLERHWADDMA_DDMAMETRICIO_INPUT_BYTESPERSAMPLE    (DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_BYTESPERSAMPLE)
#define DPU_DOPPLERHWADDMA_DDMAMETRICIO_INPUT_ISSIGNED          (DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_ISSIGNED)
#define DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_ISREAL           (1U)
#define DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE   (sizeof(uint32_t))
#define DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_ISSIGNED         (0U)

#define DPU_DOPPLERHWADDMA_SUMTXIO_INPUT_ISREAL             (DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_ISREAL)
#define DPU_DOPPLERHWADDMA_SUMTXIO_INPUT_BYTESPERSAMPLE     (DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_BYTESPERSAMPLE)
#define DPU_DOPPLERHWADDMA_SUMTXIO_INPUT_ISSIGNED           (DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_ISSIGNED)
#define DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_ISREAL            (1U)
#define DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_BYTESPERSAMPLE    (sizeof(uint16_t))
#define DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_ISSIGNED          (0U)

/******************************
* AZIM-CFAR STAGE *************
*******************************/
#define DPU_DOPPLERHWADDMA_MEM_BANK_DEMODULATE_PING_IN        4U
#define DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PING_IN           0U
#define DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PING_OUT          DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PING_IN
#define DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PING_IN              4U
#define DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PING_OUT             5U
#define DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PING_IN          DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PING_IN
#define DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PING_OUT         1U

#define DPU_DOPPLERHWADDMA_MEM_BANK_DEMODULATE_PONG_IN        6U
#define DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PONG_IN           2U
#define DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PONG_OUT          DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PONG_IN
#define DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PONG_IN              6U
#define DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PONG_OUT             7U
#define DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PONG_IN          DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PONG_IN
#define DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PONG_OUT         3U

#define DPU_DOPPLERHWADDMA_ADDR_DEMODULATE_PING_IN        HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DEMODULATE_PING_IN])
#define DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PING_IN           HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PING_IN])
#define DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PING_OUT          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PING_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_CFAR_PING_IN              HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PING_IN])
#define DPU_DOPPLERHWADDMA_ADDR_CFAR_PING_OUT             HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PING_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_LOCALMAX_PING_IN          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PING_IN])
#define DPU_DOPPLERHWADDMA_ADDR_LOCALMAX_PING_OUT         HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PING_OUT])

#define DPU_DOPPLERHWADDMA_ADDR_DEMODULATE_PONG_IN        HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DEMODULATE_PONG_IN])
#define DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PONG_IN           HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PONG_IN])
#define DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PONG_OUT          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PONG_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_CFAR_PONG_IN              HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PONG_IN])
#define DPU_DOPPLERHWADDMA_ADDR_CFAR_PONG_OUT             HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PONG_OUT])
#define DPU_DOPPLERHWADDMA_ADDR_LOCALMAX_PONG_IN          HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PONG_IN])
#define DPU_DOPPLERHWADDMA_ADDR_LOCALMAX_PONG_OUT         HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PONG_OUT])

#define DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISREAL           (0U)
#define DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE   (DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE)
#define DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISSIGNED         (DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_ISSIGNED)
#define DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_ISREAL          (1U)
#define DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE  (sizeof(uint16_t))
#define DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_ISSIGNED        (0U)

#define DPU_DOPPLERHWADDMA_CFARIO_INPUT_ISREAL              (1U)
#define DPU_DOPPLERHWADDMA_CFARIO_INPUT_BYTESPERSAMPLE      (DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE)
#define DPU_DOPPLERHWADDMA_CFARIO_INPUT_ISSIGNED            (DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_ISSIGNED)
#define DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_ISREAL             (0U)
#define DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_BYTESPERSAMPLE     (sizeof(cmplx32ImRe_t))
#define DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_ISSIGNED           (0U)

#define DPU_DOPPLERHWADDMA_LOCALMAXIO_INPUT_ISREAL          (1U)
#define DPU_DOPPLERHWADDMA_LOCALMAXIO_INPUT_BYTESPERSAMPLE  (DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE)
#define DPU_DOPPLERHWADDMA_LOCALMAXIO_INPUT_ISSIGNED        (DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_ISSIGNED)
#define DPU_DOPPLERHWADDMA_LOCALMAXIO_OUTPUT_ISREAL         (1U)
#define DPU_DOPPLERHWADDMA_LOCALMAXIO_OUTPUT_BYTESPERSAMPLE (sizeof(uint32_t))
#define DPU_DOPPLERHWADDMA_LOCALMAXIO_OUTPUT_ISSIGNED       (0U)

#define PING 0
#define PONG 1

#define EXTRACT_BIT(n, k) ((n & ((uint32_t)1U << k )) >> k)

DPU_DopplerProcHWA_Obj dopplerProcObjPool __attribute__((aligned(HeapP_BYTE_ALIGNMENT)));

// #define DOPPLERPROCHWADDMA_DPU_TIMING
#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
#define DOPPLERPROCHWADDMA_DPU_TIMING_NUM_STAMPS_TO_STORE   (12U)
volatile uint32_t stampGlobal[DOPPLERPROCHWADDMA_DPU_TIMING_NUM_STAMPS_TO_STORE] = {0U};
void insertStamp(int32_t operation)
{
    stampGlobal[operation] = CycleCounterP_getCount32();
}
#endif

#define CONST_LOG2_10 (3.3219)

#ifdef SUBSYS_M4
#define HWA_HIST_THRESH_RAM_U_BASE      CSL_CM4_DSS_HWA_HIST_THRESH_RAM_U_BASE
#define HWA_HIST_RAM_U_BASE             CSL_CM4_DSS_HWA_HIST_RAM_U_BASE
#define HWA_2DSTAT_SMPL_VAL_RAM         CSL_CM4_DSS_HWA_2DSTAT_SMPL_VAL_RAM_U_BASE
#elif defined(SUBSYS_DSS)
#define HWA_HIST_THRESH_RAM_U_BASE      CSL_DSS_HWA_HIST_THRESH_RAM_U_BASE
#define HWA_HIST_RAM_U_BASE             CSL_DSS_HWA_HIST_RAM_U_BASE
#define HWA_2DSTAT_SMPL_VAL_RAM         CSL_DSS_HWA_2DSTAT_SMPL_VAL_RAM_U_BASE
#endif


/*===========================================================
 *                    Internal Functions
 *===========================================================*/
static int32_t DPU_DopplerProcHWA_extractObjectList
(
    DPU_DopplerProcHWA_Obj      * restrict obj,
    DPU_DopplerProcHWA_Config    * restrict cfg,
    uint32_t blockIdx,
    uint32_t mergedOBIdx,
    uint32_t rangeBinIdx
);

static int32_t DPU_DopplerProcHWA_configEdmaDopplerIn
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
);

static int32_t DPU_DopplerProcHWA_configEdmaDecompressionIn
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
);

#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
/**
 *  @b Description
 *  @n
 *      Configures the range-dependent CFAR threshold scaling Look-Up Table (LUT).
 *
 *      This function populates the cfarThreshScaleLUT with pre-computed threshold
 *      adjustments for each range bin when variable threshold mode is enabled.
 *      The algorithm implements a two-region profile to optimize detection:
 *
 *      Near-field (first 25% of range bins):
 *          - Constant elevated threshold: +7dB above baseline
 *          - Reduces false alarms from near-field clutter and high-SNR targets
 *
 *      Mid-to-far field (remaining 75% of range bins):
 *          - Linearly tapered threshold: +7dB down to 0dB (baseline)
 *          - Smooth transition maintains detection continuity
 *          - Maximizes far-field sensitivity
 *
 *      The threshold values are stored in log2 5.11 fixed-point format with
 *      a scale factor of 2048, compatible with HWA CFAR hardware expectations.
 *
 *      Formula: For dB offset D, the threshold scale value is:
 *               scale = (D / 20) * log2(10) * 2048
 *
 *  @param[in,out]  cfg     Pointer to DPU configuration structure.
 *                          The cfarThreshScaleLUT array must be pre-allocated
 *                          with numRangeBins elements.
 *
 *  @retval None
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 */
static void DPU_DopplerProcHWA_configCfarThreshScaleLUT
(
    DPU_DopplerProcHWA_Config   *cfg
)
{
    uint32_t rangeIdx;
    const float log2_10 = 3.3219F;
    /* The first few range bins will have constant threshold */
    float numConstThreshBinsRatio = 0.25F;
    /* The constant threshold will be higher than the last bins threshold by: */
    float nearBinsDiffdB = 7.0F;

    if(cfg->staticCfg.cfarCfg.variableThresholdMode != 0U)
    {
        for(rangeIdx = 0; rangeIdx < cfg->staticCfg.numRangeBins; rangeIdx++)
        {
            if((float)rangeIdx < ((float)cfg->staticCfg.numRangeBins * numConstThreshBinsRatio))
            {
                cfg->hwRes.cfarThreshScaleLUT[rangeIdx] = ((float)nearBinsDiffdB / 20.0F) * log2_10 * 2048.0F;
            }
            else
            {
                float linearThreshRatio = (1.0F - (((float)rangeIdx + 1.0F) / (float)cfg->staticCfg.numRangeBins)) / (1.0F - numConstThreshBinsRatio);
                cfg->hwRes.cfarThreshScaleLUT[rangeIdx] = linearThreshRatio * (nearBinsDiffdB / 20.0F) * log2_10 * 2048.0F;
            }
        }
    }
}

/**
 *  @b Description
 *  @n
 *      Gets the range-dependent CFAR threshold for a specific range bin.
 *
 *      This function computes the final CFAR threshold by adding the range-dependent
 *      offset from the LUT to the baseline thresholdScale. This implements adaptive
 *      thresholding that is higher for near-field bins and tapers to baseline for
 *      far-field bins.
 *
 *      The function internally performs: baselineThreshold + rangeDependentOffset
 *
 *      Usage: threshold = DPU_DopplerProcHWA_getRangeDependentCfarThreshold(cfg, rangeIdx)
 *
 *  @param[in]  cfg         Pointer to DPU configuration structure containing both
 *                          the baseline thresholdScale and cfarThreshScaleLUT array.
 *  @param[in]  rangeIdx    Range bin index (0 to numRangeBins-1).
 *
 *  @retval     Final CFAR threshold value for the specified range bin (baseline + offset).
 *              Value is in log2 5.11 fixed-point format (scale factor: 2048).
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 */
static uint32_t DPU_DopplerProcHWA_getRangeDependentCfarThreshold(
    DPU_DopplerProcHWA_Config   *cfg,
    uint16_t rangeIdx
)
{
    return cfg->staticCfg.cfarCfg.thresholdScale + cfg->hwRes.cfarThreshScaleLUT[rangeIdx];
}
#endif


#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
/**
 *  @b Description
 *  @n
 *      EDMA completion call back function.
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 */
static void DPU_DopplerProcHWA_edmaDoneIsrCallback(Edma_IntrHandle intrHandle, void *args)
{
    if (args != NULL)
    {
        SemaphoreP_post((SemaphoreP_Object*)args);
    }
}
/**
 *  @b Description
 *  @n
 *      HWA completion call back function.
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 */
static void DPU_DopplerProcHWA_hwaDoneIsrCallback(uint32_t intrIdx, uint32_t paramSet, void * arg)
{
    if (arg != NULL)
    {
        SemaphoreP_post((SemaphoreP_Object*)arg);
    }

}
#else
/**
 *  @b Description
 *  @n
 *      HWA completion polling function.
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 */
static inline void DPU_DopplerProcHWA_edmaDonePoll(uint32_t baseAddr, uint32_t regionId, uint32_t tccNum)
{
    while(EDMA_readIntrStatusRegion(baseAddr, regionId, tccNum) != 1);
    EDMA_clrIntrRegion(baseAddr, regionId, tccNum);
}

/**
 *  @b Description
 *  @n
 *      HWA completion polling function.
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 */
static inline void DPU_DopplerProcHWA_hwaDonePoll(DSSHWACCRegs * restrict ctrlBaseAddr, uint8_t paramSet)
{
    int8_t numStatusBits = 32;
    if(paramSet >= numStatusBits)
    {
        while(CSL_FEXTR(ctrlBaseAddr->PARAM_DONE_SET_STATUS[1], paramSet - numStatusBits, paramSet - numStatusBits)!=1U);
        CSL_FINSR(ctrlBaseAddr->PARAM_DONE_CLR[1], paramSet - numStatusBits, paramSet - numStatusBits, 1U);
    }
    else
    {
        while(CSL_FEXTR(ctrlBaseAddr->PARAM_DONE_SET_STATUS[0], paramSet, paramSet)!=1U);
        CSL_FINSR(ctrlBaseAddr->PARAM_DONE_CLR[0], paramSet, paramSet, 1U);
    }
}
#endif

/**
 *  @b Description
 *  @n
 *      Programs Shuffle LUT based on max subband idx for DDMA Demodulation on HWA.
 *
 *  @param[in] shuffleRAMBuf - Shuffle RAM Buffer
 *  @param[in] obj          - DPU obj
 *  @param[in] cfg          - DPU configuration
 *  @param[in] rangeBinIdx  - Ping or Pong Range Bin
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval error code.
 */
static inline void DPU_DopplerProcHWA_ConfigShuffleLUT(uint16_t shuffleRAMBuf[], DPU_DopplerProcHWA_Obj *obj,
                                                       DPU_DopplerProcHWA_Config *cfg,
                                                       uint32_t rangeBinIdx)
{
    uint8_t * restrict dopMaxSubBandMat = (uint8_t *)cfg->hwRes.dopMaxSubBandScratchBuf[rangeBinIdx];
    uint16_t numDopplerSubBins = obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal;
    uint16_t * restrict shuffleRAMPtr = shuffleRAMBuf + rangeBinIdx*numDopplerSubBins;
    uint16_t mulOffset = numDopplerSubBins;
    uint16_t dopSubIdx = 0;

    /* Populate Shuffle LUT RAM contents in array.
     * Shuffle LUT is a 256 element vector (RAM) storing 12-bit numbers. Care should be taken that value should not exceed 2^!2-1. */
    for(dopSubIdx = 0; dopSubIdx < numDopplerSubBins; dopSubIdx++)
    {
        shuffleRAMPtr[dopSubIdx] = (dopMaxSubBandMat[dopSubIdx] * mulOffset) + dopSubIdx;
    }
}

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
/**
 *  @b Description
 *  @n
 *      Given an SNR, it checks if it is larger than the smallest SNR value in
 *      the SNRList array. If that is the case, it displaces the smallest value.
 *      The position of the value it displaced is returned.
 *      Using this function, we can find the largest MAX_NUM_OBJ_PER_RANGE_BIN
 *      SNRs in a very large list of SNRs.
 *
 *      SNR is expected to be positive.
 *
 *  @param[in] SNR          - SNR to check
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval 'position of the displaced SNR' or maxNumobjPerRG if the input is smaller than all the SNRs.
 */
static inline uint32_t DPU_DopplerProcHWA_updateSNRList(int32_t SNR, int32_t SNRList[], uint32_t maxNumobjPerRG)
{
#ifdef SUBSYS_DSS
    (void)_nassert((int) SNRList % 8 == 0);
#endif
    int32_t min2SNR[2];
    uint32_t min2Loc[2];
	uint32_t objIdx, minLoc, idx;
    int32_t minSNR;
	min2SNR[0] = SNRList[0];
	min2SNR[1] = SNRList[1];
	min2Loc[0] = 0;
	min2Loc[1] = 1;

	for (objIdx = 2U; objIdx < maxNumobjPerRG; objIdx++)
	{
        idx = objIdx % 2U;
        if(min2SNR[idx] > SNRList[objIdx])
        {
            min2SNR[idx] = SNRList[objIdx];
            min2Loc[idx] = objIdx;
        }
	}

	if (min2SNR[0] > min2SNR[1])
	{
		minSNR = min2SNR[1];
		minLoc = min2Loc[1];
	}
	else
	{
		minSNR = min2SNR[0];
		minLoc = min2Loc[0];
	}

	if (SNR > minSNR)
	{
		SNRList[minLoc] = SNR;
		return minLoc;
	}
	else
	{
		return maxNumobjPerRG;
	}
}
#endif


#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
/**
 *  @b Description
 *  @n
 *      Performs Object List creation
 *
 *  @param[in] obj          - DPU obj
 *  @param[in] cfg          - DPU configuration
 *  @param[in] blockIdx     - Decompressed block index
 *  @param[in] mergedOBIdx  - OuterBlock Index (Goes from 0 to Merged Number of Outer Blocks)
 *  @param[in] rangeBinIdx  - Range bin index (starts from 0 for any decompressed block)
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval error code.
 */
static int32_t DPU_DopplerProcHWA_extractObjectList(DPU_DopplerProcHWA_Obj      * restrict obj,
                                                    DPU_DopplerProcHWA_Config   * restrict cfg,
                                                    uint32_t blockIdx,
                                                    uint32_t mergedOBIdx,
                                                    uint32_t rangeBinIdx)
{
    uint8_t * restrict azimFFTMat;
    uint32_t * restrict localMaxMat;
    uint8_t * restrict dopSubMaxMat;
    uint8_t * restrict dopFFTMat;
    uint32_t baseAddr = EDMA_getBaseAddr(obj->edmaHandle);
    uint32_t edmaSrcAddr, edmaDstAddr, edmaTrigReg, edmaIntrStatusReg, edmaClrIntrStatusReg, channelMask;

    uint32_t detObj=0, localMaxVal;
    uint32_t DopIdxCurr, AzimIdx, AzimIdxCurr, CFARNoiseCurr, numRowsPerAzim;
    uint32_t rowIdx, bit;
    uint32_t azimPeakSamplem1, azimPeakSample, p1Idx, azimPeakSamplep1, dopFFTMatStartIdx;
    DetObjParams * restrict currObjParams;
    int32_t m1Idx, retVal = 0;
	uint32_t numDopplerBinsPerSubBand = (obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal);
	uint32_t numBytesPerRDBin, subBandIdx;
	uint32_t dopFFTBytesPerSample = DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE;
    uint32_t rangeIdx = (((blockIdx * obj->decompCfg.mergedNumOuterBlocks) + mergedOBIdx) * obj->decompCfg.rangeBinsPerBlock) + rangeBinIdx;

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
    int32_t SNR;
    uint32_t posIdx;
	uint32_t numObjPerRangeGate = 0;
    int32_t SNRList[MAX_NUM_OBJ_PER_RANGE_BIN] __attribute__((aligned(8))) = {0};
    uint32_t maxNumObjsCurRangeGate;
#endif

    /* Do not bother with objects in the first range bin. */
    if(rangeIdx == 0U)
    {
        return retVal;
    }

	if (obj->numObjOut >= cfg->staticCfg.maxNumObj)
	{
		return retVal;
	}

	numBytesPerRDBin = dopFFTBytesPerSample * cfg->staticCfg.numVirtualAntennas;
    edmaSrcAddr = baseAddr + EDMA_TPCC_OPT(cfg->hwRes.edmaCfg.edmaDetObjAntSamples.channel) + 0x4U;
    edmaDstAddr = edmaSrcAddr + 0x8U;
    edmaTrigReg = baseAddr + EDMA_TPCC_ESR_RN(0);
    edmaIntrStatusReg =  baseAddr + EDMA_TPCC_IPR_RN(0);
    edmaClrIntrStatusReg = baseAddr + EDMA_TPCC_ICR_RN(0);
    channelMask = (uint32_t)1U << cfg->hwRes.edmaCfg.edmaDetObjAntSamples.channel;

    /* Assign local matrix pointers based on ping/pong */
    azimFFTMat = (uint8_t *) obj->hwaMemBankAddr[(rangeBinIdx % 2U == 1U) ? DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PONG_OUT : DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PING_OUT];
    localMaxMat = (uint32_t *) obj->hwaMemBankAddr[(rangeBinIdx % 2U == 1U) ? DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PONG_OUT : DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PING_OUT];
    dopSubMaxMat = (uint8_t *) cfg->hwRes.dopMaxSubBandScratchBuf[rangeBinIdx % 2U];
    numRowsPerAzim = (obj->cfarAzimFFTCfg.numAzimFFTBins % 32U == 0U) ?
                        (obj->cfarAzimFFTCfg.numAzimFFTBins / 32U) : (obj->cfarAzimFFTCfg.numAzimFFTBins / 32U) + 1U;
    dopFFTMat = (uint8_t *) obj->hwaMemBankAddr[(rangeBinIdx % 2U == 1U) ? DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PONG_IN : DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PING_IN];

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
    maxNumObjsCurRangeGate = (((cfg->hwRes.maxObjListPerRGateSize) * (rangeIdx + 1U)) - (obj->numObjOut * sizeof(DetObjParams))) / sizeof(DetObjParams);
    /* Following is required to ensure local array SNRList in DPU_DopplerProcHWA_extractObjectList does not overflow.
     * This also allows user to set a lower limit for object detection per range gate, if desired. */
    if (maxNumObjsCurRangeGate > MAX_NUM_OBJ_PER_RANGE_BIN)
    {
        maxNumObjsCurRangeGate = MAX_NUM_OBJ_PER_RANGE_BIN;
    }
#endif
    /* This field is not populated when histogram is enabled. Setting it zero.*/
    CFARNoiseCurr = 0;
    /* Loop through CFAR peaks and check whether an object is present for a particular peak. If it is present,
       store its parameters in the output list */
    for(rowIdx = 0; rowIdx < numDopplerBinsPerSubBand * numRowsPerAzim; rowIdx++)
    {
        if(localMaxMat[rowIdx])
        {
            localMaxVal = localMaxMat[rowIdx];
            DopIdxCurr = floor(rowIdx / numRowsPerAzim);
            AzimIdx = 32U * (rowIdx - (numRowsPerAzim * DopIdxCurr));

            while(localMaxVal != 0)
            {
#ifdef SUBSYS_M4
                bit = __clz(__rbit(localMaxVal));
#elif defined(SUBSYS_DSS)
                bit =  _lmbd(1, _bitr(localMaxVal));
#endif
                /* Clear the bit */
                localMaxVal &= ~((uint32_t)1U << bit);

                AzimIdxCurr = AzimIdx + bit;

                /* Extract the signal power. */
                azimPeakSample = *(uint16_t *)(&azimFFTMat[(AzimIdxCurr + DopIdxCurr * obj->cfarAzimFFTCfg.numAzimFFTBins)
                                                * DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE]);

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
                /* We have a common peak. Now, check to see if there is space
                * in this range gate for this peak. It should be one of
                * the largest MAX_NUM_OBJ_PER_RANGE_BIN peaks in the range-
                * gate. */

                /* Compute the SNR w.r.t to the CFAR noise. */
                SNR = (int32_t) azimPeakSample - (int32_t)CFARNoiseCurr;

                /* Check if it is one of highest  MAX_NUM_OBJ_PER_RANGE_BIN SNR
                * object in the range bin. If it is, update the SNRlist with
                * that position, and return the position. */
                if (numObjPerRangeGate < maxNumObjsCurRangeGate)
                {
                    posIdx = numObjPerRangeGate;
                    SNRList[posIdx] = SNR;
                }
                else
                {
                    posIdx = DPU_DopplerProcHWA_updateSNRList(SNR, SNRList, maxNumObjsCurRangeGate);
                    if(posIdx == maxNumObjsCurRangeGate)
                    {
                        continue;
                    }
                }

                currObjParams = &cfg->hwRes.detObjList[obj->numObjOut + posIdx];
#else
                currObjParams = &cfg->hwRes.detObjList[obj->numObjOut];
#endif
                /* Object found */
                /* Place all parameters it in its alloted position. */
                currObjParams->azimIdx = AzimIdxCurr;
                currObjParams->dopIdx = DopIdxCurr;
                currObjParams->rangeIdx = rangeIdx;
                subBandIdx = (uint32_t )dopSubMaxMat[DopIdxCurr];
                currObjParams->dopIdxActual = DopIdxCurr + (subBandIdx * numDopplerBinsPerSubBand);
                if (currObjParams->dopIdxActual > obj->numDopplerBins)
                {
                    currObjParams->dopIdxActual -= obj->numDopplerBins;
                }
                /* Note that CFARNoise (the parameter to which the CUT is compared against) is not used anywhere else. */
                currObjParams->dopCfarNoise = CFARNoiseCurr;

                dopFFTMatStartIdx = DopIdxCurr * numBytesPerRDBin;

                /* Obtain doppler FFT samples corresponding to the azimuth antenna samples.  */

                /* Check the completion of previous transfer before triggering the next. */
                if(detObj > 0U)
                {
                    /* Check the interrupt status */
                    while(((*(volatile uint32_t*)((uint32_t)edmaIntrStatusReg)) & (channelMask)) != (channelMask))
                    {
                        /* Wait for completion */
                    }
                    *(volatile uint32_t*)((uint32_t)edmaClrIntrStatusReg) = channelMask;
                }

                /* Store the antenna samples */

                /* update src address */
                *(volatile uint32_t*)((uint32_t)edmaSrcAddr) = (uint32_t)SOC_virtToPhy((void*)&dopFFTMat[dopFFTMatStartIdx]);

                /* update dst address */
                *(volatile uint32_t*)((uint32_t)edmaDstAddr) = (uint32_t)SOC_virtToPhy((void*)&currObjParams->azimSamples[0]);

                /* trigger */
                *(volatile uint32_t*)((uint32_t)edmaTrigReg) = channelMask;
                detObj++;

                /* Obtain the azim values at and around the peak */
                /* m1Idx is the index before the peak. */
                m1Idx = ((int32_t)AzimIdxCurr) - 1;
                if (m1Idx < 0)
                {
                    m1Idx = (int32_t)obj->cfarAzimFFTCfg.numAzimFFTBins - 1;
                }
                /* p1Idx is the index after the peak. */
                p1Idx = (AzimIdxCurr + 1U);
                if (p1Idx >= obj->cfarAzimFFTCfg.numAzimFFTBins)
                {
                    p1Idx = 0U;
                }

                azimPeakSamplem1 = *(uint16_t *)(&azimFFTMat[(m1Idx + DopIdxCurr * obj->cfarAzimFFTCfg.numAzimFFTBins)
                                                * DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE]);
                azimPeakSamplep1 = *(uint16_t *)(&azimFFTMat[(p1Idx + DopIdxCurr * obj->cfarAzimFFTCfg.numAzimFFTBins)
                                                * DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE]);
                currObjParams->azimPeakSamples[0] = azimPeakSamplem1;
                currObjParams->azimPeakSamples[1] = azimPeakSample;
                currObjParams->azimPeakSamples[2] = azimPeakSamplep1;

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
                numObjPerRangeGate++;
                if (numObjPerRangeGate > maxNumObjsCurRangeGate)
                {
                    numObjPerRangeGate = maxNumObjsCurRangeGate;
                }

                if (numObjPerRangeGate + obj->numObjOut >= cfg->staticCfg.maxNumObj)
                {
                    obj->numObjOut += numObjPerRangeGate;
                    goto exit;
                }
#else
                obj->numObjOut++;
                /* If we have no more space for new objects, return. */
                if (obj->numObjOut >= cfg->staticCfg.maxNumObj)
                {
                    goto exit;
                }
#endif
            } /* end of bits set per row loop*/
        } /* end of if condn */
    } /* end of rows traversal loop */

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
    obj->numObjOut+= numObjPerRangeGate;
#endif

exit:
    /* Monitor the completion of last transfer here. */
    if(detObj > 0)
    {
        while(((*(volatile uint32_t*)((uint32_t)edmaIntrStatusReg)) & (channelMask)) != (channelMask))
        {
            /* wait for completion */
        }
        *(volatile uint32_t*)((uint32_t)edmaClrIntrStatusReg) = channelMask;
    }
    return retVal;
}

#else

/**
 *  @b Description
 *  @n
 *      Performs Object List creation
 *
 *  @param[in] obj          - DPU obj
 *  @param[in] cfg          - DPU configuration
 *  @param[in] blockIdx     - Decompressed block index
 *  @param[in] mergedOBIdx  - OuterBlock Index (Goes from 0 to Merged Number of Outer Blocks)
 *  @param[in] rangeBinIdx  - Range bin index (starts from 0 for any decompressed block)
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval error code.
 */
static int32_t DPU_DopplerProcHWA_extractObjectList(DPU_DopplerProcHWA_Obj      * restrict obj,
                                                    DPU_DopplerProcHWA_Config   * restrict cfg,
                                                    uint32_t blockIdx,
                                                    uint32_t mergedOBIdx,
                                                    uint32_t rangeBinIdx)
{
    uint8_t * restrict azimFFTMat;
    uint8_t * restrict cfarMat;
    uint8_t * restrict localMaxMat;
    uint8_t * restrict dopSubMaxMat;
    uint8_t * restrict dopFFTMat;
    uint32_t baseAddr = EDMA_getBaseAddr(obj->edmaHandle);
    uint32_t edmaSrcAddr, edmaDstAddr, edmaTrigReg, edmaIntrStatusReg, edmaClrIntrStatusReg, channelMask;

    uint32_t i, detObj=0; //numTxAntAzim, numTxAntElev,
    uint32_t numCfarPeaks, DopIdxCurr, AzimIdxCurr, CFARNoiseCurr, numRowsPerAzim;
    uint32_t RowIdx, BitIdx, rowVal, bit, cfarResReal, cfarResImag;
    uint32_t azimPeakSamplem1, azimPeakSample, p1Idx, azimPeakSamplep1, dopFFTMatStartIdx;
    DetObjParams * restrict currObjParams;
    int32_t m1Idx, retVal = 0;
    uint32_t cfarPeaksToLoop;
	uint32_t numDopplerBinsPerSubBand = (uint32_t)obj->numDopplerBins / (uint32_t)obj->dopplerDemodCfg.numBandsTotal;
	uint32_t numBytesPerRDBin, subBandIdx;
	uint32_t dopFFTBytesPerSample = DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE;
    uint32_t rangeIdx = (((blockIdx * obj->decompCfg.mergedNumOuterBlocks) + mergedOBIdx) * obj->decompCfg.rangeBinsPerBlock) + rangeBinIdx;

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
    int32_t SNR;
    uint32_t posIdx;
	uint32_t numObjPerRangeGate = 0;
    int32_t SNRList[MAX_NUM_OBJ_PER_RANGE_BIN] __attribute__((aligned(8))) = {0};
    uint32_t maxNumObjsCurRangeGate;
#endif

    /* Do not bother with objects in the first range bin. */
    if(rangeIdx == 0U)
    {
        return retVal;
    }

	if (obj->numObjOut >= cfg->staticCfg.maxNumObj)
	{
		return retVal;
	}

	numBytesPerRDBin = dopFFTBytesPerSample * cfg->staticCfg.numVirtualAntennas;
    edmaSrcAddr = baseAddr + EDMA_TPCC_OPT(cfg->hwRes.edmaCfg.edmaDetObjAntSamples.channel) + 0x4U;
    edmaDstAddr = edmaSrcAddr + 0x8U;
    edmaTrigReg = baseAddr + EDMA_TPCC_ESR_RN(0U);
    edmaIntrStatusReg =  baseAddr + EDMA_TPCC_IPR_RN(0U);
    edmaClrIntrStatusReg = baseAddr + EDMA_TPCC_ICR_RN(0U);
    channelMask = (uint32_t)1U << cfg->hwRes.edmaCfg.edmaDetObjAntSamples.channel;

    if(rangeBinIdx % 2U == 0U)
    {
        numCfarPeaks = obj->numCfarPeaksPing;
    }
    else
    {
        numCfarPeaks = obj->numCfarPeaksPong;
    }

    /* Assign local matrix pointers based on ping/pong */
    azimFFTMat = (uint8_t *) obj->hwaMemBankAddr[(rangeBinIdx % 2U == 1U) ? DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PONG_OUT : DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PING_OUT];
    cfarMat = (uint8_t *) obj->hwaMemBankAddr[(rangeBinIdx % 2U == 1U) ? DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PONG_OUT : DPU_DOPPLERHWADDMA_MEM_BANK_CFAR_PING_OUT];
    localMaxMat = (uint8_t *) obj->hwaMemBankAddr[(rangeBinIdx % 2U == 1U) ? DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PONG_OUT : DPU_DOPPLERHWADDMA_MEM_BANK_LOCALMAX_PING_OUT];
    dopSubMaxMat = (uint8_t *) cfg->hwRes.dopMaxSubBandScratchBuf[((rangeBinIdx % 2U) == 0U) ? 0U : 1U];
    numRowsPerAzim = (obj->cfarAzimFFTCfg.numAzimFFTBins % 32U == 0U) ?
                        ((uint32_t)obj->cfarAzimFFTCfg.numAzimFFTBins / 32U) : ((uint32_t)obj->cfarAzimFFTCfg.numAzimFFTBins / 32U) + 1U;
    dopFFTMat = (uint8_t *) obj->hwaMemBankAddr[(rangeBinIdx % 2U == 1U) ? DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PONG_IN : DPU_DOPPLERHWADDMA_MEM_BANK_AZIMFFT_PING_IN];

    cfarPeaksToLoop = (numCfarPeaks > cfg->hwRes.maxCfarPeaksToDetect) ? cfg->hwRes.maxCfarPeaksToDetect : numCfarPeaks;

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
    maxNumObjsCurRangeGate = (((cfg->hwRes.maxObjListPerRGateSize) * (rangeIdx + 1U)) - (obj->numObjOut * sizeof(DetObjParams))) / sizeof(DetObjParams);
    /* Following is required to ensure local array SNRList in DPU_DopplerProcHWA_extractObjectList does not overflow.
     * This also allows user to set a lower limit for object detection per range gate, if desired. */
    if (maxNumObjsCurRangeGate > MAX_NUM_OBJ_PER_RANGE_BIN)
    {
        maxNumObjsCurRangeGate = MAX_NUM_OBJ_PER_RANGE_BIN;
    }
#endif

    /* Loop through CFAR peaks and check whether an object is present for a particular peak. If it is present,
       store its parameters in the output list */
    for(i = 0; i < cfarPeaksToLoop; i++)
    {
        /* To get the real and imag part */
        cfarResImag = *(uint32_t *)(&cfarMat[i * sizeof(cmplx32ImRe_t)]);
        cfarResReal = *(uint32_t *)(&cfarMat[(i * sizeof(cmplx32ImRe_t)) + sizeof(cmplx32ImRe_t)/2U]);
        CFARNoiseCurr = cfarResImag;

        AzimIdxCurr = (cfarResReal) >> 12;
        DopIdxCurr = (cfarResReal) - (AzimIdxCurr << 12);

        if(DopIdxCurr > numDopplerBinsPerSubBand)
        {
			continue;
        }

        RowIdx = (DopIdxCurr * numRowsPerAzim) + (AzimIdxCurr / 32U);
        BitIdx = AzimIdxCurr - 32U * (AzimIdxCurr / 32U);

        rowVal = *(uint32_t *)(&localMaxMat[RowIdx * sizeof(uint32_t)]);
        bit = EXTRACT_BIT(rowVal, BitIdx);

        if(bit != 0U)
        {
            /* Extract the signal power. */
            azimPeakSample = *(uint16_t *)(&azimFFTMat[(AzimIdxCurr + DopIdxCurr * obj->cfarAzimFFTCfg.numAzimFFTBins) * DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE]);

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
            /* We have a common peak. Now, check to see if there is space
			 * in this range gate for this peak. It should be one of
			 * the largest MAX_NUM_OBJ_PER_RANGE_BIN peaks in the range-
			 * gate. */

            /* Compute the SNR w.r.t to the CFAR noise. */
            SNR = (int32_t) azimPeakSample - (int32_t)CFARNoiseCurr;

            /* Check if it is one of highest  MAX_NUM_OBJ_PER_RANGE_BIN SNR
             * object in the range bin. If it is, update the SNRlist with
             * that position, and return the position. */
            if (numObjPerRangeGate < maxNumObjsCurRangeGate)
            {
				posIdx = numObjPerRangeGate;
				SNRList[posIdx] = SNR;
			}
			else
			{
				posIdx = DPU_DopplerProcHWA_updateSNRList(SNR, SNRList, maxNumObjsCurRangeGate);
                if(posIdx == maxNumObjsCurRangeGate)
                {
                    continue;
                }
			}

            currObjParams = &cfg->hwRes.detObjList[obj->numObjOut + posIdx];
#else
            currObjParams = &cfg->hwRes.detObjList[obj->numObjOut];
#endif
            /* Object found */
            /* Place all parameters it in its alloted position. */
            currObjParams->azimIdx = AzimIdxCurr;
            currObjParams->dopIdx = DopIdxCurr;
            currObjParams->rangeIdx = rangeIdx;
            subBandIdx = (uint32_t )dopSubMaxMat[DopIdxCurr];
            currObjParams->dopIdxActual = (DopIdxCurr + (subBandIdx * numDopplerBinsPerSubBand));
			if (currObjParams->dopIdxActual > obj->numDopplerBins)
			{
				currObjParams->dopIdxActual -= obj->numDopplerBins;
			}
			/* Note that CFARNoise (the parameter to which the CUT is compared against) is not used anywhere else. */
			currObjParams->dopCfarNoise = CFARNoiseCurr;

            dopFFTMatStartIdx = DopIdxCurr * numBytesPerRDBin;

            /* Obtain doppler FFT samples corresponding to the azimuth antenna samples.  */

            /* Check the completion of previous transfer before triggering the next. */
            if(detObj > 0U)
            {
                /* Check the interrupt status */
                while(((*(volatile uint32_t*)((uint32_t)edmaIntrStatusReg)) & (channelMask)) != (channelMask))
                {
                    /* Wait for the transfer to complete. */
                }
                *(volatile uint32_t*)((uint32_t)edmaClrIntrStatusReg) = channelMask;
            }

            /* Store the antenna samples */

            /* update src address */
            *(volatile uint32_t*)((uint32_t)edmaSrcAddr) = (uint32_t)SOC_virtToPhy((void*)&dopFFTMat[dopFFTMatStartIdx]);

            /* update dst address */
            *(volatile uint32_t*)((uint32_t)edmaDstAddr) = (uint32_t)SOC_virtToPhy((void*)&currObjParams->azimSamples[0]);

            /* trigger */
            *(volatile uint32_t*)((uint32_t)edmaTrigReg) = channelMask;
            detObj++;

            /* Obtain the azim values at and around the peak */
			/* m1Idx is the index before the peak. */
            m1Idx = (int32_t)AzimIdxCurr - 1;
			if (m1Idx < 0)
			{
				m1Idx = (int32_t)obj->cfarAzimFFTCfg.numAzimFFTBins-1;
			}
			/* p1Idx is the index after the peak. */
			p1Idx = (AzimIdxCurr + 1U);
			if (p1Idx >= obj->cfarAzimFFTCfg.numAzimFFTBins)
			{
			    p1Idx = 0U;
			}

            azimPeakSamplem1 = *(uint16_t *)(&azimFFTMat[((uint32_t)m1Idx + DopIdxCurr * (uint32_t)obj->cfarAzimFFTCfg.numAzimFFTBins) * DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE]);
            azimPeakSamplep1 = *(uint16_t *)(&azimFFTMat[((uint32_t)p1Idx + DopIdxCurr * (uint32_t)obj->cfarAzimFFTCfg.numAzimFFTBins) * DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE]);
            currObjParams->azimPeakSamples[0] = azimPeakSamplem1;
            currObjParams->azimPeakSamples[1] = azimPeakSample;
            currObjParams->azimPeakSamples[2] = azimPeakSamplep1;

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
			numObjPerRangeGate++;
			if (numObjPerRangeGate > maxNumObjsCurRangeGate)
			{
				numObjPerRangeGate = maxNumObjsCurRangeGate;
			}

            if (numObjPerRangeGate + obj->numObjOut >= cfg->staticCfg.maxNumObj)
			{
                obj->numObjOut += numObjPerRangeGate;
				goto exit;
			}
#else
            obj->numObjOut++;
            /* If we have no more space for new objects, return. */
            if (obj->numObjOut >= cfg->staticCfg.maxNumObj)
            {
                goto exit;
            }
#endif
        }
    }
#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
    obj->numObjOut+= numObjPerRangeGate;
#endif

exit:
    /* Monitor the completion of last transfer here. */
    if(detObj > 0U)
    {
        while(((*(volatile uint32_t*)((uint32_t)edmaIntrStatusReg)) & (channelMask)) != (channelMask))
        {
            /* Wait for the transfer to complete. */
        }
        *(volatile uint32_t*)((uint32_t)edmaClrIntrStatusReg) = channelMask;
    }
    return retVal;
}
#endif

/**
 *  @b Description
 *  @n
 *      Configures HWA for Decompression stage of Doppler processing.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval error code.
 */
static int32_t DPU_DopplerProcHWA_configHwaDecompression
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{
    HWA_ParamConfig         hwaParamCfg;
    HWA_InterruptConfig     paramISRConfig;
    int32_t                 errCode;
    uint8_t                 destChanPing;
    uint8_t                 pingHwParamsetIdx = (uint8_t)cfg->hwRes.hwaCfg.decompStageHwaStateMachineCfg.paramSetStartIdx;
    uint8_t                 pongHwParamsetIdx = (uint8_t)pingHwParamsetIdx + (uint8_t)cfg->hwRes.hwaCfg.decompStageHwaStateMachineCfg.numParamSets / 2U;
    dopplerProcHWADDMADecompressionCfg* pDPDecompParams;
    uint16_t                rxAntIdx;
    pDPDecompParams = &obj->decompCfg;
    uint32_t                index;

    /* Disable paramset interrupts */
    for (index = 0; index < cfg->hwRes.hwaCfg.decompStageHwaStateMachineCfg.numParamSets; index++)
    {
        errCode = HWA_disableParamSetInterrupt(obj->hwaHandle, (uint8_t)index + pingHwParamsetIdx,
                                               HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_DMA);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    (void)memset((void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));

    /********************************************************************************/

    /*******************************/
    /* PING DECOMPRESSION PARAMSET */
    /*******************************/
{{
        /* adcbuf not mapped, HWA is triggered after edma copy is done */
        hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
        hwaParamCfg.triggerSrc = pingHwParamsetIdx;

        hwaParamCfg.accelMode = HWA_ACCELMODE_COMPRESS;

        /* ACCELMODE CONFIG */
#if defined(DATAPATH_TEST) || defined(OBJ_DETECTION_DDMA_TEST)
        hwaParamCfg.accelModeArgs.compressMode.ditherEnable = HWA_FEATURE_BIT_DISABLE; // Disable dither for datapath bit-exact test
#else
        hwaParamCfg.accelModeArgs.compressMode.ditherEnable = HWA_FEATURE_BIT_ENABLE; // Enable dither to suppress quantization spurs
#endif
        hwaParamCfg.accelModeArgs.compressMode.compressDecompress = HWA_CMP_DCMP_DECOMPRESS;
        hwaParamCfg.accelModeArgs.compressMode.method = pDPDecompParams->compressionMethod;
        hwaParamCfg.accelModeArgs.compressMode.passSelect = HWA_COMPRESS_PATHSELECT_BOTHPASSES;
        hwaParamCfg.accelModeArgs.compressMode.headerEnable = HWA_FEATURE_BIT_ENABLE;
        hwaParamCfg.accelModeArgs.compressMode.scaleFactorBW = 4; //log2(sample bits)

        /* SRC CONFIG */
        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DECOMP_PING_IN;

        hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        hwaParamCfg.source.srcSign = HWA_SAMPLES_UNSIGNED;
        hwaParamCfg.source.srcConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.source.srcScale = 0;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_DECOMP_PING_OUT;

        hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; /* 16 bit real, 16 bit imag */
        hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 0U;
        hwaParamCfg.dest.dstSkipInit = 0U;

        if (pDPDecompParams->compressionMethod == HWA_COMPRESS_METHOD_BFP)
        {
            /***************************************/
            /* PING BFP DECOMPRESSION PARAMSET RX1 */
            /***************************************/

            hwaParamCfg.accelModeArgs.compressMode.BFPMantissaBW = (uint8_t)((pDPDecompParams->inputBytesPerBlock * 8U - 
                                                                              hwaParamCfg.accelModeArgs.compressMode.scaleFactorBW) / 
                                                                              (pDPDecompParams->outputSamplesPerBlock * 2U));

            /* SRC CONFIG */
            hwaParamCfg.source.srcAcnt = pDPDecompParams->inputSamplesPerBlock - 1U;
            hwaParamCfg.source.srcAIdx = (int32_t)pDPDecompParams->bytesPerSample;
            hwaParamCfg.source.srcBcnt = (pDPDecompParams->numBlocksPerPing / pDPDecompParams->rxAntPerBlock) * (uint16_t)pDPDecompParams->mergedNumOuterBlocks - 1U;
            hwaParamCfg.source.srcBIdx = (int32_t)pDPDecompParams->inputBytesPerBlock * (int32_t)pDPDecompParams->rxAntPerBlock;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAcnt = pDPDecompParams->outputSamplesPerBlock - 1U;
            hwaParamCfg.dest.dstAIdx = (int32_t)pDPDecompParams->bytesPerSample * (int32_t)pDPDecompParams->rxAntPerBlock;
            hwaParamCfg.dest.dstBIdx = (int32_t)pDPDecompParams->outputBytesPerBlock * (int32_t)pDPDecompParams->rxAntPerBlock;

            errCode = HWA_configParamSet(obj->hwaHandle,
                                         pingHwParamsetIdx,
                                         &hwaParamCfg,NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            /***************************************/
            /* PONG BFP DECOMPRESSION PARAMSET RX1 */
            /***************************************/
            /* adcbuf not mapped, HWA is triggered after edma copy is done */
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
            hwaParamCfg.triggerSrc = pongHwParamsetIdx;

            /* SRC CONFIG */
            hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DECOMP_PONG_IN;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_DECOMP_PONG_OUT;

            errCode = HWA_configParamSet(obj->hwaHandle,
                                         pongHwParamsetIdx,
                                         &hwaParamCfg,NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            for (rxAntIdx = 1; rxAntIdx < pDPDecompParams->rxAntPerBlock; rxAntIdx++)
            {
                /***************************************/
                /* PING BFP DECOMPRESSION PARAMSET RXN */
                /***************************************/
                pingHwParamsetIdx++;
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
                hwaParamCfg.source.srcAddr = (uint32_t)DPU_DOPPLERHWADDMA_ADDR_DECOMP_PING_IN + (uint32_t)rxAntIdx * (uint32_t)pDPDecompParams->inputBytesPerBlock;
                hwaParamCfg.dest.dstAddr = (uint32_t)DPU_DOPPLERHWADDMA_ADDR_DECOMP_PING_OUT + (uint32_t)rxAntIdx * (uint32_t)pDPDecompParams->bytesPerSample;
                errCode = HWA_configParamSet(obj->hwaHandle,
                                             pingHwParamsetIdx,
                                             &hwaParamCfg, NULL);
                if (errCode != 0)
                {
                    goto exit;
                }

                /***************************************/
                /* PONG BFP DECOMPRESSION PARAMSET RXN */
                /***************************************/
                pongHwParamsetIdx++;
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
                hwaParamCfg.source.srcAddr = (uint32_t)DPU_DOPPLERHWADDMA_ADDR_DECOMP_PONG_IN + (uint32_t)rxAntIdx * (uint32_t)pDPDecompParams->inputBytesPerBlock;
                hwaParamCfg.dest.dstAddr = (uint32_t)DPU_DOPPLERHWADDMA_ADDR_DECOMP_PONG_OUT + (uint32_t)rxAntIdx * (uint32_t)pDPDecompParams->bytesPerSample;
                errCode = HWA_configParamSet(obj->hwaHandle,
                                             pongHwParamsetIdx,
                                             &hwaParamCfg,NULL);
                if (errCode != 0)
                {
                    goto exit;
                }
            }
        }

        else if(pDPDecompParams->compressionMethod == HWA_COMPRESS_METHOD_EGE)
        {
            /***********************************/
            /* PING EGE DECOMPRESSION PARAMSET */
            /***********************************/
            hwaParamCfg.accelModeArgs.compressMode.EGEKarrayLength = 3; // log2(8)

            /* SRC CONFIG */
            hwaParamCfg.source.srcAcnt = pDPDecompParams->inputSamplesPerBlock - 1U;
            hwaParamCfg.source.srcAIdx = (int32_t)pDPDecompParams->bytesPerSample;
            hwaParamCfg.source.srcBcnt = pDPDecompParams->numBlocksPerPing * (uint16_t)pDPDecompParams->mergedNumOuterBlocks - 1U;
            hwaParamCfg.source.srcBIdx = (int32_t)pDPDecompParams->inputBytesPerBlock;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAcnt = pDPDecompParams->outputSamplesPerBlock - 1U;
            hwaParamCfg.dest.dstAIdx = (int32_t)pDPDecompParams->bytesPerSample;
            hwaParamCfg.dest.dstBIdx = (int32_t)pDPDecompParams->outputBytesPerBlock;

            errCode = HWA_configParamSet(obj->hwaHandle,
                                         pingHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            /***********************************/
            /* PONG EGE DECOMPRESSION PARAMSET */
            /***********************************/
            /* adcbuf not mapped, HWA is triggered after edma copy is done */
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
            hwaParamCfg.triggerSrc = pongHwParamsetIdx;

            /* SRC CONFIG */
            hwaParamCfg.source.srcAddr = (uint32_t)DPU_DOPPLERHWADDMA_ADDR_DECOMP_PONG_IN;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAddr = (uint32_t)DPU_DOPPLERHWADDMA_ADDR_DECOMP_PONG_OUT;

            errCode = HWA_configParamSet(obj->hwaHandle,
                                         pongHwParamsetIdx,
                                         &hwaParamCfg,NULL);
            if (errCode != 0)
            {
                goto exit;
            }
        }
        else
        {
            /* There is no other valid case. */
            DebugP_assert(0);
        }

        errCode = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaOut.pingPong[PING].channel, &destChanPing);
        if (errCode != 0)
        {
            goto exit;
        }
        /* enable the DMA hookup to this paramset so that data gets copied out */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
        paramISRConfig.dma.dstChannel = destChanPing;
        errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pingHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }

        errCode = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaOut.pingPong[PONG].channel, &destChanPing);
        if (errCode != 0)
        {
            goto exit;
        }
        /* enable the DMA hookup to this paramset so that data gets copied out */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
        paramISRConfig.dma.dstChannel = destChanPing;
        errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pongHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
    }}

exit:
    return(errCode);
}

/**
 *  @b Description
 *  @n
 *      Configures HWA for Doppler processing and pre-demodulation stage.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval error code.
 */
static int32_t DPU_DopplerProcHWA_configHwaDopplerFFTDDMADemod
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{
    HWA_ParamConfig         hwaParamCfg;
    HWA_InterruptConfig     paramISRConfig;
    uint8_t                pingHwParamsetIdx = (uint8_t)cfg->hwRes.hwaCfg.dopplerStageHwaStateMachineCfg.paramSetStartIdx;
    uint8_t                pongHwParamsetIdx = pingHwParamsetIdx + cfg->hwRes.hwaCfg.dopplerStageHwaStateMachineCfg.numParamSets / 2U;
    int32_t                 errCode = 0;
    uint8_t                 destChan;
    uint32_t                fftSizeTemp;
    uint32_t                index;
    uint16_t numDopBinsDemod = (obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal);

    /* Disable paramset interrupts */
    for (index = 0; index < cfg->hwRes.hwaCfg.dopplerStageHwaStateMachineCfg.numParamSets; index++)
    {
        errCode = HWA_disableParamSetInterrupt(obj->hwaHandle, (uint8_t)index + pingHwParamsetIdx,
                                               HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_DMA);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /***********************/
    /* PING DUMMY PARAMSET */
    /***********************/
    {

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
        hwaParamCfg.triggerSrc = pingHwParamsetIdx;
        hwaParamCfg.accelMode = HWA_ACCELMODE_NONE;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg,NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    /***********************/
    /* PONG DUMMY PARAMSET */
    /***********************/
    {
        hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
        hwaParamCfg.triggerSrc = pongHwParamsetIdx;
        hwaParamCfg.accelMode = HWA_ACCELMODE_NONE;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /*******************************/
    /* PING DOPPLER FFT PARAMSET */
    /*******************************/
    {
        pingHwParamsetIdx++;

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
        hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

        /* PREPROC CONFIG */
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        /* ACCELMODE CONFIG (FFT) */
        hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
        if (obj->numDopplerBins % 3U == 0U)
        {
            hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2((uint32_t)obj->numDopplerBins / 3U);
            hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_ENABLE;
        }
        else
        {
            hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(obj->numDopplerBins);
            hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_DISABLE;
        }
        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0;
        hwaParamCfg.accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE;
        hwaParamCfg.accelModeArgs.fftMode.windowStart = (uint16_t)cfg->hwRes.hwaCfg.winRamOffset;
        hwaParamCfg.accelModeArgs.fftMode.winSymm = cfg->hwRes.hwaCfg.winSym;

        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        /* SOURCE CONFIG */
        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DOPPLERFFT_PING_IN;

        hwaParamCfg.source.srcAcnt = cfg->staticCfg.numChirps - 1U; /* this is samples - 1 */
        hwaParamCfg.source.srcAIdx = (int32_t)cfg->staticCfg.numRxAntennas * (int32_t)DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcBcnt = (uint16_t)cfg->staticCfg.numRxAntennas - 1U;
        hwaParamCfg.source.srcBIdx = (int32_t)DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE;

        hwaParamCfg.source.srcRealComplex = DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_ISREAL;

        if ((DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_ISREAL == 0U)))
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.source.srcSign = DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_ISSIGNED;
        hwaParamCfg.source.srcConjugate = 0;
        hwaParamCfg.source.srcScale = 8;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_DOPPLERFFT_PING_OUT;

        hwaParamCfg.dest.dstAcnt = obj->numDopplerBins - 1U;
        hwaParamCfg.dest.dstAIdx = (int32_t)DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE;
        hwaParamCfg.dest.dstBIdx = (int32_t)obj->numDopplerBins * (int32_t)DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE;

        hwaParamCfg.dest.dstRealComplex = DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_ISREAL;
        if ((DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_ISREAL == 0U)))
        {
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.dest.dstSign = DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_ISSIGNED;
        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 0;
        hwaParamCfg.dest.dstSkipInit = 0;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg,NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    /*******************************/
    /* PONG DOPPLER FFT PARAMSET   */
    /*******************************/
    {
        pongHwParamsetIdx++;

        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DOPPLERFFT_PONG_IN;
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_DOPPLERFFT_PONG_OUT;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg,NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /*******************************/
    /* PING LOG ABS SUM RX PARAMSET */
    /*******************************/
    {
        pingHwParamsetIdx++;

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
        hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

        /* PREPROC CONFIG */
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        /* ACCELMODE CONFIG (FFT) */
        hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_SUM_STATS;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_ENABLED;

        /* SOURCE CONFIG */
        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_LOGABS_PING_IN;

        hwaParamCfg.source.srcAcnt = (uint16_t)cfg->staticCfg.numRxAntennas - 1U;
        hwaParamCfg.source.srcAIdx = (int32_t)obj->numDopplerBins * (int32_t)DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcBIdx = (int32_t)DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcBcnt = obj->numDopplerBins - 1U;
        hwaParamCfg.source.srcRealComplex = DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_ISREAL;
        if ((DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_ISREAL == 0U)))
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.source.srcSign = DPU_DOPPLERHWADDMA_LOGABSIO_INPUT_ISSIGNED;
        hwaParamCfg.source.srcConjugate = 0;
        hwaParamCfg.source.srcScale = 8;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_SUMRX_PING_OUT;

        /* Following values are fixed in Stats Mode */
        hwaParamCfg.dest.dstAcnt = 1U - 1U; /* Output (sum) */
        hwaParamCfg.dest.dstAIdx = (int32_t)DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_BYTESPERSAMPLE;
        hwaParamCfg.dest.dstBIdx = (int32_t)DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_BYTESPERSAMPLE;
        hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
        hwaParamCfg.dest.dstSign = DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_ISSIGNED;

        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 8;
        hwaParamCfg.dest.dstSkipInit = 0;
        hwaParamCfg.dest.dstIQswap = HWA_FEATURE_BIT_ENABLE;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg,NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    /********************************/
    /* PONG LOG ABS SUM RX PARAMSET */
    /********************************/
    {
        pongHwParamsetIdx++;

        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_LOGABS_PONG_IN;
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_SUMRX_PONG_OUT;
        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg,NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /*******************************/
    /* PING DDMA METRIC PARAMSET   */
    /*******************************/
    {
        pingHwParamsetIdx++;

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
        hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

        /* PREPROC CONFIG */
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        /* ACCELMODE CONFIG (FFT) */
        hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE; /* For sum calculation */
        fftSizeTemp = mathUtils_getValidFFTSize(cfg->staticCfg.numTxAntennas);
        if (fftSizeTemp % 3U == 0U)
        {
            hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftSizeTemp / 3U);
            hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_ENABLE;
        }
        else
        {
            hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftSizeTemp);
            hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_DISABLE;
        }
        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0;
        hwaParamCfg.accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_DISABLE;

        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_ONLY_ENABLED;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        /* SOURCE CONFIG */
        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PING_IN;

        hwaParamCfg.source.srcAcnt = obj->dopplerDemodCfg.numBandsActive - 1U; /* this is samples - 1 */
        hwaParamCfg.source.srcAIdx = (int32_t)DPU_DOPPLERHWADDMA_DDMAMETRICIO_INPUT_BYTESPERSAMPLE * (int32_t)numDopBinsDemod;
        hwaParamCfg.source.srcBcnt = obj->numDopplerBins - 1U;
        hwaParamCfg.source.srcBIdx = (int32_t)DPU_DOPPLERHWADDMA_DDMAMETRICIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcRealComplex = DPU_DOPPLERHWADDMA_DDMAMETRICIO_INPUT_ISREAL;
        hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        hwaParamCfg.source.srcSign = DPU_DOPPLERHWADDMA_DDMAMETRICIO_INPUT_ISSIGNED;
        hwaParamCfg.source.srcConjugate = 0;
        hwaParamCfg.source.srcScale = 8;
        hwaParamCfg.source.wrapComb = obj->numDopplerBins * DPU_DOPPLERHWADDMA_DDMAMETRICIO_INPUT_BYTESPERSAMPLE;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PING_OUT;

        hwaParamCfg.dest.dstAcnt = 1U - 1U;
        hwaParamCfg.dest.dstAIdx = (int32_t)DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE;
        hwaParamCfg.dest.dstBIdx = (int32_t)DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE;

        hwaParamCfg.dest.dstRealComplex = DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_ISREAL;
        if ((DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_ISREAL == 0U)))
        {
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.dest.dstSign = DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_ISSIGNED;
        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 0;
        hwaParamCfg.dest.dstSkipInit = 0;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg,NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    /*******************************/
    /* PONG DDMA METRIC PARAMSET   */
    /*******************************/
    {
        pongHwParamsetIdx++;

        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PONG_IN;
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PONG_OUT;

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /*******************************/
    /* PING MAX SUBBAND PARAMSET  */
    /******************************/
    {
        /* This paramset is to find the max subband  of each dopSubBin from the DDMA Metric. */

        pingHwParamsetIdx++;

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
        hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

        /* PREPROC CONFIG */
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        /* ACCELMODE CONFIG (FFT) */
        hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE; /* For sum calculation */
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_MAX_STATS;

        /* SOURCE CONFIG */
        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PING_OUT;

        hwaParamCfg.source.srcAcnt = obj->dopplerDemodCfg.numBandsTotal - 1U; /* this is samples - 1 */
        hwaParamCfg.source.srcAIdx = (int32_t)numDopBinsDemod * (int32_t)DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcBcnt = numDopBinsDemod - 1U;
        hwaParamCfg.source.srcBIdx = (int32_t)DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcRealComplex = DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_ISREAL;
        if ((DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_ISREAL == 0U)))
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.source.srcSign = DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_ISSIGNED;
        hwaParamCfg.source.srcConjugate = 0;
        hwaParamCfg.source.srcScale = 8;

        /* DEST CONFIG */
        /* Storing the ouput at an offset is required because sumRX Out is used as an input to sumTX Parm, and thus cannot be overwritten before it's execution. */
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_MAXSUBBAND_PING_OUT + DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_BYTESPERSAMPLE * obj->numDopplerBins;

        /* Following values are fixed in Stats Mode */
        hwaParamCfg.dest.dstAcnt = 1U - 1U;
        hwaParamCfg.dest.dstAIdx = 8;
        hwaParamCfg.dest.dstBIdx = 8;
        hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
        hwaParamCfg.dest.dstSign = HWA_SAMPLES_UNSIGNED;

        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 8U;
        hwaParamCfg.dest.dstSkipInit = 0U;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }

        /* If Sum TX is enabled, then EDMA Out here may result in Mem Access Error - as EDMA and HWA will try to access the same MEMBANK (RX Out)
         * So, in such cases, EDMA Out of Max Subband Idx data is chained to sumTXOut. */
        if (cfg->staticCfg.isSumTxEnabled == 0U)
        {
            errCode = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PING].channel, &destChan);
            if (errCode != 0)
            {
                goto exit;
            }
            /* enable the DMA hookup to this paramset so that  (max subband idx) gets copied out */
            paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
            paramISRConfig.dma.dstChannel = destChan;
            errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pingHwParamsetIdx, &paramISRConfig);
            if (errCode != 0)
            {
                goto exit;
            }
        }
    }

    /*******************************/
    /* PONG MAX SUBBAND PARAMSET   */
    /*******************************/
    {
        /* This paramset is to find the max subband  of each dopSubBin from the DDMA Metric. */
        pongHwParamsetIdx++;

        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DDMAMETRIC_PONG_OUT;
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_MAXSUBBAND_PONG_OUT + (DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_BYTESPERSAMPLE * obj->numDopplerBins);

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg,NULL);
        if (errCode != 0)
        {
            goto exit;
        }

        /* If Sum TX is enabled, then EDMA Out here may result in Mem Access Error - as EDMA and HWA will try to access the same MEMBANK (RX Out)
         * So, in such cases, EDMA Out of Max Subband Idx data is chained to sumTXOut. */
        if (cfg->staticCfg.isSumTxEnabled == 0U)
        {
            errCode = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PONG].channel, &destChan);
            if (errCode != 0)
            {
                goto exit;
            }
            /* enable the DMA hookup to this paramset so that  (max subband idx) gets copied out */
            paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
            paramISRConfig.dma.dstChannel = destChan;
            errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pongHwParamsetIdx, &paramISRConfig);
            if (errCode != 0)
            {
                goto exit;
            }
        }
    }

    if (cfg->staticCfg.isSumTxEnabled == 1U)
    {
        (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
        /*******************************/
        /* PING SUM TX PARAMSET        */
        /*******************************/
        {

            pingHwParamsetIdx++;

            hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

            /* PREPROC CONFIG */
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

            /* ACCELMODE CONFIG (FFT) */
            hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;

            fftSizeTemp = mathUtils_getValidFFTSize(obj->dopplerDemodCfg.numBandsTotal);
            if (fftSizeTemp % 3U == 0U)
            {
                hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftSizeTemp / 3U);
                hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_ENABLE;
            }
            else
            {
                hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftSizeTemp);
                hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_DISABLE;
            }
            hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0;
            hwaParamCfg.accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_DISABLE;

            hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_ONLY_ENABLED;
            hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

            /* SOURCE CONFIG */
            hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_SUMTX_PING_IN;

            hwaParamCfg.source.srcAcnt = obj->dopplerDemodCfg.numBandsTotal - 1U; /* this is samples - 1 */
            hwaParamCfg.source.srcAIdx = (int32_t)DPU_DOPPLERHWADDMA_SUMTXIO_INPUT_BYTESPERSAMPLE * (int32_t)numDopBinsDemod;
            hwaParamCfg.source.srcBcnt = numDopBinsDemod - 1U;
            hwaParamCfg.source.srcBIdx = (int32_t)DPU_DOPPLERHWADDMA_SUMTXIO_INPUT_BYTESPERSAMPLE;
            hwaParamCfg.source.srcRealComplex = DPU_DOPPLERHWADDMA_SUMTXIO_INPUT_ISREAL;
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
            hwaParamCfg.source.srcSign = DPU_DOPPLERHWADDMA_SUMTXIO_INPUT_ISSIGNED;
            hwaParamCfg.source.srcConjugate = 0;
            hwaParamCfg.source.srcScale = 8;
            hwaParamCfg.source.wrapComb = obj->numDopplerBins * DPU_DOPPLERHWADDMA_SUMTXIO_INPUT_BYTESPERSAMPLE;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_SUMTX_PING_OUT +
                                       obj->numDopplerBins * DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE;

            hwaParamCfg.dest.dstAcnt = 1U - 1U;
            hwaParamCfg.dest.dstAIdx = (int32_t)DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_BYTESPERSAMPLE;
            hwaParamCfg.dest.dstBIdx = (int32_t)DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_BYTESPERSAMPLE;

            hwaParamCfg.dest.dstRealComplex = DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_ISREAL;
            if ((DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_BYTESPERSAMPLE == 2U) ||
                ((DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_BYTESPERSAMPLE == 4U) &&
                 (DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_ISREAL == 0U)))
            {
                hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
            }
            else
            {
                hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
            }
            hwaParamCfg.dest.dstSign = DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_ISSIGNED;
            hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
            hwaParamCfg.dest.dstScale = mathUtils_ceilLog2(obj->dopplerDemodCfg.numBandsTotal);
            hwaParamCfg.dest.dstSkipInit = 0;

            errCode = HWA_configParamSet(obj->hwaHandle,
                                         pingHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            errCode = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaSumLogAbsOut.pingPong[PING].channel, &destChan);
            if (errCode != 0)
            {
                goto exit;
            }
            /* enable the DMA hookup to this paramset so that data gets copied out */
            paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
            paramISRConfig.dma.dstChannel = destChan;
            errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pingHwParamsetIdx, &paramISRConfig);
            if (errCode != 0)
            {
                goto exit;
            }
        }

        /*******************************/
        /* PONG SUM TX PARAMSET        */
        /*******************************/
        {
            pongHwParamsetIdx++;

            hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_SUMTX_PONG_IN;
            hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_SUMTX_PONG_OUT +
                                       (obj->numDopplerBins * DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE);

            errCode = HWA_configParamSet(obj->hwaHandle,
                                         pongHwParamsetIdx,
                                         &hwaParamCfg,NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            errCode = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaSumLogAbsOut.pingPong[PONG].channel, &destChan);
            if (errCode != 0)
            {
                goto exit;
            }
            /* enable the DMA hookup to this paramset so that data gets copied out */
            paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
            paramISRConfig.dma.dstChannel = destChan;
            errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pongHwParamsetIdx, &paramISRConfig);
            if (errCode != 0)
            {
                goto exit;
            }
        }
    }

exit:
    return(errCode);
}

/**
 *  @b Description
 *  @n
 *      Configures Azimuth, CFAR, Local Max processing in HWA.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval error code.
 */
static int32_t DPU_DopplerProcHWA_configHwaCFARAzimFFT
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{
    HWA_ParamConfig         hwaParamCfg;
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
    HWA_InterruptConfig     paramISRConfig;
#endif
    uint8_t                 pingHwParamsetIdx = (uint8_t)cfg->hwRes.hwaCfg.azimCfarStageHwaStateMachineCfg.paramSetStartIdx;
    uint8_t                 pongHwParamsetIdx = pingHwParamsetIdx + (cfg->hwRes.hwaCfg.azimCfarStageHwaStateMachineCfg.numParamSets / 2U);
    int32_t                 errCode = 0;
    uint16_t                rxAntIdx;
    uint32_t                fftSizeTemp;
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    uint8_t                 cfarAvgRight, cfarAvgLeft, cfarGuardCells;
#endif
    uint32_t                index;

    /* Disable paramset interrupts */
    for (index = 0U; index < cfg->hwRes.hwaCfg.azimCfarStageHwaStateMachineCfg.numParamSets; index++)
    {
        errCode = HWA_disableParamSetInterrupt(obj->hwaHandle, (uint8_t)index + pingHwParamsetIdx,
                                               HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_DMA);
        if (errCode != 0)
        {
            goto exit;
        }
    }


    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /*******************************/
    /* PING DEMODULATION PARAMSET  */
    /*******************************/

    {
        /* This paramset demodulates the doppler FFT input using shuffle LUT and outputs the [antenna array][dopSubbins]. */
        hwaParamCfg.triggerMode = HWA_TRIG_MODE_SOFTWARE;
        hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

        /* PREPROC CONFIG */
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        /* ACCELMODE CONFIG (FFT) */
        hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;

        /* SOURCE CONFIG */
        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DEMODULATE_PING_IN;

        hwaParamCfg.source.srcAcnt = (uint16_t)cfg->staticCfg.numTxAntennas - 1U; /* only actve band samples, discard empty band samples */
        hwaParamCfg.source.srcAIdx = (int32_t)DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE * ((int32_t)obj->numDopplerBins / (int32_t)obj->dopplerDemodCfg.numBandsTotal);
        hwaParamCfg.source.srcBcnt = (obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal) - 1U;
        hwaParamCfg.source.srcBIdx = (int32_t)DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE; /* This offset in B-dim is controlled by Shuffle LUT. */
        hwaParamCfg.source.srcRealComplex = DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISREAL;
        if ((DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISREAL == 0U)))
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.source.srcSign = DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISSIGNED;
        hwaParamCfg.source.srcConjugate = 0;
        hwaParamCfg.source.srcScale = 8;
        hwaParamCfg.source.shuffleMode = HWA_SRC_SHUFFLE_AB_MODE_BDIM;
        hwaParamCfg.source.shuffleStart = 0;
        hwaParamCfg.source.wrapComb = obj->numDopplerBins * DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PING_IN;
        hwaParamCfg.dest.dstAcnt = (uint16_t)cfg->staticCfg.numTxAntennas - 1U;
        hwaParamCfg.dest.dstAIdx = (int32_t)cfg->staticCfg.numRxAntennas * (int32_t)DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.dest.dstBIdx = (int32_t)cfg->staticCfg.numRxAntennas * (int32_t)cfg->staticCfg.numTxAntennas * (int32_t)DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.dest.dstRealComplex = DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISREAL;
        hwaParamCfg.dest.dstWidth = hwaParamCfg.source.srcWidth;
        hwaParamCfg.dest.dstSign = DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISSIGNED;
        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 0;
        hwaParamCfg.dest.dstSkipInit = 0;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
        
        for(rxAntIdx = 1; rxAntIdx < cfg->staticCfg.numRxAntennas; rxAntIdx++)
        {
            pingHwParamsetIdx++;

            hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DEMODULATE_PING_IN + (DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE * obj->numDopplerBins * rxAntIdx);
            hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PING_IN + (DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE * rxAntIdx);

            errCode = HWA_configParamSet(obj->hwaHandle,
                                        pingHwParamsetIdx,
                                        &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }
        }
    }

    /*******************************/
    /* PONG DEMODULATION PARAMSET  */
    /*******************************/
    {
        hwaParamCfg.triggerMode = HWA_TRIG_MODE_SOFTWARE;
        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DEMODULATE_PONG_IN;
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PONG_IN;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }

        for(rxAntIdx = 1; rxAntIdx < cfg->staticCfg.numRxAntennas; rxAntIdx++)
        {
            pongHwParamsetIdx++;

            hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_DEMODULATE_PONG_IN + (DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE * obj->numDopplerBins * rxAntIdx);
            hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PONG_IN + (DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE * rxAntIdx);

            errCode = HWA_configParamSet(obj->hwaHandle,
                                        pongHwParamsetIdx,
                                        &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }
        }
    }

    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /*******************************/
    /* PING AZIM FFT PARAMSET      */
    /*******************************/
    {
        pingHwParamsetIdx++;

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
        hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

        /* PREPROC CONFIG */
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.zeroInsertEn = HWA_FEATURE_BIT_ENABLE;

        /* Enable complex multiply mode and ensure reading is from common config regs, not the RAM */
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_VECTOR_MULT;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.vectorMultiplyMode1.cmultScaleEn = HWA_FEATURE_BIT_ENABLE;

        /* ACCELMODE CONFIG (FFT) */
        hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;

        fftSizeTemp = obj->cfarAzimFFTCfg.numAzimFFTBins;
        if(fftSizeTemp % 3U == 0U)
        {
            hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftSizeTemp/3U);
            hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_ENABLE;
        }
        else
        {
            hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftSizeTemp);
            hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_DISABLE;
        }
        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0;
        hwaParamCfg.accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_DISABLE; /* No windowing at this stage */

        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_ENABLED;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;

#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.histogramMode = HWA_HISTOGRAM_MODE_CDF_THRESHOLD;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.histogramScaleSelect = HIST_SCALE_SELECT;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.histogramSizeSelect = HIST_SIZE_SELECT;
#endif

        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.max2Denable = HWA_FEATURE_BIT_ENABLE;

        /* SOURCE CONFIG */
        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PING_IN;

        hwaParamCfg.source.srcAcnt = ((uint16_t)cfg->staticCfg.numRxAntennas * (uint16_t)cfg->staticCfg.numAzimTxAntennas) - 1U; /* this is samples - 1 */
        hwaParamCfg.source.shuffleMode  = HWA_SRC_SHUFFLE_AB_MODE_ADIM;
        hwaParamCfg.source.shuffleStart = (uint8_t)(obj->numDopplerBins / (obj->dopplerDemodCfg.numBandsTotal * 16U));
        if(obj->numDopplerBins % (obj->dopplerDemodCfg.numBandsTotal * 16U) != 0U)
        {
            hwaParamCfg.source.shuffleStart++; /* Calculating ceil effectively */
        }
        hwaParamCfg.source.srcAIdx = (int32_t)DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcBcnt = obj->numDopplerBins/obj->dopplerDemodCfg.numBandsTotal - 1U;
        hwaParamCfg.source.srcBIdx = (int32_t)cfg->staticCfg.numRxAntennas * (int32_t)cfg->staticCfg.numTxAntennas * (int32_t)DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcRealComplex = DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISREAL;
        if ((DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISREAL == 0U)))
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.source.srcSign = DPU_DOPPLERHWADDMA_AZIMFFTIO_INPUT_ISSIGNED;
        hwaParamCfg.source.srcConjugate = 0;
        hwaParamCfg.source.srcScale = 8;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PING_OUT;

        hwaParamCfg.dest.dstAcnt = (uint16_t)fftSizeTemp - 1U;
        hwaParamCfg.dest.dstAIdx = (int32_t)DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE;
        hwaParamCfg.dest.dstBIdx = (int32_t)fftSizeTemp * (int32_t)DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE;

        hwaParamCfg.dest.dstRealComplex = DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_ISREAL;
        if ((DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_ISREAL == 0U)))
        {
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.dest.dstSign = DPU_DOPPLERHWADDMA_AZIMFFTIO_OUTPUT_ISSIGNED;
        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 0;
        hwaParamCfg.dest.dstSkipInit = 0;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
        /* enable the hookup to this paramset so that CPU gets interrupt */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
        paramISRConfig.cpu.callbackFn = DPU_DopplerProcHWA_hwaDoneIsrCallback;
        paramISRConfig.cpu.callbackArg = &obj->azimFFTHwaDoneSemaHandle[PING];
        errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pingHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
#endif
#else
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
        /* enable the hookup to this paramset so that CPU gets interrupt */
        /* This interrupt is to tell the CPU that it can safely program the shuffle LUT RAM */
        /* The shuffle RAM is available only after this paramset, as it was observed that the writes to shuffle RAM
           are not happening correctly just after the previous (demodulation) paramset. Possibly, this is due to
           the shuffle RAM is not released by HWA even though the Demodulation paramset has been completed. */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
        paramISRConfig.cpu.callbackFn = DPU_DopplerProcHWA_hwaDoneIsrCallback;
        paramISRConfig.cpu.callbackArg = &obj->demodHwaDoneSemaHandle;
        errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pingHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
#endif
#endif
    }
    /*******************************/
    /* PONG AZIM FFT PARAMSET      */
    /*******************************/
    {

        pongHwParamsetIdx++;

        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PONG_IN;
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_AZIMFFT_PONG_OUT;

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
        /* enable the hookup to this paramset so that CPU gets interrupt */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
        paramISRConfig.cpu.callbackFn = DPU_DopplerProcHWA_hwaDoneIsrCallback;
        paramISRConfig.cpu.callbackArg = &obj->azimFFTHwaDoneSemaHandle[PONG];
        errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pongHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
#endif
#endif
    }

#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /*******************************/
    /* PING DOPPLER CFAR PARAMSET  */
    /*******************************/
    {

        cfarAvgRight = obj->cfarAzimFFTCfg.cfarCfg->winLen >> 1;
        cfarAvgLeft = obj->cfarAzimFFTCfg.cfarCfg->winLen >> 1;
        cfarGuardCells = obj->cfarAzimFFTCfg.cfarCfg->guardLen;

        pingHwParamsetIdx++;

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
        hwaParamCfg.accelMode = HWA_ACCELMODE_CFAR;

        hwaParamCfg.accelModeArgs.cfarMode.peakGroupEn = obj->cfarAzimFFTCfg.cfarCfg->peakGroupingEn;
        hwaParamCfg.accelModeArgs.cfarMode.operMode = HWA_CFAR_OPER_MODE_LOG_INPUT_REAL; /* cfarInpMode = 1, cfarLogMode = 1, cfarAbsMode = 00b */
        hwaParamCfg.accelModeArgs.cfarMode.numGuardCells = cfarGuardCells;
        hwaParamCfg.accelModeArgs.cfarMode.nAvgDivFactor = obj->cfarAzimFFTCfg.cfarCfg->noiseDivShift;/* not applicable in CFAR_OS */
        hwaParamCfg.accelModeArgs.cfarMode.cyclicModeEn = obj->cfarAzimFFTCfg.cfarCfg->cyclicMode;
        hwaParamCfg.accelModeArgs.cfarMode.nAvgMode = obj->cfarAzimFFTCfg.cfarCfg->averageMode;
        hwaParamCfg.accelModeArgs.cfarMode.numNoiseSamplesRight = cfarAvgRight;
        hwaParamCfg.accelModeArgs.cfarMode.numNoiseSamplesLeft =  cfarAvgLeft;
        hwaParamCfg.accelModeArgs.cfarMode.outputMode = HWA_CFAR_OUTPUT_MODE_I_PEAK_IDX_Q_NEIGHBOR_NOISE_VAL;
        if (obj->cfarAzimFFTCfg.cfarCfg->averageMode == HWA_NOISE_AVG_MODE_CFAR_OS)
        {
            hwaParamCfg.accelModeArgs.cfarMode.cfarOsKvalue = obj->cfarAzimFFTCfg.cfarCfg->osKvalue;
            hwaParamCfg.accelModeArgs.cfarMode.cfarOsEdgeKScaleEn = obj->cfarAzimFFTCfg.cfarCfg->osEdgeKscaleEn;
        }

        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_CFAR_PING_IN;

        if(obj->cfarAzimFFTCfg.cfarCfg->cyclicMode == 1U)
        {
            /* ACNT is equal to the number of samples to be loaded into the sliding buffer including the cyclic rotation */
            hwaParamCfg.source.srcAcnt = (obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal) - 1U +
                                     (2U * (uint16_t)cfarAvgRight) + (2U * (uint16_t)cfarGuardCells) + (2U * (uint16_t)cfarAvgLeft);
            /* Since for the first sample the window starts from the end due to cyclic rotation, this gives offset of first sample of the window for first cell under test*/
            hwaParamCfg.source.srcAcircShift = (obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal) - ((2U * (uint16_t)cfarAvgRight) + (uint16_t)cfarGuardCells);

            /* Finally this tells the size of the array, around which cyclic rotation has to be done */
            if ((obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal) % 3U == 0U)/* If numSamples % 3 == 0 */
            {
                hwaParamCfg.source.srcCircShiftWrap3 = 1U; /* 'b001, means wrap in A dim */
                hwaParamCfg.source.srcAcircShiftWrap = mathUtils_ceilLog2(((uint32_t)obj->numDopplerBins / (uint32_t)obj->dopplerDemodCfg.numBandsTotal) / 3U);
            }
            else
            {
                hwaParamCfg.source.srcCircShiftWrap3 = 0U;
                hwaParamCfg.source.srcAcircShiftWrap = mathUtils_ceilLog2((uint32_t)obj->numDopplerBins / (uint32_t)obj->dopplerDemodCfg.numBandsTotal);
            }
        }
        else
        {
            hwaParamCfg.source.srcAcnt = (obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal) - 1U;
        }
        hwaParamCfg.source.srcAIdx = (int32_t)obj->cfarAzimFFTCfg.numAzimFFTBins * (int32_t)DPU_DOPPLERHWADDMA_CFARIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcBIdx = (int32_t)DPU_DOPPLERHWADDMA_CFARIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcBcnt = (uint16_t)obj->cfarAzimFFTCfg.numAzimFFTBins - 1U;
        hwaParamCfg.source.srcRealComplex = DPU_DOPPLERHWADDMA_CFARIO_INPUT_ISREAL;
        hwaParamCfg.source.srcScale = 8;
        if ((DPU_DOPPLERHWADDMA_CFARIO_INPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_CFARIO_INPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_CFARIO_INPUT_ISREAL == 0U)))
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.source.srcSign = DPU_DOPPLERHWADDMA_CFARIO_INPUT_ISSIGNED;
        hwaParamCfg.source.srcConjugate = 0;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_CFAR_PING_OUT;

        hwaParamCfg.dest.dstAcnt = (uint16_t)cfg->hwRes.maxCfarPeaksToDetect - 1U;
        hwaParamCfg.dest.dstAIdx = (int32_t)DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_BYTESPERSAMPLE;
        hwaParamCfg.dest.dstBIdx = ((int32_t)obj->numDopplerBins / (int32_t)obj->dopplerDemodCfg.numBandsTotal) * (int32_t)DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_BYTESPERSAMPLE;

        hwaParamCfg.dest.dstRealComplex = DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_ISREAL;
        if ((DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_BYTESPERSAMPLE == 2U) ||
            ((DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_BYTESPERSAMPLE == 4U) &&
             (DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_ISREAL == 0U)))
        {
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
        }
        else
        {
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
        }
        hwaParamCfg.dest.dstSign = DPU_DOPPLERHWADDMA_CFARIO_OUTPUT_ISSIGNED;
        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 8;
        hwaParamCfg.dest.dstSkipInit = 0;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg,NULL);
        if (errCode != 0)
        {
            goto exit;
        }

#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
        /* enable the hookup to this paramset so that CPU gets interrupt */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
        paramISRConfig.cpu.callbackFn = DPU_DopplerProcHWA_hwaDoneIsrCallback;
        paramISRConfig.cpu.callbackArg = &obj->cfarHwaDoneSemaHandle;
        errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pingHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
#endif
    }
    /*******************************/
    /* PONG CFAR PARAMSET       */
    /*******************************/
    {
        pongHwParamsetIdx++;

        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_CFAR_PONG_IN;
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_CFAR_PONG_OUT;

        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }
#endif

    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /*******************************/
    /* PING LOCAL MAX PARAMSET     */
    /*******************************/
    {

        pingHwParamsetIdx++;

#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
        hwaParamCfg.triggerMode = HWA_TRIG_MODE_SOFTWARE;
#else
        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
#endif
        hwaParamCfg.accelMode = HWA_ACCELMODE_LOCALMAX;

        /* PREPROC CONFIG */
        hwaParamCfg.accelModeArgs.localMaxMode.neighbourBitmask = 85; /* 0 1 0 1 0 1 0 1, "+" shaped comparison */
        hwaParamCfg.accelModeArgs.localMaxMode.thresholdBitMask = 0;  /* ~ (1 1), enable comparison row wise and column wise */
        hwaParamCfg.accelModeArgs.localMaxMode.thresholdMode = 3;     /* 1 1, use Max2D internal statistics for thresholding instead of SW based thresholds */

        /* SOURCE CONFIG */
        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_LOCALMAX_PING_IN;

        hwaParamCfg.source.srcAcnt = 3U - 1U; /* Fixed for Local Max */
        hwaParamCfg.source.srcAIdx = (int32_t)obj->cfarAzimFFTCfg.numAzimFFTBins * (int32_t)DPU_DOPPLERHWADDMA_LOCALMAXIO_INPUT_BYTESPERSAMPLE;
        hwaParamCfg.source.srcBcnt = ((uint16_t)obj->cfarAzimFFTCfg.numAzimFFTBins / 4U) - 1U + 1U;
        hwaParamCfg.source.srcBIdx = 8; /* Fixed for Local Max */
        hwaParamCfg.source.srcCcnt = (obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal) - 1U;
        hwaParamCfg.source.srcCIdx = (int32_t)obj->cfarAzimFFTCfg.numAzimFFTBins * (int32_t)DPU_DOPPLERHWADDMA_LOCALMAXIO_INPUT_BYTESPERSAMPLE;
        if (((obj->cfarAzimFFTCfg.numAzimFFTBins) % 3U) == 0U) /* If numSamples % 3 == 0 */
        {
            hwaParamCfg.source.srcCircShiftWrap3 = 2U; /* 'b020, means wrap in B dim */
            hwaParamCfg.source.srcBcircShiftWrap = mathUtils_ceilLog2(((uint32_t)obj->cfarAzimFFTCfg.numAzimFFTBins / 3U) / 4U);
        }
        else
        {
            hwaParamCfg.source.srcCircShiftWrap3 = 0U;
            hwaParamCfg.source.srcBcircShiftWrap = mathUtils_ceilLog2((uint32_t)obj->cfarAzimFFTCfg.numAzimFFTBins / 4U);
        }
        hwaParamCfg.source.wrapComb = ((uint32_t)obj->numDopplerBins / (uint32_t)obj->dopplerDemodCfg.numBandsTotal) * (uint32_t)obj->cfarAzimFFTCfg.numAzimFFTBins * (uint32_t)DPU_DOPPLERHWADDMA_LOCALMAXIO_INPUT_BYTESPERSAMPLE;

        hwaParamCfg.source.srcRealComplex = 0;                 /* Fixed for Local Max */
        hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_32BIT; /* Fixed for Local Max */
        hwaParamCfg.source.srcSign = DPU_DOPPLERHWADDMA_LOCALMAXIO_INPUT_ISSIGNED;
        hwaParamCfg.source.srcConjugate = 0;
        hwaParamCfg.source.srcScale = 0;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_LOCALMAX_PING_OUT;

        /* dstAcnt = ceil(numAzimFFTBins/32) - 1
           dstBIdx = 4 * ceil(numAzimFFTBins/32) */
        if ((obj->cfarAzimFFTCfg.numAzimFFTBins % 32U) == 0U)
        {
            hwaParamCfg.dest.dstAcnt = (uint16_t)obj->cfarAzimFFTCfg.numAzimFFTBins / 32U - 1U;
            hwaParamCfg.dest.dstBIdx = 4 * (int32_t)obj->cfarAzimFFTCfg.numAzimFFTBins / 32;
        }
        else
        {
            hwaParamCfg.dest.dstAcnt = (uint16_t)obj->cfarAzimFFTCfg.numAzimFFTBins / 32U + 1U - 1U;
            hwaParamCfg.dest.dstBIdx = 4 * ((int32_t)obj->cfarAzimFFTCfg.numAzimFFTBins / 32 + 1);
        }

        hwaParamCfg.dest.dstAIdx = 4; /* Fixed for Local Max */

        hwaParamCfg.dest.dstRealComplex = DPU_DOPPLERHWADDMA_LOCALMAXIO_OUTPUT_ISREAL;
        hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT; /* Fixed for Local Max */
        hwaParamCfg.dest.dstSign = DPU_DOPPLERHWADDMA_LOCALMAXIO_OUTPUT_ISSIGNED;
        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 0;
        hwaParamCfg.dest.dstSkipInit = 0;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }

#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
        /* enable the hookup to this paramset so that CPU gets interrupt */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
        paramISRConfig.cpu.callbackFn = DPU_DopplerProcHWA_hwaDoneIsrCallback;
        paramISRConfig.cpu.callbackArg = &obj->localMaxHwaDoneSemaHandle[PING];
        errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pingHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
#endif
    }
    /*******************************/
    /* PONG LOCAL MAX PARAMSET     */
    /*******************************/
    {
        pongHwParamsetIdx++;

        hwaParamCfg.source.srcAddr = DPU_DOPPLERHWADDMA_ADDR_LOCALMAX_PONG_IN;
        hwaParamCfg.dest.dstAddr = DPU_DOPPLERHWADDMA_ADDR_LOCALMAX_PONG_OUT;

        errCode = HWA_configParamSet(obj->hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg,NULL);
        if (errCode != 0)
        {
            goto exit;
        }

#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
        /* enable the hookup to this paramset so that CPU gets interrupt */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
        paramISRConfig.cpu.callbackFn = DPU_DopplerProcHWA_hwaDoneIsrCallback;
        paramISRConfig.cpu.callbackArg = &obj->localMaxHwaDoneSemaHandle[PONG];
        errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pongHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
#endif
    }

exit:
    return(errCode);
}

/**
 *  @b Description
 *  @n
 *  Doppler DPU EDMA configuration that sends Doppler FFT In data (decompression out data)
 *  from L3 to HWA memory
 *  This implementation of doppler processing involves Ping/Pong
 *  Mechanism, hence there are two sets of EDMA transfer.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static int32_t DPU_DopplerProcHWA_configEdmaDopplerIn
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{

    DPEDMA_syncABCfg            syncABCfg;
    DPEDMA_ChainingCfg          chainingCfg;
    int32_t                     retVal;

    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    /* Program EDMA Data in from Decompressed Radar Cube scratch buffer to HWA Memory */
    /* PING */
    {{
    chainingCfg.chainingChannel               = (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaInSignature.pingPong[PING].channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = true;

    syncABCfg.srcAddress  = (uint32_t)(cfg->hwRes.decompScratchBuf);
    syncABCfg.destAddress = (uint32_t)(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PING_IN]);
    syncABCfg.aCount      = (uint16_t)cfg->staticCfg.numRxAntennas * (uint16_t)DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE;
    syncABCfg.bCount      = (uint16_t)cfg->staticCfg.numChirps;
    syncABCfg.cCount      = (uint16_t)obj->decompCfg.rangeBinsPerBlock / 2U; /* Ping and Pong */
    syncABCfg.srcBIdx     = (int32_t)cfg->staticCfg.numRxAntennas * (int32_t)obj->decompCfg.rangeBinsPerBlock * (int32_t)DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE;
    syncABCfg.dstBIdx     = (int32_t)cfg->staticCfg.numRxAntennas * (int32_t)DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE;
    syncABCfg.srcCIdx     = (int16_t)DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE * (int16_t)cfg->staticCfg.numRxAntennas * 2; /* Ping and Pong */
    syncABCfg.dstCIdx     = 0; /* One range bin in a block is processed at a time */

    retVal = DPEDMA_configSyncAB(cfg->hwRes.edmaCfg.edmaHandle,
                                    &cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaIn.pingPong[PING],
                                    &chainingCfg,
                                    &syncABCfg,
                                    false,//isEventTriggered
                                    false, //isIntermediateTransferCompletionEnabled
                                    false,//isTransferCompletionEnabled
                                    NULL, //transferCompletionCallbackFxn
                                    NULL,
                                    NULL);//transferCompletionCallbackFxnArg
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* One Hot Signature to trigger the HWA */
    retVal = DPEDMAHWA_configOneHotSignature(cfg->hwRes.edmaCfg.edmaHandle,
                                                  &cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaInSignature.pingPong[PING],
                                                  obj->hwaHandle,
                                                  obj->dopplerDemodCfg.hwaDmaTriggerSourcePingPongIn[PING],
                                                  false);
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }
    }}

    /* PONG */
    {{
    chainingCfg.chainingChannel = (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaInSignature.pingPong[PONG].channel;
    syncABCfg.srcAddress  = (uint32_t)((uint8_t *)cfg->hwRes.decompScratchBuf +
                                        (DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE * cfg->staticCfg.numRxAntennas));
    syncABCfg.destAddress = (uint32_t)(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DOPPLERFFT_PONG_IN]);
    retVal = DPEDMA_configSyncAB(cfg->hwRes.edmaCfg.edmaHandle,
                                    &cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaIn.pingPong[PONG],
                                    &chainingCfg,
                                    &syncABCfg,
                                    false,//isEventTriggered
                                    false,  //isIntermediateTransferCompletionEnabled
                                    false,//isTransferCompletionEnabled
                                    NULL, //transferCompletionCallbackFxn
                                    NULL,//transferCompletionCallbackFxnArg
                                    NULL);
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* One Hot Signature to trigger the HWA */
    retVal = DPEDMAHWA_configOneHotSignature(cfg->hwRes.edmaCfg.edmaHandle,
                                                  &cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaInSignature.pingPong[PONG],
                                                  obj->hwaHandle,
                                                  obj->dopplerDemodCfg.hwaDmaTriggerSourcePingPongIn[PONG],
                                                  false);
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    }}

exit:
    return(retVal);

}

/**
 *  @b Description
 *  @n
 *  Doppler DPU EDMA configuration that sends compressed radar cube data to HWA memory for
 *  decompression.
 *  This implementation of doppler processing involves Ping/Pong
 *  Mechanism, hence there are two sets of EDMA transfer.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static int32_t DPU_DopplerProcHWA_configEdmaDecompressionIn
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{

    DPEDMA_syncABCfg            syncABCfg;
    DPEDMA_ChainingCfg          chainingCfg;
    int32_t                     retVal;

    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    /* PING */
    chainingCfg.chainingChannel               = (uint8_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaInSignature.pingPong[PING].channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = true;

    syncABCfg.srcAddress  = (uint32_t)(obj->decompCfg.decompEdmaToHwaStartAddress);
    syncABCfg.destAddress = (uint32_t)(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PING_IN]);
    syncABCfg.aCount      = obj->decompCfg.inputBytesPerBlock * obj->decompCfg.numBlocksPerPing;
    syncABCfg.bCount      = (uint16_t)obj->decompCfg.mergedNumOuterBlocks;
    syncABCfg.cCount      = obj->decompCfg.numLoops;
    syncABCfg.srcBIdx     = (int32_t)obj->decompCfg.inputBytesPerBlock * (int32_t)obj->decompCfg.numBlocksPerPing * (int32_t)obj->decompCfg.numLoops * 2;
    syncABCfg.dstBIdx     = (int32_t)obj->decompCfg.inputBytesPerBlock * (int32_t)obj->decompCfg.numBlocksPerPing;
    syncABCfg.srcCIdx     = (int16_t)obj->decompCfg.inputBytesPerBlock * (int16_t)obj->decompCfg.numBlocksPerPing * 2;
    syncABCfg.dstCIdx     = 0;

    retVal = DPEDMA_configSyncAB(cfg->hwRes.edmaCfg.edmaHandle,
                                    &cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIn.pingPong[PING],
                                    &chainingCfg,
                                    &syncABCfg,
                                    false,//isEventTriggered
                                    false, //isIntermediateTransferCompletionEnabled
                                    false,//isTransferCompletionEnabled
                                    NULL, //transferCompletionCallbackFxn
                                    NULL, //transferCompletionCallbackFxnArg
                                    NULL); /* intrObj */
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* One Hot Signature to trigger the HWA */
    retVal = DPEDMAHWA_configOneHotSignature(cfg->hwRes.edmaCfg.edmaHandle,
                                                  &cfg->hwRes.edmaCfg.decompEdmaCfg.edmaInSignature.pingPong[PING],
                                                  obj->hwaHandle,
                                                  obj->decompCfg.hwaDmaTriggerSourcePingPongIn[PING],
                                                  false);

    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* PONG */
    chainingCfg.chainingChannel = (uint8_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaInSignature.pingPong[PONG].channel;
    syncABCfg.srcAddress  = (uint32_t)(((uint8_t *)obj->decompCfg.decompEdmaToHwaStartAddress) +
                                        (obj->decompCfg.inputBytesPerBlock * obj->decompCfg.numBlocksPerPing));
    syncABCfg.destAddress = (uint32_t)(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PONG_IN]);
    retVal = DPEDMA_configSyncAB(cfg->hwRes.edmaCfg.edmaHandle,
                                    &cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIn.pingPong[PONG],
                                    &chainingCfg,
                                    &syncABCfg,
                                    false,//isEventTriggered
                                    false, //isIntermediateTransferCompletionEnabled
                                    false,//isTransferCompletionEnabled
                                    NULL, //transferCompletionCallbackFxn
                                    NULL, //transferCompletionCallbackFxnArg
                                    NULL); /* intrObj */
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* One Hot Signature to trigger the HWA */
    retVal = DPEDMAHWA_configOneHotSignature(cfg->hwRes.edmaCfg.edmaHandle,
                                                  &cfg->hwRes.edmaCfg.decompEdmaCfg.edmaInSignature.pingPong[PONG],
                                                  obj->hwaHandle,
                                                  obj->decompCfg.hwaDmaTriggerSourcePingPongIn[PONG],
                                                  false);
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

exit:
    return(retVal);

}

/**
 *  @b Description
 *  @n
 *  Doppler DPU EDMA configuration for Decompressed data out of HWA into L3.
 *  This implementation of doppler processing involves Ping/Pong
 *  Mechanism, hence there are two sets of EDMA transfer.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static  int32_t DPU_DopplerProcHWA_configEdmaDecompressionOut
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{

    DPEDMA_syncABCfg            syncABCfg;
    DPEDMA_ChainingCfg          chainingCfg;
    int32_t                     retVal;

    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    {{

    /* PING */
    chainingCfg.chainingChannel               = (uint8_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIn.pingPong[PING].channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = false;

    syncABCfg.srcAddress  = (uint32_t)(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PING_OUT]);
    syncABCfg.destAddress = (uint32_t)(cfg->hwRes.decompScratchBuf);
    syncABCfg.aCount      = obj->decompCfg.outputBytesPerBlock * obj->decompCfg.numBlocksPerPing;
    syncABCfg.bCount      = (uint16_t)obj->decompCfg.mergedNumOuterBlocks;
    syncABCfg.cCount      = obj->decompCfg.numLoops;
    syncABCfg.srcBIdx     = (int32_t)obj->decompCfg.outputBytesPerBlock * (int32_t)obj->decompCfg.numBlocksPerPing;
    syncABCfg.dstBIdx     = (int32_t)obj->decompCfg.outputBytesPerBlock * (int32_t)obj->decompCfg.numBlocksPerPing * (int32_t)obj->decompCfg.numLoops * 2;
    syncABCfg.srcCIdx     = 0;
    syncABCfg.dstCIdx     = (int16_t)obj->decompCfg.outputBytesPerBlock * (int16_t)obj->decompCfg.numBlocksPerPing * 2;

    retVal = DPEDMA_configSyncAB(   obj->edmaHandle,
                                    &cfg->hwRes.edmaCfg.decompEdmaCfg.edmaOut.pingPong[PING],
                                    &chainingCfg,
                                    &syncABCfg,
                                    true,//isEventTriggered
                                    false, //isIntermediateTransferCompletionEnabled
                                    false,//isTransferCompletionEnabled
                                    NULL, //transferCompletionCallbackFxn
                                    NULL, //transferCompletionCallbackFxnArg
                                    NULL); /* intrObj */

    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* PONG */
    chainingCfg.chainingChannel = (uint8_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIn.pingPong[PONG].channel;
    syncABCfg.srcAddress  = (uint32_t)(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DECOMP_PONG_OUT]);
    syncABCfg.destAddress = (uint32_t)(cfg->hwRes.decompScratchBuf +
                                    (obj->decompCfg.outputBytesPerBlock * obj->decompCfg.numBlocksPerPing));
    retVal = DPEDMA_configSyncAB(   obj->edmaHandle,
                                    &cfg->hwRes.edmaCfg.decompEdmaCfg.edmaOut.pingPong[PONG],
                                    &chainingCfg,
                                    &syncABCfg,
                                    true,//isEventTriggered
                                    false, //isIntermediateTransferCompletionEnabled
                                    true,//isTransferCompletionEnabled
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                                    DPU_DopplerProcHWA_edmaDoneIsrCallback, //transferCompletionCallbackFxn
                                    (void *)((uint32_t)&obj->decompEdmaOutDoneSemaHandle), //transferCompletionCallbackFxnArg
                                    cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIntrObjDecompOut); /* intrObj */
#else
                                    NULL, //transferCompletionCallbackFxn
                                    NULL, //transferCompletionCallbackFxnArg
                                    NULL); /* intrObj */
#endif
    }}

exit:
    return(retVal);

}


/**
 *  @b Description
 *  @n
 *  EDMA Configuration to sendmax subband output out to L2 from HWA
 *  This implementation of doppler processing involves Ping/Pong
 *  Mechanism, hence there are two sets of EDMA transfer.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static  int32_t DPU_DopplerProcHWA_configEdmaMaxSubbandOut
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{

    DPEDMA_syncABCfg            syncABCfg;
    DPEDMA_ChainingCfg          chainingCfg;
    int32_t                     retVal;
#ifndef DOPPLERPROCHWADDMA_INTERRUPTS
    Edma_EventCallback doneCllbackFunc[2] = {NULL, NULL};
    uint32_t doneCllbackFuncArg[2] = {0, 0};
#endif
    bool doneTransferCompletionEnabled[2] = {true, true};

    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    {{

    /* PING */
    chainingCfg.chainingChannel               = (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PING].channel;
    chainingCfg.isIntermediateChainingEnabled = false;
    chainingCfg.isFinalChainingEnabled        = false;

    syncABCfg.srcAddress  = (uint32_t)(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_MAXSUBBAND_PING_OUT] +
                                       (DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_BYTESPERSAMPLE * obj->numDopplerBins));
    syncABCfg.destAddress = (uint32_t)(cfg->hwRes.dopMaxSubBandScratchBuf[PING]);

    /* Read only max subband indices from membank (effectively 1 byte out of 8 bytes of I-Q [MaxVal MaxIdx])*/
    syncABCfg.aCount      = 1;
    syncABCfg.bCount      = obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal;
    syncABCfg.cCount      = 1;
    syncABCfg.srcBIdx     = 8;
    syncABCfg.dstBIdx     = 1;
    syncABCfg.srcCIdx     = 1;
    syncABCfg.dstCIdx     = 1;

    retVal = DPEDMA_configSyncAB(   obj->edmaHandle,
                                    &cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PING],
                                    &chainingCfg,
                                    &syncABCfg,
                                    true,//isEventTriggered // UPON HWA COMPLETION
                                    false, //isIntermediateTransferCompletionEnabled
                                    doneTransferCompletionEnabled[PING],//isTransferCompletionEnabled
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                                    DPU_DopplerProcHWA_edmaDoneIsrCallback, //transferCompletionCallbackFxn
                                    (void *)(&obj->maxSubbandEdmaOutDoneSemaHandle[PING]), //transferCompletionCallbackFxnArg
                                    cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaIntrObjMaxSubbandOut.pingPong[PING]); /* Interrupt object */
#else
                                    doneCllbackFunc[PING], //transferCompletionCallbackFxn
                                    (void *)doneCllbackFuncArg[PING], //transferCompletionCallbackFxnArg
                                    NULL); /* Interrupt object */
#endif
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* PONG */
    chainingCfg.chainingChannel = (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PONG].channel;

    syncABCfg.srcAddress  = (uint32_t)(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_MAXSUBBAND_PONG_OUT] + 
                                       (DPU_DOPPLERHWADDMA_SUMRXIO_OUTPUT_BYTESPERSAMPLE * obj->numDopplerBins));
    syncABCfg.destAddress = (uint32_t)(cfg->hwRes.dopMaxSubBandScratchBuf[PONG]);

    retVal = DPEDMA_configSyncAB(   obj->edmaHandle,
                                    &cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PONG],
                                    &chainingCfg,
                                    &syncABCfg,
                                    true,//isEventTriggered
                                    false, //isIntermediateTransferCompletionEnabled
                                    doneTransferCompletionEnabled[PONG],//isTransferCompletionEnabled
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                                    DPU_DopplerProcHWA_edmaDoneIsrCallback, //transferCompletionCallbackFxn
                                    (void *)(&obj->maxSubbandEdmaOutDoneSemaHandle[PONG]), //transferCompletionCallbackFxnArg
                                    cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaIntrObjMaxSubbandOut.pingPong[PONG]); /* Interrupt object */
#else
                                    doneCllbackFunc[PONG], //transferCompletionCallbackFxn
                                    (void *)doneCllbackFuncArg[PONG], //transferCompletionCallbackFxnArg
                                    NULL); /* Interrupt object */
#endif
    }}

exit:
    return(retVal);

}

/**
 *  @b Description
 *  @n
 *  Doppler DPU EDMA configuration to transfer sumTx data from HWA Memory to L3.
 *  This implementation of doppler processing involves Ping/Pong
 *  Mechanism, hence there are two sets of EDMA transfer.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static  int32_t DPU_DopplerProcHWA_configEdmaDopplerFFTSumTxOut
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{

    DPEDMA_syncACfg            syncACfg;
    DPEDMA_ChainingCfg          chainingCfg;
    int32_t                     retVal;
    Edma_EventCallback doneCllbackFunc[2] = {NULL, NULL};
    uint32_t doneCllbackFuncArg[2] = {0, 0};

    /* disable the interrupt for this channel if chaining is enabled. Because, monitoring the completion of chained channel is sufficient. */
    bool doneTransferCompletionEnabled[2] = {false, false};

    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }


    {{

    /* PING */
    /* Chained to EDMA transfer of max subabnd output. */
    chainingCfg.chainingChannel               = (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PING].channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = true;

    syncACfg.srcAddress  = (obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PING_OUT]
                                    + (DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE * obj->numDopplerBins));
    syncACfg.destAddress = (uint32_t)(cfg->hwRes.detMatrix.data);
    syncACfg.aCount      = (uint16_t)DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_BYTESPERSAMPLE
                            * (uint16_t)obj->numDopplerBins / (uint16_t)obj->dopplerDemodCfg.numBandsTotal;
    syncACfg.bCount      = obj->decompCfg.rangeBinsPerBlock / 2U;
    syncACfg.cCount      = (uint16_t)obj->decompCfg.numOuterBlocks * (uint16_t)obj->decompCfg.mergedNumOuterBlocks;
    syncACfg.srcBIdx     = 0;
    syncACfg.dstBIdx     = (int32_t)DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_BYTESPERSAMPLE
                            * ((int32_t)obj->numDopplerBins / (int32_t)obj->dopplerDemodCfg.numBandsTotal)
                            * 2;
    syncACfg.srcCIdx     = 0;
    syncACfg.dstCIdx     = (int16_t)DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_BYTESPERSAMPLE
                            * ((int16_t)obj->numDopplerBins / (int16_t)obj->dopplerDemodCfg.numBandsTotal)
                            * 2;

    retVal = DPEDMA_configSyncA(   obj->edmaHandle,
                                    &cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaSumLogAbsOut.pingPong[PING],
                                    &chainingCfg,
                                    &syncACfg,
                                    true,//isEventTriggered // UPON HWA COMPLETION
                                    doneTransferCompletionEnabled[PING], //isIntermediateTransferCompletionEnabled
                                    doneTransferCompletionEnabled[PING],//isTransferCompletionEnabled
                                    doneCllbackFunc[PING], //transferCompletionCallbackFxn
                                    (void *)doneCllbackFuncArg[PING], //transferCompletionCallbackFxnArg
                                    NULL);

    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* PONG */
    chainingCfg.chainingChannel = (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PONG].channel;

    syncACfg.srcAddress  = (uint32_t)(obj->hwaMemBankAddr[DPU_DOPPLERHWADDMA_MEM_BANK_DDMAMETRIC_PONG_OUT]
                                     + (DPU_DOPPLERHWADDMA_DDMAMETRICIO_OUTPUT_BYTESPERSAMPLE * obj->numDopplerBins));
    syncACfg.destAddress = (uint32_t)((uint8_t *)cfg->hwRes.detMatrix.data
                                     + (DPU_DOPPLERHWADDMA_SUMTXIO_OUTPUT_BYTESPERSAMPLE
                                         * obj->numDopplerBins / obj->dopplerDemodCfg.numBandsTotal));
    retVal = DPEDMA_configSyncA(   obj->edmaHandle,
                                    &cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaSumLogAbsOut.pingPong[PONG],
                                    &chainingCfg,
                                    &syncACfg,
                                    true,//isEventTriggered
                                    doneTransferCompletionEnabled[PONG], //isIntermediateTransferCompletionEnabled
                                    doneTransferCompletionEnabled[PONG],//isTransferCompletionEnabled
                                    doneCllbackFunc[PONG], //transferCompletionCallbackFxn
                                    (void *)doneCllbackFuncArg[PONG], //transferCompletionCallbackFxnArg
                                    NULL);  /* Interrupt object */

    }}

exit:
    return(retVal);

}

/**
 *  @b Description
 *  @n
 *  EDMA Configuration to copy deteected objects to L3.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static  int32_t DPU_DopplerProcHWA_configEdmaDetObjs
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{

    DPEDMA_syncACfg            syncACfg;
    DPEDMA_ChainingCfg          chainingCfg;
    int32_t                     retVal;
    uint16_t                    numBytesPerRDBin;

    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }
    numBytesPerRDBin = (uint16_t)DPU_DOPPLERHWADDMA_DOPPLERIO_OUTPUT_BYTESPERSAMPLE * (uint16_t)cfg->staticCfg.numVirtualAntennas;

    /* PING */
    chainingCfg.chainingChannel               = (uint8_t)cfg->hwRes.edmaCfg.edmaDetObjAntSamples.channel;
    chainingCfg.isIntermediateChainingEnabled = false;
    chainingCfg.isFinalChainingEnabled        = false;

    syncACfg.srcAddress  = 0; // doesn't matter, will be modified
    syncACfg.destAddress = (uint32_t)(cfg->hwRes.detObjList);
    syncACfg.aCount      = numBytesPerRDBin;
    syncACfg.bCount      = 1;
    syncACfg.cCount      = 1;
    syncACfg.srcBIdx     = (int32_t)numBytesPerRDBin; // doesn't matter
    syncACfg.dstBIdx     = (int32_t)numBytesPerRDBin; // doesn't matter
    syncACfg.srcCIdx     = (int16_t)numBytesPerRDBin; // doesn't matter
    syncACfg.dstCIdx     = (int16_t)numBytesPerRDBin; // doesn't matter

    retVal = DPEDMA_configSyncA_singleFrame( obj->edmaHandle,
                                    &cfg->hwRes.edmaCfg.edmaDetObjAntSamples,
                                    &chainingCfg,
                                    &syncACfg,
                                    false,
                                    false, //isIntermediateTransferCompletionEnabled
                                    true,//isTransferCompletionEnabled
                                    NULL, //transferCompletionCallbackFxn
                                    NULL, //transferCompletionCallbackFxnArg
                                    NULL); /* intrObj */
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }
exit:
    return(retVal);
}


/*===========================================================
 *                    Doppler Proc External APIs
 *===========================================================*/

/**
 *  @b Description
 *  @n
 *      dopplerProc DPU init function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]   initCfg Pointer to initial configuration parameters
 *  @param[in]   subframeCounter subFrame index for dpu initialization
 *  @param[out]  errCode Pointer to errCode generates by the API
 *
 *  \ingroup    DPU_DOPPLERPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid handle
 *  @retval
 *      Error       - NULL
 */
DPU_DopplerProcHWA_Handle DPU_DopplerProcHWA_init
(
    DPU_DopplerProcHWA_InitParams *initCfg,
    volatile uint8_t       subframeCounter,
    int32_t                       *errCode
)
{
    DPU_DopplerProcHWA_Obj  *obj = NULL;
    HWA_MemInfo             hwaMemInfo;
    uint32_t                index;
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
    int32_t             status = SystemP_SUCCESS;
#endif

    *errCode       = 0;

    if((initCfg == NULL) || (initCfg->hwaHandle == NULL))
    {
        *errCode = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    DebugP_assert(subframeCounter < RL_MAX_SUBFRAMES);

    obj = (DPU_DopplerProcHWA_Obj *)&dopplerProcObjPool;

    /* Initialize memory */
    (void)memset((void *)obj, 0, sizeof(DPU_DopplerProcHWA_Obj));

    /* Save init config params */
    obj->hwaHandle   = initCfg->hwaHandle;

    /* Creating semaphores */
    {{

#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
    /* Create semaphore for HWA decompression done */
    status = SemaphoreP_constructBinary(&obj->decompEdmaOutDoneSemaHandle, 0);
    if(status != SystemP_SUCCESS)
    {
        *errCode = DPU_DOPPLERPROCHWA_ESEMA;
        goto exit;
    }
    /* Create semaphore for HWA Demodulation done */
    status = SemaphoreP_constructBinary(&obj->demodHwaDoneSemaHandle, 0);
    if(status != SystemP_SUCCESS)
    {
        *errCode = DPU_DOPPLERPROCHWA_ESEMA;
        goto exit;
    }
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    /* Create semaphore for HWA CFAR done */
    status = SemaphoreP_constructBinary(&obj->cfarHwaDoneSemaHandle, 0);
    if(status != SystemP_SUCCESS)
    {
        *errCode = DPU_DOPPLERPROCHWA_ESEMA;
        goto exit;
    }
#endif

    for(index = 0U; index < 2U; index++)
    {
        /* Create semaphore for HWA DDMA Metric done */
        status = SemaphoreP_constructBinary(&obj->maxSubbandEdmaOutDoneSemaHandle[index], 0);
        if(status != SystemP_SUCCESS)
        {
            *errCode = DPU_DOPPLERPROCHWA_ESEMA;
            goto exit;
        }


#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
        /* Create semaphore for HWA azimuth FFT done */
        status = SemaphoreP_constructBinary(&obj->azimFFTHwaDoneSemaHandle[index], 0);
        if(status != SystemP_SUCCESS)
        {
            *errCode = DPU_DOPPLERPROCHWA_ESEMA;
            goto exit;
        }
#endif

        /* Create semaphore for HWA local max done */
        status = SemaphoreP_constructBinary(&obj->localMaxHwaDoneSemaHandle[index], 0);
        if(status != SystemP_SUCCESS)
        {
            *errCode = DPU_DOPPLERPROCHWA_ESEMA;
            goto exit;
        }
    }
#endif
    }}

    /* Populate HWA base addresses and offsets. This is done only once, at init time.*/
    *errCode =  HWA_getHWAMemInfo(obj->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {
        goto exit;
    }

    /* check if we have enough memory banks*/
    if(hwaMemInfo.numBanks < DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS)
    {
        *errCode = DPU_DOPPLERPROCHWA_EHWARES;
        goto exit;
    }

    for (index = 0U; index < DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS; index++)
    {
        obj->hwaMemBankAddr[index] = hwaMemInfo.baseAddress + (index * hwaMemInfo.bankSize);
    }

exit:
    if(*errCode < 0)
    {
        if(obj != NULL)
        {
            obj = NULL;
        }
    }
   return ((DPU_DopplerProcHWA_Handle)obj);
}

/**
  *  @b Description
  *  @n
  *   Doppler DPU configuration
  *
  *  @param[in]   handle     DPU handle.
  *  @param[in]   cfg        Pointer to configuration parameters.
  *  @param[in]   isFullCfg  Perform a full DPU Config (as opposed to a minimal one).
  *
  *  \ingroup    DPU_DOPPLERPROC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success      = 0
  *  @retval
  *      Error       != 0 @ref DPU_DOPPLERPROC_ERROR_CODE
  */
int32_t DPU_DopplerProcHWA_config
(
    DPU_DopplerProcHWA_Handle    handle,
    DPU_DopplerProcHWA_Config    *cfg,
	int32_t isFullCfg
)
{
    DPU_DopplerProcHWA_Obj   *obj;
    int32_t                  retVal = 0;
    float                    temp;

    obj = (DPU_DopplerProcHWA_Obj *)handle;
    if((obj == NULL) || (cfg==NULL))
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

	if (isFullCfg == 1)
	{
		obj->edmaHandle  = cfg->hwRes.edmaCfg.edmaHandle;

		obj->numDopplerChirps = cfg->staticCfg.numChirps;
		obj->numDopplerBins = (uint16_t)mathUtils_getValidFFTSize(cfg->staticCfg.numChirps);

		/* Decompression params */
		{{
	    obj->decompCfg.isEnabled = cfg->staticCfg.decompCfg.isEnabled;

	    obj->decompCfg.compressionMethod = cfg->staticCfg.decompCfg.compressionMethod;
	    obj->decompCfg.bytesPerSample = (uint16_t)sizeof(cmplx16ImRe_t);
	    obj->decompCfg.rxAntPerBlock = cfg->staticCfg.decompCfg.numRxAntennaPerBlock;
	    obj->decompCfg.rangeBinsPerBlock = cfg->staticCfg.decompCfg.rangeBinsPerBlock;
        if(obj->decompCfg.compressionMethod == HWA_COMPRESS_METHOD_BFP)
        {
            obj->decompCfg.outputSamplesPerBlock = cfg->staticCfg.decompCfg.rangeBinsPerBlock;
        }
        else
        {
            obj->decompCfg.outputSamplesPerBlock = obj->decompCfg.rxAntPerBlock * cfg->staticCfg.decompCfg.rangeBinsPerBlock;
        }
	    obj->decompCfg.outputBytesPerBlock = obj->decompCfg.outputSamplesPerBlock * obj->decompCfg.bytesPerSample;
	    obj->decompCfg.numBlocks = cfg->staticCfg.numRangeBins * cfg->staticCfg.numRxAntennas / obj->decompCfg.outputSamplesPerBlock;
        temp = (((cfg->staticCfg.decompCfg.compressionRatio * (float)obj->decompCfg.outputBytesPerBlock) + 3.99F) / 4.0F);
	    obj->decompCfg.inputBytesPerBlock = (uint16_t)temp * 4U; /* Word aligned */
	    obj->decompCfg.inputSamplesPerBlock = obj->decompCfg.inputBytesPerBlock / obj->decompCfg.bytesPerSample;
	    obj->decompCfg.achievedCompressionRatio = (float) obj->decompCfg.inputBytesPerBlock / (float)obj->decompCfg.outputBytesPerBlock;
	    obj->decompCfg.decompEdmaToHwaStartAddress = (void *)cfg->hwRes.radarCube.data;
        if (obj->decompCfg.rangeBinsPerBlock < 8U) 
        {
            obj->decompCfg.mergedNumOuterBlocks = 8U / (uint32_t)obj->decompCfg.rangeBinsPerBlock;
        }
        else 
        {
            obj->decompCfg.mergedNumOuterBlocks = 1U;
        }

        if(obj->decompCfg.compressionMethod == HWA_COMPRESS_METHOD_BFP)
        {
            obj->decompCfg.numChirpsPerPing = DECOMP_HWA_MEMBANK_SIZE / ((uint16_t)obj->decompCfg.outputBytesPerBlock * 
                                                                            (uint16_t)obj->decompCfg.rxAntPerBlock * (uint16_t)obj->decompCfg.mergedNumOuterBlocks);

            /* dstCIdx of DPU_DopplerProcHWA_configEdmaDecompressionOut must be less that 32768 */
            if((obj->decompCfg.numChirpsPerPing * obj->decompCfg.rxAntPerBlock * obj->decompCfg.outputBytesPerBlock * 2U) >= 32768U)
            {
                obj->decompCfg.numChirpsPerPing = obj->decompCfg.numChirpsPerPing / 2U;
            }
            obj->decompCfg.outerBlockSizeCompressed = (uint32_t)obj->decompCfg.inputBytesPerBlock * (uint32_t)obj->decompCfg.rxAntPerBlock * 
                                                      (uint32_t)cfg->staticCfg.numChirps * (uint32_t)obj->decompCfg.mergedNumOuterBlocks;
        }
        else
        {
            obj->decompCfg.numChirpsPerPing = DECOMP_HWA_MEMBANK_SIZE / ((uint16_t)obj->decompCfg.outputBytesPerBlock 
                                                                            * (uint16_t)obj->decompCfg.mergedNumOuterBlocks);

            /* dstCIdx of DPU_DopplerProcHWA_configEdmaDecompressionOut must be less that 32768 */
            if((obj->decompCfg.numChirpsPerPing * obj->decompCfg.outputBytesPerBlock * 2U) >= 32768U)
            {
                obj->decompCfg.numChirpsPerPing = obj->decompCfg.numChirpsPerPing / 2U;
            }
            obj->decompCfg.outerBlockSizeCompressed = (uint32_t)obj->decompCfg.inputBytesPerBlock * (uint32_t)cfg->staticCfg.numChirps * 
                                                      (uint32_t)obj->decompCfg.mergedNumOuterBlocks;
        }

	    obj->decompCfg.numOuterBlocks = (cfg->staticCfg.numRangeBins / (cfg->staticCfg.decompCfg.rangeBinsPerBlock * obj->decompCfg.mergedNumOuterBlocks)) ;

	    /* numLoops is ceil(numChirps/numChirpsPerPing)/2 */
	    if((cfg->staticCfg.numChirps % obj->decompCfg.numChirpsPerPing) == 0U)
        {
	        obj->decompCfg.numLoops = (cfg->staticCfg.numChirps / obj->decompCfg.numChirpsPerPing);
	        if((obj->decompCfg.numLoops % 2U) == 0U)
            {
	            obj->decompCfg.numLoops = obj->decompCfg.numLoops / 2U;
	        }
	    }
	    else
        {
	        obj->decompCfg.numLoops = (cfg->staticCfg.numChirps / obj->decompCfg.numChirpsPerPing) + 1U;
	        if((obj->decompCfg.numLoops % 2U) == 0U)
            {
	            obj->decompCfg.numLoops = obj->decompCfg.numLoops / 2U;
	        }
	    }
	    if(obj->decompCfg.numLoops < 1U)
        {
	        obj->decompCfg.numLoops = 2U;
	    }

	    obj->decompCfg.numChirpsPerPing = cfg->staticCfg.numChirps / (obj->decompCfg.numLoops * 2U);
	    if(obj->decompCfg.numChirpsPerPing < 1U)
        {
	        retVal = DPU_DOPPLERPROCHWA_ERROR_NUMCHIRPSPERPING;
	        goto exit;
	    }

        if(obj->decompCfg.compressionMethod == HWA_COMPRESS_METHOD_BFP)
        {
            obj->decompCfg.numBlocksPerPing = obj->decompCfg.numChirpsPerPing*obj->decompCfg.rxAntPerBlock;
        }
        else
        {
            obj->decompCfg.numBlocksPerPing = obj->decompCfg.numChirpsPerPing;
        }

	    if(obj->decompCfg.rxAntPerBlock != cfg->staticCfg.numRxAntennas)
        {
	        retVal = DPU_DOPPLERPROCHWA_ERROR_NUMRXANTPERBLOCK_DECOMPRESSION;
	        goto exit;
	    }

	    /* RangeBinsPerBlock should be a power of 2 and greater > 1 */
	    if((obj->decompCfg.rangeBinsPerBlock <= 1U) || ((obj->decompCfg.rangeBinsPerBlock & (obj->decompCfg.rangeBinsPerBlock - 1U)) != 0U))
        {
	        retVal = DPU_DOPPLERPROCHWA_ERROR_RANGEBINSPERBLOCK_DECOMPRESSION;
	        goto exit;
	    }

	    if((obj->decompCfg.compressionMethod != HWA_COMPRESS_METHOD_EGE)&&(obj->decompCfg.compressionMethod != HWA_COMPRESS_METHOD_BFP))
        {
	        retVal = DPU_DOPPLERPROCHWA_ERROR_METHOD_DECOMPRESSION;
	        goto exit;
	    }


	    obj->decompCfg.hwaDmaTriggerSourcePingPongIn[PING] = (uint8_t)cfg->hwRes.hwaCfg.decompStageHwaStateMachineCfg.paramSetStartIdx + DECOMP_PING_HWA_PARAMSET_RELATIVE_IDX;
	    obj->decompCfg.hwaDmaTriggerSourcePingPongIn[PONG] = (uint8_t)cfg->hwRes.hwaCfg.decompStageHwaStateMachineCfg.paramSetStartIdx + DECOMP_PONG_HWA_PARAMSET_RELATIVE_IDX + 
                                                             ((uint8_t)cfg->staticCfg.decompCfg.bfpCompExtraParamSets / 2U);

	    retVal = DPU_DopplerProcHWA_configHwaDecompression(obj, cfg);
	    if (retVal != 0)
	    {
	        goto exit;
	    }

	    retVal = DPU_DopplerProcHWA_configEdmaDecompressionIn(obj, cfg);
	    if (retVal != 0)
	    {
	        goto exit;
	    }

	    retVal = DPU_DopplerProcHWA_configEdmaDecompressionOut(obj, cfg);
	    if (retVal != 0)
	    {
	        goto exit;
	    }

	    }}

	    /* Doppler demod params */
	    {{
	    obj->dopplerDemodCfg.numBandsActive = cfg->staticCfg.numTxAntennas;

	    /* Empty subbands */
	    switch (cfg->staticCfg.numTxAntennas)
	    {
	        case 2:
	            obj->dopplerDemodCfg.numBandsEmpty = 1;
	            break;
	        case 3:
	            obj->dopplerDemodCfg.numBandsEmpty = 1;
	            break;
	        case 4:
	            obj->dopplerDemodCfg.numBandsEmpty = 2;
	            break;
	        default:
	            retVal = DPU_DOPPLERPROCHWA_EINVAL;
                break;
        }
        if(retVal != 0)
        {
            goto exit;
        }

	    obj->dopplerDemodCfg.numBandsTotal = obj->dopplerDemodCfg.numBandsActive + obj->dopplerDemodCfg.numBandsEmpty;
	    if (obj->dopplerDemodCfg.numBandsTotal != cfg->staticCfg.numBandsTotal)
        {
	        retVal = DPU_DOPPLERPROCHWA_EINVAL;
	        goto exit;
	    }

	    obj->dopplerDemodCfg.hwaDmaTriggerSourcePingPongIn[PING] = (uint8_t)cfg->hwRes.hwaCfg.dopplerStageHwaStateMachineCfg.paramSetStartIdx;
	    if(cfg->staticCfg.isSumTxEnabled == 1U)
        {
	        obj->dopplerDemodCfg.hwaDmaTriggerSourcePingPongIn[PONG] = (uint8_t)cfg->hwRes.hwaCfg.dopplerStageHwaStateMachineCfg.paramSetStartIdx +
                                                                       (DOPPLERPROCHWA_DDMA_DOPPLER_NUM_HWA_PARAMSETS / 2U);
	    }
	    else
        {
	        obj->dopplerDemodCfg.hwaDmaTriggerSourcePingPongIn[PONG] = (uint8_t)cfg->hwRes.hwaCfg.dopplerStageHwaStateMachineCfg.paramSetStartIdx + 
                                                                       (DOPPLERPROCHWA_DDMA_DOPPLER_NUM_HWA_PARAMSETS / 2U) - 1U;
	    }

	    retVal = DPU_DopplerProcHWA_configHwaDopplerFFTDDMADemod(obj, cfg);
	    if (retVal != 0)
	    {
	        goto exit;
	    }

	    retVal = DPU_DopplerProcHWA_configEdmaDopplerIn(obj, cfg);
	    if (retVal != 0)
	    {
	        goto exit;
	    }

	    retVal = DPU_DopplerProcHWA_configEdmaMaxSubbandOut(obj, cfg);
	    if (retVal != 0)
	    {
	        goto exit;
	    }

	    if(cfg->staticCfg.isSumTxEnabled == 1U)
        {
	        retVal = DPU_DopplerProcHWA_configEdmaDopplerFFTSumTxOut(obj, cfg);
	        if (retVal != 0)
	        {
	            goto exit;
	        }
	    }
	    }}

	    /* Azimuth FFT - CFAR params */
	    {{

	    obj->cfarAzimFFTCfg.numAzimFFTBins = cfg->staticCfg.numAzimFFTBins;

#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
	    obj->cfarAzimFFTCfg.cfarCfg = (dopplerProcHWADDMAcfarCfg *)&cfg->staticCfg.cfarCfg;
        if(obj->cfarAzimFFTCfg.cfarCfg->averageMode == HWA_NOISE_AVG_MODE_CFAR_OS)
        {
            /* Guard Window is not required in case of CFAR OS */
            if(obj->cfarAzimFFTCfg.cfarCfg->guardLen != 0U)
            {
                retVal = DPU_DOPPLERPROCHWA_ERROR_METHOD_CFAR;
                goto exit;
            }
	    }

        /* Only CFAR_OS and CFAR_CASO are supported in SDK */
        if((obj->cfarAzimFFTCfg.cfarCfg->averageMode != HWA_NOISE_AVG_MODE_CFAR_OS)
            && (obj->cfarAzimFFTCfg.cfarCfg->averageMode != HWA_NOISE_AVG_MODE_CFAR_CASO))
        {
            retVal = DPU_DOPPLERPROCHWA_ERROR_METHOD_CFAR;
            goto exit;
        }
#endif

	    obj->cfarAzimFFTCfg.localMaxCfg.azimThreshold = cfg->staticCfg.localMaxCfg.azimThreshold;
	    obj->cfarAzimFFTCfg.localMaxCfg.dopplerThreshold = cfg->staticCfg.localMaxCfg.dopplerThreshold;

	    /* Evaluate the float calb params into quantized values acceptable to HWA */
	    retVal = mathUtils_asymQuantInt(cfg->staticCfg.antennaCalibParams,
	                                    (void *)obj->cfarAzimFFTCfg.antennaCalibParamsQuantized,
	                                    (uint32_t)cfg->staticCfg.numTxAntennas * (uint32_t)cfg->staticCfg.numRxAntennas * 2U,
	                                    1,
	                                    20,
	                                    1); /* Signed */
	    if (retVal != 0)
	    {
	        goto exit;
	    }

	    /* Configure HWA paramsets */
	    retVal = DPU_DopplerProcHWA_configHwaCFARAzimFFT(obj, cfg);
	    if (retVal != 0)
	    {
	        goto exit;
	    }
        
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
        /* Configure CFAR Thresh Scale LUT for range gate based thresholds */
        DPU_DopplerProcHWA_configCfarThreshScaleLUT(cfg);
#endif

        /* Configure EDMA to copy doppler CFAR and local max intersected objects. */
	    retVal = DPU_DopplerProcHWA_configEdmaDetObjs(obj, cfg);
	    if (retVal != 0)
	    {
	        goto exit;
	    }

	    }}

        /* Configure LUT RAM for Zero insertion. */
        retVal = HWA_configRam(obj->hwaHandle, HWA_RAM_TYPE_LUT_FREQ_DEROTATE_RAM, (uint8_t *)&cfg->staticCfg.zeroInsrtMaskAzim, sizeof(uint64_t), 0);
        if (retVal != 0)
        {
            goto exit;
        }

	    /* Populate Shuffle LUT RAM in HWA */
        uint32_t shuffleRamOffset = (((uint32_t)obj->numDopplerBins + ((uint32_t)obj->dopplerDemodCfg.numBandsTotal * 16U) - 1U)
                                        / ((uint32_t)obj->dopplerDemodCfg.numBandsTotal * 16U)) * 16U * (uint32_t)sizeof(uint16_t);
	    retVal = HWA_configRam(obj->hwaHandle, HWA_RAM_TYPE_SHUFFLE_RAM, (uint8_t *)&cfg->staticCfg.antennaGeometryCfg[0], \
                                                    sizeof(uint16_t) * NUM_TXANT_AZIM * NUM_RXANT, shuffleRamOffset);
	    if (retVal != 0)
	    {
	        goto exit;
	    }

	    /* Validate params */
	    if((cfg->hwRes.edmaCfg.edmaHandle == NULL) ||
	       (cfg->hwRes.hwaCfg.window == NULL) ||
	       (cfg->hwRes.radarCube.data == NULL)
	      )
	    {
	        retVal = DPU_DOPPLERPROCHWA_EINVAL;
	        goto exit;
	    }

		if (cfg->staticCfg.isSumTxEnabled == 1U)
		{
            if(cfg->hwRes.detMatrix.data == NULL)
            {
                retVal = DPU_DOPPLERPROCHWA_EINVAL;
                goto exit;
	        }
            /* Check if detection matrix size is sufficient*/
		    if(cfg->hwRes.detMatrix.dataSize < (cfg->staticCfg.numRangeBins *
		                                        (obj->numDopplerBins/cfg->staticCfg.numBandsTotal) * (uint16_t)sizeof(uint16_t)))
		    {
		        retVal = DPU_DOPPLERPROCHWA_EDETMSIZE;
		        goto exit;
		    }
		}

	    /* Even though Window RAM is not used by the first stage, there's no issue
	    in programming it at this point itself */
	    /* HWA window configuration */
	    retVal = HWA_configRam(obj->hwaHandle,
	                           HWA_RAM_TYPE_WINDOW_RAM,
	                           (uint8_t *)cfg->hwRes.hwaCfg.window,
	                           cfg->hwRes.hwaCfg.windowSize, //size in bytes
	                           cfg->hwRes.hwaCfg.winRamOffset * sizeof(int32_t));
	    if (retVal != 0)
	    {
	        goto exit;
	    }
	}


exit:
    return retVal;
}


#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
/**
  *  @b Description
  *  @n
  *   Local Max Threhold configuration
  *
  *  @param[in]   ctrlBaseAddr    HWA Static Config Base Address.
  *  @param[in]   numAzimFFTBins  Number of Azimuth FFT Bins.
  *
  *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
  *
  *  @retval    None
  */
static inline void DPU_DopplerProcHwa_configLocalMaxThresh(DSSHWACCRegs * restrict ctrlBaseAddr, uint16_t numAzimFFTBins)
{
    uint32_t * restrict CDFThresholdRam = ( uint32_t *)HWA_HIST_THRESH_RAM_U_BASE;
    uint32_t * restrict maxValRam = ( uint32_t *)HWA_2DSTAT_SMPL_VAL_RAM;
    uint16_t * restrict histRam = ( uint16_t *)HWA_HIST_RAM_U_BASE;

    uint8_t azimBin;
    uint32_t cdfBin, CDFThreshRamVal;
    uint32_t histVal = 0;
    uint32_t histRamOffset = 1U << HIST_SIZE_SELECT;
    uint32_t den;
    uint32_t num;
    uint16_t histVal1;
#ifdef SUBSYS_DSS
    float interpolatedCDFBin;
    uint32_t histScale = 1U << HIST_SCALE_SELECT;
#elif defined SUBSYS_M4
    uint32_t interpolatedCDFBin;
#endif

    /* Read order is different in case of 48 histograms */
    if( numAzimFFTBins == 48U)
    {
        /* Read CDF result. */
        for(azimBin= 0; azimBin < 24; azimBin++)
        {
            CDFThreshRamVal = *CDFThresholdRam;
            /* CDF Bin Number Bits: 29:24 */
            cdfBin = CDFThreshRamVal >> 24U;
            /* Hist Val Bits: 11:0 */
            histVal = CDFThreshRamVal & 0xFFFU;
            /* Histogram Value at the next bin */
            histVal1 = *(histRam + cdfBin + 1U);

            /* Linear interpolation of the CDF Bin
             * interpolatedCDFBin = ((cdfBin * histVal) + ((cdfBin+1) * histVal1)) /(histVal + histVal1) */
            den = histVal + histVal1;
#ifdef SUBSYS_DSS
            num = ((cdfBin * den) + histVal1);
            interpolatedCDFBin = divsp(num, den);
            *((uint32_t *)maxValRam) = (uint32_t)(interpolatedCDFBin * histScale);
#elif defined(SUBSYS_M4)
            num = ((cdfBin * den) + histVal1) << HIST_SCALE_SELECT;
            interpolatedCDFBin = num / den;
            *((uint32_t *)maxValRam) = interpolatedCDFBin;
#endif
            CDFThresholdRam++;
            maxValRam++;
            histRam += histRamOffset;
        }

        CDFThresholdRam = ( uint32_t *)HWA_HIST_THRESH_RAM_U_BASE;
        CDFThresholdRam += 32U;

        histRam = ( uint16_t *)HWA_HIST_RAM_U_BASE;
        histRam += (32U << HIST_SIZE_SELECT);

        for(azimBin= 0; azimBin < 24U; azimBin++)
        {
            CDFThreshRamVal = *CDFThresholdRam;
            /* CDF Bin Number Bits: 29:24 */
            cdfBin = CDFThreshRamVal >> 24U;
            /* Hist Val Bits: 11:0 */
            histVal = CDFThreshRamVal & 0xFFFU;
            /* Histogram Value at the next bin */
            histVal1 = *(histRam + cdfBin + 1U);

            /* Linear interpolation of the CDF Bin
             * interpolatedCDFBin = ((cdfBin * histVal) + ((cdfBin+1) * histVal1)) /(histVal + histVal1) */
            den = histVal + histVal1;
#ifdef SUBSYS_DSS
            num = ((cdfBin * den) + histVal1);
            interpolatedCDFBin = divsp(num, den);
            *((uint32_t *)maxValRam) = (uint32_t)(interpolatedCDFBin * histScale);
#elif defined(SUBSYS_M4)
            num = ((cdfBin * den) + histVal1) << HIST_SCALE_SELECT;
            interpolatedCDFBin = num / den;
            *((uint32_t *)maxValRam) = interpolatedCDFBin;
#endif
            CDFThresholdRam++;
            maxValRam++;
            histRam += histRamOffset;
        }
    }
    else
    {
        for(azimBin= 0; azimBin < numAzimFFTBins; azimBin++)
        {
            CDFThreshRamVal = *CDFThresholdRam;
            /* CDF Bin Number Bits: 29:24 */
            cdfBin = CDFThreshRamVal >> 24U;
            /* Hist Val Bits: 11:0 */
            histVal = CDFThreshRamVal & 0xFFFU;
            /* Histogram Value at the next bin */
            histVal1 = *(histRam + cdfBin + 1U);

            /* Linear interpolation of the CDF Bin
             * interpolatedCDFBin = ((cdfBin * histVal) + ((cdfBin+1) * histVal1)) /(histVal + histVal1) */
            den = histVal + histVal1;
#ifdef SUBSYS_DSS
            num = ((cdfBin * den) + histVal1);
            interpolatedCDFBin = divsp(num, den);
            *((uint32_t *)maxValRam) = (uint32_t)(interpolatedCDFBin * histScale);
#elif defined(SUBSYS_M4)
            num = ((cdfBin * den) + histVal1) << HIST_SCALE_SELECT;
            interpolatedCDFBin = num / den;
            *((uint32_t *)maxValRam) = interpolatedCDFBin;
#endif
            CDFThresholdRam++;
            maxValRam++;
            histRam += histRamOffset;
        }

    }
}
#endif

/**
 *  @b Description
 *  @n
 *      Internal function to config HWA common
 *
 *  @param[in]   obj        DPU handle.
 *  @param[in]   cfg        DPU config.
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static inline int32_t DPU_DopplerProcHWA_configHWACommon
(
    DPU_DopplerProcHWA_Obj *obj,
    DPU_DopplerProcHWA_Config    *cfg
)
{
    int32_t retVal;
    uint8_t zerosCounter;
    uint8_t zeroInsertConfigNumber;
    uint32_t idx;
    uint8_t EGEKparam[8] = {3, 4, 5, 7, 9, 11, 13, 15};
    uint32_t zeroInsertConfigMask[HWA_ZEROINSERT_LENGTH_INWORDS];

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    retVal = HWA_initConfig(obj->hwaHandle);
    if (retVal != 0)
    {
        goto exit;
    }
#if defined(DATAPATH_TEST) || defined(OBJ_DETECTION_DDMA_TEST)
    retVal = HWA_configTwidDitherEnable(HWA_FEATURE_BIT_DISABLE); // Disable dither for datapath bit-exact test
#else
    retVal = HWA_configTwidDitherEnable(HWA_FEATURE_BIT_ENABLE); // Enable Dither for FFT twiddle factor to attenuate quantization spurs.
#endif
    if (retVal != 0)
    {
        goto exit;
    }
    if (obj->decompCfg.compressionMethod == HWA_COMPRESS_METHOD_EGE)
    {
        retVal = HWA_configEGECompressKParams(EGEKparam);
        if (retVal != 0)
        {
            goto exit;
        }
    }
    retVal = HWA_configFFTLFSRSeed(0x0B); /*Some non-zero value*/
    if (retVal != 0)
    {
        goto exit;
    }
    retVal = HWA_configCmpLFSRSeed0(0x0000000B);
    if (retVal != 0)
    {
        goto exit;
    }
    retVal = HWA_configFFTSumDiv(mathUtils_ceilLog2(cfg->staticCfg.numRxAntennas));
    if (retVal != 0)
    {
        goto exit;
    }

    /* configure the number of zeros to insert. */
    zeroInsertConfigNumber = 0;
    zerosCounter = 0;
    for (idx = 0U; idx < 64U; idx++)
    {
        if (((cfg->staticCfg.zeroInsrtMaskAzim >> idx) & 0x1U) == 0U)
        {
            zerosCounter++;
        }
        else
        {
            zeroInsertConfigNumber += zerosCounter;
            zerosCounter = 0;
        }
    }

    retVal = HWA_configZeroInsertNumMask(zeroInsertConfigNumber, zeroInsertConfigMask);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = HWA_configComplexMultScaleArrayQ20(obj->cfarAzimFFTCfg.antennaCalibParamsQuantized);
    if (retVal != 0)
    {
        goto exit;
    }

/* 2D maximum value offset */
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    retVal = HWA_config2DMaxOffset((int32_t)-round((double)(obj->cfarAzimFFTCfg.localMaxCfg.azimThreshold) * ((1.0 / 20.0) * CONST_LOG2_10 * 2048.0)),
            (int32_t)round((double)(HIST_DOPPLER_DIM_OFFSET_DB)* ((1.0/20.0) * CONST_LOG2_10 * 2048.0)));
    if (retVal != 0)
    {
        goto exit;
    }
#else
    retVal = HWA_config2DMaxOffset((int32_t)-round((double)(obj->cfarAzimFFTCfg.localMaxCfg.azimThreshold) * ((1.0 / 20.0) * CONST_LOG2_10 * 2048.0)),
            (int32_t)-round((double)(obj->cfarAzimFFTCfg.localMaxCfg.dopplerThreshold) * ((1.0 / 20.0) * CONST_LOG2_10 * 2048.0)));
    if (retVal != 0)
    {
        goto exit;
    }
    if(obj->cfarAzimFFTCfg.cfarCfg->variableThresholdMode != 0U)
    {
        /* Variable CFAR Threshold Mode: Apply range-dependent threshold
         *
         * Initialize the CFAR threshold for range bin 0 (first bin to be processed).
         * DPU_DopplerProcHWA_getRangeDependentCfarThreshold() returns the final threshold
         * value by adding the baseline thresholdScale to the range-dependent offset from
         * cfarThreshScaleLUT (typically +7dB for near-field bins).
         *
         * The threshold will be dynamically updated for each subsequent range bin during
         * the processing loop to maintain optimal detection sensitivity across all ranges.
         */
        retVal = HWA_configCFARThresholdScale(DPU_DopplerProcHWA_getRangeDependentCfarThreshold(cfg, 0U));
    }
    else
    {
        /* Fixed CFAR Threshold Mode: Use constant threshold for all range bins */
        retVal = HWA_configCFARThresholdScale(obj->cfarAzimFFTCfg.cfarCfg->thresholdScale);
    }
    if (retVal != 0)
    {
        goto exit;
    }
#endif

#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    retVal = HWA_configCDFCountThreshold(floor(CDF_CNT_THRES * cfg->staticCfg.numDopplerFFTBins / cfg->staticCfg.numBandsTotal));
    if (retVal != 0)
    {
        goto exit;
    }
#endif

exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      Internal function to config HWA state machine
 *
 *  @param[in]   ctrlBaseAddr       HWA control base address
 *  @param[in]   numLoops           number of loops to run from paramStartIdx to paramStopIdx
 *  @param[in]   paramStartIdx      start index of paramset through which state machine loops through
 *  @param[in]   paramStopIdx       stop index of paramset through which state machine loops through
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static inline void DPU_DopplerProcHWA_configStateMachine(DSSHWACCRegs *ctrlBaseAddr, uint16_t numLoops, uint16_t paramStartIdx, uint16_t paramStopIdx)
{
    /* Disable HWA */
    CSL_FINSR(ctrlBaseAddr->HWA_ENABLE, HWA_ENABLE_HWA_EN_END, HWA_ENABLE_HWA_EN_START, 0x0U);

    ctrlBaseAddr->PARAM_RAM_LOOP = numLoops;

    ctrlBaseAddr->PARAM_RAM_IDX = ((uint32_t)paramStopIdx << 16) | (uint32_t)paramStartIdx;

    /* Enable HWA */
    CSL_FINSR(ctrlBaseAddr->HWA_ENABLE, HWA_ENABLE_HWA_EN_END, HWA_ENABLE_HWA_EN_START, 0x7U);

}

 /**
  *  @b Description
  *  @n Doppler DPU process function.
  *     This is the core doppler processing function and deserves an
  * extensive comment. There are multiple processes happening in
  * parallel in this function. The parallelism is necessitated by the
  * need for performance. Hence, the function is complicated. This
  * comment will describe the process simply.
  *
  * The doppler processing stage has the following stages
  * STAGE I (DECOMPRESSION)
  * 1. Trigger a DMA to bring a compressed block of
  *    'staticCfg.decompCfg.rangeBinsPerBlock' Range gates from L3
  *    to the HWA.
  * 2. Decompress 'staticCfg.decompCfg.rangeBinsPerBlock' Range gates.
  *    Each Range gate consists of all the samples across chirps and
  *    rx antennas of a particular range bin.
  * 3. Move the decompressed data to another buffer in L3.
  *
  * For each range gate in a decompressed block, the second STAGE of the
  * this processing kicks in.
  * STAGE II (Doppler Processing)
  * 1. Move one range gate (from the decompressed data buffer in L3) to
  *    the HWA.
  * 2. Compute Doppler FFT and use it to compute the DDMA Metric and max
  *    subband index (and optionally the sum across Tx. )
  * 3. Move the Max Subband Indices from the HWA to the L2 and
  *    Doppler FFT to another HWA Membank after transpose.
  * 4. In the processor, use the max suband indices to program the Shuffle
  *    LUT, such that correct order of TXs' is picked during demodulation
  *    operation on HWA.
  * 5. In the HWA, rearrange/demodulate the Doppler FFT bin in the
  *    the 'correct' order (the correct order has all the virtual
  *    channels organized in a contiguous manner.  It also discards the
  *    empty sub-band for that bin.). On the demodulated Doppler FFT
  *    output, compute the azimuth FFT across the virtual channels
  *    for the doppler FFT (i.e. for the current range gate). On the
  *    output compute a CFAR in the doppler dimension and a 2D local max
  *    in both the doppler and the azimuth dimension. The result of CFAR
  *    is a list of detected objects, and the output of the local max is
  *    a 2D bit array.
  * 6. Send the rearranged Doppler FFT, azimuth FFT, CFAR and local maxima
  *    outputs from the HWA to the L2.
  * 7. Compute the intersection of the CFAR and local maxima outputs to
  *    generate the list of detected objects per range gate. The detected
  *    points of the azimuth peak as well as their immediate neighbours
  *    are also stored.
  *
  * These operations use the HWA, EDMA and DSP/DSS_CM4 concurrently. Hence we process
  * two range gates at a time, so that while the HWA is processing one,
  * the DSP/DSS_CM4 can process the other.
  *
  * Inside the function the following comment snippet is used to explain
  * what is expected to happen in the three cores (DSP/DSS_CM4, HWA and EDMA) at
  * each stage of the processing.
  *
  * DSP/DSS_CM4 : Some operation.
  * HWA : Another HWA operation.
  * EDMA: A data transfer operation.
  *
  *  @param[in]   handle     DPU handle.
  *  @param[in]   cfg        DPU config.
  *  @param[out]  outParams  Output parameters.
  *
  *  \ingroup    DPU_DOPPLERPROC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success     =0
  *  @retval
  *      Error      !=0 @ref DPU_DOPPLERPROC_ERROR_CODE
  */
int32_t DPU_DopplerProcHWA_process
(
    DPU_DopplerProcHWA_Handle    handle,
    DPU_DopplerProcHWA_Config    *cfg,
    DPU_DopplerProcHWA_OutParams *outParams
)
{
    uint16_t shuffleRAMBuf[256] = {0};
    volatile uint32_t   startTime;
    DPU_DopplerProcHWA_Obj *obj;
    int32_t retVal = SystemP_SUCCESS;
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    uint32_t rangeIdx;
#endif
#ifndef DOPPLERPROCHWADDMA_INTERRUPTS
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    uint8_t cfarPingParam, cfarPongParam;
#else
    uint8_t azimuthPingParam, azimuthPongParam;
#endif
    uint8_t demodPingParam, demodPongParam;
    uint8_t localMaxPingParam, localMaxPongParam;
#else
    int32_t status = SystemP_SUCCESS;
#endif
    uint32_t rangeBinIdx = 0U, mergedOBIdx = 0U;
    uint32_t dopInEDMAtoHWAStartAddress;
    uint32_t blockIdx = 0;
    uint16_t decompNumLoops, decompParamStart, decompParamStop;
    uint16_t dopplerAzimNumLoops, dopplerAzimParamStart, dopplerAzimParamStop;
    volatile DSSHWACCRegs * ctrlBaseAddr;
    uint16_t *shuffleRamBaseAddr;
    uint16_t shuffleLUTSize;
    uint32_t baseAddr, regionId;
    uint32_t processingTime;

    obj = (DPU_DopplerProcHWA_Obj *)handle;
    if (obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    ctrlBaseAddr = (DSSHWACCRegs *)(((HWA_Object *)obj->hwaHandle)->hwAttrs->ctrlBaseAddr);
    shuffleRamBaseAddr = (uint16_t *)gHwaRamCfg[HWA_RAM_TYPE_SHUFFLE_RAM].ramBaseAddress;
    shuffleLUTSize = (obj->numDopplerBins/obj->dopplerDemodCfg.numBandsTotal) * (uint16_t)sizeof (uint16_t);

    /* Set inProgress state */
    obj->inProgress = true;

    obj->numObjOut = 0;

    startTime = CycleCounterP_getCount32();
    baseAddr = EDMA_getBaseAddr(obj->edmaHandle);
    regionId = EDMA_getRegionId(obj->edmaHandle);

    /* decompEdmaToHwaStartAddress gets updated every block to fetch the compressed radar cube for next block.
     * This needs to be reset to the start address of radar cube data before the doppler processing starts.
     */
    obj->decompCfg.decompEdmaToHwaStartAddress = (void*)cfg->hwRes.radarCube.data;

    decompNumLoops = obj->decompCfg.numLoops;
    decompParamStart = (uint16_t)cfg->hwRes.hwaCfg.decompStageHwaStateMachineCfg.paramSetStartIdx;
    decompParamStop = (uint16_t)cfg->hwRes.hwaCfg.decompStageHwaStateMachineCfg.paramSetStartIdx + (uint16_t)cfg->hwRes.hwaCfg.decompStageHwaStateMachineCfg.numParamSets - 1U;

    dopplerAzimNumLoops = cfg->staticCfg.decompCfg.rangeBinsPerBlock / 2U * (uint16_t)obj->decompCfg.mergedNumOuterBlocks;
    dopplerAzimParamStart = (uint16_t)cfg->hwRes.hwaCfg.dopplerStageHwaStateMachineCfg.paramSetStartIdx;
    dopplerAzimParamStop = (uint16_t)cfg->hwRes.hwaCfg.azimCfarStageHwaStateMachineCfg.paramSetStartIdx + (uint16_t)cfg->hwRes.hwaCfg.azimCfarStageHwaStateMachineCfg.numParamSets - 1U;

#ifndef DOPPLERPROCHWADDMA_INTERRUPTS
    demodPingParam = cfg->hwRes.hwaCfg.azimCfarStageHwaStateMachineCfg.paramSetStartIdx + cfg->staticCfg.numRxAntennas - 1U;
    demodPongParam = demodPingParam + (cfg->hwRes.hwaCfg.azimCfarStageHwaStateMachineCfg.numParamSets / 2U);
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    cfarPingParam = demodPingParam + 2U;
    cfarPongParam = demodPongParam + 2U;
    localMaxPingParam = cfarPingParam + 1U;
    localMaxPongParam = cfarPongParam + 1U;
#else
    azimuthPingParam = demodPingParam + 1U;
    azimuthPongParam = demodPongParam + 1U;
    localMaxPingParam = demodPingParam + 2U;
    localMaxPongParam = demodPongParam + 2U;
#endif
#endif

    retVal = DPU_DopplerProcHWA_configHWACommon(obj, cfg);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Run block-wise */
    for(blockIdx = 0U; blockIdx < obj->decompCfg.numOuterBlocks; blockIdx++)
    {
        /* STAGE I (DECOMPRESSION)                      */
        /* Configure the HWA to perform decompression. */
        DPU_DopplerProcHWA_configStateMachine(ctrlBaseAddr, decompNumLoops, decompParamStart, decompParamStop);

        /* Update the source address for the EDMA that brings in compressed data from L3 to HWA for decompression*/
        if(blockIdx != 0U)
        {

            obj->decompCfg.decompEdmaToHwaStartAddress = (int32_t *)((uint8_t *)obj->decompCfg.decompEdmaToHwaStartAddress +
                                                                    obj->decompCfg.outerBlockSizeCompressed);

            retVal = DPEDMA_updateAddressAndTrigger(obj->edmaHandle,
                                (uint32_t)obj->decompCfg.decompEdmaToHwaStartAddress, /* src addr */
                                0,                                                 /* don't update dest addr */
                                (uint8_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIn.pingPong[PING].channel, /* param Id */
                                false);                                               /* don't trigger channel */
            if (retVal != 0)
            {
                goto exit;
            }

            retVal = DPEDMA_updateAddressAndTrigger(obj->edmaHandle,
                                (uint32_t)obj->decompCfg.decompEdmaToHwaStartAddress
                                        + ((uint32_t)obj->decompCfg.inputBytesPerBlock * (uint32_t)obj->decompCfg.numBlocksPerPing), /* src addr */
                                0,                                                 /* don't update dest addr */
                                (uint8_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIn.pingPong[PONG].channel, /* param Id */
                                false);                                               /* don't trigger channel */
            if (retVal != 0)
            {
                goto exit;
            }

        }

        /* Decomp (Ping) : Start ping DMA Transfer to bring compressed data from L3 to HWA for decompression */
        EDMA_setEvtRegion(baseAddr, regionId, (uint32_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIn.pingPong[PING].channel);

        /* Decomp (Pong) : Start pong DMA Transfer to bring compressed data from L3 to HWA for decompression */
        EDMA_setEvtRegion(baseAddr, regionId, (uint32_t)cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIn.pingPong[PONG].channel);

        /* ObjectList Create (Pong) : Given the CFAR & local Maxima results, create an object list of the last block's pong stage.
         * Note: The following three operations will happen in parallel.
           DSP/DSS_CM4: Extract object list of the previous block's last pong stage.
           HWA    : Decompression of the current block of range gates.
           EDMA   : Movement of compressed data to the HWA from L3 and decompressed data from HWA to L3. */
        if(blockIdx != 0U)
        {
            retVal = DPU_DopplerProcHWA_extractObjectList(obj, cfg, blockIdx - 1U, mergedOBIdx - 1U, rangeBinIdx - 1U);
            if (retVal != 0)
            {
                goto exit;
            }
        }

        /* Wait for EDMA completion indicating that the decompressed data has been moved to L3. */
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
        status = SemaphoreP_pend(&obj->decompEdmaOutDoneSemaHandle, SystemP_WAIT_FOREVER);
        if (status != SystemP_SUCCESS)
        {
            retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
            goto exit;
        }
#else
        DPU_DopplerProcHWA_edmaDonePoll(baseAddr, regionId, cfg->hwRes.edmaCfg.decompEdmaCfg.edmaIn.pingPong[PONG].channel);
#endif

        /* STAGE II (DOPPLER FFT, DEMOD, AZIM, OBJLIST) */
        /* Disable the HWA, Configure common registers, and re-enable the HWA to perform Doppler FFT and DDMA metric computation */
        DPU_DopplerProcHWA_configStateMachine(ctrlBaseAddr, dopplerAzimNumLoops, dopplerAzimParamStart, dopplerAzimParamStop);

        /* Reset the doppler In EDMA Start Address Variable. */
        dopInEDMAtoHWAStartAddress = (uint32_t)cfg->hwRes.decompScratchBuf;

        for(mergedOBIdx = 0U; mergedOBIdx < obj->decompCfg.mergedNumOuterBlocks; mergedOBIdx++)
        {

            if(mergedOBIdx != 0U)
            {
                dopInEDMAtoHWAStartAddress = dopInEDMAtoHWAStartAddress + (DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE * 
                                                                           cfg->staticCfg.numRxAntennas * obj->decompCfg.rangeBinsPerBlock * cfg->staticCfg.numChirps);

                /* Update the source address for the EDMA that brings in decompressed data from L3 to HWA for doppler FFT */
                retVal = DPEDMA_updateAddressAndTrigger(obj->edmaHandle,
                                    (uint32_t)dopInEDMAtoHWAStartAddress, /* src addr */
                                    0,                                                 /* don't update dest addr */
                                    (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaIn.pingPong[PING].channel, /* param Id */
                                    false);                                               /* don't trigger channel */
                if (retVal != 0)
                {
                    goto exit;
                }

                retVal = DPEDMA_updateAddressAndTrigger(obj->edmaHandle,
                                    (uint32_t)(dopInEDMAtoHWAStartAddress + (DPU_DOPPLERHWADDMA_DOPPLERIO_INPUT_BYTESPERSAMPLE * cfg->staticCfg.numRxAntennas)), /* src addr */
                                    0,                                                 /* don't update dest addr */
                                    (uint8_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaIn.pingPong[PONG].channel, /* param Id */
                                    false);                                               /* don't trigger channel */
                if (retVal != 0)
                {
                    goto exit;
                }
            }

            /* DopFFT and DDMA Metric (Ping) : Send the first decompressed range gate from L3 to HWA for Doppler FFT calculation
            (ping), for the first range bin in the block. */
            EDMA_setEvtRegion(baseAddr, regionId, (uint32_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaIn.pingPong[PING].channel);
            
            /* Extract object list of the previous pong stage */
            /* ObjectList Create (Pong) : Given the CFAR & local Maxima results, create an object list of the current block's previous pong stage.
            * Note: The following operations are now to happen in parallel.
                DSP/DSS_CM4 : Extract object list of the previous block's last range gate (Pong)
                HWA : Doppler FFT, DDMA metric and Max Subband computation of the current ping range gate.
                EDMA: Movement of decompressed data from L3 to HWA (Ping) and movement of Max Subband from HWA to the DSP/DSS_CM4 (Ping).  */
            if(mergedOBIdx != 0U)
            {
                retVal = DPU_DopplerProcHWA_extractObjectList(obj, cfg, blockIdx, mergedOBIdx - 1U, rangeBinIdx - 1U);
                if (retVal != 0)
                {
                    goto exit;
                }
            }

            /* In this loop we process all the range gates in the block of decompressed data. Note that the loop increments by
            * 2 indicating that 2 range gates (called ping (or even) and pong (or odd)) are processed in parallel. */
            for(rangeBinIdx = 0U; rangeBinIdx < cfg->staticCfg.decompCfg.rangeBinsPerBlock; rangeBinIdx+=2U)
            {
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
                rangeIdx = (((blockIdx * obj->decompCfg.mergedNumOuterBlocks) + mergedOBIdx) * obj->decompCfg.rangeBinsPerBlock) + rangeBinIdx;
#endif

#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(0);
#endif
                /* Extract object list of the previous pong stage */
                /* ObjectList Create (Pong) : Given the CFAR & local Maxima results, create an object list of the current block's previous pong stage.
                * Note: The following operations are now to happen in parallel.
                    DSP/DSS_CM4 : Extract object list of the previous block's last range gate (Pong)
                    HWA : Doppler FFT, DDMA metric and Max Subband computation of the current ping range gate.
                    EDMA: Movement of decompressed data from L3 to HWA (Ping) and movement of Max Subband from HWA to the DSP/DSS_CM4 (Ping).  */
                if(rangeBinIdx != 0U)
                {
                    retVal = DPU_DopplerProcHWA_extractObjectList(obj, cfg, blockIdx, mergedOBIdx, rangeBinIdx - 1U);
                    if (retVal != 0)
                    {
                        goto exit;
                    }
                }
#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(1);
#endif
                /* Send out decompressed range bin to HWA for Doppler FFT calculation (pong) */
                EDMA_setEvtRegion(baseAddr, regionId, (uint32_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaIn.pingPong[PONG].channel);
                
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
                /* Initialize histogram RAM here. */
                ctrlBaseAddr->MEM_INIT_START = HWA_APP_MEMINIT_HIST_EVEN_RAM | HWA_APP_MEMINIT_HIST_ODD_RAM;
#endif

                /* Wait for ping data transfer of Max Suband Index. */
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                status = SemaphoreP_pend(&obj->maxSubbandEdmaOutDoneSemaHandle[PING], SystemP_WAIT_FOREVER);
                if (status != SystemP_SUCCESS)
                {
                    retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
                    goto exit;
                }
#else
                DPU_DopplerProcHWA_edmaDonePoll(baseAddr, regionId, cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PING].channel);
#endif

#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(2);
#endif
                /* Note: The following operations are now to happen in parallel.
                    DSP/DSS_CM4 : Program the shuffle LUT to rearrange the Doppler FFT output in HWA so that the empty band is removed and the antenna data is in the correct order.
                    HWA : Doppler FFT, DDMA metric and Max Subband computation of the current pong range gate.
                    EDMA: Movement of decompressed data from L3 to HWA (Pong), doppler FFT data from HWA SrcMemBank to DstMemBank after band-wise tanspose (Ping). */
                DPU_DopplerProcHWA_ConfigShuffleLUT((void *)&shuffleRamBaseAddr[0], obj, cfg, PING);

#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(3);
#endif
                /* Trigger (SW Trigger) the execution of Demodulation (Ping) Paramsets. */
                CSL_FINSR(ctrlBaseAddr->FW2HWA_TRIG_0, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_END, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_START, 1U);

                /* Wait for pong data transfer of Max Suband Index. */
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                status = SemaphoreP_pend(&obj->maxSubbandEdmaOutDoneSemaHandle[PONG], SystemP_WAIT_FOREVER);
                if (status != SystemP_SUCCESS)
                {
                    retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
                    goto exit;
                }
#else
                DPU_DopplerProcHWA_edmaDonePoll(baseAddr, regionId, cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[PONG].channel);
#endif

#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(4);
#endif

                /* Note: The following operations are now to happen in parallel.
                DSP/DSS_CM4 : Compute the shuffle LUT values with PONG max Subabnd Indices.
                HWA : Demodulation, Azimuth FFT over the ping Doppler FFT, and CFAR, Local Max on the ping azimuth output.
                EDMA: Movement of Doppler FFT data from HWA SrcMemBank to DstMemBank after band-wise tanspose (Pong) and of Local Max, Azimuth FFT, CFAR and Demodulated Output from HWA to DSP/DSS_CM4 (Ping). */
                DPU_DopplerProcHWA_ConfigShuffleLUT(shuffleRAMBuf, obj, cfg, PONG);

#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(5);
#endif
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
                /* Wait for Demod ping paramset done interrupt from HWA */
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                status = SemaphoreP_pend(&obj->demodHwaDoneSemaHandle, SystemP_WAIT_FOREVER);
                if (status != SystemP_SUCCESS)
                {
                    retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
                    goto exit;
                }
#else
                DPU_DopplerProcHWA_hwaDonePoll(ctrlBaseAddr, demodPingParam);
#endif
#else
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                status = SemaphoreP_pend(&obj->azimFFTHwaDoneSemaHandle[PING], SystemP_WAIT_FOREVER);
                if (status != SystemP_SUCCESS)
                {
                    retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
                    goto exit;
                }
#else
                DPU_DopplerProcHWA_hwaDonePoll(ctrlBaseAddr, azimuthPingParam);
#endif
#endif
#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(6);
#endif
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
                /* Shuffle LUT is programmed here after the ping demodulation param is done so that shuffle LUT does not get overwritten with the pong values. */
                (void)memcpy((void *)&shuffleRamBaseAddr[0],(void*)((uint8_t *)shuffleRAMBuf + shuffleLUTSize), shuffleLUTSize);

                /* Wait for CFAR ping paramset done interrupt from HWA */
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                status = SemaphoreP_pend(&obj->cfarHwaDoneSemaHandle, SystemP_WAIT_FOREVER);
                if (status != SystemP_SUCCESS)
                {
                    retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
                    goto exit;
                }
#else
                DPU_DopplerProcHWA_hwaDonePoll(ctrlBaseAddr, cfarPingParam);
#endif

                /* Read ping CFAR peak reg count in HWA */
                obj->numCfarPeaksPing  = CSL_FEXTR(ctrlBaseAddr->CFAR_PEAKCNT, CFAR_PEAKCNT_CFAR_PEAKCNT_END, CFAR_PEAKCNT_CFAR_PEAKCNT_START);
                if(obj->cfarAzimFFTCfg.cfarCfg->variableThresholdMode != 0U)
                {
                    /* Variable CFAR Threshold Mode: Update threshold for next pong range bin
                     *
                     * After processing the current ping range bin (rangeIdx), pre-configure the
                     * HWA CFAR threshold for the next pong range bin (rangeIdx + 1) before
                     * triggering its execution. This ensures the correct range-dependent threshold
                     * is applied when the pong paramset runs.
                     *
                     * Ping-pong processing: While DSP extracts objects for ping, HWA processes pong
                     * with its appropriately scaled threshold.
                     */
                    retVal = HWA_configCFARThresholdScale(
                        DPU_DopplerProcHWA_getRangeDependentCfarThreshold(cfg, rangeIdx + 1U));
                    if (retVal != 0)
                    {
                        goto exit;
                    }
                }
                /* Trigger the execution of Azimuth Stage (Pong) Paramsets.
                This trigger is done later here since we need to read the CFAR peak count reg which cannot
                be done while another paramset is executing. Hence we cannot have any pong paramset running while we
                read the CFAR peak count register. The pong paramset can run while the DSP/DSS_CM4 is creating the object list at the ping side, hence the EDMA transferred is triggered next here. */
                CSL_FINSR(ctrlBaseAddr->FW2HWA_TRIG_0, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_END, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_START, 1U);
#endif

#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(7);
#endif

#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION

                /* Configure Local Max Thresholds */
                DPU_DopplerProcHwa_configLocalMaxThresh(ctrlBaseAddr, cfg->staticCfg.numAzimFFTBins);

                /* Trigger LM ping param */
                CSL_FINSR(ctrlBaseAddr->FW2HWA_TRIG_0, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_END, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_START, 1U);

                /* Clear Histogram RAM */
                ctrlBaseAddr->MEM_INIT_START = HWA_APP_MEMINIT_HIST_EVEN_RAM | HWA_APP_MEMINIT_HIST_ODD_RAM;

                /* Shuffle LUT is programmed here after the ping demodulation param is done so that shuffle LUT does not get overwritten with the pong values. */
                (void)memcpy((void *)&shuffleRamBaseAddr[0],(uint8_t *)shuffleRAMBuf + shuffleLUTSize, shuffleLUTSize);

                /* Trigger the execution of Demodulation Paramset.
                The pong paramset can run while the DSP/DSS_CM4 is creating the object list at the ping side, hence the EDMA transferred is triggered next here. */
                CSL_FINSR(ctrlBaseAddr->FW2HWA_TRIG_0, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_END, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_START, 1U);
#endif

#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(8);
#endif
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                status = SemaphoreP_pend(&obj->localMaxHwaDoneSemaHandle[PING], SystemP_WAIT_FOREVER);
                if (status != SystemP_SUCCESS)
                {
                    retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
                    goto exit;
                }
#else
                DPU_DopplerProcHWA_hwaDonePoll(ctrlBaseAddr, localMaxPingParam);
#endif

#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(9);
#endif
                /* Extract ping object list on the DSP/DSS_CM4 - In this function the intersection of the localmax and CFAR on the doppler Azimuth operations is computed and the detected object list is created.
                * Note: The following operations are now to happen in parallel.
                    DSP/DSS_CM4 : Extract object list (ping)
                    HWA : Demodulation, Azimuth FFT over the pong Doppler FFT, Local Max and CFAR on the azimuth output.
                    EDMA: Movement of pong Azimuth FFT data from DSP/DSS_CM4 L2 to the HWA and of local max and CFAR from HWA to DSP/DSS_CM4.*/
                retVal = DPU_DopplerProcHWA_extractObjectList(obj, cfg, blockIdx, mergedOBIdx, rangeBinIdx);
                if (retVal != 0)
                {
                    goto exit;
                }

#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(10);
#endif
                /* The following steps prepare for the next range bin of the block. */
                if(rangeBinIdx != cfg->staticCfg.decompCfg.rangeBinsPerBlock-2U)
                {
                    /* Send out decompressed range bin to HWA for Doppler FFT calculation (ping) */
                    EDMA_setEvtRegion(baseAddr, regionId, (uint32_t)cfg->hwRes.edmaCfg.dopplerEdmaCfg.edmaIn.pingPong[PING].channel);
                }

#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
                /* Wait for Demod pong paramset done interrupt from HWA */
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                status = SemaphoreP_pend(&obj->azimFFTHwaDoneSemaHandle[PONG], SystemP_WAIT_FOREVER);
                if (status != SystemP_SUCCESS)
                {
                    retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
                    goto exit;
                }
#else
                DPU_DopplerProcHWA_hwaDonePoll(ctrlBaseAddr, azimuthPongParam);
#endif

                DPU_DopplerProcHwa_configLocalMaxThresh(ctrlBaseAddr, cfg->staticCfg.numAzimFFTBins);

                /* Trigger LM pong param */
                CSL_FINSR(ctrlBaseAddr->FW2HWA_TRIG_0, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_END, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_START, 1U);
#endif

#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
                status = SemaphoreP_pend(&obj->localMaxHwaDoneSemaHandle[PONG], SystemP_WAIT_FOREVER);
                if (status != SystemP_SUCCESS)
                {
                    retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
                    goto exit;
                }
#else
                DPU_DopplerProcHWA_hwaDonePoll(ctrlBaseAddr, localMaxPongParam);
#endif
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
                /* Read pong CFAR peak reg count in HWA */
                obj->numCfarPeaksPong  = CSL_FEXTR(ctrlBaseAddr->CFAR_PEAKCNT, CFAR_PEAKCNT_CFAR_PEAKCNT_END, CFAR_PEAKCNT_CFAR_PEAKCNT_START);
                if((obj->cfarAzimFFTCfg.cfarCfg->variableThresholdMode != 0U) &&
                    (rangeBinIdx != (cfg->staticCfg.decompCfg.rangeBinsPerBlock - 2U)))
                {
                    /* Variable CFAR Threshold Mode: Update threshold for next ping range bin
                     *
                     * After processing the current pong range bin (rangeIdx + 1), pre-configure
                     * the HWA CFAR threshold for the upcoming ping range bin (rangeIdx + 2).
                     * This maintains the ping-pong pipeline with correct range-dependent thresholds.
                     *
                     * Note: Skip the update on the second-to-last iteration (rangeBinIdx ==
                     * rangeBinsPerBlock - 2) to avoid accessing beyond the LUT bounds, as
                     * rangeIdx + 2 would exceed numRangeBins for the current outer block.
                     */
                    retVal = HWA_configCFARThresholdScale(
                        DPU_DopplerProcHWA_getRangeDependentCfarThreshold(cfg, rangeIdx + 2U));
                    if (retVal != 0)
                    {
                        goto exit;
                    }
                }
#endif
#ifdef DOPPLERPROCHWADDMA_DPU_TIMING
                insertStamp(11);
#endif
            } /* End of RangeBins Per Block Loop. */

        } /* End of Merged Outer Blocks Loop. */
    } /* End of Outer Blocks Loop. */

	/* Extract object list of the last pong stage */
    if (obj->decompCfg.numOuterBlocks != 0U)
    {
        retVal = DPU_DopplerProcHWA_extractObjectList(obj, cfg, blockIdx - 1U, mergedOBIdx - 1U, rangeBinIdx - 1U);
        if (retVal != 0)
        {
            goto exit;
        }
    }

    outParams->numObjOut = obj->numObjOut;
    outParams->stats.numProcess++;
    processingTime = CycleCounterP_getCount32() - startTime;
    outParams->stats.processingTime = (uint64_t)processingTime;

exit:
    if (obj != NULL)
    {
        obj->inProgress = false;
    }

    return retVal;
}

/**
  *  @b Description
  *  @n
  *  Doppler DPU deinit
  *
  *  @param[in]   handle   DPU handle.
  *
  *  \ingroup    DPU_DOPPLERPROC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success      =0
  *  @retval
  *      Error       !=0 @ref DPU_DOPPLERPROC_ERROR_CODE
  */
int32_t DPU_DopplerProcHWA_deinit(DPU_DopplerProcHWA_Handle handle)
{
    DPU_DopplerProcHWA_Obj  *obj = NULL;
    int32_t     retVal = 0;
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
    uint8_t index = 0U;
#endif
    /* Sanity Check */
    obj = (DPU_DopplerProcHWA_Obj*)handle;
    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    /* Delete Semaphores */
#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
    SemaphoreP_destruct(&obj->decompEdmaOutDoneSemaHandle);
    SemaphoreP_destruct(&obj->demodHwaDoneSemaHandle);
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    SemaphoreP_destruct(&obj->cfarHwaDoneSemaHandle);
#endif

    for(index = 0U; index < 2U; index++)
    {
        SemaphoreP_destruct(&obj->maxSubbandEdmaOutDoneSemaHandle[index]);
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
        SemaphoreP_destruct(&obj->azimFFTHwaDoneSemaHandle[index]);
#endif
        SemaphoreP_destruct(&obj->localMaxHwaDoneSemaHandle[index]);
    }
#endif
exit:
    return retVal;
}
