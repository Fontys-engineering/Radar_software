/**
 *   @file  mathutils.c
 *
 *   @brief
 *      This file contains math utility functions.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 - 2025 Texas Instruments, Inc.
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

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include <ti/utils/mathutils/mathutils.h>
#if defined(SUBSYS_MSS) || defined(SUBSYS_M4)
#include <ti_compatibility.h>
#endif

/**
 *  @b Description
 *  @n
 *      Takes a 32bit integer to find floor(log2(x))
 *      Note: returns zero for zero input.
 *
 *  @param[in]  x       32bit integer.
 *
 *  @retval     floor(log2(x))
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint8_t mathUtils_floorLog2(uint32_t x)
{
    uint8_t n;
    uint8_t result = 0U;
    if (x != 0U)
    {
        n = _norm(x);
#ifdef SUBSYS_DSS
        if (n == 0U)
        {
            if (x & 0x80000000)
            {
                n--;
            }
        }
        result = (30U - n); // On DSP _norm(x) counts number of number of redundant sign bits
#else
        result = (31U - n); // On Arm _norm(x) counts number of leading zeros
#endif
    }
    return result;
}

/**
 *  @b Description
 *  @n
 *      Takes a 32bit integer to find ceil(log2(x))
 *      Note: returns zero for zero input.
 *
 *  @param[in]  x       32bit integer.
 *
 *  @retval     ceil(log2(x))
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint8_t mathUtils_ceilLog2(uint32_t x)
{
    uint8_t    idx = 0U;

    if (x != 0U)
    {
        idx = mathUtils_floorLog2(x);

        if((0x1U<<idx) != x)
        {
            idx++;
        }
    }
    return (idx);
}

/**
 *  @b Description
 *  @n
 *      Takes a 32bit integer to roundup value of power of 2.
 *
 *  @param[in]  x       32bit integer.
 *
 *  @retval     power of 2 roundup value.
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint32_t mathUtils_pow2roundup(uint32_t x)
{
    uint32_t power = 1U;

    while(power < x)
        power*=2U;

    return (power);
}

/**
 *  @b Description
 *  @n
 *      Takes a 32bit integer to raise 10 to the power of x, x being an unsigned integer
 *
 *  @param[in]  x       32bit integer.
 *
 *  @retval     power of 10.
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint32_t mathUtils_pow10(uint32_t x)
{
    uint32_t power = 1U;
    uint32_t i;
    for(i = x; i > 0; i--){
        power*=10;
    }

    return (power);
}
/**
 *  @b Description
 *  @n
 *      Function to generate window coefficients for FFT.
 *
 *  @param[out] win Pointer to output calculated window samples in Q15 format.
 *  @param[in]  winLen Length of window.
 *  @param[in]  winGenLen Length of window to be generated.
 *  @param[in]  winType Type of window, one of @ref FFT_WINDOW_TYPES
 *  @param[in]  qFormat Q format of generated window samples
 *  @retval none.
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
void mathUtils_genWindowQ15(uint16_t *win, uint32_t winLen,
                         uint32_t winGenLen, uint32_t winType, uint16_t qFormat)
{
    uint16_t  oneQFormat = (1U << qFormat);
    uint32_t    winIndx;
    float       phi;
    uint16_t    *pWinBuf;

    pWinBuf = win;

    phi = 2.0f * PI_ / ((float) winLen - 1);

    for(winIndx = 0; winIndx < winGenLen; winIndx++)
    {
        if(winType == MATHUTILS_WIN_BLACKMAN)
        {
            //Blackman window
            float a0 = 0.42;
            float a1 = 0.5;
            float a2 = 0.08;
            *pWinBuf = (uint16_t) ((oneQFormat * (a0 - a1*cos(phi * winIndx) +
                a2*cos(2 * phi * winIndx))) + 0.5);
            if(*pWinBuf >= oneQFormat)
            {
                *pWinBuf = oneQFormat - 1;
            }
        }
        else if(winType == MATHUTILS_WIN_HANNING)
        {
            //Hanning window
            *pWinBuf = (uint16_t) ((oneQFormat * 0.5* (1 - cos(phi * winIndx))) + 0.5);
            if(*pWinBuf >= oneQFormat)
            {
                *pWinBuf = oneQFormat - 1;
            }
        }
        else if(winType == MATHUTILS_WIN_RECT)
        {
            //Rectangular window
            *pWinBuf= (uint16_t) (oneQFormat/16);
        }
        pWinBuf++;
    }
}

/**
 *  @b Description
 *  @n
 *      Function to generate window coefficients for FFT.
 *
 *  @param[out] win Pointer to output calculated window samples in Q17 format.
 *  @param[in]  winLen Length of window.
 *  @param[in]  winGenLen Length of window to be generated.
 *  @param[in]  winType Type of window, one of @ref FFT_WINDOW_TYPES
 *  @param[in]  qFormat Q format of generated window samples
 *  @retval none.
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
void mathUtils_genWindow(uint32_t *win, uint32_t winLen,
                         uint32_t winGenLen, uint32_t winType, uint32_t qFormat)
{
    uint32_t  oneQFormat = (1U << qFormat);
    uint32_t    winIndx;
    float       phi;
    uint32_t    *pWinBuf;

    pWinBuf = win;

    phi = 2.0f * PI_ / ((float) winLen - 1);

    for(winIndx = 0; winIndx < winGenLen; winIndx++)
    {
        if(winType == MATHUTILS_WIN_BLACKMAN)
        {
            //Blackman window
            float a0 = 0.42;
            float a1 = 0.5;
            float a2 = 0.08;
            *pWinBuf = (uint32_t) ((oneQFormat * (a0 - a1*cos(phi * winIndx) +
                a2*cos(2 * phi * winIndx))) + 0.5);
            if(*pWinBuf >= oneQFormat)
            {
                *pWinBuf = oneQFormat - 1;
            }
        }
        else if(winType == MATHUTILS_WIN_HANNING)
        {
            //Hanning window
            *pWinBuf = (uint32_t) ((oneQFormat * 0.5* (1 - cos(phi * winIndx))) + 0.5);
            if(*pWinBuf >= oneQFormat)
            {
                *pWinBuf = oneQFormat - 1;
            }
        }
        else if(winType == MATHUTILS_WIN_RECT)
        {
            //Rectangular window
            *pWinBuf= (uint32_t) (oneQFormat/16);
        }
        pWinBuf++;
    }
}

/**
 *  @b Description
 *  @n
 *      Function to return the best FFT size with respect to the HWA.
 *      HWA supports FFT sizes 2^x and 3*(2^x). This function checks which among
 *      2^x or 3*(2^y) is closest to the given input and returns that value.
 *      This would ensure that zero padding is as less as possible.
 *
 *  @param[in]  numSamples Number of input samples.
 *
 *  @retval Best suited FFT size.
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint32_t mathUtils_getValidFFTSize(uint32_t numSamples)
{

    /* Do the minimum amount of zeropadding by checking whether the
        radix 3 mode results in fewer zeros. */

    uint16_t numZeros_rad2_only;
    uint16_t numZeros_rad3_and_rad2;
    uint32_t fftSize = 0;

    numZeros_rad2_only = mathUtils_pow2roundup(numSamples) - numSamples;
    numZeros_rad3_and_rad2 = 3 * mathUtils_pow2roundup(numSamples/3) - numSamples;

    if(numZeros_rad2_only <= numZeros_rad3_and_rad2){
        fftSize = mathUtils_pow2roundup(numSamples);
    }
    else{
        fftSize = 3 * mathUtils_pow2roundup(numSamples/3);
    }

    return fftSize;

}

/**
 *  @b Description
 *  @n
 *  Quantizes a real-world value x to an integer equivalent in the <I.F, S> notation.
 *
 *  @param[in]  inVec Input Vector
 *  @param[out]  outVec Output Vector
 *  @param[in]  numSamples Number of samples
 *  @param[in]  I I value <I.F, S>
 *  @param[in]  F F value <I.F, S>
 *  @param[in]  S Signed
 *
 *  @retval Error code
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
int32_t mathUtils_asymQuantInt(float * inVec, void * outVec, uint32_t numSamples, uint32_t I, uint32_t F, uint32_t S)
{

    uint32_t bits, peak, i;
    int32_t xVal, q;
    int32_t retVal = 0;

    if(inVec == NULL || outVec == NULL){
        retVal = -1;
    }
    else
    {
        /* Number of bits and clip value for the asymmetric quantizer */
        bits = I + F + 1 - S;
        peak = 1 << (I - S);
    
        q = 1 << (bits - 1);
    
        for(i = 0; i < numSamples; i++){
    
            xVal = MATHUTILS_ROUND_FLOAT(q * inVec[i] / peak);
            xVal = (xVal > q - 1) ? (q - 1) : xVal;
            if(S){
                xVal = (xVal < -q) ? (-q) : (xVal);
            }
            else{
                xVal = (xVal < 0) ? (0) : (xVal);
            }
            if (S){
                *((int32_t *)outVec + i) = xVal;
            }
            else{
                *((uint32_t *)outVec + i) = xVal;
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *  Function to get no of set bits in binary representation of positive integer n.
 *
 *  @param[in] n input integer

 *  @retval number of set bits
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint32_t mathUtils_countSetBits(uint64_t n)
{
    uint32_t count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}
