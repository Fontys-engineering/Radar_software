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
 *   @file  mailbox_driver.h
 *
 *   @brief
 *      MSS/DSS - BSS Mailbox Communication Test Utility
 */

 /** @mainpage Mailbox Communication Test Utility (AWR294X, AWR2544 and AWR2x44P)
 *
 *      Mailbox Communication Test Utility: This utility provides an example
 *      for message exchange between MSS/DSS and BSS. This test specifically
 *      issues version command to BSS in a loop of ten and prints the
 *      response from BSS on console.
 *
 *      This test case is a simple example to provide the usage mailbox driver
 *      API's for communicating with BSS.
 *
 * In this test case:
 *
 *  - MSS/DSS comes out of reset and initializes mailbox driver.
 *  - Wait for the BSS to Powerup.
 *  - MSS/DSS writes Version command to BSS mailbox memory.
 *  - Prints the version of HW and Firmware on console.
 *
 * ### Sample Output
 *
 * @code
 *
 * [Cortex_R5_0]
 * ******************************************
 * Debug: MSS Mailbox Driver Test Case.
 * ******************************************
 * Test Iteration    : 1
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * Test Iteration    : 2
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * Test Iteration    : 3
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * Test Iteration    : 4
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * Test Iteration    : 5
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * Test Iteration    : 6
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * Test Iteration    : 7
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * Test Iteration    : 8
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * Test Iteration    : 9
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * Test Iteration    : 10
 * RF H/W Version    : 02.00
 * RF F/W Version    : 02.04.05.03 (20.04.22)
 * RF Patch Version  : 02.04.09.05
 *
 * ------ end of test --------
 * All tests have passed!!
 *
 * @endcode
 *
*/