/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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

#ifndef GPADC_SOC_AWR294X_H_
#define GPADC_SOC_AWR294X_H_

#ifdef __cplusplus
extern "C"
{
#endif


/**
 *
 *  Definitions for maximum temperature sensors supported
 *
 *  @{
 */
#define GPADC_MAX_TEMP_SENSORS     (3U)
/** @} */

/** \brief
 *  The Temperature sensor values structure
 */
typedef struct
{
   /** \brief Digital Dsp Temperature value */
   int16_t     DigDspTempValue;
   /** \brief Digital Hwa Temperature value */
   int16_t     DigHwaTempValue;
   /** \brief Digital Hsm Temperature value */
   int16_t     DigHsmTempValue;
} GPADC_TempSensValueType;

/**
 * \brief  Enumeration which describes the temperature sensors available for GPADC measurement
 */
typedef enum
{
    /** \brief   0x0 - DIG_DSP_TEMP_SENSOR */
    GPADC_DIG_DSP_TEMP_SENSOR = 0,
    /** \brief   0x1 - DIG_HWA_TEMP_SENSOR */
    GPADC_DIG_HWA_TEMP_SENSOR ,
    /** \brief   0x2 - DIG_HSM_TEMP_SENSOR */
    GPADC_DIG_HSM_TEMP_SENSOR ,
    /** \brief   0x3 - MAX_TEMP_SENSORS */
    MAX_GPADC_TEMP_SENSORS
}GPADC_TempSensorSrcType;

/**
 * \brief  This API releases Reset for GPADC Analog and Digital Module.
 */
void GPADC_socResetRelease(void);

#ifdef __cplusplus
}
#endif

#endif /* GPADC_SOC_AWR294X_H_ */
