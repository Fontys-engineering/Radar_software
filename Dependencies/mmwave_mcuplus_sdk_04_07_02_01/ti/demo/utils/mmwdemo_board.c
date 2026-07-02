/**
 *   @file  mmwdemo_board.c
 *
 *   @brief
 *      The file implements the functions which are required to access QSPI flash 
 *   from mmw demo.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2021 Texas Instruments, Inc.
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
#include <stdio.h>

/* mmWave PDK Include Files: */
// #include <ti/csl/soc.h>
#include <source/drivers/hw_include/cslr.h>

/* mmWave SDK Include Files: */
#include <ti/common/syscommon.h>
#include <ti/demo/utils/mmwdemo_board.h>

uint32_t countTrailingZero(uint32_t x)
{

  if(x==0){
      return 0;
  }
  uint32_t count = 0;
  while ((x & 1) == 0)
  {
      x = x >> 1;
      count++;
  }
  return count;
}

// Function to enable the clock gating
// Usage example: mmwDemo_clockGatingEnable(MSS_SPIB_CLK_GATE_ENABLE_MASK | MSS_SCIB_CLK_GATE_ENABLE_MASK)
void mmwDemo_clockGatingEnable(uint32_t clockGateEnableMask, uint32_t regStartAddress){

    uint8_t count = 0;
    uint32_t toGate;
    volatile uint32_t regAddr;
    volatile uint32_t regVal;
    while (clockGateEnableMask) {
        toGate = clockGateEnableMask & 1;
        if(toGate){
            // Find the correct register address to gate
            regAddr = (regStartAddress + (4 * count));
            regVal = CSL_REG32_RD(regAddr);
            // Write 7 to clock gate enable register
            CSL_REG32_WR(regAddr, (regVal | CLOCK_GATE_ENABLE_VALUE));
        }
        count += 1;
        clockGateEnableMask >>= 1;
    }

    return;
}

// Function to disable the clock gating
// Usage example: mmwDemo_clockGatingDisable(MSS_SPIB_CLK_GATE_ENABLE_MASK | MSS_SCIB_CLK_GATE_ENABLE_MASK)
void mmwDemo_clockGatingDisable(uint32_t clockGateDisableMask, uint32_t regStartAddress){

    uint8_t count = 0;
    uint32_t toUngate;
    uint32_t regAddr;
    volatile uint32_t regVal;
    while (clockGateDisableMask) {
        toUngate = clockGateDisableMask & 1;
        if(toUngate){
            // Find the register to ungate
            regAddr = (regStartAddress + 4 * count);
            // printf("Gate Disable. regAddr = %x\n", regAddr);
            regVal = CSL_REG32_RD(regAddr);
            // Write 000b to LSB side of the register
            CSL_REG32_WR(regAddr, (regVal & CLOCK_GATE_DISABLE_VALUE));
        }
        count += 1;
        clockGateDisableMask >>= 1;
    }

    return;
}

void mmwDemo_clockGateEnableFunction(uint32_t clockGateEnableMask, Board_WhichClock whichClock)
{
    switch (whichClock)
    {
        case PER_CLOCK:
            mmwDemo_clockGatingEnable(clockGateEnableMask, 
                                        PER_CLOCK_GATE_REG_START_ADDRESS);
            break;

        case MSS_TOPRCM_CLOCK:
            mmwDemo_clockGatingEnable((clockGateEnableMask & MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS1_MASK)
                                                >> countTrailingZero(MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS1_MASK), 
                                        MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS1);
            mmwDemo_clockGatingEnable((clockGateEnableMask & MSS_TOPRCM_ANA_HSI2DIGCLK_GATE_MASK)
                                                >> countTrailingZero(MSS_TOPRCM_ANA_HSI2DIGCLK_GATE_MASK), 
                                        MSS_TOPRCM_ANA_HSI2DIGCLK_GATE);  
            mmwDemo_clockGatingEnable((clockGateEnableMask & MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS3_MASK)
                                                >> countTrailingZero(MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS3_MASK), 
                                        MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS3);   
            mmwDemo_clockGatingEnable((clockGateEnableMask & MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS4_MASK) 
                                                >> countTrailingZero(MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS4_MASK), 
                                        MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS4);                                
            break;

        case DSS_RCM_CLOCK:
            mmwDemo_clockGatingEnable(clockGateEnableMask, 
                                        DSS_RCM_CLOCK_GATE_REG_START_ADDRESS1);
            break;

        case RSS_RCM_CLOCK:
            mmwDemo_clockGatingEnable(clockGateEnableMask & RSS_RCM_CLOCK_GATE_REG_START_ADDRESS1_MASK, 
                                        RSS_RCM_CLOCK_GATE_REG_START_ADDRESS1);   
            mmwDemo_clockGatingEnable((clockGateEnableMask & RSS_RCM_RSS_FRC_CLK_GATE_MASK)
                                                >> countTrailingZero(RSS_RCM_RSS_FRC_CLK_GATE_MASK), 
                                        RSS_RCM_RSS_FRC_CLK_GATE); 
            break;

        default:
            printf("Invalid Clock\n");
            break;
            
    }
}

void mmwDemo_clockGateDisableFunction(uint32_t clockGateEnableMask, Board_WhichClock whichClock){
    switch (whichClock)
    {
        case PER_CLOCK:
            mmwDemo_clockGatingDisable(clockGateEnableMask, 
                                        PER_CLOCK_GATE_REG_START_ADDRESS);
            break;

        case MSS_TOPRCM_CLOCK:
            mmwDemo_clockGatingDisable(clockGateEnableMask & MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS1_MASK, 
                                        MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS1);
            mmwDemo_clockGatingDisable(clockGateEnableMask & MSS_TOPRCM_ANA_HSI2DIGCLK_GATE_MASK, 
                                        MSS_TOPRCM_ANA_HSI2DIGCLK_GATE);  
            mmwDemo_clockGatingDisable(clockGateEnableMask & MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS3_MASK, 
                                        MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS3);   
            mmwDemo_clockGatingDisable(clockGateEnableMask & MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS4_MASK, 
                                        MSS_TOPRCM_CLOCK_GATE_REG_START_ADDRESS4);                                
            break;

        case DSS_RCM_CLOCK:
            mmwDemo_clockGatingDisable(clockGateEnableMask, 
                                        DSS_RCM_CLOCK_GATE_REG_START_ADDRESS1);
            break;

        case RSS_RCM_CLOCK:
            mmwDemo_clockGatingDisable(clockGateEnableMask & RSS_RCM_CLOCK_GATE_REG_START_ADDRESS1_MASK, 
                                        RSS_RCM_CLOCK_GATE_REG_START_ADDRESS1);   
            mmwDemo_clockGatingDisable(clockGateEnableMask & RSS_RCM_RSS_FRC_CLK_GATE_MASK, 
                                        RSS_RCM_RSS_FRC_CLK_GATE); 
            break;

        default:
            printf("Invalid Clock\n");
            break;
            
    }
}


// Function to enable the clock gating
// Usage example: mmwDemo_clockGatingEnable(MSS_SPIB_CLK_GATE_ENABLE_MASK | MSS_SCIB_CLK_GATE_ENABLE_MASK)
void mmwDemo_HSDIVClockPowerDown(uint32_t clockPowerDownMask, uint32_t regStartAddress){

    uint8_t count = 0;
    uint32_t toGate;
    volatile uint32_t regAddr;
    volatile uint32_t regVal;
    while (clockPowerDownMask) {
        toGate = clockPowerDownMask & 1;
        if(toGate){
            // Find the correct register address to gate
            regAddr = (regStartAddress + (4 * count));
            regVal = CSL_REG32_RD(regAddr);
            // Write 7 to clock gate enable register
            CSL_REG32_WR(regAddr, (regVal | CLOCK_POWER_DOWN_VALUE));
        }
        count += 1;
        clockPowerDownMask >>= 1;
    }

    return;
}


void mmwDemo_HSDIVPowerDownFunction(uint32_t clockPowerDownMask, Board_Which_HSD_Clock whichHSDClock){
    
    switch (whichHSDClock)
    {
        case PLL_CORE_HSD:
            mmwDemo_clockGatingDisable(clockPowerDownMask, 
                                        PLL_CORE_HSDIVIDER_CLKOUT_PD_REG_START_ADDRESS1);
            break;

        case PLL_DSP_HSD:
            mmwDemo_clockGatingDisable(clockPowerDownMask, 
                                        PLL_DSP_HSDIVIDER_CLKOUT_PD_REG_START_ADDRESS1);                         
            break;

        case PLL_PER_HSD:
            mmwDemo_clockGatingDisable(clockPowerDownMask, 
                                        PLL_PER_HSDIVIDER_CLKOUT_PD_REG_START_ADDRESS1);
            break;

        default:
            printf("Invalid Clock\n");
            break;
            
    }

}

