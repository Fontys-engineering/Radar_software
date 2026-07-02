

/*
* TI TSPA License
* TECHNOLOGY AND SOFTWARE PUBLICLY AVAILABLE
* SOFTWARE LICENSE
*
* Copyright (c) 2024, Texas Instruments Incorporated.
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to
* make, have made, use, import, offer to sell and sell ("Utilize") this software,
* but solely to the extent that any such patent is necessary to Utilize the
* software alone. The patent license shall not apply to any combinations which
* include this software.  No hardware per se is licensed hereunder.
*
* Redistribution and use in binary form, without modification, are permitted
* provided that the following conditions are met:
*
* * Redistributions must preserve existing copyright notices and reproduce this
* license (including the above copyright notice and the disclaimer below) in the
* documentation and/or other materials provided with the distribution.
*
* * Neither the name of Texas Instruments Incorporated nor the names of its
* suppliers may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* * No reverse engineering, decompilation, or disassembly of this software is
* permitted.
*
* * Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TIS LICENSORS "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
* EVENT SHALL TI AND TIS LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/*
 * NOTE:
 * This is only a placeholder for awr294x HSMRT image that is loaded via SBL for HS-SE device
 * This needs to be replaced with a valid hsmrt image file. Without valid hsmrt, features for HS-SE devices
 * such as secure boot, mpu firewall settings, hsm client services, crypto examples will not work.
 * Get the TIFS package for AWR294X device from MySecure portal that has the necessary files to generate the
 * hsmRtImg.h to be replaced. The crypto examples and SBL for supporting secure boot can then be rebuilt and executed on HS-SE devices
 * For further information please contact your TI representative
 * or refer https://e2e.ti.com/support/sensors-group/sensors/f/sensors-forum
 */

#ifndef HSMRTIMG_H_
#define HSMRTIMG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define HSMRT_IMG_SIZE_IN_BYTES (0U)

#define HSMRT_IMG { \
    \
} /* 0 byte */

/* This error line is added to ensure invalid HSMRT is not used for HS-SE device. This error line should not exist in valid HSMRT file*/
#if defined (DEVICE_TYPE_HS)
#error "Valid HSMRT not found! Please use valid HSMRT for HS device while building the SBL"
#endif

#ifdef __cplusplus
}
#endif

#endif /* HSMRTIMG_H_ */
