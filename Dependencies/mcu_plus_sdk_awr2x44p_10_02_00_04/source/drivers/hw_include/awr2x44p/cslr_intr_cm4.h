
/********************************************************************
* Copyright (C) 2024 Texas Instruments Incorporated.
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
*    distribution.efine
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
*
*/
#ifndef CSLR_INTR_CM4_H_
#define CSLR_INTR_CM4_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* List of intr sources for receiver: CM4 */
#define CSL_CM4_INTR_RSS_CSI2A_EOL_CNTX0_INT	            16		/*	RSS_CSI2A End of Line Interrupt for Context 0	*/
#define CSL_CM4_INTR_RSS_CSI2A_EOL_CNTX1_INT	            17		/*	RSS_CSI2A End of Line Interrupt for Context 1	*/
#define CSL_CM4_INTR_RSS_CSI2A_EOL_CNTX2_INT	            18		/*	RSS_CSI2A End of Line Interrupt for Context 2	*/
#define CSL_CM4_INTR_RSS_CSI2A_EOL_CNTX3_INT	            19		/*	RSS_CSI2A End of Line Interrupt for Context 3	*/
#define CSL_CM4_INTR_RSS_CSI2A_EOL_CNTX4_INT	            20		/*	RSS_CSI2A End of Line Interrupt for Context 4	*/
#define CSL_CM4_INTR_RSS_CSI2A_EOL_CNTX5_INT	            21		/*	RSS_CSI2A End of Line Interrupt for Context 5	*/
#define CSL_CM4_INTR_RSS_CSI2A_EOL_CNTX6_INT	            22		/*	RSS_CSI2A End of Line Interrupt for Context 6	*/
#define CSL_CM4_INTR_RSS_CSI2A_EOL_CNTX7_INT	            23		/*	RSS_CSI2A End of Line Interrupt for Context 7	*/
#define CSL_CM4_INTR_RSS_TPCC_A_INTAGG	                    24		/*	RSS_TPCC_A Aggregated Functional Interrupt	*/
#define CSL_CM4_INTR_RSS_TPCC_A_ERRAGG	                    25		/*	RSS_TPCC_A Aggregated Error Interrupt	*/
#define CSL_CM4_INTR_ADC_VALID_FALL_EDGE	                26		/*	Interrupt is trigger during Falling edge of ADC valid	*/
#define CSL_CM4_INTR_DFE_FRAME_START_TO_MSS	                27		/*	Frame start interrupt from BSS which is masked with “BSS_GPCFG:: MSS_FS_INTR_MASK” register	*/
#define CSL_CM4_INTR_DFE_CHIRP_CYCLE_START	                28		/*	Chirp cycle start interrupt from dfe	*/
#define CSL_CM4_INTR_DFE_CHIRP_CYCLE_END	                29		/*	Chirp cycle end interrupt from dfe	*/
#define CSL_CM4_INTR_DFE_END_OF_FRAME	                    30		/*	End of Frame interrupt from Dfe	*/
#define CSL_CM4_INTR_RSS_FRC_FRAME_START	                31		/*	Frame start interrupt from FRC 	*/
#define CSL_CM4_INTR_RSS_CSI2A_SOF_INT0	                    32		/*	RSS_CSI2A Frame Start Interrupt 0(Selective frame start based on Register RSS_CSI2A_CFG in RSS_CTRL)	*/
#define CSL_CM4_INTR_RSS_CSI2A_SOF_INT1	                    33		/*	RSS_CSI2A Frame Start Interrupt 1(Selective frame start based on Register RSS_CSI2A_CFG in RSS_CTRL)	*/
#define CSL_CM4_INTR_RSS_ADC_CAPTURE_COMPLETE_DITH	        34		/*	ADC capture complete interrupt from DFE-DSP bridge after dithering 	*/
#define CSL_CM4_INTR_RSS_DATA_CAPTURE_ENABLE_FALL	        35		/*	Interrupt is triggered data_capute enable fall from DFE-DSP bridge (toggles after completion of every chirp)	*/
#define CSL_CM4_INTR_DSS_HWA_THREAD1_LOOP_INT	            36		/*	DSS_HWA  thread 1  Loop complete interrupt	*/
#define CSL_CM4_INTR_DSS_HWA_PARAM_DONE_INTR1	            37		/*	DSS_HWA Param done interrupt1	*/
#define CSL_CM4_INTR_DSS_HWA_THREAD2_LOOP_INT	            38		/*	DSS_HWA thread 2  Loop complete interrupt	*/
#define CSL_CM4_INTR_DSS_HWA_PARAM_DONE_INTR2	            39		/*	DSS_HWA  Param done interrupt2	*/
#define CSL_CM4_INTR_DSS_HWA_LOCAL_RAM_ERR	                40		/*	DSS_HWA Local RAM access error	*/
#define CSL_CM4_INTR_DSS_HWA_SPARE0	                        41		/*	DSS_HWA Spare 0	*/
#define CSL_CM4_INTR_DSS_HWA_SPARE1	                        42		/*	DSS_HWA Spare 1	*/
#define CSL_CM4_INTR_DSS_HWA_DMA_REQ_ORED	                43		/*	DSS_HWA DMA 32 Request Lines Ored	*/
#define CSL_CM4_INTR_DSS_HWA_ERR_ORED	                    44		/*	DSS_HWA ESM Group1 and Group2 Errors Ored	*/
#define CSL_CM4_INTR_MSS_MCANA	                            45		/*	MSS_MCANA Interrupt	*/
#define CSL_CM4_INTR_MSS_MCANB	                            46		/*	MSS_MCANB Interrupt	*/
#define CSL_CM4_INTR_MSS_CPSW_TH_TRSH_INT	                47		/*	MSS CPSW T-host threshold interrupt	*/
#define CSL_CM4_INTR_DSS_CM4_MBOX_READ_REQ	                48		/*	DSS CM4 Mailbox Read Request	*/
#define CSL_CM4_INTR_DSS_CM4_MBOX_READ_ACK                	49		/*	DSS CM4 Mailbox Read Acknowledge	*/
#define CSL_CM4_INTR_DSS_WDT_NMI_REQ	                    50		/*	DSS_WDT Non Maskable Interrupt	*/
#define CSL_CM4_INTR_DSS_CM4_UERR_AGG	                    51		/*	DSS CM4 uncorrectbale aggregated error. Refer DSS_CM4_CTRL:HWA_CM4_ECC_ERRAGG for more details	*/
#define CSL_CM4_INTR_DSS_RTIA_0                           	52		/*	DSS_RTIA Interrupt 0	*/
#define CSL_CM4_INTR_DSS_RTIA_1                           	53		/*	DSS_RTIA Interrupt 1	*/
#define CSL_CM4_INTR_DSS_RTIA_2                          	54		/*	DSS_RTIA Interrupt 2	*/
#define CSL_CM4_INTR_DSS_RTIA_3                          	55		/*	DSS_RTIA Interrupt 3	*/
#define CSL_CM4_INTR_DSS_RTIA_OVERFLOW_0                 	56		/*	DSS_RTIA Overflow 0	*/
#define CSL_CM4_INTR_DSS_RTIA_OVERFLOW_1                 	57		/*	DSS_RTIA Overflow 1	*/
#define CSL_CM4_INTR_DSS_CM4_STC_DONE	                    58		/*	DSS_CM4 STC Done interrupt	*/
#define CSL_CM4_INTR_DSS_CM4_PERIPH_ACCESS_ERRAGG        	59		/*	Aggregation of access-errros from DSS CM4 peripherals. See Error access Response Section for more details 	*/
#define CSL_CM4_INTR_DSS_CM4_AHB_ACCESS_ERR_AGG	            60		/*	Write access error on the processor AHB Master ports  	*/
#define CSL_CM4_INTR_DSS_CM4_CTI_TRIGOUT2	                61		/*	DSS CM4 CTI Trigout 2	*/
#define CSL_CM4_INTR_DSS_CM4_CTI_TRIGOUT3	                62		/*	DSS CM4 CTI Trigout 3	*/
#define CSL_CM4_INTR_DSS_CM4_SW_INT0	                    63		/*	Software Interrupt from DSS_CM4_CTRL.HWA_CM4_IRQ_REQ[0]	*/
#define CSL_CM4_INTR_DSS_CM4_SW_INT1	                    64		/*	Software Interrupt from DSS_CM4_CTRL.HWA_CM4_IRQ_REQ[1]	*/
#define CSL_CM4_INTR_DSS_TPCC_A_INTAGG	                    65		/*	DSS_TPCC_A Aggregated Functional Interrupt	*/
#define CSL_CM4_INTR_DSS_TPCC_B_INTAGG	                    66		/*	DSS_TPCC_B Aggregated Functional Interrupt	*/
#define CSL_CM4_INTR_DSS_TPCC_C_INTAGG                    	67		/*	DSS_TPCC_C Aggregated Functional Interrupt	*/
#define CSL_CM4_INTR_DSS_DSP_SW_INT0                       	68		/*	Software Interrupt from DSS_CTRL.DSS_SW_INT[0]	*/
#define CSL_CM4_INTR_DSS_DSP_SW_INT1                       	69		/*	Software Interrupt from DSS_CTRL.DSS_SW_INT[1]	*/
#define CSL_CM4_INTR_MSS_CR5_SW_INT0	                    70		/*	Software Interrupt from MSS_CTRL	*/
#define CSL_CM4_INTR_MSS_CR5_SW_INT1	                    71		/*	Software Interrupt from MSS_CTRL	*/
#define CSL_CM4_INTR_RSS_ADC_CAPTURE_COMPLETE             	72		/*	Raw ADC capture complete interrupt from DFE-DSP bridge	*/

#ifdef __cplusplus
}
#endif
#endif /* CSLR_INTR_CM4_H_*/
