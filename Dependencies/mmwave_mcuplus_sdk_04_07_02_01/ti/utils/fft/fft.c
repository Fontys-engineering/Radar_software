/**
 *  @file       fft.c
 *
 *  @brief      FFT function
 *
 * \par
 *  NOTE:
 *      (C) Copyright 2020 - 2021 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <math.h>
#include <ti/common/sys_types.h>
#include "fft.h"

/**
 *  @b Description
 *  @n
 *      Calculates 2-point FFT
 *
 *  @param[inout]    x     - input/output floating point complex array of length 2
 *
 *  @retval None
 */
void fft2(cplxf_t *x)
{
    cplxf_t sum;
    cplxf_t diff;

    sum.real = x[0].real + x[1].real;
    sum.imag = x[0].imag + x[1].imag;
    diff.real = x[0].real - x[1].real;
    diff.imag = x[0].imag - x[1].imag;

    x[0] = sum;
    x[1] = diff;
}

/**
 *  @b Description
 *  @n
 *      Initialization of sine table for FFT function
 *      Sine table contains MAX_FFT_SIZE/4+1 sine values from 0 to pi/2
 *      (Table = sin(2*pi*[0:MAX_FFT_SIZE/4]/MAX_FFT_SIZE))
 *
 *  @param[in]    SinCosTab     - Sine table
 *
 *  @retval None
 */
void fftInit(float * SinCosTab)
{
    int32_t i;
    double phaseIncr = 2. * 3.1415926535897 / (double) MAX_FFT_SIZE;

    for (i = 0; i <= (MAX_FFT_SIZE/4); i++)
    {
        SinCosTab[i] = sin(phaseIncr * i);
    }
}

/**
 *  @b Description
 *  @n
 *      Inplace FFT function using lookup table
 *
 *  @param[in]    SinCosTab     - Sine table
 *  @param[inout] x             - input/output floating point complex array
 *  @param[in]    M             - Log2 of the FFT size, M > 1
 *
 *  @pre fftInit has been called
 *
 *  @retval None
 */
void fft(float *SinCosTab, cplxf_t *x, int32_t M)
{
    cplxf_t temp1;
    cplxf_t ww;
    int32_t i,j,k;
    int32_t N = 1 << M;
    int32_t N2 = N/2;
    int32_t L;
    int32_t LE;
    int32_t LE1;

    int32_t stepLUT;

	j = 0;
	for (i=1; i<(N-1); i++)
    {
        k = N2;
        while (k<=j)
        {
            j = j - k;
            k = k/2;
        }
        j = j+k;

        if (i<j)
        {
            temp1.real = (x[j]).real;
            temp1.imag = (x[j]).imag;
            (x[j]).real = (x[i]).real;
            (x[j]).imag = (x[i]).imag;
            (x[i]).real = temp1.real;
            (x[i]).imag = temp1.imag;
        }
    }

	LE = 2;
	LE1 = LE/2;
	for (i=0; i<N; i=i+LE)
    {
        temp1.real = (x[i]).real + (x[i+LE1]).real;
        temp1.imag = (x[i]).imag + (x[i+LE1]).imag;

        (x[i+LE1]).real = (x[i]).real - (x[i+LE1]).real;
        (x[i+LE1]).imag = (x[i]).imag - (x[i+LE1]).imag;

        (x[i]).real = temp1.real;
        (x[i]).imag = temp1.imag;
	}

	LE = 4;
	LE1 = LE/2;
	for (i=0; i<N; i=i+LE)
    {
        temp1.real = (x[i]).real + (x[i+LE1]).real;
        temp1.imag = (x[i]).imag + (x[i+LE1]).imag;

        (x[i+LE1]).real = (x[i]).real - (x[i+LE1]).real;
        (x[i+LE1]).imag = (x[i]).imag - (x[i+LE1]).imag;

        (x[i]).real = temp1.real;
        (x[i]).imag = temp1.imag;

        j = i+1;
        temp1.real = (x[j+LE1]).imag;
        temp1.imag = -(x[j+LE1]).real;

        (x[j+LE1]).real = (x[j]).real - temp1.real;
        (x[j+LE1]).imag = (x[j]).imag - temp1.imag;

        (x[j]).real = (x[j]).real + temp1.real;
        (x[j]).imag = (x[j]).imag + temp1.imag;
	}

	for (L=3; L<=M; L++)
    {
        LE = 1 << L;
        LE1 = LE/2;

        stepLUT = MAX_FFT_SIZE >> L;

        for (i=0; i<N; i=i+LE)
        {
            for (j=i; j<(i+LE1); j++)
            {
                k = j-i;

                k = k*stepLUT;
                {
                    uint32_t sinSign = (k >> (MAX_FFT_SIZE_LOG2-1)) & 0x1;
                    uint32_t sinRev =  (k >> (MAX_FFT_SIZE_LOG2-2)) & 0x1;
                    uint32_t cosSign = sinSign ^ sinRev;
                    uint32_t kRev;

                    k = k & (MAX_FFT_SIZE/4 - 1);
                    kRev = (MAX_FFT_SIZE/4) - k;

                    if (sinRev)
                    {
                        ww.imag = SinCosTab[kRev];
                        ww.real = SinCosTab[k];
                    }
                    else
                    {
                        ww.imag = SinCosTab[k];
                        ww.real = SinCosTab[kRev];
                    }
                    if (!sinSign)
                    {
                        ww.imag = - ww.imag;
                    }
                    if (cosSign)
                    {
                        ww.real = - ww.real;
                    }
                }

                temp1.real = (x[j+LE1]).real*ww.real -
                           (x[j+LE1]).imag*ww.imag;
                temp1.imag = (x[j+LE1]).imag*ww.real +
                           (x[j+LE1]).real*ww.imag;

                (x[j+LE1]).real = (x[j]).real - temp1.real;
                (x[j+LE1]).imag = (x[j]).imag - temp1.imag;

                (x[j]).real = (x[j]).real + temp1.real;
                (x[j]).imag = (x[j]).imag + temp1.imag;
            }
        }
    }
}



