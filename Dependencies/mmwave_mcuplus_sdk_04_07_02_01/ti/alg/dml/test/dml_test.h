/**
 *   @file  dml_test.h
 *
 *   @brief
 *      Implements Test for DML algorithm.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2025 Texas Instruments, Inc.
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

/**
 * DML test can be run either by generating bij and b1ij in MATLAB or by
 * generating in the test application using DML_generate_bij_and_b1ij. To
 * generate bij and b1ij using MATLAB, run DML_config.m file which generates
 * dml_test_gen.h file. Copy dml_test_gen.h file and paste it in the test
 * folder. To use MATLAB generated values in the test application,
 * USE_GENERATED_BIJ_AND_B1IJ macro should be defined. Whereas if bij and b1ij
 * should be generated in the application, USE_GENERATED_BIJ_AND_B1IJ macro
 * should not be defined. While generating bij and b1ij in the test application,
 * ensure that one of the test (macro TEST_<n>) is enabled.
 *
 *
 * DMLtest_numAntennas 		-> Number of azimuth samples (without zero insertion)
 * DMLzeroInsertionMask		-> Zero insertion mask to describe the antenna pattern.abort
 * 							   If a bit is set, it tells that there is a virtual antenna
 * 							   at that location.
 * DMLVirtualAntennaSize	-> Size of the virtual antenna array (azimuth)
 * DMLAzimFFTBins			-> Number of samples after zero padding, to perform FFT.
 * 							   This value decides the resolution of the DML. This is the
 * 							   total available steering vectors to choose from. (In the
 * 							   given FOV, these vectors are uniformly spaced)
 * DMLtest_n1, DMLtest_n2	-> Represents the range of steering vectors chosen to perform DML
 * 							   {n1, n2} ∈ [0, DMLAzimFFTBins-1]
 * DMLwindowRamOffset		-> Window RAM offset
 * 							   If the algorithm is being used in the processing chain, and
 * 							   the window RAM is already in use, provide the offset where
 * 							   the samples can be loaded.
 * DMLVectorMultRAMOffset	-> Vector multiplication RAM offset
 * 						       If the algorithm is being used in the processing chain, and
 * 							   the Vector Multiplication RAM is already in use, provide the offset
 * 							   from where the samples can be loaded.
 * DMLPeakLoc[2]			-> Simultated objects are present at these indices
 * 							   (range - [0, DMLVirtualAntennaSize))
 */

#ifndef DML_TEST_H
#define DML_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= */
/*                             Include Files                                 */
/* ========================================================================= */

/* Standard include files */
#include <stdint.h>

// #define USE_GENERATED_BIJ_AND_B1IJ
#ifdef USE_GENERATED_BIJ_AND_B1IJ
#include "dml_test_gen.h"
#endif
/* ========================================================================= */
/*                           Macros & Typedefs                               */
/* ========================================================================= */

/**
 * @brief   Use generated bij and b1ij
 * Generating bij and b1ij consumes additional memory. Since bij and b1ij will remain same
 * for a given configuration, it's suggested to generate once and use the generated bij and b1ij
 * in the application.
 */
#ifndef USE_GENERATED_BIJ_AND_B1IJ
/* Define one test which will be used for testing the DML algorithm in the DML test application */
#define TEST_1
// #define TEST_2
// #define TEST_3
// #define TEST_4
// #define TEST_5
// #define TEST_6
#endif
/* ========================================================================= */
/*                            Global Variables                               */
/* ========================================================================= */
#ifndef USE_GENERATED_BIJ_AND_B1IJ

uint32_t DML_scale_factor_b1ij;

#ifdef TEST_1
uint16_t DMLtest_numAntennas = 12;
uint32_t DMLtest_n1 = 0;
uint32_t DMLtest_n2 = 127;
uint64_t DMLzeroInsertionMask = 0xFFF;
uint32_t DMLVirtualAntennaSize = 12;
/* DMLtest_numAntennas + Number of zeros inserted should be equal to DMLVirtualAntennaSize */
uint32_t DMLwindowRamOffset = 0;
uint32_t DMLVectorMultRAMOffset = 0;
uint32_t DMLAzimFFTBins = 128;
uint32_t DMLPeakLoc[2] = {4, 5};
// uint32_t DML_scale_factor_b1ij = 6;

int32_t peak_index = 43;
int32_t peak_cij = 6525;

/* Imaginary part of the samples followed by real part of the samples */
int32_t DMLtest_azimSamples[] =
{
      1301320, 4727899,
      2175684, -3666094,
      -3395320, 787146,
      2167266, 1309432,
      -344522, -1239383,
      88364, -77567,
      -1174712, 311439,
      2263487, 1278614,
      -881302, -3225458,
      -2251048, 3939147,
      4738551, -1338362,
      -4262149, -2336562
};

#elif defined(TEST_2)
uint16_t DMLtest_numAntennas = 12;
uint32_t DMLtest_n1 = 10;
uint32_t DMLtest_n2 = 40;
uint64_t DMLzeroInsertionMask = 0xFFF;
uint32_t DMLVirtualAntennaSize = 12;
uint32_t DMLwindowRamOffset = 0;
uint32_t DMLVectorMultRAMOffset = 0;
uint32_t DMLAzimFFTBins = 64;
uint32_t DMLPeakLoc[2] = {3, 5};

int32_t peak_index = 6;
int32_t peak_cij = 26319;

int32_t DMLtest_azimSamples[] = {
	 1301320,
	 4727899,
	 2512842,
	-2407803,
	-1215895,
	 -471145,
	 -349316,
	-1207150,
	-2523946,
	 2535490,
	 4784370,
	 1180724,
	-1174712,
	-4721726,
	-2432520,
	 2536906,
	 1298122,
	  549416,
	  265534,
	 1422564,
	 2559126,
	-2596653,
	-4599307,
	-1078270
};

#elif defined(TEST_3)
uint16_t DMLtest_numAntennas = 12;
uint32_t DMLtest_n1 = 30;
uint32_t DMLtest_n2 = 25;
uint64_t DMLzeroInsertionMask = 0x36BB9;
uint32_t DMLVirtualAntennaSize = 18;
uint32_t DMLwindowRamOffset = 100;
uint32_t DMLVectorMultRAMOffset = 59;
uint32_t DMLAzimFFTBins = 32;
uint32_t DMLPeakLoc[2] = {7, 5};

int32_t peak_index = 11;
int32_t peak_cij = 173772;

int32_t DMLtest_azimSamples[] = {
	 1341870,
	 4727899,
	  849509,
	 3488857,
	 1546636,
	-1485184,
	 -327022,
	 -191103,
	-1979976,
	 2061673,
	 4025558,
	 1279350,
	-1276290,
	-4531734,
	 4359465,
	 1360649,
	-1572132,
	 1500352,
	  360320,
	  155932,
	 1866706,
	-2048714,
	-3897122,
	 -978729
};

#elif defined(TEST_4)
uint16_t DMLtest_numAntennas = 12;
uint32_t DMLtest_n1 = 0;
uint32_t DMLtest_n2 = 127;
uint64_t DMLzeroInsertionMask = 0x1EBB9;
uint32_t DMLVirtualAntennaSize = 17;
uint32_t DMLwindowRamOffset = 0;
uint32_t DMLVectorMultRAMOffset = 0;
uint32_t DMLAzimFFTBins = 128;
uint32_t DMLPeakLoc[2] = {4, 5};

int32_t peak_index = 32;
int32_t peak_cij = 13283;

int32_t DMLtest_azimSamples[] = {
	 1346655,
	 4727899,
	-2919063,
	 1820765,
	 1531091,
	 2210628,
	 1458898,
	-1329356,
	  -18656,
	   81449,
	 -653108,
	 -245500,
	 -758981,
	 1714791,
	  978849,
	-3011452,
	 -428281,
	 4440732,
	 4750630,
	  116609,
	 -448324,
	-5001279,
	-4994438,
	  838666
};

#elif defined(TEST_5)
uint16_t DMLtest_numAntennas = 12;
uint32_t DMLtest_n1 = 55;
uint32_t DMLtest_n2 = 25;
uint64_t DMLzeroInsertionMask = 0x377F;
uint32_t DMLVirtualAntennaSize = 14;
uint32_t DMLwindowRamOffset = 0;
uint32_t DMLVectorMultRAMOffset = 0;
uint32_t DMLAzimFFTBins = 64;
uint32_t DMLPeakLoc[2] = {3, 13};

int32_t peak_index = 4;
int32_t peak_cij = 43109;

int32_t DMLtest_azimSamples[] = {
	 1300687,
	 4727899,
	 2629393,
	 3178317,
	  165130,
	  -58729,
	-3723965,
	  193788,
	-4288742,
	 2329747,
	-1312507,
	 1814718,
	  414019,
	-2003353,
	-2599007,
	-2857280,
	  -75794,
	   89015,
	 3841328,
	 -222718,
	 1439686,
	-1849257,
	 -392187,
	 1973893
};

#elif defined(TEST_6)
uint16_t DMLtest_numAntennas = 12;
uint32_t DMLtest_n1 = 30;
uint32_t DMLtest_n2 = 25;
uint64_t DMLzeroInsertionMask = 0xDD1B5;
uint32_t DMLVirtualAntennaSize = 20;
uint32_t DMLwindowRamOffset = 0;
uint32_t DMLVectorMultRAMOffset = 0;
uint32_t DMLAzimFFTBins = 128;
uint32_t DMLPeakLoc[2] = {6, 7};

int32_t peak_index = 10;
int32_t peak_cij = 52960;

int32_t DMLtest_azimSamples[] = {
	 1717552,
	 6293235,
	-5211855,
	-2150961,
	 3570955,
	-2281265,
	-2844525,
	-1755289,
	 1256645,
	 -547185,
	 -255518,
	   -8773,
	 1246437,
	-3369218,
	 2665832,
	 4416347,
	 2991219,
	-5045323,
	-6237277,
	 -335257,
	 4298791,
	-5124612,
	-6629799,
	-1303566
};

#endif
#endif

#ifdef __cplusplus
}
#endif

#endif // end of _DML_TEST_H_ definition
