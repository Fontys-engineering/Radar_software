;******************************************************************************
;* TMS320C6x C/C++ Codegen                                          PC v7.4.2 *
;* Date/Time created: Tue May 17 02:35:45 2016                                *
;******************************************************************************
	.compiler_opts --abi=coffabi --c64p_l1d_workaround=off --endian=big --hll_source=on --long_precision_bits=40 --mem_model:code=near --mem_model:const=data --mem_model:data=far --object_format=coff --silicon_version=6600 --symdebug:dwarf 

;******************************************************************************
;* GLOBAL FILE PARAMETERS                                                     *
;*                                                                            *
;*   Architecture      : TMS320C66xx                                          *
;*   Optimization      : Disabled                                             *
;*   Optimizing for    : Compile time, Ease of Development                    *
;*                       Based on options: no -o, no -ms                      *
;*   Endian            : Big                                                  *
;*   Interrupt Thrshld : Disabled                                             *
;*   Data Access Model : Far                                                  *
;*   Pipelining        : Disabled                                             *
;*   Memory Aliases    : Presume are aliases (pessimistic)                    *
;*   Debug Info        : DWARF Debug                                          *
;*                                                                            *
;******************************************************************************

	.asg	A15, FP
	.asg	B14, DP
	.asg	B15, SP
	.global	$bss


$C$DW$CU	.dwtag  DW_TAG_compile_unit
	.dwattr $C$DW$CU, DW_AT_name("C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c")
	.dwattr $C$DW$CU, DW_AT_producer("TMS320C6x C/C++ Codegen PC v7.4.2 Copyright (c) 1996-2012 Texas Instruments Incorporated")
	.dwattr $C$DW$CU, DW_AT_TI_version(0x01)
	.dwattr $C$DW$CU, DW_AT_comp_dir("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug")
;*****************************************************************************
;* CINIT RECORDS                                                             *
;*****************************************************************************
	.sect	".cinit"
	.align	8
	.field  	$C$IR_1,32
	.field  	_output+0,32
	.field	_output_data,32		; _output[0] @ 0
	.field	_output_data+16000,32		; _output[1] @ 32
	.field	_output_data+32000,32		; _output[2] @ 64
	.field	_output_data+48000,32		; _output[3] @ 96
	.field	_output_data+64000,32		; _output[4] @ 128
$C$IR_1:	.set	20

	.sect	".cinit"
	.align	8
	.field  	$C$IR_2,32
	.field  	_input+0,32
	.field	_input_data,32		; _input[0] @ 0
	.field	_input_data+16000,32		; _input[1] @ 32
$C$IR_2:	.set	8


$C$DW$1	.dwtag  DW_TAG_subprogram, DW_AT_name("fabs")
	.dwattr $C$DW$1, DW_AT_TI_symbol_name("_fabs")
	.dwattr $C$DW$1, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$1, DW_AT_declaration
	.dwattr $C$DW$1, DW_AT_external
$C$DW$2	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$2, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$1


$C$DW$3	.dwtag  DW_TAG_subprogram, DW_AT_name("__isinf")
	.dwattr $C$DW$3, DW_AT_TI_symbol_name("___isinf")
	.dwattr $C$DW$3, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$3, DW_AT_declaration
	.dwattr $C$DW$3, DW_AT_external
$C$DW$4	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$4, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$3


$C$DW$5	.dwtag  DW_TAG_subprogram, DW_AT_name("__isnan")
	.dwattr $C$DW$5, DW_AT_TI_symbol_name("___isnan")
	.dwattr $C$DW$5, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$5, DW_AT_declaration
	.dwattr $C$DW$5, DW_AT_external
$C$DW$6	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$6, DW_AT_type(*$C$DW$T$21)
	.dwendtag $C$DW$5


$C$DW$7	.dwtag  DW_TAG_subprogram, DW_AT_name("rand")
	.dwattr $C$DW$7, DW_AT_TI_symbol_name("_rand")
	.dwattr $C$DW$7, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$7, DW_AT_declaration
	.dwattr $C$DW$7, DW_AT_external
	.global	_a_ext
_a_ext:	.usect	".far",16000,8
$C$DW$8	.dwtag  DW_TAG_variable, DW_AT_name("a_ext")
	.dwattr $C$DW$8, DW_AT_TI_symbol_name("_a_ext")
	.dwattr $C$DW$8, DW_AT_location[DW_OP_addr _a_ext]
	.dwattr $C$DW$8, DW_AT_type(*$C$DW$T$33)
	.dwattr $C$DW$8, DW_AT_external
	.global	_b_ext
_b_ext:	.usect	".far",16000,8
$C$DW$9	.dwtag  DW_TAG_variable, DW_AT_name("b_ext")
	.dwattr $C$DW$9, DW_AT_TI_symbol_name("_b_ext")
	.dwattr $C$DW$9, DW_AT_location[DW_OP_addr _b_ext]
	.dwattr $C$DW$9, DW_AT_type(*$C$DW$T$33)
	.dwattr $C$DW$9, DW_AT_external
	.global	_a_sc
_a_sc:	.usect	".far",800,8
$C$DW$10	.dwtag  DW_TAG_variable, DW_AT_name("a_sc")
	.dwattr $C$DW$10, DW_AT_TI_symbol_name("_a_sc")
	.dwattr $C$DW$10, DW_AT_location[DW_OP_addr _a_sc]
	.dwattr $C$DW$10, DW_AT_type(*$C$DW$T$38)
	.dwattr $C$DW$10, DW_AT_external
	.global	_b_sc
_b_sc:	.usect	".far",800,8
$C$DW$11	.dwtag  DW_TAG_variable, DW_AT_name("b_sc")
	.dwattr $C$DW$11, DW_AT_TI_symbol_name("_b_sc")
	.dwattr $C$DW$11, DW_AT_location[DW_OP_addr _b_sc]
	.dwattr $C$DW$11, DW_AT_type(*$C$DW$T$38)
	.dwattr $C$DW$11, DW_AT_external
	.global	_output_data
_output_data:	.usect	".far",80000,8
$C$DW$12	.dwtag  DW_TAG_variable, DW_AT_name("output_data")
	.dwattr $C$DW$12, DW_AT_TI_symbol_name("_output_data")
	.dwattr $C$DW$12, DW_AT_location[DW_OP_addr _output_data]
	.dwattr $C$DW$12, DW_AT_type(*$C$DW$T$34)
	.dwattr $C$DW$12, DW_AT_external
	.global	_output
_output:	.usect	".far",20,8
$C$DW$13	.dwtag  DW_TAG_variable, DW_AT_name("output")
	.dwattr $C$DW$13, DW_AT_TI_symbol_name("_output")
	.dwattr $C$DW$13, DW_AT_location[DW_OP_addr _output]
	.dwattr $C$DW$13, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$13, DW_AT_external
	.global	_input_data
_input_data:	.usect	".far",32000,8
$C$DW$14	.dwtag  DW_TAG_variable, DW_AT_name("input_data")
	.dwattr $C$DW$14, DW_AT_TI_symbol_name("_input_data")
	.dwattr $C$DW$14, DW_AT_location[DW_OP_addr _input_data]
	.dwattr $C$DW$14, DW_AT_type(*$C$DW$T$36)
	.dwattr $C$DW$14, DW_AT_external
	.global	_input
_input:	.usect	".far",8,8
$C$DW$15	.dwtag  DW_TAG_variable, DW_AT_name("input")
	.dwattr $C$DW$15, DW_AT_TI_symbol_name("_input")
	.dwattr $C$DW$15, DW_AT_location[DW_OP_addr _input]
	.dwattr $C$DW$15, DW_AT_type(*$C$DW$T$32)
	.dwattr $C$DW$15, DW_AT_external
;	C:\MATHLIB_Tools\CCSV5_5_0\ccsv5\tools\compiler\c6000_7.4.2\bin\acp6x.exe -@C:\\Users\\gtbldadm\\AppData\\Local\\Temp\\0278814 
	.sect	".text"
	.clink
	.global	_gimme_random

$C$DW$16	.dwtag  DW_TAG_subprogram, DW_AT_name("gimme_random")
	.dwattr $C$DW$16, DW_AT_low_pc(_gimme_random)
	.dwattr $C$DW$16, DW_AT_high_pc(0x00)
	.dwattr $C$DW$16, DW_AT_TI_symbol_name("_gimme_random")
	.dwattr $C$DW$16, DW_AT_external
	.dwattr $C$DW$16, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$16, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c")
	.dwattr $C$DW$16, DW_AT_TI_begin_line(0x48)
	.dwattr $C$DW$16, DW_AT_TI_begin_column(0x08)
	.dwattr $C$DW$16, DW_AT_TI_max_frame_size(0x18)
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 73,column 1,is_stmt,address _gimme_random

	.dwfde $C$DW$CIE, _gimme_random
$C$DW$17	.dwtag  DW_TAG_formal_parameter, DW_AT_name("range")
	.dwattr $C$DW$17, DW_AT_TI_symbol_name("_range")
	.dwattr $C$DW$17, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$17, DW_AT_location[DW_OP_reg4]
$C$DW$18	.dwtag  DW_TAG_formal_parameter, DW_AT_name("offset")
	.dwattr $C$DW$18, DW_AT_TI_symbol_name("_offset")
	.dwattr $C$DW$18, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$18, DW_AT_location[DW_OP_reg20]

;******************************************************************************
;* FUNCTION NAME: gimme_random                                                *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,DP,SP,A16,A17,A18,A19,A20,A21,A22,A23,  *
;*                           A24,A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19, *
;*                           B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31  *
;*   Local Frame Size  : 0 Args + 20 Auto + 4 Save = 24 byte                  *
;******************************************************************************
_gimme_random:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 40
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           STW     .D2T2   B3,*SP--(24)      ; |73| 
	.dwcfi	cfa_offset, 24
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$19	.dwtag  DW_TAG_variable, DW_AT_name("range")
	.dwattr $C$DW$19, DW_AT_TI_symbol_name("_range")
	.dwattr $C$DW$19, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$19, DW_AT_location[DW_OP_breg31 8]
$C$DW$20	.dwtag  DW_TAG_variable, DW_AT_name("offset")
	.dwattr $C$DW$20, DW_AT_TI_symbol_name("_offset")
	.dwattr $C$DW$20, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$20, DW_AT_location[DW_OP_breg31 16]

           STDW    .D2T1   A5:A4,*+SP(8)     ; |73| 
||         DADD    .L1X    0,B5:B4,A5:A4     ; |73| 

           STDW    .D2T1   A5:A4,*+SP(16)    ; |73| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 74,column 3,is_stmt
$C$DW$21	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$21, DW_AT_low_pc(0x00)
	.dwattr $C$DW$21, DW_AT_name("_rand")
	.dwattr $C$DW$21, DW_AT_TI_call
           CALLP   .S2     _rand,B3
$C$RL0:    ; CALL OCCURS {_rand} {0}         ; |74| 
           MVKL    .S2     0x40dfffc0,B5
           MVKH    .S2     0x40dfffc0,B5
$C$DW$22	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$22, DW_AT_low_pc(0x00)
	.dwattr $C$DW$22, DW_AT_name("__divd")
	.dwattr $C$DW$22, DW_AT_TI_call

           CALLP   .S2     __divd,B3
||         INTDP   .L1     A4,A5:A4          ; |74| 
||         ZERO    .L2     B4                ; |74| 

$C$RL1:    ; CALL OCCURS {__divd} {0}        ; |74| 
           LDDW    .D2T2   *+SP(8),B7:B6     ; |74| 
           LDDW    .D2T2   *+SP(16),B5:B4    ; |74| 
           NOP             3
           FMPYDP  .M1X    B7:B6,A5:A4,A5:A4 ; |74| 
           NOP             3
           FADDDP  .L1X    B5:B4,A5:A4,A5:A4 ; |74| 
           NOP             2
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 75,column 1,is_stmt
           LDW     .D2T2   *++SP(24),B3      ; |75| 
           NOP             4
	.dwcfi	cfa_offset, 0
	.dwcfi	restore_reg, 19
	.dwcfi	cfa_offset, 0
$C$DW$23	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$23, DW_AT_low_pc(0x00)
	.dwattr $C$DW$23, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |75| 
           ; BRANCH OCCURS {B3}              ; |75| 
	.dwattr $C$DW$16, DW_AT_TI_end_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c")
	.dwattr $C$DW$16, DW_AT_TI_end_line(0x4b)
	.dwattr $C$DW$16, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$16

	.sect	".text"
	.clink
	.global	_isequal

$C$DW$24	.dwtag  DW_TAG_subprogram, DW_AT_name("isequal")
	.dwattr $C$DW$24, DW_AT_low_pc(_isequal)
	.dwattr $C$DW$24, DW_AT_high_pc(0x00)
	.dwattr $C$DW$24, DW_AT_TI_symbol_name("_isequal")
	.dwattr $C$DW$24, DW_AT_external
	.dwattr $C$DW$24, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$24, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c")
	.dwattr $C$DW$24, DW_AT_TI_begin_line(0x4f)
	.dwattr $C$DW$24, DW_AT_TI_begin_column(0x05)
	.dwattr $C$DW$24, DW_AT_TI_max_frame_size(0x70)
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 80,column 1,is_stmt,address _isequal

	.dwfde $C$DW$CIE, _isequal
$C$DW$25	.dwtag  DW_TAG_formal_parameter, DW_AT_name("arg1")
	.dwattr $C$DW$25, DW_AT_TI_symbol_name("_arg1")
	.dwattr $C$DW$25, DW_AT_type(*$C$DW$T$26)
	.dwattr $C$DW$25, DW_AT_location[DW_OP_reg4]
$C$DW$26	.dwtag  DW_TAG_formal_parameter, DW_AT_name("arg2")
	.dwattr $C$DW$26, DW_AT_TI_symbol_name("_arg2")
	.dwattr $C$DW$26, DW_AT_type(*$C$DW$T$26)
	.dwattr $C$DW$26, DW_AT_location[DW_OP_reg20]
$C$DW$27	.dwtag  DW_TAG_formal_parameter, DW_AT_name("size")
	.dwattr $C$DW$27, DW_AT_TI_symbol_name("_size")
	.dwattr $C$DW$27, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$27, DW_AT_location[DW_OP_reg6]
$C$DW$28	.dwtag  DW_TAG_formal_parameter, DW_AT_name("tol")
	.dwattr $C$DW$28, DW_AT_TI_symbol_name("_tol")
	.dwattr $C$DW$28, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$28, DW_AT_location[DW_OP_reg22]
$C$DW$29	.dwtag  DW_TAG_formal_parameter, DW_AT_name("flags")
	.dwattr $C$DW$29, DW_AT_TI_symbol_name("_flags")
	.dwattr $C$DW$29, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$29, DW_AT_location[DW_OP_reg8]

;******************************************************************************
;* FUNCTION NAME: isequal                                                     *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,B10,SP,A16,A17,A18,A19,A20,A21,A22,A23, *
;*                           A24,A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19, *
;*                           B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31  *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,B10,DP,SP,A16,A17,A18,A19,A20,A21,A22,  *
;*                           A23,A24,A25,A26,A27,A28,A29,A30,A31,B16,B17,B18, *
;*                           B19,B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,B30, *
;*                           B31                                              *
;*   Local Frame Size  : 0 Args + 100 Auto + 8 Save = 108 byte                *
;******************************************************************************
_isequal:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 28
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           STW     .D2T2   B10,*SP--(112)    ; |80| 
	.dwcfi	cfa_offset, 112
	.dwcfi	save_reg_to_mem, 26, 0
           STW     .D2T2   B3,*+SP(108)      ; |80| 
	.dwcfi	save_reg_to_mem, 19, -4
$C$DW$30	.dwtag  DW_TAG_variable, DW_AT_name("arg1")
	.dwattr $C$DW$30, DW_AT_TI_symbol_name("_arg1")
	.dwattr $C$DW$30, DW_AT_type(*$C$DW$T$26)
	.dwattr $C$DW$30, DW_AT_location[DW_OP_breg31 4]
$C$DW$31	.dwtag  DW_TAG_variable, DW_AT_name("arg2")
	.dwattr $C$DW$31, DW_AT_TI_symbol_name("_arg2")
	.dwattr $C$DW$31, DW_AT_type(*$C$DW$T$26)
	.dwattr $C$DW$31, DW_AT_location[DW_OP_breg31 8]
$C$DW$32	.dwtag  DW_TAG_variable, DW_AT_name("size")
	.dwattr $C$DW$32, DW_AT_TI_symbol_name("_size")
	.dwattr $C$DW$32, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$32, DW_AT_location[DW_OP_breg31 12]
$C$DW$33	.dwtag  DW_TAG_variable, DW_AT_name("tol")
	.dwattr $C$DW$33, DW_AT_TI_symbol_name("_tol")
	.dwattr $C$DW$33, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$33, DW_AT_location[DW_OP_breg31 16]
$C$DW$34	.dwtag  DW_TAG_variable, DW_AT_name("flags")
	.dwattr $C$DW$34, DW_AT_TI_symbol_name("_flags")
	.dwattr $C$DW$34, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$34, DW_AT_location[DW_OP_breg31 24]
$C$DW$35	.dwtag  DW_TAG_variable, DW_AT_name("diff")
	.dwattr $C$DW$35, DW_AT_TI_symbol_name("_diff")
	.dwattr $C$DW$35, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$35, DW_AT_location[DW_OP_breg31 32]
$C$DW$36	.dwtag  DW_TAG_variable, DW_AT_name("thresh")
	.dwattr $C$DW$36, DW_AT_TI_symbol_name("_thresh")
	.dwattr $C$DW$36, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$36, DW_AT_location[DW_OP_breg31 40]
$C$DW$37	.dwtag  DW_TAG_variable, DW_AT_name("big")
	.dwattr $C$DW$37, DW_AT_TI_symbol_name("_big")
	.dwattr $C$DW$37, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$37, DW_AT_location[DW_OP_breg31 48]
$C$DW$38	.dwtag  DW_TAG_variable, DW_AT_name("small")
	.dwattr $C$DW$38, DW_AT_TI_symbol_name("_small")
	.dwattr $C$DW$38, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$38, DW_AT_location[DW_OP_breg31 56]
$C$DW$39	.dwtag  DW_TAG_variable, DW_AT_name("equal")
	.dwattr $C$DW$39, DW_AT_TI_symbol_name("_equal")
	.dwattr $C$DW$39, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$39, DW_AT_location[DW_OP_breg31 64]
$C$DW$40	.dwtag  DW_TAG_variable, DW_AT_name("req_percent")
	.dwattr $C$DW$40, DW_AT_TI_symbol_name("_req_percent")
	.dwattr $C$DW$40, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$40, DW_AT_location[DW_OP_breg31 68]
$C$DW$41	.dwtag  DW_TAG_variable, DW_AT_name("req_notzero")
	.dwattr $C$DW$41, DW_AT_TI_symbol_name("_req_notzero")
	.dwattr $C$DW$41, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$41, DW_AT_location[DW_OP_breg31 72]
$C$DW$42	.dwtag  DW_TAG_variable, DW_AT_name("req_notnan")
	.dwattr $C$DW$42, DW_AT_TI_symbol_name("_req_notnan")
	.dwattr $C$DW$42, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$42, DW_AT_location[DW_OP_breg31 76]
$C$DW$43	.dwtag  DW_TAG_variable, DW_AT_name("ignore_inf")
	.dwattr $C$DW$43, DW_AT_TI_symbol_name("_ignore_inf")
	.dwattr $C$DW$43, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$43, DW_AT_location[DW_OP_breg31 80]
$C$DW$44	.dwtag  DW_TAG_variable, DW_AT_name("ignore_small")
	.dwattr $C$DW$44, DW_AT_TI_symbol_name("_ignore_small")
	.dwattr $C$DW$44, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$44, DW_AT_location[DW_OP_breg31 84]
$C$DW$45	.dwtag  DW_TAG_variable, DW_AT_name("req1")
	.dwattr $C$DW$45, DW_AT_TI_symbol_name("_req1")
	.dwattr $C$DW$45, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$45, DW_AT_location[DW_OP_breg31 88]
$C$DW$46	.dwtag  DW_TAG_variable, DW_AT_name("req2")
	.dwattr $C$DW$46, DW_AT_TI_symbol_name("_req2")
	.dwattr $C$DW$46, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$46, DW_AT_location[DW_OP_breg31 92]
$C$DW$47	.dwtag  DW_TAG_variable, DW_AT_name("ignr")
	.dwattr $C$DW$47, DW_AT_TI_symbol_name("_ignr")
	.dwattr $C$DW$47, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$47, DW_AT_location[DW_OP_breg31 96]
$C$DW$48	.dwtag  DW_TAG_variable, DW_AT_name("i")
	.dwattr $C$DW$48, DW_AT_TI_symbol_name("_i")
	.dwattr $C$DW$48, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$48, DW_AT_location[DW_OP_breg31 100]
           STDW    .D2T2   B7:B6,*+SP(16)    ; |80| 
           STW     .D2T1   A8,*+SP(24)       ; |80| 
           STW     .D2T1   A6,*+SP(12)       ; |80| 

           MV      .L1X    B4,A3             ; |80| 
||         STW     .D2T1   A4,*+SP(4)        ; |80| 

           STW     .D2T1   A3,*+SP(8)        ; |80| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 81,column 24,is_stmt
           MVKL    .S1     0x7fd1ccf3,A5
           MVKL    .S1     0x85ebc8a0,A4
           MVKH    .S1     0x7fd1ccf3,A5
           MVKH    .S1     0x85ebc8a0,A4
           STDW    .D2T1   A5:A4,*+SP(48)    ; |81| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 81,column 38,is_stmt
           MVKL    .S2     0x21fa18,B5
           MVKL    .S2     0x2c40c60d,B4
           MVKH    .S2     0x21fa18,B5
           MVKH    .S2     0x2c40c60d,B4
           STDW    .D2T2   B5:B4,*+SP(56)    ; |81| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 82,column 7,is_stmt
           MVK     .L2     1,B4              ; |82| 
           STW     .D2T2   B4,*+SP(64)       ; |82| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 88,column 3,is_stmt
           MV      .L2X    A8,B4
           AND     .L2     1,B4,B4           ; |88| 
           CMPEQ   .L2     B4,1,B0           ; |88| 
   [ B0]   BNOP    .S1     $C$L1,4           ; |88| 
           MVK     .L2     0x1,B4            ; |88| 
           ; BRANCHCC OCCURS {$C$L1}         ; |88| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1
           ZERO    .L2     B4                ; |88| 
;** --------------------------------------------------------------------------*
$C$L1:    
;          EXCLUSIVE CPU CYCLES: 10
           STW     .D2T2   B4,*+SP(68)       ; |88| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 89,column 3,is_stmt
           MV      .L2X    A8,B4
           AND     .L2     2,B4,B4           ; |89| 
           CMPEQ   .L2     B4,2,B0           ; |89| 
   [ B0]   BNOP    .S1     $C$L2,4           ; |89| 
           MVK     .L2     0x1,B4            ; |89| 
           ; BRANCHCC OCCURS {$C$L2}         ; |89| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1
           ZERO    .L2     B4                ; |89| 
;** --------------------------------------------------------------------------*
$C$L2:    
;          EXCLUSIVE CPU CYCLES: 10
           STW     .D2T2   B4,*+SP(72)       ; |89| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 90,column 3,is_stmt
           MV      .L2X    A8,B4
           AND     .L2     4,B4,B4           ; |90| 
           CMPEQ   .L2     B4,4,B0           ; |90| 
   [ B0]   BNOP    .S1     $C$L3,4           ; |90| 
           MVK     .L2     0x1,B4            ; |90| 
           ; BRANCHCC OCCURS {$C$L3}         ; |90| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1
           ZERO    .L2     B4                ; |90| 
;** --------------------------------------------------------------------------*
$C$L3:    
;          EXCLUSIVE CPU CYCLES: 10
           STW     .D2T2   B4,*+SP(76)       ; |90| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 91,column 3,is_stmt
           MV      .L2X    A8,B4
           AND     .L2     8,B4,B4           ; |91| 
           CMPEQ   .L2     B4,8,B0           ; |91| 
   [ B0]   BNOP    .S1     $C$L4,4           ; |91| 
           MVK     .L2     0x1,B4            ; |91| 
           ; BRANCHCC OCCURS {$C$L4}         ; |91| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1
           ZERO    .L2     B4                ; |91| 
;** --------------------------------------------------------------------------*
$C$L4:    
;          EXCLUSIVE CPU CYCLES: 15
           STW     .D2T2   B4,*+SP(80)       ; |91| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 93,column 8,is_stmt
           ZERO    .L2     B4                ; |93| 
           STW     .D2T2   B4,*+SP(100)      ; |93| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 93,column 13,is_stmt
           LDW     .D2T2   *+SP(100),B5      ; |93| 
           NOP             4
           CMPLT   .L2X    B5,A6,B0          ; |93| 
   [!B0]   BNOP    .S1     $C$L19,5          ; |93| 
           ; BRANCHCC OCCURS {$C$L19}        ; |93| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains control code
;*----------------------------------------------------------------------------*
$C$L5:    
$C$DW$L$_isequal$10$B:
;          EXCLUSIVE CPU CYCLES: 41
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 95,column 5,is_stmt
           LDW     .D2T2   *+SP(4),B7        ; |95| 

           MV      .L2     B5,B6
||         MV      .S2     B5,B4
||         LDW     .D2T2   *+SP(8),B5        ; |95| 

           NOP             3
           LDDW    .D2T2   *+B7[B6],B7:B6    ; |95| 
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |95| 
           NOP             4
           FSUBDP  .L2     B7:B6,B5:B4,B5:B4 ; |95| 
           NOP             2
           ABSDP   .S2     B5:B4,B5:B4       ; |95| 
           NOP             1
           STDW    .D2T2   B5:B4,*+SP(32)    ; |95| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 97,column 5,is_stmt
           LDW     .D2T2   *+SP(100),B4      ; |97| 
           LDW     .D2T2   *+SP(4),B5        ; |97| 
           NOP             4
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |97| 
           NOP             4
$C$DW$49	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$49, DW_AT_low_pc(0x00)
	.dwattr $C$DW$49, DW_AT_name("___isinf")
	.dwattr $C$DW$49, DW_AT_TI_call

           CALLP   .S2     ___isinf,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |97| 

$C$RL2:    ; CALL OCCURS {___isinf} {0}      ; |97| 
           MV      .L1     A4,A0             ; |97| 
   [ A0]   BNOP    .S1     $C$L6,4           ; |97| 
           ZERO    .L2     B10               ; |97| 
           ; BRANCHCC OCCURS {$C$L6}         ; |97| 
$C$DW$L$_isequal$10$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$11$B:
;          EXCLUSIVE CPU CYCLES: 24
           LDW     .D2T2   *+SP(100),B4      ; |97| 
           LDW     .D2T2   *+SP(4),B5        ; |97| 
           NOP             4
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |97| 
           NOP             4
$C$DW$50	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$50, DW_AT_low_pc(0x00)
	.dwattr $C$DW$50, DW_AT_name("___isnan")
	.dwattr $C$DW$50, DW_AT_TI_call

           CALLP   .S2     ___isnan,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |97| 

$C$RL3:    ; CALL OCCURS {___isnan} {0}      ; |97| 
           MV      .L1     A4,A0             ; |97| 
   [ A0]   BNOP    .S1     $C$L6,5           ; |97| 
           ; BRANCHCC OCCURS {$C$L6}         ; |97| 
$C$DW$L$_isequal$11$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$12$B:
;          EXCLUSIVE CPU CYCLES: 21
           LDW     .D2T2   *+SP(100),B4      ; |97| 
           LDW     .D2T2   *+SP(4),B5        ; |97| 
           LDDW    .D2T2   *+SP(48),B7:B6    ; |97| 
           NOP             3
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |97| 
           NOP             4
           ABSDP   .S2     B5:B4,B5:B4       ; |97| 
           NOP             1
           CMPLTDP .S2     B5:B4,B7:B6,B0    ; |97| 
           NOP             1
   [ B0]   BNOP    .S1     $C$L8,5           ; |97| 
           ; BRANCHCC OCCURS {$C$L8}         ; |97| 
$C$DW$L$_isequal$12$E:
;** --------------------------------------------------------------------------*
$C$L6:    
$C$DW$L$_isequal$13$B:
;          EXCLUSIVE CPU CYCLES: 24
           LDW     .D2T2   *+SP(100),B4      ; |97| 
           LDW     .D2T2   *+SP(8),B5        ; |97| 
           NOP             4
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |97| 
           NOP             4
$C$DW$51	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$51, DW_AT_low_pc(0x00)
	.dwattr $C$DW$51, DW_AT_name("___isinf")
	.dwattr $C$DW$51, DW_AT_TI_call

           CALLP   .S2     ___isinf,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |97| 

$C$RL4:    ; CALL OCCURS {___isinf} {0}      ; |97| 
           MV      .L1     A4,A0             ; |97| 
   [ A0]   BNOP    .S1     $C$L7,5           ; |97| 
           ; BRANCHCC OCCURS {$C$L7}         ; |97| 
$C$DW$L$_isequal$13$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$14$B:
;          EXCLUSIVE CPU CYCLES: 24
           LDW     .D2T2   *+SP(100),B4      ; |97| 
           LDW     .D2T2   *+SP(8),B5        ; |97| 
           NOP             4
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |97| 
           NOP             4
$C$DW$52	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$52, DW_AT_low_pc(0x00)
	.dwattr $C$DW$52, DW_AT_name("___isnan")
	.dwattr $C$DW$52, DW_AT_TI_call

           CALLP   .S2     ___isnan,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |97| 

$C$RL5:    ; CALL OCCURS {___isnan} {0}      ; |97| 
           MV      .L1     A4,A0             ; |97| 
   [ A0]   BNOP    .S1     $C$L7,5           ; |97| 
           ; BRANCHCC OCCURS {$C$L7}         ; |97| 
$C$DW$L$_isequal$14$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$15$B:
;          EXCLUSIVE CPU CYCLES: 21
           LDW     .D2T2   *+SP(100),B4      ; |97| 
           LDW     .D2T2   *+SP(8),B5        ; |97| 
           LDDW    .D2T2   *+SP(48),B7:B6    ; |97| 
           NOP             3
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |97| 
           NOP             4
           ABSDP   .S2     B5:B4,B5:B4       ; |97| 
           NOP             1
           CMPLTDP .S2     B5:B4,B7:B6,B0    ; |97| 
           NOP             1
   [ B0]   BNOP    .S1     $C$L8,5           ; |97| 
           ; BRANCHCC OCCURS {$C$L8}         ; |97| 
$C$DW$L$_isequal$15$E:
;** --------------------------------------------------------------------------*
$C$L7:    
$C$DW$L$_isequal$16$B:
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *+SP(80),B0       ; |97| 
           NOP             4
   [!B0]   BNOP    .S1     $C$L8,5           ; |97| 
           ; BRANCHCC OCCURS {$C$L8}         ; |97| 
$C$DW$L$_isequal$16$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$17$B:
;          EXCLUSIVE CPU CYCLES: 1
           MVK     .L2     0x1,B10           ; |97| 
$C$DW$L$_isequal$17$E:
;** --------------------------------------------------------------------------*
$C$L8:    
$C$DW$L$_isequal$18$B:
;          EXCLUSIVE CPU CYCLES: 34
           STW     .D2T2   B10,*+SP(96)      ; |97| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 101,column 5,is_stmt
           LDDW    .D2T2   *+SP(56),B7:B6    ; |101| 
           LDDW    .D2T2   *+SP(16),B5:B4    ; |101| 
           NOP             3
$C$DW$53	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$53, DW_AT_low_pc(0x04)
	.dwattr $C$DW$53, DW_AT_name("__divd")
	.dwattr $C$DW$53, DW_AT_TI_call

           DADD    .L1X    0,B7:B6,A5:A4     ; |101| 
||         CALLP   .S2     __divd,B3

$C$RL6:    ; CALL OCCURS {__divd} {0}        ; |101| 
           LDW     .D2T2   *+SP(100),B4      ; |101| 
           LDW     .D2T2   *+SP(4),B5        ; |101| 
           ZERO    .L2     B10               ; |101| 
           NOP             3
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |101| 
           NOP             4
           ABSDP   .S2     B5:B4,B5:B4       ; |101| 
           NOP             2
           CMPLTDP .S1X    B5:B4,A5:A4,A0    ; |101| 
           NOP             1
   [!A0]   BNOP    .S1     $C$L9,5           ; |101| 
           ; BRANCHCC OCCURS {$C$L9}         ; |101| 
$C$DW$L$_isequal$18$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$19$B:
;          EXCLUSIVE CPU CYCLES: 33
           LDDW    .D2T2   *+SP(56),B7:B6    ; |101| 
           LDDW    .D2T2   *+SP(16),B5:B4    ; |101| 
           NOP             3
$C$DW$54	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$54, DW_AT_low_pc(0x04)
	.dwattr $C$DW$54, DW_AT_name("__divd")
	.dwattr $C$DW$54, DW_AT_TI_call

           DADD    .L1X    0,B7:B6,A5:A4     ; |101| 
||         CALLP   .S2     __divd,B3

$C$RL7:    ; CALL OCCURS {__divd} {0}        ; |101| 
           LDW     .D2T2   *+SP(100),B4      ; |101| 
           LDW     .D2T2   *+SP(8),B5        ; |101| 
           NOP             4
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |101| 
           NOP             4
           ABSDP   .S2     B5:B4,B5:B4       ; |101| 
           NOP             2
           CMPLTDP .S1X    B5:B4,A5:A4,A0    ; |101| 
           NOP             1
   [!A0]   BNOP    .S1     $C$L9,5           ; |101| 
           ; BRANCHCC OCCURS {$C$L9}         ; |101| 
$C$DW$L$_isequal$19$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$20$B:
;          EXCLUSIVE CPU CYCLES: 1
           MVK     .L2     0x1,B10           ; |101| 
$C$DW$L$_isequal$20$E:
;** --------------------------------------------------------------------------*
$C$L9:    
$C$DW$L$_isequal$21$B:
;          EXCLUSIVE CPU CYCLES: 12
           STW     .D2T2   B10,*+SP(84)      ; |101| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 104,column 5,is_stmt
           LDW     .D2T2   *+SP(68),B0       ; |104| 
           NOP             4
   [!B0]   BNOP    .S1     $C$L10,5          ; |104| 
           ; BRANCHCC OCCURS {$C$L10}        ; |104| 
$C$DW$L$_isequal$21$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$22$B:
;          EXCLUSIVE CPU CYCLES: 18
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 105,column 7,is_stmt
           LDW     .D2T2   *+SP(100),B4      ; |105| 
           LDW     .D2T2   *+SP(4),B5        ; |105| 
           LDDW    .D2T2   *+SP(16),B7:B6    ; |105| 
           NOP             3
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |105| 
           NOP             4
           ABSDP   .S2     B5:B4,B5:B4       ; |105| 
           NOP             1
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |105| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(40)    ; |105| 
$C$DW$L$_isequal$22$E:
;** --------------------------------------------------------------------------*
$C$L10:    
$C$DW$L$_isequal$23$B:
;          EXCLUSIVE CPU CYCLES: 19
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 109,column 5,is_stmt
           LDW     .D2T2   *+SP(100),B4      ; |109| 
           LDW     .D2T2   *+SP(4),B5        ; |109| 
           NOP             4
           LDDW    .D2T2   *+B5[B4],B7:B6    ; |109| 
           ZERO    .L2     B5:B4             ; |109| 
           NOP             3
           CMPEQDP .S2     B7:B6,B5:B4,B0    ; |109| 
           NOP             1
   [!B0]   BNOP    .S1     $C$L11,5          ; |109| 
           ; BRANCHCC OCCURS {$C$L11}        ; |109| 
$C$DW$L$_isequal$23$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$24$B:
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *+SP(72),B0       ; |109| 
           NOP             4
   [ B0]   BNOP    .S1     $C$L12,5          ; |109| 
           ; BRANCHCC OCCURS {$C$L12}        ; |109| 
$C$DW$L$_isequal$24$E:
;** --------------------------------------------------------------------------*
$C$L11:    
$C$DW$L$_isequal$25$B:
;          EXCLUSIVE CPU CYCLES: 6
           BNOP    .S1     $C$L13,4          ; |109| 
           MVK     .L2     0x1,B4            ; |109| 
           ; BRANCH OCCURS {$C$L13}          ; |109| 
$C$DW$L$_isequal$25$E:
;** --------------------------------------------------------------------------*
$C$L12:    
$C$DW$L$_isequal$26$B:
;          EXCLUSIVE CPU CYCLES: 1
           ZERO    .L2     B4                ; |109| 
$C$DW$L$_isequal$26$E:
;** --------------------------------------------------------------------------*
$C$L13:    
$C$DW$L$_isequal$27$B:
;          EXCLUSIVE CPU CYCLES: 25
           STW     .D2T2   B4,*+SP(88)       ; |109| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 112,column 5,is_stmt
           LDW     .D2T2   *+SP(4),B5        ; |112| 
           LDW     .D2T2   *+SP(100),B4      ; |112| 
           NOP             4
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |112| 
           NOP             4
$C$DW$55	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$55, DW_AT_low_pc(0x00)
	.dwattr $C$DW$55, DW_AT_name("___isnan")
	.dwattr $C$DW$55, DW_AT_TI_call

           CALLP   .S2     ___isnan,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |112| 

$C$RL8:    ; CALL OCCURS {___isnan} {0}      ; |112| 
           MV      .L1     A4,A0             ; |112| 
   [ A0]   BNOP    .S1     $C$L14,5          ; |112| 
           ; BRANCHCC OCCURS {$C$L14}        ; |112| 
$C$DW$L$_isequal$27$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$28$B:
;          EXCLUSIVE CPU CYCLES: 24
           LDW     .D2T2   *+SP(100),B4      ; |112| 
           LDW     .D2T2   *+SP(8),B5        ; |112| 
           NOP             4
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |112| 
           NOP             4
$C$DW$56	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$56, DW_AT_low_pc(0x00)
	.dwattr $C$DW$56, DW_AT_name("___isnan")
	.dwattr $C$DW$56, DW_AT_TI_call

           CALLP   .S2     ___isnan,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |112| 

$C$RL9:    ; CALL OCCURS {___isnan} {0}      ; |112| 
           MV      .L1     A4,A0             ; |112| 
   [!A0]   BNOP    .S1     $C$L15,5          ; |112| 
           ; BRANCHCC OCCURS {$C$L15}        ; |112| 
$C$DW$L$_isequal$28$E:
;** --------------------------------------------------------------------------*
$C$L14:    
$C$DW$L$_isequal$29$B:
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *+SP(76),B0       ; |112| 
           NOP             4
   [ B0]   BNOP    .S1     $C$L16,5          ; |112| 
           ; BRANCHCC OCCURS {$C$L16}        ; |112| 
$C$DW$L$_isequal$29$E:
;** --------------------------------------------------------------------------*
$C$L15:    
$C$DW$L$_isequal$30$B:
;          EXCLUSIVE CPU CYCLES: 6
           BNOP    .S1     $C$L17,4          ; |112| 
           MVK     .L2     0x1,B4            ; |112| 
           ; BRANCH OCCURS {$C$L17}          ; |112| 
$C$DW$L$_isequal$30$E:
;** --------------------------------------------------------------------------*
$C$L16:    
$C$DW$L$_isequal$31$B:
;          EXCLUSIVE CPU CYCLES: 1
           ZERO    .L2     B4                ; |112| 
$C$DW$L$_isequal$31$E:
;** --------------------------------------------------------------------------*
$C$L17:    
$C$DW$L$_isequal$32$B:
;          EXCLUSIVE CPU CYCLES: 15
           STW     .D2T2   B4,*+SP(92)       ; |112| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 114,column 5,is_stmt
           LDDW    .D2T2   *+SP(32),B7:B6    ; |114| 
           LDDW    .D2T2   *+SP(40),B5:B4    ; |114| 
           NOP             4
           CMPGTDP .S2     B7:B6,B5:B4,B0    ; |114| 
           NOP             1
   [!B0]   BNOP    .S1     $C$L18,5          ; |114| 
           ; BRANCHCC OCCURS {$C$L18}        ; |114| 
$C$DW$L$_isequal$32$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$33$B:
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *+SP(84),B0       ; |114| 
           NOP             4
   [ B0]   BNOP    .S1     $C$L18,5          ; |114| 
           ; BRANCHCC OCCURS {$C$L18}        ; |114| 
$C$DW$L$_isequal$33$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$34$B:
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *+SP(96),B0       ; |114| 
           NOP             4
   [ B0]   BNOP    .S1     $C$L18,5          ; |114| 
           ; BRANCHCC OCCURS {$C$L18}        ; |114| 
$C$DW$L$_isequal$34$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$35$B:
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *+SP(88),B0       ; |114| 
           NOP             4
   [!B0]   BNOP    .S1     $C$L18,5          ; |114| 
           ; BRANCHCC OCCURS {$C$L18}        ; |114| 
$C$DW$L$_isequal$35$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$36$B:
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *+SP(92),B0       ; |114| 
           NOP             4
   [!B0]   BNOP    .S1     $C$L18,5          ; |114| 
           ; BRANCHCC OCCURS {$C$L18}        ; |114| 
$C$DW$L$_isequal$36$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_isequal$37$B:
;          EXCLUSIVE CPU CYCLES: 2
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 115,column 7,is_stmt
           ZERO    .L2     B4                ; |115| 
           STW     .D2T2   B4,*+SP(64)       ; |115| 
$C$DW$L$_isequal$37$E:
;** --------------------------------------------------------------------------*
$C$L18:    
$C$DW$L$_isequal$38$B:
;          EXCLUSIVE CPU CYCLES: 20
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 93,column 21,is_stmt
           LDW     .D2T2   *+SP(100),B4      ; |93| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |93| 
           STW     .D2T2   B4,*+SP(100)      ; |93| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 93,column 13,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |93| 
           LDW     .D2T2   *+SP(100),B5      ; |93| 
           NOP             4
           CMPLT   .L2     B5,B4,B0          ; |93| 
   [ B0]   BNOP    .S1     $C$L5,5           ; |93| 
           ; BRANCHCC OCCURS {$C$L5}         ; |93| 
$C$DW$L$_isequal$38$E:
;** --------------------------------------------------------------------------*
$C$L19:    
;          EXCLUSIVE CPU CYCLES: 21
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 119,column 3,is_stmt
           LDW     .D2T1   *+SP(64),A4       ; |119| 
           NOP             4
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c",line 120,column 1,is_stmt
           LDW     .D2T2   *+SP(108),B3      ; |120| 
           NOP             4
	.dwcfi	restore_reg, 19
           LDW     .D2T2   *++SP(112),B10    ; |120| 
           NOP             4
	.dwcfi	cfa_offset, 0
	.dwcfi	restore_reg, 26
	.dwcfi	cfa_offset, 0
$C$DW$57	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$57, DW_AT_low_pc(0x00)
	.dwattr $C$DW$57, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |120| 
           ; BRANCH OCCURS {B3}              ; |120| 

$C$DW$58	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$58, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\drvdp.asm:$C$L5:1:1463466945")
	.dwattr $C$DW$58, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c")
	.dwattr $C$DW$58, DW_AT_TI_begin_line(0x5d)
	.dwattr $C$DW$58, DW_AT_TI_end_line(0x75)
$C$DW$59	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$59, DW_AT_low_pc($C$DW$L$_isequal$10$B)
	.dwattr $C$DW$59, DW_AT_high_pc($C$DW$L$_isequal$10$E)
$C$DW$60	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$60, DW_AT_low_pc($C$DW$L$_isequal$13$B)
	.dwattr $C$DW$60, DW_AT_high_pc($C$DW$L$_isequal$13$E)
$C$DW$61	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$61, DW_AT_low_pc($C$DW$L$_isequal$14$B)
	.dwattr $C$DW$61, DW_AT_high_pc($C$DW$L$_isequal$14$E)
$C$DW$62	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$62, DW_AT_low_pc($C$DW$L$_isequal$11$B)
	.dwattr $C$DW$62, DW_AT_high_pc($C$DW$L$_isequal$11$E)
$C$DW$63	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$63, DW_AT_low_pc($C$DW$L$_isequal$12$B)
	.dwattr $C$DW$63, DW_AT_high_pc($C$DW$L$_isequal$12$E)
$C$DW$64	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$64, DW_AT_low_pc($C$DW$L$_isequal$15$B)
	.dwattr $C$DW$64, DW_AT_high_pc($C$DW$L$_isequal$15$E)
$C$DW$65	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$65, DW_AT_low_pc($C$DW$L$_isequal$16$B)
	.dwattr $C$DW$65, DW_AT_high_pc($C$DW$L$_isequal$16$E)
$C$DW$66	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$66, DW_AT_low_pc($C$DW$L$_isequal$17$B)
	.dwattr $C$DW$66, DW_AT_high_pc($C$DW$L$_isequal$17$E)
$C$DW$67	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$67, DW_AT_low_pc($C$DW$L$_isequal$18$B)
	.dwattr $C$DW$67, DW_AT_high_pc($C$DW$L$_isequal$18$E)
$C$DW$68	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$68, DW_AT_low_pc($C$DW$L$_isequal$19$B)
	.dwattr $C$DW$68, DW_AT_high_pc($C$DW$L$_isequal$19$E)
$C$DW$69	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$69, DW_AT_low_pc($C$DW$L$_isequal$20$B)
	.dwattr $C$DW$69, DW_AT_high_pc($C$DW$L$_isequal$20$E)
$C$DW$70	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$70, DW_AT_low_pc($C$DW$L$_isequal$21$B)
	.dwattr $C$DW$70, DW_AT_high_pc($C$DW$L$_isequal$21$E)
$C$DW$71	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$71, DW_AT_low_pc($C$DW$L$_isequal$22$B)
	.dwattr $C$DW$71, DW_AT_high_pc($C$DW$L$_isequal$22$E)
$C$DW$72	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$72, DW_AT_low_pc($C$DW$L$_isequal$23$B)
	.dwattr $C$DW$72, DW_AT_high_pc($C$DW$L$_isequal$23$E)
$C$DW$73	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$73, DW_AT_low_pc($C$DW$L$_isequal$24$B)
	.dwattr $C$DW$73, DW_AT_high_pc($C$DW$L$_isequal$24$E)
$C$DW$74	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$74, DW_AT_low_pc($C$DW$L$_isequal$25$B)
	.dwattr $C$DW$74, DW_AT_high_pc($C$DW$L$_isequal$25$E)
$C$DW$75	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$75, DW_AT_low_pc($C$DW$L$_isequal$26$B)
	.dwattr $C$DW$75, DW_AT_high_pc($C$DW$L$_isequal$26$E)
$C$DW$76	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$76, DW_AT_low_pc($C$DW$L$_isequal$27$B)
	.dwattr $C$DW$76, DW_AT_high_pc($C$DW$L$_isequal$27$E)
$C$DW$77	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$77, DW_AT_low_pc($C$DW$L$_isequal$28$B)
	.dwattr $C$DW$77, DW_AT_high_pc($C$DW$L$_isequal$28$E)
$C$DW$78	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$78, DW_AT_low_pc($C$DW$L$_isequal$29$B)
	.dwattr $C$DW$78, DW_AT_high_pc($C$DW$L$_isequal$29$E)
$C$DW$79	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$79, DW_AT_low_pc($C$DW$L$_isequal$30$B)
	.dwattr $C$DW$79, DW_AT_high_pc($C$DW$L$_isequal$30$E)
$C$DW$80	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$80, DW_AT_low_pc($C$DW$L$_isequal$31$B)
	.dwattr $C$DW$80, DW_AT_high_pc($C$DW$L$_isequal$31$E)
$C$DW$81	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$81, DW_AT_low_pc($C$DW$L$_isequal$32$B)
	.dwattr $C$DW$81, DW_AT_high_pc($C$DW$L$_isequal$32$E)
$C$DW$82	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$82, DW_AT_low_pc($C$DW$L$_isequal$33$B)
	.dwattr $C$DW$82, DW_AT_high_pc($C$DW$L$_isequal$33$E)
$C$DW$83	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$83, DW_AT_low_pc($C$DW$L$_isequal$34$B)
	.dwattr $C$DW$83, DW_AT_high_pc($C$DW$L$_isequal$34$E)
$C$DW$84	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$84, DW_AT_low_pc($C$DW$L$_isequal$35$B)
	.dwattr $C$DW$84, DW_AT_high_pc($C$DW$L$_isequal$35$E)
$C$DW$85	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$85, DW_AT_low_pc($C$DW$L$_isequal$36$B)
	.dwattr $C$DW$85, DW_AT_high_pc($C$DW$L$_isequal$36$E)
$C$DW$86	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$86, DW_AT_low_pc($C$DW$L$_isequal$37$B)
	.dwattr $C$DW$86, DW_AT_high_pc($C$DW$L$_isequal$37$E)
$C$DW$87	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$87, DW_AT_low_pc($C$DW$L$_isequal$38$B)
	.dwattr $C$DW$87, DW_AT_high_pc($C$DW$L$_isequal$38$E)
	.dwendtag $C$DW$58

	.dwattr $C$DW$24, DW_AT_TI_end_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/drvdp.c")
	.dwattr $C$DW$24, DW_AT_TI_end_line(0x78)
	.dwattr $C$DW$24, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$24

;*****************************************************************************
;* UNDEFINED EXTERNAL REFERENCES                                             *
;*****************************************************************************
	.global	___isinf
	.global	___isnan
	.global	_rand
	.global	__divd

;******************************************************************************
;* BUILD ATTRIBUTES                                                           *
;******************************************************************************
	.battr "TI", Tag_File, 1, Tag_ABI_stack_align_needed(0)
	.battr "TI", Tag_File, 1, Tag_ABI_stack_align_preserved(0)
	.battr "TI", Tag_File, 1, Tag_Tramps_Use_SOC(1)

;******************************************************************************
;* TYPE INFORMATION                                                           *
;******************************************************************************
$C$DW$T$4	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$4, DW_AT_encoding(DW_ATE_boolean)
	.dwattr $C$DW$T$4, DW_AT_name("bool")
	.dwattr $C$DW$T$4, DW_AT_byte_size(0x01)
$C$DW$T$5	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$5, DW_AT_encoding(DW_ATE_signed_char)
	.dwattr $C$DW$T$5, DW_AT_name("signed char")
	.dwattr $C$DW$T$5, DW_AT_byte_size(0x01)
$C$DW$T$6	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$6, DW_AT_encoding(DW_ATE_unsigned_char)
	.dwattr $C$DW$T$6, DW_AT_name("unsigned char")
	.dwattr $C$DW$T$6, DW_AT_byte_size(0x01)
$C$DW$T$7	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$7, DW_AT_encoding(DW_ATE_signed_char)
	.dwattr $C$DW$T$7, DW_AT_name("wchar_t")
	.dwattr $C$DW$T$7, DW_AT_byte_size(0x02)
$C$DW$T$8	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$8, DW_AT_encoding(DW_ATE_signed)
	.dwattr $C$DW$T$8, DW_AT_name("short")
	.dwattr $C$DW$T$8, DW_AT_byte_size(0x02)
$C$DW$T$9	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$9, DW_AT_encoding(DW_ATE_unsigned)
	.dwattr $C$DW$T$9, DW_AT_name("unsigned short")
	.dwattr $C$DW$T$9, DW_AT_byte_size(0x02)
$C$DW$T$10	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$10, DW_AT_encoding(DW_ATE_signed)
	.dwattr $C$DW$T$10, DW_AT_name("int")
	.dwattr $C$DW$T$10, DW_AT_byte_size(0x04)
$C$DW$T$11	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$11, DW_AT_encoding(DW_ATE_unsigned)
	.dwattr $C$DW$T$11, DW_AT_name("unsigned int")
	.dwattr $C$DW$T$11, DW_AT_byte_size(0x04)
$C$DW$T$12	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$12, DW_AT_encoding(DW_ATE_signed)
	.dwattr $C$DW$T$12, DW_AT_name("long")
	.dwattr $C$DW$T$12, DW_AT_byte_size(0x08)
	.dwattr $C$DW$T$12, DW_AT_bit_size(0x28)
	.dwattr $C$DW$T$12, DW_AT_bit_offset(0x18)
$C$DW$T$13	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$13, DW_AT_encoding(DW_ATE_unsigned)
	.dwattr $C$DW$T$13, DW_AT_name("unsigned long")
	.dwattr $C$DW$T$13, DW_AT_byte_size(0x08)
	.dwattr $C$DW$T$13, DW_AT_bit_size(0x28)
	.dwattr $C$DW$T$13, DW_AT_bit_offset(0x18)
$C$DW$T$14	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$14, DW_AT_encoding(DW_ATE_signed)
	.dwattr $C$DW$T$14, DW_AT_name("long long")
	.dwattr $C$DW$T$14, DW_AT_byte_size(0x08)
$C$DW$T$15	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$15, DW_AT_encoding(DW_ATE_unsigned)
	.dwattr $C$DW$T$15, DW_AT_name("unsigned long long")
	.dwattr $C$DW$T$15, DW_AT_byte_size(0x08)
$C$DW$T$16	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$16, DW_AT_encoding(DW_ATE_float)
	.dwattr $C$DW$T$16, DW_AT_name("float")
	.dwattr $C$DW$T$16, DW_AT_byte_size(0x04)
$C$DW$T$17	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$17, DW_AT_encoding(DW_ATE_float)
	.dwattr $C$DW$T$17, DW_AT_name("double")
	.dwattr $C$DW$T$17, DW_AT_byte_size(0x08)
$C$DW$T$21	.dwtag  DW_TAG_volatile_type
	.dwattr $C$DW$T$21, DW_AT_type(*$C$DW$T$17)
$C$DW$T$26	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$26, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$T$26, DW_AT_address_class(0x20)
$C$DW$T$30	.dwtag  DW_TAG_TI_restrict_type
	.dwattr $C$DW$T$30, DW_AT_type(*$C$DW$T$26)

$C$DW$T$31	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$31, DW_AT_type(*$C$DW$T$30)
	.dwattr $C$DW$T$31, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$31, DW_AT_byte_size(0x14)
$C$DW$88	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$88, DW_AT_upper_bound(0x04)
	.dwendtag $C$DW$T$31


$C$DW$T$32	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$32, DW_AT_type(*$C$DW$T$30)
	.dwattr $C$DW$T$32, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$32, DW_AT_byte_size(0x08)
$C$DW$89	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$89, DW_AT_upper_bound(0x01)
	.dwendtag $C$DW$T$32


$C$DW$T$33	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$33, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$T$33, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$33, DW_AT_byte_size(0x3e80)
$C$DW$90	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$90, DW_AT_upper_bound(0x7cf)
	.dwendtag $C$DW$T$33


$C$DW$T$34	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$34, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$T$34, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$34, DW_AT_byte_size(0x13880)
$C$DW$91	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$91, DW_AT_upper_bound(0x04)
$C$DW$92	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$92, DW_AT_upper_bound(0x7cf)
	.dwendtag $C$DW$T$34


$C$DW$T$36	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$36, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$T$36, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$36, DW_AT_byte_size(0x7d00)
$C$DW$93	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$93, DW_AT_upper_bound(0x01)
$C$DW$94	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$94, DW_AT_upper_bound(0x7cf)
	.dwendtag $C$DW$T$36


$C$DW$T$38	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$38, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$T$38, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$38, DW_AT_byte_size(0x320)
$C$DW$95	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$95, DW_AT_upper_bound(0x63)
	.dwendtag $C$DW$T$38

$C$DW$T$18	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$18, DW_AT_encoding(DW_ATE_float)
	.dwattr $C$DW$T$18, DW_AT_name("long double")
	.dwattr $C$DW$T$18, DW_AT_byte_size(0x08)
	.dwattr $C$DW$CU, DW_AT_language(DW_LANG_C)

;***************************************************************
;* DWARF CIE ENTRIES                                           *
;***************************************************************

$C$DW$CIE	.dwcie 228
	.dwcfi	cfa_register, 31
	.dwcfi	cfa_offset, 0
	.dwcfi	undefined, 0
	.dwcfi	undefined, 1
	.dwcfi	undefined, 2
	.dwcfi	undefined, 3
	.dwcfi	undefined, 4
	.dwcfi	undefined, 5
	.dwcfi	undefined, 6
	.dwcfi	undefined, 7
	.dwcfi	undefined, 8
	.dwcfi	undefined, 9
	.dwcfi	same_value, 10
	.dwcfi	same_value, 11
	.dwcfi	same_value, 12
	.dwcfi	same_value, 13
	.dwcfi	same_value, 14
	.dwcfi	same_value, 15
	.dwcfi	undefined, 16
	.dwcfi	undefined, 17
	.dwcfi	undefined, 18
	.dwcfi	undefined, 19
	.dwcfi	undefined, 20
	.dwcfi	undefined, 21
	.dwcfi	undefined, 22
	.dwcfi	undefined, 23
	.dwcfi	undefined, 24
	.dwcfi	undefined, 25
	.dwcfi	same_value, 26
	.dwcfi	same_value, 27
	.dwcfi	same_value, 28
	.dwcfi	same_value, 29
	.dwcfi	same_value, 30
	.dwcfi	same_value, 31
	.dwcfi	same_value, 32
	.dwcfi	undefined, 33
	.dwcfi	undefined, 34
	.dwcfi	undefined, 35
	.dwcfi	undefined, 36
	.dwcfi	undefined, 37
	.dwcfi	undefined, 38
	.dwcfi	undefined, 39
	.dwcfi	undefined, 40
	.dwcfi	undefined, 41
	.dwcfi	undefined, 42
	.dwcfi	undefined, 43
	.dwcfi	undefined, 44
	.dwcfi	undefined, 45
	.dwcfi	undefined, 46
	.dwcfi	undefined, 47
	.dwcfi	undefined, 48
	.dwcfi	undefined, 49
	.dwcfi	undefined, 50
	.dwcfi	undefined, 51
	.dwcfi	undefined, 52
	.dwcfi	undefined, 53
	.dwcfi	undefined, 54
	.dwcfi	undefined, 55
	.dwcfi	undefined, 56
	.dwcfi	undefined, 57
	.dwcfi	undefined, 58
	.dwcfi	undefined, 59
	.dwcfi	undefined, 60
	.dwcfi	undefined, 61
	.dwcfi	undefined, 62
	.dwcfi	undefined, 63
	.dwcfi	undefined, 64
	.dwcfi	undefined, 65
	.dwcfi	undefined, 66
	.dwcfi	undefined, 67
	.dwcfi	undefined, 68
	.dwcfi	undefined, 69
	.dwcfi	undefined, 70
	.dwcfi	undefined, 71
	.dwcfi	undefined, 72
	.dwcfi	undefined, 73
	.dwcfi	undefined, 74
	.dwcfi	undefined, 75
	.dwcfi	undefined, 76
	.dwcfi	undefined, 77
	.dwcfi	undefined, 78
	.dwcfi	undefined, 79
	.dwcfi	undefined, 80
	.dwcfi	undefined, 81
	.dwcfi	undefined, 82
	.dwcfi	undefined, 83
	.dwcfi	undefined, 84
	.dwcfi	undefined, 85
	.dwcfi	undefined, 86
	.dwcfi	undefined, 87
	.dwcfi	undefined, 88
	.dwcfi	undefined, 89
	.dwcfi	undefined, 90
	.dwcfi	undefined, 91
	.dwcfi	undefined, 92
	.dwcfi	undefined, 93
	.dwcfi	undefined, 94
	.dwcfi	undefined, 95
	.dwcfi	undefined, 96
	.dwcfi	undefined, 97
	.dwcfi	undefined, 98
	.dwcfi	undefined, 99
	.dwcfi	undefined, 100
	.dwcfi	undefined, 101
	.dwcfi	undefined, 102
	.dwcfi	undefined, 103
	.dwcfi	undefined, 104
	.dwcfi	undefined, 105
	.dwcfi	undefined, 106
	.dwcfi	undefined, 107
	.dwcfi	undefined, 108
	.dwcfi	undefined, 109
	.dwcfi	undefined, 110
	.dwcfi	undefined, 111
	.dwcfi	undefined, 112
	.dwcfi	undefined, 113
	.dwcfi	undefined, 114
	.dwcfi	undefined, 115
	.dwcfi	undefined, 116
	.dwcfi	undefined, 117
	.dwcfi	undefined, 118
	.dwcfi	undefined, 119
	.dwcfi	undefined, 120
	.dwcfi	undefined, 121
	.dwcfi	undefined, 122
	.dwcfi	undefined, 123
	.dwcfi	undefined, 124
	.dwcfi	undefined, 125
	.dwcfi	undefined, 126
	.dwcfi	undefined, 127
	.dwcfi	undefined, 128
	.dwcfi	undefined, 129
	.dwcfi	undefined, 130
	.dwcfi	undefined, 131
	.dwcfi	undefined, 132
	.dwcfi	undefined, 133
	.dwcfi	undefined, 134
	.dwcfi	undefined, 135
	.dwcfi	undefined, 136
	.dwcfi	undefined, 137
	.dwcfi	undefined, 138
	.dwcfi	undefined, 139
	.dwcfi	undefined, 140
	.dwcfi	undefined, 141
	.dwcfi	undefined, 142
	.dwcfi	undefined, 143
	.dwcfi	undefined, 144
	.dwcfi	undefined, 145
	.dwcfi	undefined, 146
	.dwcfi	undefined, 147
	.dwcfi	undefined, 148
	.dwcfi	undefined, 149
	.dwcfi	undefined, 150
	.dwcfi	undefined, 151
	.dwcfi	undefined, 152
	.dwcfi	undefined, 153
	.dwcfi	undefined, 154
	.dwcfi	undefined, 155
	.dwcfi	undefined, 156
	.dwcfi	undefined, 157
	.dwcfi	undefined, 158
	.dwcfi	undefined, 159
	.dwcfi	undefined, 160
	.dwcfi	undefined, 161
	.dwcfi	undefined, 162
	.dwcfi	undefined, 163
	.dwcfi	undefined, 164
	.dwcfi	undefined, 165
	.dwcfi	undefined, 166
	.dwcfi	undefined, 167
	.dwcfi	undefined, 168
	.dwcfi	undefined, 169
	.dwcfi	undefined, 170
	.dwcfi	undefined, 171
	.dwcfi	undefined, 172
	.dwcfi	undefined, 173
	.dwcfi	undefined, 174
	.dwcfi	undefined, 175
	.dwcfi	undefined, 176
	.dwcfi	undefined, 177
	.dwcfi	undefined, 178
	.dwcfi	undefined, 179
	.dwcfi	undefined, 180
	.dwcfi	undefined, 181
	.dwcfi	undefined, 182
	.dwcfi	undefined, 183
	.dwcfi	undefined, 184
	.dwcfi	undefined, 185
	.dwcfi	undefined, 186
	.dwcfi	undefined, 187
	.dwcfi	undefined, 188
	.dwcfi	undefined, 189
	.dwcfi	undefined, 190
	.dwcfi	undefined, 191
	.dwcfi	undefined, 192
	.dwcfi	undefined, 193
	.dwcfi	undefined, 194
	.dwcfi	undefined, 195
	.dwcfi	undefined, 196
	.dwcfi	undefined, 197
	.dwcfi	undefined, 198
	.dwcfi	undefined, 199
	.dwcfi	undefined, 200
	.dwcfi	undefined, 201
	.dwcfi	undefined, 202
	.dwcfi	undefined, 203
	.dwcfi	undefined, 204
	.dwcfi	undefined, 205
	.dwcfi	undefined, 206
	.dwcfi	undefined, 207
	.dwcfi	undefined, 208
	.dwcfi	undefined, 209
	.dwcfi	undefined, 210
	.dwcfi	undefined, 211
	.dwcfi	undefined, 212
	.dwcfi	undefined, 213
	.dwcfi	undefined, 214
	.dwcfi	undefined, 215
	.dwcfi	undefined, 216
	.dwcfi	undefined, 217
	.dwcfi	undefined, 218
	.dwcfi	undefined, 219
	.dwcfi	undefined, 220
	.dwcfi	undefined, 221
	.dwcfi	undefined, 222
	.dwcfi	undefined, 223
	.dwcfi	undefined, 224
	.dwcfi	undefined, 225
	.dwcfi	undefined, 226
	.dwcfi	undefined, 227
	.dwcfi	undefined, 228
	.dwendentry

;***************************************************************
;* DWARF REGISTER MAP                                          *
;***************************************************************

$C$DW$96	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A0")
	.dwattr $C$DW$96, DW_AT_location[DW_OP_reg0]
$C$DW$97	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A1")
	.dwattr $C$DW$97, DW_AT_location[DW_OP_reg1]
$C$DW$98	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A2")
	.dwattr $C$DW$98, DW_AT_location[DW_OP_reg2]
$C$DW$99	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A3")
	.dwattr $C$DW$99, DW_AT_location[DW_OP_reg3]
$C$DW$100	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A4")
	.dwattr $C$DW$100, DW_AT_location[DW_OP_reg4]
$C$DW$101	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A5")
	.dwattr $C$DW$101, DW_AT_location[DW_OP_reg5]
$C$DW$102	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A6")
	.dwattr $C$DW$102, DW_AT_location[DW_OP_reg6]
$C$DW$103	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A7")
	.dwattr $C$DW$103, DW_AT_location[DW_OP_reg7]
$C$DW$104	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A8")
	.dwattr $C$DW$104, DW_AT_location[DW_OP_reg8]
$C$DW$105	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A9")
	.dwattr $C$DW$105, DW_AT_location[DW_OP_reg9]
$C$DW$106	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A10")
	.dwattr $C$DW$106, DW_AT_location[DW_OP_reg10]
$C$DW$107	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A11")
	.dwattr $C$DW$107, DW_AT_location[DW_OP_reg11]
$C$DW$108	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A12")
	.dwattr $C$DW$108, DW_AT_location[DW_OP_reg12]
$C$DW$109	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A13")
	.dwattr $C$DW$109, DW_AT_location[DW_OP_reg13]
$C$DW$110	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A14")
	.dwattr $C$DW$110, DW_AT_location[DW_OP_reg14]
$C$DW$111	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A15")
	.dwattr $C$DW$111, DW_AT_location[DW_OP_reg15]
$C$DW$112	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B0")
	.dwattr $C$DW$112, DW_AT_location[DW_OP_reg16]
$C$DW$113	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B1")
	.dwattr $C$DW$113, DW_AT_location[DW_OP_reg17]
$C$DW$114	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B2")
	.dwattr $C$DW$114, DW_AT_location[DW_OP_reg18]
$C$DW$115	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B3")
	.dwattr $C$DW$115, DW_AT_location[DW_OP_reg19]
$C$DW$116	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B4")
	.dwattr $C$DW$116, DW_AT_location[DW_OP_reg20]
$C$DW$117	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B5")
	.dwattr $C$DW$117, DW_AT_location[DW_OP_reg21]
$C$DW$118	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B6")
	.dwattr $C$DW$118, DW_AT_location[DW_OP_reg22]
$C$DW$119	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B7")
	.dwattr $C$DW$119, DW_AT_location[DW_OP_reg23]
$C$DW$120	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B8")
	.dwattr $C$DW$120, DW_AT_location[DW_OP_reg24]
$C$DW$121	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B9")
	.dwattr $C$DW$121, DW_AT_location[DW_OP_reg25]
$C$DW$122	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B10")
	.dwattr $C$DW$122, DW_AT_location[DW_OP_reg26]
$C$DW$123	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B11")
	.dwattr $C$DW$123, DW_AT_location[DW_OP_reg27]
$C$DW$124	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B12")
	.dwattr $C$DW$124, DW_AT_location[DW_OP_reg28]
$C$DW$125	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B13")
	.dwattr $C$DW$125, DW_AT_location[DW_OP_reg29]
$C$DW$126	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DP")
	.dwattr $C$DW$126, DW_AT_location[DW_OP_reg30]
$C$DW$127	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SP")
	.dwattr $C$DW$127, DW_AT_location[DW_OP_reg31]
$C$DW$128	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FP")
	.dwattr $C$DW$128, DW_AT_location[DW_OP_regx 0x20]
$C$DW$129	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("PC")
	.dwattr $C$DW$129, DW_AT_location[DW_OP_regx 0x21]
$C$DW$130	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IRP")
	.dwattr $C$DW$130, DW_AT_location[DW_OP_regx 0x22]
$C$DW$131	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IFR")
	.dwattr $C$DW$131, DW_AT_location[DW_OP_regx 0x23]
$C$DW$132	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("NRP")
	.dwattr $C$DW$132, DW_AT_location[DW_OP_regx 0x24]
$C$DW$133	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A16")
	.dwattr $C$DW$133, DW_AT_location[DW_OP_regx 0x25]
$C$DW$134	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A17")
	.dwattr $C$DW$134, DW_AT_location[DW_OP_regx 0x26]
$C$DW$135	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A18")
	.dwattr $C$DW$135, DW_AT_location[DW_OP_regx 0x27]
$C$DW$136	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A19")
	.dwattr $C$DW$136, DW_AT_location[DW_OP_regx 0x28]
$C$DW$137	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A20")
	.dwattr $C$DW$137, DW_AT_location[DW_OP_regx 0x29]
$C$DW$138	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A21")
	.dwattr $C$DW$138, DW_AT_location[DW_OP_regx 0x2a]
$C$DW$139	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A22")
	.dwattr $C$DW$139, DW_AT_location[DW_OP_regx 0x2b]
$C$DW$140	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A23")
	.dwattr $C$DW$140, DW_AT_location[DW_OP_regx 0x2c]
$C$DW$141	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A24")
	.dwattr $C$DW$141, DW_AT_location[DW_OP_regx 0x2d]
$C$DW$142	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A25")
	.dwattr $C$DW$142, DW_AT_location[DW_OP_regx 0x2e]
$C$DW$143	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A26")
	.dwattr $C$DW$143, DW_AT_location[DW_OP_regx 0x2f]
$C$DW$144	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A27")
	.dwattr $C$DW$144, DW_AT_location[DW_OP_regx 0x30]
$C$DW$145	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A28")
	.dwattr $C$DW$145, DW_AT_location[DW_OP_regx 0x31]
$C$DW$146	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A29")
	.dwattr $C$DW$146, DW_AT_location[DW_OP_regx 0x32]
$C$DW$147	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A30")
	.dwattr $C$DW$147, DW_AT_location[DW_OP_regx 0x33]
$C$DW$148	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A31")
	.dwattr $C$DW$148, DW_AT_location[DW_OP_regx 0x34]
$C$DW$149	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B16")
	.dwattr $C$DW$149, DW_AT_location[DW_OP_regx 0x35]
$C$DW$150	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B17")
	.dwattr $C$DW$150, DW_AT_location[DW_OP_regx 0x36]
$C$DW$151	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B18")
	.dwattr $C$DW$151, DW_AT_location[DW_OP_regx 0x37]
$C$DW$152	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B19")
	.dwattr $C$DW$152, DW_AT_location[DW_OP_regx 0x38]
$C$DW$153	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B20")
	.dwattr $C$DW$153, DW_AT_location[DW_OP_regx 0x39]
$C$DW$154	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B21")
	.dwattr $C$DW$154, DW_AT_location[DW_OP_regx 0x3a]
$C$DW$155	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B22")
	.dwattr $C$DW$155, DW_AT_location[DW_OP_regx 0x3b]
$C$DW$156	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B23")
	.dwattr $C$DW$156, DW_AT_location[DW_OP_regx 0x3c]
$C$DW$157	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B24")
	.dwattr $C$DW$157, DW_AT_location[DW_OP_regx 0x3d]
$C$DW$158	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B25")
	.dwattr $C$DW$158, DW_AT_location[DW_OP_regx 0x3e]
$C$DW$159	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B26")
	.dwattr $C$DW$159, DW_AT_location[DW_OP_regx 0x3f]
$C$DW$160	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B27")
	.dwattr $C$DW$160, DW_AT_location[DW_OP_regx 0x40]
$C$DW$161	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B28")
	.dwattr $C$DW$161, DW_AT_location[DW_OP_regx 0x41]
$C$DW$162	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B29")
	.dwattr $C$DW$162, DW_AT_location[DW_OP_regx 0x42]
$C$DW$163	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B30")
	.dwattr $C$DW$163, DW_AT_location[DW_OP_regx 0x43]
$C$DW$164	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B31")
	.dwattr $C$DW$164, DW_AT_location[DW_OP_regx 0x44]
$C$DW$165	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("AMR")
	.dwattr $C$DW$165, DW_AT_location[DW_OP_regx 0x45]
$C$DW$166	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CSR")
	.dwattr $C$DW$166, DW_AT_location[DW_OP_regx 0x46]
$C$DW$167	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ISR")
	.dwattr $C$DW$167, DW_AT_location[DW_OP_regx 0x47]
$C$DW$168	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ICR")
	.dwattr $C$DW$168, DW_AT_location[DW_OP_regx 0x48]
$C$DW$169	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IER")
	.dwattr $C$DW$169, DW_AT_location[DW_OP_regx 0x49]
$C$DW$170	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ISTP")
	.dwattr $C$DW$170, DW_AT_location[DW_OP_regx 0x4a]
$C$DW$171	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IN")
	.dwattr $C$DW$171, DW_AT_location[DW_OP_regx 0x4b]
$C$DW$172	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("OUT")
	.dwattr $C$DW$172, DW_AT_location[DW_OP_regx 0x4c]
$C$DW$173	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ACR")
	.dwattr $C$DW$173, DW_AT_location[DW_OP_regx 0x4d]
$C$DW$174	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ADR")
	.dwattr $C$DW$174, DW_AT_location[DW_OP_regx 0x4e]
$C$DW$175	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FADCR")
	.dwattr $C$DW$175, DW_AT_location[DW_OP_regx 0x4f]
$C$DW$176	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FAUCR")
	.dwattr $C$DW$176, DW_AT_location[DW_OP_regx 0x50]
$C$DW$177	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FMCR")
	.dwattr $C$DW$177, DW_AT_location[DW_OP_regx 0x51]
$C$DW$178	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GFPGFR")
	.dwattr $C$DW$178, DW_AT_location[DW_OP_regx 0x52]
$C$DW$179	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DIER")
	.dwattr $C$DW$179, DW_AT_location[DW_OP_regx 0x53]
$C$DW$180	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("REP")
	.dwattr $C$DW$180, DW_AT_location[DW_OP_regx 0x54]
$C$DW$181	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSCL")
	.dwattr $C$DW$181, DW_AT_location[DW_OP_regx 0x55]
$C$DW$182	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSCH")
	.dwattr $C$DW$182, DW_AT_location[DW_OP_regx 0x56]
$C$DW$183	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ARP")
	.dwattr $C$DW$183, DW_AT_location[DW_OP_regx 0x57]
$C$DW$184	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ILC")
	.dwattr $C$DW$184, DW_AT_location[DW_OP_regx 0x58]
$C$DW$185	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RILC")
	.dwattr $C$DW$185, DW_AT_location[DW_OP_regx 0x59]
$C$DW$186	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DNUM")
	.dwattr $C$DW$186, DW_AT_location[DW_OP_regx 0x5a]
$C$DW$187	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SSR")
	.dwattr $C$DW$187, DW_AT_location[DW_OP_regx 0x5b]
$C$DW$188	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GPLYA")
	.dwattr $C$DW$188, DW_AT_location[DW_OP_regx 0x5c]
$C$DW$189	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GPLYB")
	.dwattr $C$DW$189, DW_AT_location[DW_OP_regx 0x5d]
$C$DW$190	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSR")
	.dwattr $C$DW$190, DW_AT_location[DW_OP_regx 0x5e]
$C$DW$191	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ITSR")
	.dwattr $C$DW$191, DW_AT_location[DW_OP_regx 0x5f]
$C$DW$192	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("NTSR")
	.dwattr $C$DW$192, DW_AT_location[DW_OP_regx 0x60]
$C$DW$193	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("EFR")
	.dwattr $C$DW$193, DW_AT_location[DW_OP_regx 0x61]
$C$DW$194	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ECR")
	.dwattr $C$DW$194, DW_AT_location[DW_OP_regx 0x62]
$C$DW$195	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IERR")
	.dwattr $C$DW$195, DW_AT_location[DW_OP_regx 0x63]
$C$DW$196	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DMSG")
	.dwattr $C$DW$196, DW_AT_location[DW_OP_regx 0x64]
$C$DW$197	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CMSG")
	.dwattr $C$DW$197, DW_AT_location[DW_OP_regx 0x65]
$C$DW$198	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_ADDR")
	.dwattr $C$DW$198, DW_AT_location[DW_OP_regx 0x66]
$C$DW$199	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_DATA")
	.dwattr $C$DW$199, DW_AT_location[DW_OP_regx 0x67]
$C$DW$200	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_CNTL")
	.dwattr $C$DW$200, DW_AT_location[DW_OP_regx 0x68]
$C$DW$201	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TCU_CNTL")
	.dwattr $C$DW$201, DW_AT_location[DW_OP_regx 0x69]
$C$DW$202	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_REC_CNTL")
	.dwattr $C$DW$202, DW_AT_location[DW_OP_regx 0x6a]
$C$DW$203	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_XMT_CNTL")
	.dwattr $C$DW$203, DW_AT_location[DW_OP_regx 0x6b]
$C$DW$204	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_CFG")
	.dwattr $C$DW$204, DW_AT_location[DW_OP_regx 0x6c]
$C$DW$205	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_RDATA")
	.dwattr $C$DW$205, DW_AT_location[DW_OP_regx 0x6d]
$C$DW$206	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_WDATA")
	.dwattr $C$DW$206, DW_AT_location[DW_OP_regx 0x6e]
$C$DW$207	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_RADDR")
	.dwattr $C$DW$207, DW_AT_location[DW_OP_regx 0x6f]
$C$DW$208	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_WADDR")
	.dwattr $C$DW$208, DW_AT_location[DW_OP_regx 0x70]
$C$DW$209	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("MFREG0")
	.dwattr $C$DW$209, DW_AT_location[DW_OP_regx 0x71]
$C$DW$210	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DBG_STAT")
	.dwattr $C$DW$210, DW_AT_location[DW_OP_regx 0x72]
$C$DW$211	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("BRK_EN")
	.dwattr $C$DW$211, DW_AT_location[DW_OP_regx 0x73]
$C$DW$212	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP0_CNT")
	.dwattr $C$DW$212, DW_AT_location[DW_OP_regx 0x74]
$C$DW$213	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP0")
	.dwattr $C$DW$213, DW_AT_location[DW_OP_regx 0x75]
$C$DW$214	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP1")
	.dwattr $C$DW$214, DW_AT_location[DW_OP_regx 0x76]
$C$DW$215	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP2")
	.dwattr $C$DW$215, DW_AT_location[DW_OP_regx 0x77]
$C$DW$216	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP3")
	.dwattr $C$DW$216, DW_AT_location[DW_OP_regx 0x78]
$C$DW$217	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("OVERLAY")
	.dwattr $C$DW$217, DW_AT_location[DW_OP_regx 0x79]
$C$DW$218	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("PC_PROF")
	.dwattr $C$DW$218, DW_AT_location[DW_OP_regx 0x7a]
$C$DW$219	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ATSR")
	.dwattr $C$DW$219, DW_AT_location[DW_OP_regx 0x7b]
$C$DW$220	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TRR")
	.dwattr $C$DW$220, DW_AT_location[DW_OP_regx 0x7c]
$C$DW$221	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TCRR")
	.dwattr $C$DW$221, DW_AT_location[DW_OP_regx 0x7d]
$C$DW$222	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DESR")
	.dwattr $C$DW$222, DW_AT_location[DW_OP_regx 0x7e]
$C$DW$223	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DETR")
	.dwattr $C$DW$223, DW_AT_location[DW_OP_regx 0x7f]
$C$DW$224	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CIE_RETA")
	.dwattr $C$DW$224, DW_AT_location[DW_OP_regx 0xe4]
	.dwendtag $C$DW$CU

