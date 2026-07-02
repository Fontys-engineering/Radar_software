/*
 *   @file  objectdetection_elevEst.c
 *
 *   @brief
 *      Object Detection DPC - angle estimation implementation.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2024-25 Texas Instruments, Inc.
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
#include <stdint.h>
#include <string.h>

/* mmWave SDK Include Files: */
#include <ti/common/syscommon.h>
#include <ti/utils/mathutils/mathutils.h>
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/objectdetection.h>
#define QVALUE_NOISE          (11U)
#define QVALUE_SIGNAL         (11U)

/*! @brief  Unsigned round (for floats). */
#define ROUND_UNSIGNED(x) ((x) + 0.5f)
#define AOA_DFT_LEN (128U)

#ifdef SUBSYS_DSS
/* C66x mathlib */
/* Suppress the mathlib.h warnings
 *  #48-D: incompatible redefinition of macro "TRUE"
 *  #48-D: incompatible redefinition of macro "FALSE"
 */
#pragma diag_push
#pragma diag_suppress 48
#include <ti/mathlib/mathlib.h>
#pragma diag_pop

/* DSP Mathlib include files */
#include <ti/mathlib/src/cossp/c66/cossp.h>
#include <ti/dsplib/src/DSPF_sp_dotp_cplx/DSPF_sp_dotp_cplx.h>
#endif

#ifdef SUBSYS_MSS
#include <kernel/dpl/CacheP.h>
#define divsp(x,y) ((x)/(y))
#endif
/*! @brief  Complex data type, natural for C66x complex
 * multiplication instructions. */
typedef struct cmplxfImRe_t_
{
    float imag; /*!< @brief imaginary part */
    float real; /*!< @brief real part */
} cmplxfImRe_t;
/*! @brief  Complex union type, natural for C66x intrinsic
 * instructions. */
typedef union cmplxfUnion_t_
{
	cmplxfImRe_t cmplx;
	float dat[2];
	double ddat;
}cmplxfUnion_t;


/* A simple sin-cos LUT used for the DFT computations in
 * DPC_ObjDet_estimateXYZ */
cmplxfImRe_t dftSinCosTable[AOA_DFT_LEN] __attribute__((aligned(8))) = {
#include "cossintable.c"
};

void arm_cmplx_mult
(
    const int * pSrcA,
    const float * pSrcB,
    float * pDst,
    uint32_t numSamples
);

void arm_cmplx_conj_mult_cmplx_f32
(
    const float * pSrcA,
    const float * pSrcB,
    float * pDst,
    uint32_t numSamples
);

void arm_cmplx_dot_prod_f32
(
    const float * pSrcA,
    const float * pSrcB,
    uint32_t numSamples,
    float * realResult,
    float * imagResult
);


/**
 *  @b Description
 *  @n
 *     Function performs quadratic interpolation around a peak
 *
 *  @param[in]  y A Three sample array ([y0,y1,y2]) where
 *              (y1 > y2) and (y1 > y0)
 *
 *  @retval   location of the interpolated peak, relative to y1.
 *
 * \ingroup DPC_OBJDET__INTERNAL_FUNCTION
 */
static inline float DPC_ObjDet_quadInterpAroundPeak(const uint32_t * restrict y)
{

    float ym1, y0, yp1;
    float thetapk; //, yOut;

    ym1 = (float) y[0]; /* y(peak-1) */
    y0  = (float) y[1]; /* y(peak) */
    yp1 = (float) y[2]; /* y(peak+1) */

    thetapk = divsp((yp1 - ym1), (2.0F * (2.0F * y0 - yp1 - ym1)));
    /* yOut = y0 + (((yp1 - ym1) / 4) * thetapk); */

    return thetapk;

}


#ifdef SUBSYS_DSS
/**
 *  This routine calculates the dot product of 2 single-precision complex
 *  float vectors. The even numbered locations hold the real parts of the
 *  complex numbers while the odd numbered locations contain the imaginary
 *  portions. It is an exact copy of the DSPF_sp_dotp_cmplx function from
 *  the DSPLIB.
 *
 *         @param x   Pointer to array holding the first floating-point vector
 *         @param y   Pointer to array holding the second floating-point vector
 *         @param nx  Number of values in the x and y vectors
 *         @param re  Pointer to the location storing the real part of the result
 *         @param im  Pointer to the location storing the imaginary part of the result
 *
 * @par Assumptions:
 *   Loop counter must be multiple of 4 and > 0. <BR>
 *   The x and y arrays must be double-word aligned. <BR>
 *
 *
 */
static inline void dotpCmplxf(const float * restrict x, const float * restrict y, int nx,
                       float * restrict re, float * restrict im)
{
    int i;
    __float2_t x0_im_re, y0_im_re, result0 = 0;
    __float2_t x1_im_re, y1_im_re, result1 = 0;
    __float2_t x2_im_re, y2_im_re, result2 = 0;
    __float2_t x3_im_re, y3_im_re, result3 = 0;
    __float2_t result;

    _nassert(nx % 4 == 0);
    _nassert(nx > 0);
    _nassert((int)x % 8 == 0);
    _nassert((int)y % 8 == 0);

    for(i = 0; i < 2 * nx; i += 8)
    {
        /* load 4 sets of input data */
        x0_im_re = _amem8_f2((void*)&x[i]);
        y0_im_re = _amem8_f2((void*)&y[i]);

        x1_im_re = _amem8_f2((void*)&x[i+2]);
        y1_im_re = _amem8_f2((void*)&y[i+2]);

        x2_im_re = _amem8_f2((void*)&x[i+4]);
        y2_im_re = _amem8_f2((void*)&y[i+4]);

        x3_im_re = _amem8_f2((void*)&x[i+6]);
        y3_im_re = _amem8_f2((void*)&y[i+6]);

        /* calculate 4 running sums */
        result0 = _daddsp(_complex_mpysp(x0_im_re, y0_im_re), result0);
        result1 = _daddsp(_complex_mpysp(x1_im_re, y1_im_re), result1);
        result2 = _daddsp(_complex_mpysp(x2_im_re, y2_im_re), result2);
        result3 = _daddsp(_complex_mpysp(x3_im_re, y3_im_re), result3);
    }

    result = _daddsp(_daddsp(result0,result1),_daddsp(result2,result3));
    *re =  _hif2(result);
    *im =  _lof2(result);
}

/**
 *  @b Description
 *  @n
 *     Function estimates XYZ coordinates of objects in the object list
 *
 *  @param[in]  elevEstCfg   subframe object
 *  @param[in]  detObjList  Detected object list
 *  @param[out] objOut      List with x, y, z coordinates populated for each object
 *  @param[in]  numObjOut   Number of detected objects
 *  @param[out] finalNumObjOut  Number of validated objects
 *
 *  @retval   None
 *
 * \ingroup DPC_OBJDET__INTERNAL_FUNCTION
 */
extern int32_t DPC_ObjDet_estimateXYZ(DPC_ObjectDetection_ElevEstCfg * restrict elevEstCfg,
                               const DetObjParams * restrict detObjList,
                               DPIF_PointCloudCartesian * restrict objOut,
                               uint32_t numObjOut,
                               uint32_t * restrict finalNumObjOut)
{
    DPC_ObjectDetection_ElevEstCommonCfg *commonCfg = &elevEstCfg->commonCfg;
    DPC_ObjectDetection_ExecuteResult *result = commonCfg->result;
    uint8_t subFrameIdx = result->subFrameIdx;
    DPC_ObjectDetection_ElevEstSubframeCfg *subframeCfg = &elevEstCfg->subframeCfg[subFrameIdx];
    uint16_t azimFFTSize = subframeCfg->numAzimFFTBins;
    const float invAzimFFTSize = divsp(1.0f,(float) azimFFTSize);
    uint32_t objIdx, sampIdx, idx;
    uint32_t maxAzimMaskWidth = 8*sizeof(commonCfg->zeroInsrtMaskCfg.zeroInsrtMaskAzim);
    uint32_t maxElevMaskWidth = 8*sizeof(commonCfg->zeroInsrtMaskCfg.zeroInsrtMaskElev);
    int32_t currLoc;
    float noisedB, signaldB, snrdB;
    float peakIdxOffset, peakIdxFlt;
    int32_t peakLoc;
    float    azimSinPhase;
    cmplxfUnion_t DFTValAzim, DFTValElev, elevOutput;
    float  elevSinPhase, elevCosPhase;
    float rangeStep, range, dopplerStep, x, ySquared, z;
    float wz,peakLocFlt, peakIdxFlt_DFT;
    int32_t dopIdx;
    uint32_t numDopplerBins = subframeCfg->numDopplerBins;
    int16_t ValidObjIdx;
    double *antennaCalibParams = (double*)commonCfg->antennaCalibParams;

    /* Alignment is to be done because we use the antenna calib params for
     * multiplication, using optimized DSP routines, which require a 8 byte alignment */
    cmplxfImRe_t samplesCalib[MAX_NUM_VIRT_ANT] __attribute__((aligned(8)));
    cmplx32ImRe_t rearrangedAzimSamples[MAX_NUM_AZIM_VIRT_ANT] __attribute__((aligned(8)));
    cmplx32ImRe_t rearrangedElevSamples[MAX_NUM_ELEV_VIRT_ANT] __attribute__((aligned(8)));

    int32_t retVal = 0;
    cmplxfImRe_t dftFactorsAzim[MAX_NUM_AZIM_VIRT_ANT] __attribute__((aligned(8)));
    cmplxfImRe_t dftFactorsElev[MAX_NUM_ELEV_VIRT_ANT] __attribute__((aligned(8)));
    double * azimSamplesCalib = (double *)&samplesCalib[0];
    double * elevSamplesCalib = (double *)&samplesCalib[MAX_NUM_AZIM_VIRT_ANT];
    int64_t  * restrict azimSamples;
    int64_t  * restrict elevSamples;
    double samplesFlt2;

    rangeStep = subframeCfg->rangeStep;
    dopplerStep = subframeCfg->dopplerStep;

    /* This variable will index the final object list */
    ValidObjIdx = 0;

    for (objIdx = 0; objIdx < numObjOut; objIdx++)
    {
        /* 1. Interpolate around peak to get fractional estimate of Azimuth index */
        peakIdxOffset = DPC_ObjDet_quadInterpAroundPeak(detObjList[objIdx].azimPeakSamples);

        /* Correct peak index with the fractional index*/
        peakIdxFlt = (float)detObjList[objIdx].azimIdx + peakIdxOffset;
        peakIdxFlt_DFT = peakIdxFlt * (invAzimFFTSize * AOA_DFT_LEN);
        peakLoc = ROUND_UNSIGNED(peakIdxFlt_DFT);


        /* 2a. Calculate DFT Factors corresponding to wx for Row 1.
            *  i.e. calculate \f$\e^{j wx}\f$
            */
        idx = 0;
        for (sampIdx = 0; sampIdx < maxAzimMaskWidth; sampIdx ++)
        {
            if((commonCfg->zeroInsrtMaskCfg.zeroInsrtMaskAzim >> sampIdx) & 0x1U)
            {
                currLoc = (peakLoc*sampIdx)%AOA_DFT_LEN;
                dftFactorsAzim[idx++] = dftSinCosTable[currLoc];
            }

            /* Break the loop after computing all dft factors in azimuth dimension */
            if(idx == MAX_NUM_AZIM_VIRT_ANT)
                break;
        }

        /* 2b. Calculate DFT Factors corresponding to wx for Row 0.
            *  i.e. calculate \f$\e^{j wx}\f$
            */
        idx = 0;
        for (sampIdx = 0; sampIdx < maxElevMaskWidth; sampIdx ++)
        {
            if((commonCfg->zeroInsrtMaskCfg.zeroInsrtMaskElev >> sampIdx) & 0x1U)
            {
                currLoc = (peakLoc*sampIdx)%AOA_DFT_LEN;
                dftFactorsElev[idx++] = dftSinCosTable[currLoc];
            }

            /* Break the loop after computing all dft factors in elevation dimension */
            if(idx == MAX_NUM_ELEV_VIRT_ANT)
                break;
        }

        /* 2c. Rearrange the antenna samples according to the virtual antenna mapping.  */
        for (sampIdx = 0; sampIdx < MAX_NUM_AZIM_VIRT_ANT; sampIdx ++)
        {
            rearrangedAzimSamples[sampIdx] = detObjList[objIdx].azimSamples[commonCfg->antennaGeometryCfg[sampIdx]];
        }

        for (sampIdx = 0; sampIdx < MAX_NUM_ELEV_VIRT_ANT; sampIdx ++)
        {
            rearrangedElevSamples[sampIdx] = detObjList[objIdx].elevSamples[commonCfg->antennaGeometryCfg[MAX_NUM_AZIM_VIRT_ANT+sampIdx]];
        }

        azimSamples = (int64_t*) rearrangedAzimSamples;
        elevSamples = (int64_t*) rearrangedElevSamples;

        /* 3. Azimuth Antenna Calibration:
            * Multiply azimuth samples (azimSamples) of Doppler FFT with antenna calib params (antennaCalibParams)
            */

        for (sampIdx = 0; sampIdx < MAX_NUM_AZIM_VIRT_ANT; sampIdx++)
        {
            samplesFlt2 = _dintsp(azimSamples[sampIdx]);
            azimSamplesCalib[sampIdx] =  _complex_mpysp(samplesFlt2,antennaCalibParams[sampIdx]);

        }

        /* 4. Elevation Antenna calibration
            * Multiply elev samples with antenna calib params  */
        for (sampIdx = MAX_NUM_AZIM_VIRT_ANT; sampIdx < MAX_NUM_VIRT_ANT ; sampIdx++ )
        {
            samplesFlt2 = _dintsp(elevSamples[sampIdx - MAX_NUM_AZIM_VIRT_ANT]);
            elevSamplesCalib[sampIdx-MAX_NUM_AZIM_VIRT_ANT] = _complex_mpysp(samplesFlt2, antennaCalibParams[sampIdx]);
        }

        /* 5. Single Bin DFT on the azimuth antennas to estimate phase at peak.
            *
            \f[
            X_{azim} (\omega_x) = \sum_{k=0}^{N_{azim} - 1} azimSample(k)  e^{-j k \omega_x}
            \f]
            * Multiply DFT factors with azimuth of Doppler FFT samples corrected for antenna calibration.
            */
        dotpCmplxf((float *)&azimSamplesCalib[0], (float *)&dftFactorsAzim[0], MAX_NUM_AZIM_VIRT_ANT, &DFTValAzim.cmplx.real, &DFTValAzim.cmplx.imag);


        /* 6.  Single Bin DFT on the elevation antennas to estimate phase at peak.
            *
            \f[
            X_{elev} (\omega_x) = \sum_{k=0}^{N_{elev} - 1} elevSample(k)  e^{-j (k+2) \omega_x}
            \f]
            * The elevation antennas (essentially the 4 virtual antennas corresponding to the
            * elevation offset Tx antenna) are 4 in number and offset by 3 positions from the
            * azimuth virtual array. Hence when the DFT is computed, begin from the 3rd DFT parameter.
            *
            * Both elevSamplesCalib and dftFactorsElev[4] are double-word aligned. */
        dotpCmplxf((float *)&elevSamplesCalib[0], (float *)&dftFactorsElev[0], MAX_NUM_ELEV_VIRT_ANT, &DFTValElev.cmplx.real, &DFTValElev.cmplx.imag);

        /* 7. Estimate phase difference between the peak location at azimuth antennas and elevation antennas at peak.
            *  - 1. compute the conjugate product to get the phase difference (i.e. AzimVal * conj(ElevVal))
            *  - 2. _complex_conjugate_mpysp(a,b) == _complex_mpysp(conj(a), b)) */
        elevOutput.ddat =  _complex_conjugate_mpysp (DFTValElev.ddat, DFTValAzim.ddat);


        /* - 2. Compute the angle of the product to estimate the phase change in elevation.
            \f[
            \omega_z = angle (\ X_{elev} (\omega_x)' \times X_{azim} (\omega_x) )\
            \f]
        */
        if (fabsf(elevOutput.cmplx.imag) < (0.15f*fabsf(elevOutput.cmplx.real)))
        {
            // small angle approximation.
            wz = divsp(elevOutput.cmplx.imag, elevOutput.cmplx.real);
        }
        else
        {
            wz = atan2sp(elevOutput.cmplx.imag, elevOutput.cmplx.real);
            if (wz > PI_)
            {
                    wz -= 2.0f*PI_;
            }
        }

        /* 8. Obtain range using the range resolution and the range Index  */
        range = rangeStep * (float)detObjList[objIdx].rangeIdx;

        /* 9. Obtain z, x coordinates.
            \f[
            \Phi = asin(\frac{\omega_z}{2 \pi d_z})
            \f]

        \f[
            z = range \times sin(\phi) = range * \frac{\omega_z}{2 \pi d_z}
        \f]

        */
        elevSinPhase = wz * (1.0f / (2.0f * PI_ * commonCfg->antennaSpacing.zSpacingByLambda));
        if ((elevSinPhase > subframeCfg->aoaFovSinVal.minElevationDeg) && (elevSinPhase < subframeCfg->aoaFovSinVal.maxElevationDeg))
        {

            z = range * elevSinPhase;

            /*
            \f[
                x = range  cos(\phi)  sin(\theta) =  range  /frac{\omega_x}{2 \pi d_x}
            \f]

            */
            peakLocFlt = peakLoc * (1.0f/ AOA_DFT_LEN);
            if (peakLocFlt > 0.5f)
            {
                peakLocFlt -= 1.0f;
            }

            x = range * peakLocFlt * (1.0f / commonCfg->antennaSpacing.xSpacingByLambda);

            /* Obtain 'square of y' coordinate
            \f[
                y^2 = range^2 -x^2 - z^2
            \f]
            */
            ySquared = (range * range) - (z * z) - (x * x);

            /* It is possible that ySquared is less than zero (i.e. a degenerate case). In such a case ignore the object.
            * If the case is not degenerate, proceed to check if the object is in the field of view (FoV).
            * If so , store the newly validated object in the final object list.*/
            if (ySquared > 0)
            {
                /* Estimate azimuth phase.
                \f[
                    sin(\Theta) = \frac{x}{range \times cos(\Phi)}
                \f]
                */
                elevCosPhase = sqrtsp(1 - (elevSinPhase * elevSinPhase));
                azimSinPhase = divsp(x, (range * elevCosPhase));

                /* Check if object is in azimuth FoV, If object is in FoV, proceed to store the coordinates in the final object list */
                if ((azimSinPhase > subframeCfg->aoaFovSinVal.minAzimuthDeg) && (azimSinPhase < subframeCfg->aoaFovSinVal.maxAzimuthDeg))
                {

                    /* Store x, y, z values */
                    objOut[ValidObjIdx].z = z;
                    objOut[ValidObjIdx].x = x;
                    objOut[ValidObjIdx].y = sqrtsp(ySquared);

                    /* Obtain and store Velocity */
                    if (detObjList[objIdx].dopIdxActual > numDopplerBins / 2)
                    {
                        dopIdx = detObjList[objIdx].dopIdxActual - numDopplerBins;
                    }
                    else
                    {
                        dopIdx = detObjList[objIdx].dopIdxActual;
                    }
                    objOut[ValidObjIdx].velocity = dopIdx * dopplerStep;

                    /* Calcute the side info of final detected object */
                    /* output is 20*log10(2)*value/2^(QVALUE) */
                    noisedB = 6.0 * ((float)detObjList[objIdx].dopCfarNoise) * (1.0f/(1 << QVALUE_NOISE));
                    signaldB = 6.0 * ((float)detObjList[objIdx].azimPeakSamples[1]) * (1.0f/(1<<QVALUE_SIGNAL));
                    snrdB = signaldB - noisedB;

                    result->objOutSideInfo[ValidObjIdx].snr = (int)(10*snrdB);
                    result->objOutSideInfo[ValidObjIdx].noise = (int)(10*noisedB);

                    /* Increment output list index */
                    ValidObjIdx++;
                } /* End of azimuth FoV check cond */
            } /* End of degenerate condition check */
        } /* End of elevation FoV check cond */

    }
    *finalNumObjOut = ValidObjIdx;

    goto exit;
exit:
    return retVal;
}

#endif
#ifdef SUBSYS_MSS

/**
  @brief         Floating-point complex-by-complex multiplication.
  @param[in]     pSrcA       points to first input vector [Im,Re]
  @param[in]     pSrcB       points to second input vector [Im,Re]
  @param[out]    pDst        points to output vector [Im,Re]
  @param[in]     numSamples  number of samples in each vector
  @return        none
 */

void arm_cmplx_mult(
  const int * pSrcA,
  const float * pSrcB,
        float * pDst,
        uint32_t numSamples)
{
    uint32_t blkCnt;                               /* Loop counter */
    int a, b;
    float c, d;  /* Temporary variables to store real and imaginary values */

    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
        /* Imag: C[2 * i    ] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */
        /* Real: C[2 * i + 1] = A[2 * i + 1] * B[2 * i + 1] - A[2 * i] * B[2 * i    ]. */

        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;
        /* store result in destination buffer. */
        *pDst++ = ((float)a * d) + ((float)b * c);
        *pDst++ = ((float)b * d) - ((float)a * c);


        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;
        *pDst++ = ((float)a * d) + ((float)b * c);
        *pDst++ = ((float)b * d) - ((float)a * c);

        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;
        *pDst++ = ((float)a * d) + ((float)b * c);
        *pDst++ = ((float)b * d) - ((float)a * c);

        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;
        *pDst++ = ((float)a * d) + ((float)b * c);
        *pDst++ = ((float)b * d) - ((float)a * c);

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = numSamples % 0x4U;

    while (blkCnt > 0U)
    {
        /* Imag: C[2 * i    ] = A[2 * i] * B[2 * i + 1] + A[2 * i + 1] * B[2 * i    ]. */
        /* Real: C[2 * i + 1] = A[2 * i + 1] * B[2 * i + 1] - A[2 * i] * B[2 * i    ]. */

        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;

        /* store result in destination buffer. */
        *pDst++ = ((float)a * d) + ((float)b * c);
        *pDst++ = ((float)b * d) - ((float)a * c);
        /* Decrement loop counter */
        blkCnt--;
    }
}

/**
  @brief         Floating-point complex-by-complex multiplication.
  @param[in]     pSrcA       points to first input vector [Im,Re]
  @param[in]     pSrcB       points to second input vector [Im,Re]
  @param[out]    pDst        points to output vector [Im,Re] = conj(pSrcA)*pSrcB
  @param[in]     numSamples  number of samples in each vector
  @return        none
 */

void arm_cmplx_conj_mult_cmplx_f32(
  const float * pSrcA,
  const float * pSrcB,
        float * pDst,
        uint32_t numSamples)
{
    uint32_t blkCnt;                               /* Loop counter */
    float a, b, c, d;  /* Temporary variables to store real and imaginary values */

    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
        /* Imag: C[2 * i    ] = A[2 * i + 1] * B[2 * i    ] - A[2 * i] * B[2 * i + 1]. */
        /* Real: C[2 * i + 1] = A[2 * i + 1] * B[2 * i + 1] + A[2 * i] * B[2 * i    ]. */

        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;
        /* store result in destination buffer. */
        *pDst++ = (b * c) - (a * d);
        *pDst++ = (b * d) + (a * c);


        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;
        *pDst++ = (b * c) - (a * d);
        *pDst++ = (b * d) + (a * c);

        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;
        *pDst++ = (b * c) - (a * d);
        *pDst++ = (b * d) + (a * c);

        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;
        *pDst++ = (b * c) - (a * d);
        *pDst++ = (b * d) + (a * c);

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = numSamples % 0x4U;

    while (blkCnt > 0U)
    {
        /* Imag: C[2 * i    ] = A[2 * i + 1] * B[2 * i    ] - A[2 * i] * B[2 * i + 1]. */
        /* Real: C[2 * i + 1] = A[2 * i + 1] * B[2 * i + 1] + A[2 * i] * B[2 * i    ]. */

        a = *pSrcA++;
        b = *pSrcA++;
        c = *pSrcB++;
        d = *pSrcB++;

        /* store result in destination buffer. */
        *pDst++ = (b * c) - (a * d);
        *pDst++ = (b * d) + (a * c);
        /* Decrement loop counter */
        blkCnt--;
    }
}

/**
  @brief         Floating-point complex dot product.
  @param[in]     pSrcA       points to the first input vector [Im,Re]
  @param[in]     pSrcB       points to the second input vector [Im,Re]
  @param[in]     numSamples  number of samples in each vector
  @param[out]    realResult  real part of the result returned here
  @param[out]    imagResult  imaginary part of the result returned here
  @return        none
 */
void arm_cmplx_dot_prod_f32(
  const float * pSrcA,
  const float * pSrcB,
        uint32_t numSamples,
        float * realResult,
        float * imagResult)
{
    uint32_t blkCnt;                               /* Loop counter */
    float real_sum = 0.0f, imag_sum = 0.0f;    /* Temporary result variables */
    float a0,b0,c0,d0;

    /* Loop unrolling: Compute 4 outputs at a time */
    blkCnt = numSamples >> 2U;

    while (blkCnt > 0U)
    {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += b0 * d0 - a0 * c0;
        imag_sum += a0 * d0 + b0 * c0;

        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += b0 * d0 - a0 * c0;
        imag_sum += a0 * d0 + b0 * c0;

        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += b0 * d0 - a0 * c0;
        imag_sum += a0 * d0 + b0 * c0;

        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += b0 * d0 - a0 * c0;
        imag_sum += a0 * d0 + b0 * c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Loop unrolling: Compute remaining outputs */
    blkCnt = numSamples % 0x4U;

    while (blkCnt > 0U)
    {
        a0 = *pSrcA++;
        b0 = *pSrcA++;
        c0 = *pSrcB++;
        d0 = *pSrcB++;

        real_sum += b0 * d0 - a0 * c0;
        imag_sum += a0 * d0 + b0 * c0;

        /* Decrement loop counter */
        blkCnt--;
    }

    /* Store real and imaginary result in destination buffer. */
    *realResult = real_sum;
    *imagResult = imag_sum;
}


/**
 *  @b Description
 *  @n
 *     Function estimates XYZ coordinates of objects in the object list
 *
 *  @param[in]  elevEstCfg   subframe object
 *  @param[in]  detObjList  Detected object list
 *  @param[out] objOut      List with x, y, z coordinates populated for each object
 *  @param[in]  numObjOut   Number of detected objects
 *  @param[out] finalNumObjOut  Number of validated objects
 *
 *  @retval   None
 *
 * \ingroup DPC_OBJDET__INTERNAL_FUNCTION
 */
extern int32_t DPC_ObjDet_estimateXYZ(DPC_ObjectDetection_ElevEstCfg * restrict elevEstCfg,
                               const DetObjParams * restrict detObjList,
                               DPIF_PointCloudCartesian * restrict objOut,
                               uint32_t numObjOut,
                               uint32_t * restrict finalNumObjOut)
{
    DPC_ObjectDetection_ElevEstCommonCfg *commonCfg = &elevEstCfg->commonCfg;
    DPC_ObjectDetection_ExecuteResult *result = commonCfg->result;
    uint8_t subFrameIdx = result->subFrameIdx;
    DPC_ObjectDetection_ElevEstSubframeCfg *subframeCfg = &elevEstCfg->subframeCfg[subFrameIdx];
    uint16_t azimFFTSize = subframeCfg->numAzimFFTBins;
    const float invAzimFFTSize = divsp(1.0f,(float) azimFFTSize);
    uint32_t objIdx, sampIdx, idx;
    uint32_t maxAzimMaskWidth = 8U * (uint32_t)sizeof(commonCfg->zeroInsrtMaskCfg.zeroInsrtMaskAzim);
    uint32_t maxElevMaskWidth = 8U * (uint32_t)sizeof(commonCfg->zeroInsrtMaskCfg.zeroInsrtMaskElev);
    int32_t currLoc;
    float noisedB, signaldB, snrdB;
    float peakIdxOffset, peakIdxFlt;
    int32_t peakLoc;
    float    azimSinPhase;
    cmplxfUnion_t DFTValAzim, DFTValElev, elevOutput;
    float  elevSinPhase, elevCosPhase;
    float rangeStep, range, dopplerStep, x, ySquared, z;
    float wz,peakLocFlt, peakIdxFlt_DFT;
    int32_t dopIdx;
    uint32_t numDopplerBins = subframeCfg->numDopplerBins;
    uint32_t ValidObjIdx;
    uint32_t temp;

    /* Alignment is to be done because we use the antenna calib params for
     * multiplication, using optimized DSP routines, which require a 8 byte alignment */
    cmplxfImRe_t samplesCalib[MAX_NUM_VIRT_ANT] __attribute__((aligned(8)));
    cmplx32ImRe_t rearrangedAzimSamples[MAX_NUM_AZIM_VIRT_ANT] __attribute__((aligned(8)));
    cmplx32ImRe_t rearrangedElevSamples[MAX_NUM_ELEV_VIRT_ANT] __attribute__((aligned(8)));

    int32_t retVal = 0;
    cmplxfImRe_t dftFactorsAzim[MAX_NUM_AZIM_VIRT_ANT] __attribute__((aligned(8)));
    cmplxfImRe_t dftFactorsElev[MAX_NUM_ELEV_VIRT_ANT] __attribute__((aligned(8)));
    float * azimSamplesCalib = (float *)&samplesCalib[0];
    float * elevSamplesCalib = (float *)&samplesCalib[MAX_NUM_AZIM_VIRT_ANT];

    rangeStep = subframeCfg->rangeStep;
    dopplerStep = subframeCfg->dopplerStep;
    cmplxfImRe_t *antennaCalibParams = (cmplxfImRe_t*)commonCfg->antennaCalibParams;

    /* This variable will index the final object list */
    ValidObjIdx = 0;

    for (objIdx = 0; objIdx < numObjOut; objIdx++)
    {
        /* 1. Interpolate around peak to get fractional estimate of Azimuth index */
        peakIdxOffset = DPC_ObjDet_quadInterpAroundPeak(detObjList[objIdx].azimPeakSamples);

        /* Correct peak index with the fractional index*/
        peakIdxFlt = (float)detObjList[objIdx].azimIdx + peakIdxOffset;
        peakIdxFlt_DFT = ROUND_UNSIGNED(peakIdxFlt * (invAzimFFTSize * (float)AOA_DFT_LEN));
        peakLoc = (int32_t)peakIdxFlt_DFT;


        /* 2a. Calculate DFT Factors corresponding to wx for Row 1.
            *  i.e. calculate \f$\e^{j wx}\f$
            */
        idx = 0;
        for (sampIdx = 0; sampIdx < maxAzimMaskWidth; sampIdx ++)
        {
            if(((commonCfg->zeroInsrtMaskCfg.zeroInsrtMaskAzim >> sampIdx) & 0x1U) != 0U)
            {
                currLoc = (peakLoc * (int32_t)sampIdx) % (int32_t)AOA_DFT_LEN;
                dftFactorsAzim[idx++] = dftSinCosTable[currLoc];
            }

            /* Break the loop after computing all dft factors in azimuth dimension */
            if(idx == MAX_NUM_AZIM_VIRT_ANT)
            {
                break;
            }
        }

        /* 2b. Calculate DFT Factors corresponding to wx for Row 0.
            *  i.e. calculate \f$\e^{j wx}\f$
            */
        idx = 0;
        for (sampIdx = 0; sampIdx < maxElevMaskWidth; sampIdx ++)
        {
            if(((commonCfg->zeroInsrtMaskCfg.zeroInsrtMaskElev >> sampIdx) & 0x1U) != 0U)
            {
                currLoc = (peakLoc * (int32_t)sampIdx) % (int32_t)AOA_DFT_LEN;
                dftFactorsElev[idx++] = dftSinCosTable[currLoc];
            }

            /* Break the loop after computing all dft factors in elevation dimension */
            if(idx == MAX_NUM_ELEV_VIRT_ANT)
            {
                break;
            }    
        }

        /* 2c. Rearrange the antenna samples according to the virtual antenna mapping.  */
        for (sampIdx = 0; sampIdx < MAX_NUM_AZIM_VIRT_ANT; sampIdx ++)
        {
            rearrangedAzimSamples[sampIdx] = detObjList[objIdx].azimSamples[commonCfg->antennaGeometryCfg[sampIdx]];
        }

        for (sampIdx = 0; sampIdx < MAX_NUM_ELEV_VIRT_ANT; sampIdx ++)
        {
            rearrangedElevSamples[sampIdx] = detObjList[objIdx].elevSamples[commonCfg->antennaGeometryCfg[MAX_NUM_AZIM_VIRT_ANT+sampIdx]];
        }

        /* 3. Azimuth Antenna Calibration:
            * Multiply azimuth samples (azimSamples) of Doppler FFT with antenna calib params (antennaCalibParams)
            */
        arm_cmplx_mult((int*)rearrangedAzimSamples, (float*)antennaCalibParams, (float*)azimSamplesCalib, MAX_NUM_AZIM_VIRT_ANT);

        /* 4. Elevation Antenna calibration
            * Multiply elev samples with antenna calib params  */
        arm_cmplx_mult((int*)rearrangedElevSamples, (float*)&antennaCalibParams[MAX_NUM_AZIM_VIRT_ANT], (float*)elevSamplesCalib, MAX_NUM_ELEV_VIRT_ANT);

        /* 5. Single Bin DFT on the azimuth antennas to estimate phase at peak.
            *
            \f[
            X_{azim} (\omega_x) = \sum_{k=0}^{N_{azim} - 1} azimSample(k)  e^{-j k \omega_x}
            \f]
            * Multiply DFT factors with azimuth of Doppler FFT samples corrected for antenna calibration.
            */
        arm_cmplx_dot_prod_f32((float *)&azimSamplesCalib[0], (float *)&dftFactorsAzim[0], MAX_NUM_AZIM_VIRT_ANT, &DFTValAzim.cmplx.real, &DFTValAzim.cmplx.imag);


        /* 6.  Single Bin DFT on the elevation antennas to estimate phase at peak.
            *
            \f[
            X_{elev} (\omega_x) = \sum_{k=0}^{N_{elev} - 1} elevSample(k)  e^{-j (k+2) \omega_x}
            \f]
            * The elevation antennas (essentially the 4 virtual antennas corresponding to the
            * elevation offset Tx antenna) are 4 in number and offset by 3 positions from the
            * azimuth virtual array. Hence when the DFT is computed, begin from the 3rd DFT parameter.
            *
            * Both elevSamplesCalib and dftFactorsElev[4] are double-word aligned. */
        arm_cmplx_dot_prod_f32((float *)&elevSamplesCalib[0], (float *)&dftFactorsElev[0], MAX_NUM_ELEV_VIRT_ANT, &DFTValElev.cmplx.real, &DFTValElev.cmplx.imag);

        /* 7. Estimate phase difference between the peak location at azimuth antennas and elevation antennas at peak.
            *  - 1. compute the conjugate product to get the phase difference (i.e. AzimVal * conj(ElevVal)) */
        arm_cmplx_conj_mult_cmplx_f32 ((float*)&DFTValElev.cmplx, (float*)&DFTValAzim.cmplx, (float*)&elevOutput.cmplx, 1);


        /* - 2. Compute the angle of the product to estimate the phase change in elevation.
            \f[
            \omega_z = angle (\ X_{elev} (\omega_x)' \times X_{azim} (\omega_x) )\
            \f]
        */
        if (fabsf(elevOutput.cmplx.imag) < (0.15f*fabsf(elevOutput.cmplx.real)))
        {
            // small angle approximation.
            wz = divsp(elevOutput.cmplx.imag, elevOutput.cmplx.real);
        }
        else
        {
            wz = (float)atan2(elevOutput.cmplx.imag, elevOutput.cmplx.real);
            if (wz > PI_)
            {
                    wz -= 2.0f*PI_;
            }
        }

        /* 8. Obtain range using the range resolution and the range Index  */
        range = rangeStep * (float)detObjList[objIdx].rangeIdx;

        /* 9. Obtain z, x coordinates.
            \f[
            \Phi = asin(\frac{\omega_z}{2 \pi d_z})
            \f]

        \f[
            z = range \times sin(\phi) = range * \frac{\omega_z}{2 \pi d_z}
        \f]

        */
        elevSinPhase = wz * (1.0f / (2.0f * PI_ * commonCfg->antennaSpacing.zSpacingByLambda));
        if ((elevSinPhase > subframeCfg->aoaFovSinVal.minElevationDeg) && (elevSinPhase < subframeCfg->aoaFovSinVal.maxElevationDeg))
        {

            z = range * elevSinPhase;

            /*
            \f[
                x = range  cos(\phi)  sin(\theta) =  range  /frac{\omega_x}{2 \pi d_x}
            \f]

            */
            peakLocFlt = (float)peakLoc * (1.0f / (float)AOA_DFT_LEN);
            if (peakLocFlt > 0.5f)
            {
                peakLocFlt -= 1.0f;
            }

            x = range * peakLocFlt * (1.0f / commonCfg->antennaSpacing.xSpacingByLambda);

            /* Obtain 'square of y' coordinate
            \f[
                y^2 = range^2 -x^2 - z^2
            \f]
            */
            ySquared = (range * range) - (z * z) - (x * x);

            /* It is possible that ySquared is less than zero (i.e. a degenerate case). In such a case ignore the object.
            * If the case is not degenerate, proceed to check if the object is in the field of view (FoV).
            * If so , store the newly validated object in the final object list.*/
            if (ySquared > 0.0F)
            {
                /* Estimate azimuth phase.
                \f[
                    sin(\Theta) = \frac{x}{range \times cos(\Phi)}
                \f]
                */
                elevCosPhase = (float)sqrt(1.0 - ((double)elevSinPhase * (double)elevSinPhase));
                azimSinPhase = divsp(x, (range * elevCosPhase));

                /* Check if object is in azimuth FoV, If object is in FoV, proceed to store the coordinates in the final object list */
                if ((azimSinPhase > subframeCfg->aoaFovSinVal.minAzimuthDeg) && (azimSinPhase < subframeCfg->aoaFovSinVal.maxAzimuthDeg))
                {


                    /* Store x, y, z values */
                    objOut[ValidObjIdx].z = z;
                    objOut[ValidObjIdx].x = x;
                    objOut[ValidObjIdx].y = (float)sqrt(ySquared);

                    /* Obtain and store Velocity */
                    if (detObjList[objIdx].dopIdxActual > numDopplerBins / 2U)
                    {
                        dopIdx = (int32_t)detObjList[objIdx].dopIdxActual - (int32_t)numDopplerBins;
                    }
                    else
                    {
                        dopIdx = (int32_t)detObjList[objIdx].dopIdxActual;
                    }
                    objOut[ValidObjIdx].velocity = (float)dopIdx * dopplerStep;

                    /* Calcute the side info of final detected object */
                    /* output is 20*log10(2)*value/2^(QVALUE) */
                    temp = (uint32_t)1U << QVALUE_NOISE;
                    noisedB = 6.0F * ((float)detObjList[objIdx].dopCfarNoise) * (1.0f/(float)(temp));
                    temp = (uint32_t)1U << QVALUE_SIGNAL;
                    signaldB = 6.0F * ((float)detObjList[objIdx].azimPeakSamples[1]) * (1.0f/(float)(temp));
                    snrdB = signaldB - noisedB;

                    result->objOutSideInfo[ValidObjIdx].snr = 10 * (int16_t)snrdB;
                    result->objOutSideInfo[ValidObjIdx].noise = 10 * (int16_t)noisedB;

                    /* Increment output list index */
                    ValidObjIdx++;
                } /* End of azimuth FoV check cond */
            } /* End of degenerate condition check */
        } /* End of elevation FoV check cond */
    }
    *finalNumObjOut = ValidObjIdx;
    return retVal;
}
#endif
