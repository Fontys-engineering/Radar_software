 /*
 *  Copyright (c) Texas Instruments Incorporated 2024
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
 *   @file  vmon.h
 *
 *   @brief
 *      MSS VMONs Self-Test Utility.
 */

 /** @mainpage MSS VMONs Self-Test Utility (AWR294X, AWR2544 and AWR2x44P)
 *
 *      MSS VMONs Self-Test Utility: This utility performs self-test for MSS
 *      Voltage monitors (VDD1.2V UV, VDD1.2V OV, VDDA_OSC 1.8V UV,
 *      VDDS33 3.3V UV). These tests report pass only when ESM interrupt is
 *      triggered and respective VMON status bits are toggled.
 *
 *      This application registers ESM group#2 interrupt using sysconfig tool.
 *      Alternatively, ESM group#1 also can be registered, when configuring ESM
 *      group#1 ANALOG_WU_STATUS_REG_GRP1_MASK register of MSS_CTRL base needs
 *      to be configured accordingly for detection of interrupt.
 *
 */