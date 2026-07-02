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
	.dwattr $C$DW$CU, DW_AT_name("C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c")
	.dwattr $C$DW$CU, DW_AT_producer("TMS320C6x C/C++ Codegen PC v7.4.2 Copyright (c) 1996-2012 Texas Instruments Incorporated")
	.dwattr $C$DW$CU, DW_AT_TI_version(0x01)
	.dwattr $C$DW$CU, DW_AT_comp_dir("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug")

$C$DW$1	.dwtag  DW_TAG_subprogram, DW_AT_name("fclose")
	.dwattr $C$DW$1, DW_AT_TI_symbol_name("_fclose")
	.dwattr $C$DW$1, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$1, DW_AT_declaration
	.dwattr $C$DW$1, DW_AT_external
$C$DW$2	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$2, DW_AT_type(*$C$DW$T$22)
	.dwendtag $C$DW$1


$C$DW$3	.dwtag  DW_TAG_subprogram, DW_AT_name("fopen")
	.dwattr $C$DW$3, DW_AT_TI_symbol_name("_fopen")
	.dwattr $C$DW$3, DW_AT_type(*$C$DW$T$22)
	.dwattr $C$DW$3, DW_AT_declaration
	.dwattr $C$DW$3, DW_AT_external
$C$DW$4	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$4, DW_AT_type(*$C$DW$T$25)
$C$DW$5	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$5, DW_AT_type(*$C$DW$T$25)
	.dwendtag $C$DW$3


$C$DW$6	.dwtag  DW_TAG_subprogram, DW_AT_name("fprintf")
	.dwattr $C$DW$6, DW_AT_TI_symbol_name("_fprintf")
	.dwattr $C$DW$6, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$6, DW_AT_declaration
	.dwattr $C$DW$6, DW_AT_external
$C$DW$7	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$7, DW_AT_type(*$C$DW$T$22)
$C$DW$8	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$8, DW_AT_type(*$C$DW$T$25)
$C$DW$9	.dwtag  DW_TAG_unspecified_parameters
	.dwendtag $C$DW$6


$C$DW$10	.dwtag  DW_TAG_subprogram, DW_AT_name("printf")
	.dwattr $C$DW$10, DW_AT_TI_symbol_name("_printf")
	.dwattr $C$DW$10, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$10, DW_AT_declaration
	.dwattr $C$DW$10, DW_AT_external
$C$DW$11	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$11, DW_AT_type(*$C$DW$T$25)
$C$DW$12	.dwtag  DW_TAG_unspecified_parameters
	.dwendtag $C$DW$10


$C$DW$13	.dwtag  DW_TAG_subprogram, DW_AT_name("rand")
	.dwattr $C$DW$13, DW_AT_TI_symbol_name("_rand")
	.dwattr $C$DW$13, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$13, DW_AT_declaration
	.dwattr $C$DW$13, DW_AT_external

$C$DW$14	.dwtag  DW_TAG_subprogram, DW_AT_name("srand")
	.dwattr $C$DW$14, DW_AT_TI_symbol_name("_srand")
	.dwattr $C$DW$14, DW_AT_declaration
	.dwattr $C$DW$14, DW_AT_external
$C$DW$15	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$15, DW_AT_type(*$C$DW$T$11)
	.dwendtag $C$DW$14


$C$DW$16	.dwtag  DW_TAG_subprogram, DW_AT_name("exit")
	.dwattr $C$DW$16, DW_AT_TI_symbol_name("_exit")
	.dwattr $C$DW$16, DW_AT_declaration
	.dwattr $C$DW$16, DW_AT_external
$C$DW$17	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$17, DW_AT_type(*$C$DW$T$10)
	.dwendtag $C$DW$16

$C$DW$18	.dwtag  DW_TAG_variable, DW_AT_name("kernel_asm_size")
	.dwattr $C$DW$18, DW_AT_TI_symbol_name("_kernel_asm_size")
	.dwattr $C$DW$18, DW_AT_type(*$C$DW$T$23)
	.dwattr $C$DW$18, DW_AT_declaration
	.dwattr $C$DW$18, DW_AT_external
$C$DW$19	.dwtag  DW_TAG_variable, DW_AT_name("kernel_vec_size")
	.dwattr $C$DW$19, DW_AT_TI_symbol_name("_kernel_vec_size")
	.dwattr $C$DW$19, DW_AT_type(*$C$DW$T$23)
	.dwattr $C$DW$19, DW_AT_declaration
	.dwattr $C$DW$19, DW_AT_external
$C$DW$20	.dwtag  DW_TAG_variable, DW_AT_name("kernel_ci_size")
	.dwattr $C$DW$20, DW_AT_TI_symbol_name("_kernel_ci_size")
	.dwattr $C$DW$20, DW_AT_type(*$C$DW$T$23)
	.dwattr $C$DW$20, DW_AT_declaration
	.dwattr $C$DW$20, DW_AT_external
	.global	_seed
_seed:	.usect	".far",4,4
$C$DW$21	.dwtag  DW_TAG_variable, DW_AT_name("seed")
	.dwattr $C$DW$21, DW_AT_TI_symbol_name("_seed")
	.dwattr $C$DW$21, DW_AT_location[DW_OP_addr _seed]
	.dwattr $C$DW$21, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$21, DW_AT_external
	.global	_fp
_fp:	.usect	".far",4,4
$C$DW$22	.dwtag  DW_TAG_variable, DW_AT_name("fp")
	.dwattr $C$DW$22, DW_AT_TI_symbol_name("_fp")
	.dwattr $C$DW$22, DW_AT_location[DW_OP_addr _fp]
	.dwattr $C$DW$22, DW_AT_type(*$C$DW$T$22)
	.dwattr $C$DW$22, DW_AT_external
	.global	_t_start
_t_start:	.usect	".far",8,8
$C$DW$23	.dwtag  DW_TAG_variable, DW_AT_name("t_start")
	.dwattr $C$DW$23, DW_AT_TI_symbol_name("_t_start")
	.dwattr $C$DW$23, DW_AT_location[DW_OP_addr _t_start]
	.dwattr $C$DW$23, DW_AT_type(*$C$DW$T$14)
	.dwattr $C$DW$23, DW_AT_external
	.global	_t_stop
_t_stop:	.usect	".far",8,8
$C$DW$24	.dwtag  DW_TAG_variable, DW_AT_name("t_stop")
	.dwattr $C$DW$24, DW_AT_TI_symbol_name("_t_stop")
	.dwattr $C$DW$24, DW_AT_location[DW_OP_addr _t_stop]
	.dwattr $C$DW$24, DW_AT_type(*$C$DW$T$14)
	.dwattr $C$DW$24, DW_AT_external
	.global	_t_offset
_t_offset:	.usect	".far",8,8
$C$DW$25	.dwtag  DW_TAG_variable, DW_AT_name("t_offset")
	.dwattr $C$DW$25, DW_AT_TI_symbol_name("_t_offset")
	.dwattr $C$DW$25, DW_AT_location[DW_OP_addr _t_offset]
	.dwattr $C$DW$25, DW_AT_type(*$C$DW$T$14)
	.dwattr $C$DW$25, DW_AT_external
	.global	_cycle_counts
_cycle_counts:	.usect	".far",40,8
$C$DW$26	.dwtag  DW_TAG_variable, DW_AT_name("cycle_counts")
	.dwattr $C$DW$26, DW_AT_TI_symbol_name("_cycle_counts")
	.dwattr $C$DW$26, DW_AT_location[DW_OP_addr _cycle_counts]
	.dwattr $C$DW$26, DW_AT_type(*$C$DW$T$44)
	.dwattr $C$DW$26, DW_AT_external
	.global	_fcn_pass
_fcn_pass:	.usect	".far",20,8
$C$DW$27	.dwtag  DW_TAG_variable, DW_AT_name("fcn_pass")
	.dwattr $C$DW$27, DW_AT_TI_symbol_name("_fcn_pass")
	.dwattr $C$DW$27, DW_AT_location[DW_OP_addr _fcn_pass]
	.dwattr $C$DW$27, DW_AT_type(*$C$DW$T$42)
	.dwattr $C$DW$27, DW_AT_external
	.global	_all_pass
_all_pass:	.usect	".far",4,4
$C$DW$28	.dwtag  DW_TAG_variable, DW_AT_name("all_pass")
	.dwattr $C$DW$28, DW_AT_TI_symbol_name("_all_pass")
	.dwattr $C$DW$28, DW_AT_location[DW_OP_addr _all_pass]
	.dwattr $C$DW$28, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$28, DW_AT_external
;	C:\MATHLIB_Tools\CCSV5_5_0\ccsv5\tools\compiler\c6000_7.4.2\bin\acp6x.exe -@C:\\Users\\gtbldadm\\AppData\\Local\\Temp\\0198018 
	.sect	".text"
	.clink
	.global	_driver_init

$C$DW$29	.dwtag  DW_TAG_subprogram, DW_AT_name("driver_init")
	.dwattr $C$DW$29, DW_AT_low_pc(_driver_init)
	.dwattr $C$DW$29, DW_AT_high_pc(0x00)
	.dwattr $C$DW$29, DW_AT_TI_symbol_name("_driver_init")
	.dwattr $C$DW$29, DW_AT_external
	.dwattr $C$DW$29, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c")
	.dwattr $C$DW$29, DW_AT_TI_begin_line(0x40)
	.dwattr $C$DW$29, DW_AT_TI_begin_column(0x06)
	.dwattr $C$DW$29, DW_AT_TI_max_frame_size(0x10)
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 65,column 1,is_stmt,address _driver_init

	.dwfde $C$DW$CIE, _driver_init
$C$DW$30	.dwtag  DW_TAG_formal_parameter, DW_AT_name("str")
	.dwattr $C$DW$30, DW_AT_TI_symbol_name("_str")
	.dwattr $C$DW$30, DW_AT_type(*$C$DW$T$32)
	.dwattr $C$DW$30, DW_AT_location[DW_OP_reg4]

;******************************************************************************
;* FUNCTION NAME: driver_init                                                 *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,DP,SP,A16,A17,A18,A19,A20,A21,A22,A23,  *
;*                           A24,A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19, *
;*                           B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31  *
;*   Local Frame Size  : 8 Args + 4 Auto + 4 Save = 16 byte                   *
;******************************************************************************
_driver_init:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 76
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           STW     .D2T2   B3,*SP--(16)      ; |65| 
	.dwcfi	cfa_offset, 16
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$31	.dwtag  DW_TAG_variable, DW_AT_name("str")
	.dwattr $C$DW$31, DW_AT_TI_symbol_name("_str")
	.dwattr $C$DW$31, DW_AT_type(*$C$DW$T$32)
	.dwattr $C$DW$31, DW_AT_location[DW_OP_breg31 12]
           STW     .D2T1   A4,*+SP(12)       ; |65| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 67,column 3,is_stmt
           MVKL    .S2     $C$SL1+0,B4
           MVKH    .S2     $C$SL1+0,B4
$C$DW$32	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$32, DW_AT_low_pc(0x00)
	.dwattr $C$DW$32, DW_AT_name("_printf")
	.dwattr $C$DW$32, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |67| 

$C$RL0:    ; CALL OCCURS {_printf} {0}       ; |67| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 68,column 3,is_stmt
           MVKL    .S2     $C$SL2+0,B4
           MVKH    .S2     $C$SL2+0,B4
$C$DW$33	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$33, DW_AT_low_pc(0x00)
	.dwattr $C$DW$33, DW_AT_name("_printf")
	.dwattr $C$DW$33, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |68| 

$C$RL1:    ; CALL OCCURS {_printf} {0}       ; |68| 
           MVKL    .S2     $C$SL3+0,B4
           MVKH    .S2     $C$SL3+0,B4
$C$DW$34	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$34, DW_AT_low_pc(0x00)
	.dwattr $C$DW$34, DW_AT_name("_printf")
	.dwattr $C$DW$34, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |68| 

$C$RL2:    ; CALL OCCURS {_printf} {0}       ; |68| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 69,column 3,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |69| 
           MVKL    .S2     $C$SL4+0,B5
           MVKH    .S2     $C$SL4+0,B5
           STW     .D2T2   B5,*+SP(4)        ; |69| 
           NOP             1
$C$DW$35	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$35, DW_AT_low_pc(0x00)
	.dwattr $C$DW$35, DW_AT_name("_printf")
	.dwattr $C$DW$35, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(8)        ; |69| 

$C$RL3:    ; CALL OCCURS {_printf} {0}       ; |69| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 70,column 3,is_stmt
           MVKL    .S2     $C$SL2+0,B4
           MVKH    .S2     $C$SL2+0,B4
$C$DW$36	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$36, DW_AT_low_pc(0x00)
	.dwattr $C$DW$36, DW_AT_name("_printf")
	.dwattr $C$DW$36, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |70| 

$C$RL4:    ; CALL OCCURS {_printf} {0}       ; |70| 
           MVKL    .S2     $C$SL3+0,B4
           MVKH    .S2     $C$SL3+0,B4
$C$DW$37	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$37, DW_AT_low_pc(0x00)
	.dwattr $C$DW$37, DW_AT_name("_printf")
	.dwattr $C$DW$37, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |70| 

$C$RL5:    ; CALL OCCURS {_printf} {0}       ; |70| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 73,column 3,is_stmt
           MVKL    .S2     0x2a3a4a5a,B4
           MVKH    .S2     0x2a3a4a5a,B4
           MVKL    .S2     _seed,B5
           MVKH    .S2     _seed,B5
           STW     .D2T2   B4,*B5            ; |73| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 78,column 3,is_stmt
           MV      .L1X    B5,A3
$C$DW$38	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$38, DW_AT_low_pc(0x00)
	.dwattr $C$DW$38, DW_AT_name("_srand")
	.dwattr $C$DW$38, DW_AT_TI_call

           CALLP   .S2     _srand,B3
||         LDW     .D1T1   *A3,A4            ; |78| 

$C$RL6:    ; CALL OCCURS {_srand} {0}        ; |78| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 79,column 1,is_stmt
           LDW     .D2T2   *++SP(16),B3      ; |79| 
           NOP             4
	.dwcfi	cfa_offset, 0
	.dwcfi	restore_reg, 19
	.dwcfi	cfa_offset, 0
$C$DW$39	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$39, DW_AT_low_pc(0x00)
	.dwattr $C$DW$39, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |79| 
           ; BRANCH OCCURS {B3}              ; |79| 
	.dwattr $C$DW$29, DW_AT_TI_end_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c")
	.dwattr $C$DW$29, DW_AT_TI_end_line(0x4f)
	.dwattr $C$DW$29, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$29

	.sect	".text"
	.clink
	.global	_print_profile_results

$C$DW$40	.dwtag  DW_TAG_subprogram, DW_AT_name("print_profile_results")
	.dwattr $C$DW$40, DW_AT_low_pc(_print_profile_results)
	.dwattr $C$DW$40, DW_AT_high_pc(0x00)
	.dwattr $C$DW$40, DW_AT_TI_symbol_name("_print_profile_results")
	.dwattr $C$DW$40, DW_AT_external
	.dwattr $C$DW$40, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c")
	.dwattr $C$DW$40, DW_AT_TI_begin_line(0x53)
	.dwattr $C$DW$40, DW_AT_TI_begin_column(0x06)
	.dwattr $C$DW$40, DW_AT_TI_max_frame_size(0x10)
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 84,column 1,is_stmt,address _print_profile_results

	.dwfde $C$DW$CIE, _print_profile_results
$C$DW$41	.dwtag  DW_TAG_formal_parameter, DW_AT_name("str")
	.dwattr $C$DW$41, DW_AT_TI_symbol_name("_str")
	.dwattr $C$DW$41, DW_AT_type(*$C$DW$T$32)
	.dwattr $C$DW$41, DW_AT_location[DW_OP_reg4]

;******************************************************************************
;* FUNCTION NAME: print_profile_results                                       *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,DP,SP,A16,A17,A18,A19,A20,A21,A22,A23,  *
;*                           A24,A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19, *
;*                           B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31  *
;*   Local Frame Size  : 8 Args + 4 Auto + 4 Save = 16 byte                   *
;******************************************************************************
_print_profile_results:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 127
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           STW     .D2T2   B3,*SP--(16)      ; |84| 
	.dwcfi	cfa_offset, 16
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$42	.dwtag  DW_TAG_variable, DW_AT_name("str")
	.dwattr $C$DW$42, DW_AT_TI_symbol_name("_str")
	.dwattr $C$DW$42, DW_AT_type(*$C$DW$T$32)
	.dwattr $C$DW$42, DW_AT_location[DW_OP_breg31 12]
           STW     .D2T1   A4,*+SP(12)       ; |84| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 85,column 3,is_stmt
           MVKL    .S1     $C$SL5+0,A3
           MVKH    .S1     $C$SL5+0,A3
$C$DW$43	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$43, DW_AT_low_pc(0x00)
	.dwattr $C$DW$43, DW_AT_name("_printf")
	.dwattr $C$DW$43, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(4)        ; |85| 

$C$RL7:    ; CALL OCCURS {_printf} {0}       ; |85| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 86,column 3,is_stmt
           MVKL    .S2     $C$SL2+0,B4
           MVKH    .S2     $C$SL2+0,B4
$C$DW$44	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$44, DW_AT_low_pc(0x00)
	.dwattr $C$DW$44, DW_AT_name("_printf")
	.dwattr $C$DW$44, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |86| 

$C$RL8:    ; CALL OCCURS {_printf} {0}       ; |86| 
           MVKL    .S1     $C$SL3+0,A3
           MVKH    .S1     $C$SL3+0,A3
$C$DW$45	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$45, DW_AT_low_pc(0x00)
	.dwattr $C$DW$45, DW_AT_name("_printf")
	.dwattr $C$DW$45, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(4)        ; |86| 

$C$RL9:    ; CALL OCCURS {_printf} {0}       ; |86| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 87,column 3,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |87| 
           MVKL    .S2     $C$SL6+0,B5
           MVKH    .S2     $C$SL6+0,B5
           STW     .D2T2   B5,*+SP(4)        ; |87| 
           NOP             1
$C$DW$46	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$46, DW_AT_low_pc(0x00)
	.dwattr $C$DW$46, DW_AT_name("_printf")
	.dwattr $C$DW$46, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(8)        ; |87| 

$C$RL10:   ; CALL OCCURS {_printf} {0}       ; |87| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 88,column 3,is_stmt
           MVKL    .S1     $C$SL2+0,A3
           MVKH    .S1     $C$SL2+0,A3
$C$DW$47	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$47, DW_AT_low_pc(0x00)
	.dwattr $C$DW$47, DW_AT_name("_printf")
	.dwattr $C$DW$47, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(4)        ; |88| 

$C$RL11:   ; CALL OCCURS {_printf} {0}       ; |88| 
           MVKL    .S2     $C$SL3+0,B4
           MVKH    .S2     $C$SL3+0,B4
$C$DW$48	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$48, DW_AT_low_pc(0x00)
	.dwattr $C$DW$48, DW_AT_name("_printf")
	.dwattr $C$DW$48, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |88| 

$C$RL12:   ; CALL OCCURS {_printf} {0}       ; |88| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 89,column 3,is_stmt
           MVKL    .S1     _cycle_counts+4,A3
           MVKL    .S1     $C$SL7+0,A4
           MVKH    .S1     $C$SL7+0,A4

           STW     .D2T1   A4,*+SP(4)        ; |89| 
||         MVKH    .S1     _cycle_counts+4,A3

           LDW     .D1T1   *A3,A3            ; |89| 
           NOP             4
$C$DW$49	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$49, DW_AT_low_pc(0x00)
	.dwattr $C$DW$49, DW_AT_name("_printf")
	.dwattr $C$DW$49, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(8)        ; |89| 

$C$RL13:   ; CALL OCCURS {_printf} {0}       ; |89| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 90,column 3,is_stmt
           MVKL    .S2     _cycle_counts+12,B4
           MVKL    .S2     $C$SL8+0,B5
           MVKH    .S2     $C$SL8+0,B5

           STW     .D2T2   B5,*+SP(4)        ; |90| 
||         MVKH    .S2     _cycle_counts+12,B4

           LDW     .D2T2   *B4,B4            ; |90| 
           NOP             4
$C$DW$50	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$50, DW_AT_low_pc(0x00)
	.dwattr $C$DW$50, DW_AT_name("_printf")
	.dwattr $C$DW$50, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(8)        ; |90| 

$C$RL14:   ; CALL OCCURS {_printf} {0}       ; |90| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 91,column 3,is_stmt
           MVKL    .S1     $C$SL9+0,A4
           MVKL    .S1     _cycle_counts+20,A3
           MVKH    .S1     $C$SL9+0,A4

           STW     .D2T1   A4,*+SP(4)        ; |91| 
||         MVKH    .S1     _cycle_counts+20,A3

           LDW     .D1T1   *A3,A3            ; |91| 
           NOP             4
$C$DW$51	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$51, DW_AT_low_pc(0x00)
	.dwattr $C$DW$51, DW_AT_name("_printf")
	.dwattr $C$DW$51, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(8)        ; |91| 

$C$RL15:   ; CALL OCCURS {_printf} {0}       ; |91| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 92,column 3,is_stmt
           MVKL    .S2     $C$SL10+0,B5
           MVKL    .S2     _cycle_counts+28,B4
           MVKH    .S2     $C$SL10+0,B5

           STW     .D2T2   B5,*+SP(4)        ; |92| 
||         MVKH    .S2     _cycle_counts+28,B4

           LDW     .D2T2   *B4,B4            ; |92| 
           NOP             4
$C$DW$52	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$52, DW_AT_low_pc(0x00)
	.dwattr $C$DW$52, DW_AT_name("_printf")
	.dwattr $C$DW$52, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(8)        ; |92| 

$C$RL16:   ; CALL OCCURS {_printf} {0}       ; |92| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 93,column 3,is_stmt
           MVKL    .S2     $C$SL11+0,B4

           MVKH    .S2     $C$SL11+0,B4
||         MVKL    .S1     _cycle_counts+36,A3

           STW     .D2T2   B4,*+SP(4)        ; |93| 
||         MVKH    .S1     _cycle_counts+36,A3

           LDW     .D1T1   *A3,A3            ; |93| 
           NOP             4
$C$DW$53	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$53, DW_AT_low_pc(0x00)
	.dwattr $C$DW$53, DW_AT_name("_printf")
	.dwattr $C$DW$53, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(8)        ; |93| 

$C$RL17:   ; CALL OCCURS {_printf} {0}       ; |93| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 27
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 94,column 3,is_stmt
           MVKL    .S1     $C$SL2+0,A3
           MVKH    .S1     $C$SL2+0,A3
$C$DW$54	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$54, DW_AT_low_pc(0x00)
	.dwattr $C$DW$54, DW_AT_name("_printf")
	.dwattr $C$DW$54, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(4)        ; |94| 

$C$RL18:   ; CALL OCCURS {_printf} {0}       ; |94| 
           MVKL    .S2     $C$SL3+0,B4
           MVKH    .S2     $C$SL3+0,B4
$C$DW$55	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$55, DW_AT_low_pc(0x00)
	.dwattr $C$DW$55, DW_AT_name("_printf")
	.dwattr $C$DW$55, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |94| 

$C$RL19:   ; CALL OCCURS {_printf} {0}       ; |94| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 95,column 1,is_stmt
           LDW     .D2T2   *++SP(16),B3      ; |95| 
           NOP             4
	.dwcfi	cfa_offset, 0
	.dwcfi	restore_reg, 19
	.dwcfi	cfa_offset, 0
$C$DW$56	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$56, DW_AT_low_pc(0x00)
	.dwattr $C$DW$56, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |95| 
           ; BRANCH OCCURS {B3}              ; |95| 
	.dwattr $C$DW$40, DW_AT_TI_end_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c")
	.dwattr $C$DW$40, DW_AT_TI_end_line(0x5f)
	.dwattr $C$DW$40, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$40

	.sect	".text"
	.clink
	.global	_print_memory_results

$C$DW$57	.dwtag  DW_TAG_subprogram, DW_AT_name("print_memory_results")
	.dwattr $C$DW$57, DW_AT_low_pc(_print_memory_results)
	.dwattr $C$DW$57, DW_AT_high_pc(0x00)
	.dwattr $C$DW$57, DW_AT_TI_symbol_name("_print_memory_results")
	.dwattr $C$DW$57, DW_AT_external
	.dwattr $C$DW$57, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c")
	.dwattr $C$DW$57, DW_AT_TI_begin_line(0x63)
	.dwattr $C$DW$57, DW_AT_TI_begin_column(0x06)
	.dwattr $C$DW$57, DW_AT_TI_max_frame_size(0x10)
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 100,column 1,is_stmt,address _print_memory_results

	.dwfde $C$DW$CIE, _print_memory_results
$C$DW$58	.dwtag  DW_TAG_formal_parameter, DW_AT_name("str")
	.dwattr $C$DW$58, DW_AT_TI_symbol_name("_str")
	.dwattr $C$DW$58, DW_AT_type(*$C$DW$T$32)
	.dwattr $C$DW$58, DW_AT_location[DW_OP_reg4]

;******************************************************************************
;* FUNCTION NAME: print_memory_results                                        *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,DP,SP,A16,A17,A18,A19,A20,A21,A22,A23,  *
;*                           A24,A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19, *
;*                           B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31  *
;*   Local Frame Size  : 8 Args + 4 Auto + 4 Save = 16 byte                   *
;******************************************************************************
_print_memory_results:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 96
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           STW     .D2T2   B3,*SP--(16)      ; |100| 
	.dwcfi	cfa_offset, 16
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$59	.dwtag  DW_TAG_variable, DW_AT_name("str")
	.dwattr $C$DW$59, DW_AT_TI_symbol_name("_str")
	.dwattr $C$DW$59, DW_AT_type(*$C$DW$T$32)
	.dwattr $C$DW$59, DW_AT_location[DW_OP_breg31 12]
           STW     .D2T1   A4,*+SP(12)       ; |100| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 101,column 3,is_stmt
           MVKL    .S1     $C$SL5+0,A3
           MVKH    .S1     $C$SL5+0,A3
$C$DW$60	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$60, DW_AT_low_pc(0x00)
	.dwattr $C$DW$60, DW_AT_name("_printf")
	.dwattr $C$DW$60, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(4)        ; |101| 

$C$RL20:   ; CALL OCCURS {_printf} {0}       ; |101| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 102,column 3,is_stmt
           MVKL    .S2     $C$SL2+0,B4
           MVKH    .S2     $C$SL2+0,B4
$C$DW$61	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$61, DW_AT_low_pc(0x00)
	.dwattr $C$DW$61, DW_AT_name("_printf")
	.dwattr $C$DW$61, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |102| 

$C$RL21:   ; CALL OCCURS {_printf} {0}       ; |102| 
           MVKL    .S1     $C$SL3+0,A3
           MVKH    .S1     $C$SL3+0,A3
$C$DW$62	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$62, DW_AT_low_pc(0x00)
	.dwattr $C$DW$62, DW_AT_name("_printf")
	.dwattr $C$DW$62, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(4)        ; |102| 

$C$RL22:   ; CALL OCCURS {_printf} {0}       ; |102| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 103,column 3,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |103| 
           MVKL    .S2     $C$SL12+0,B5
           MVKH    .S2     $C$SL12+0,B5
           STW     .D2T2   B5,*+SP(4)        ; |103| 
           NOP             1
$C$DW$63	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$63, DW_AT_low_pc(0x00)
	.dwattr $C$DW$63, DW_AT_name("_printf")
	.dwattr $C$DW$63, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(8)        ; |103| 

$C$RL23:   ; CALL OCCURS {_printf} {0}       ; |103| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 104,column 3,is_stmt
           MVKL    .S1     $C$SL2+0,A3
           MVKH    .S1     $C$SL2+0,A3
$C$DW$64	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$64, DW_AT_low_pc(0x00)
	.dwattr $C$DW$64, DW_AT_name("_printf")
	.dwattr $C$DW$64, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(4)        ; |104| 

$C$RL24:   ; CALL OCCURS {_printf} {0}       ; |104| 
           MVKL    .S2     $C$SL3+0,B4
           MVKH    .S2     $C$SL3+0,B4
$C$DW$65	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$65, DW_AT_low_pc(0x00)
	.dwattr $C$DW$65, DW_AT_name("_printf")
	.dwattr $C$DW$65, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |104| 

$C$RL25:   ; CALL OCCURS {_printf} {0}       ; |104| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 105,column 3,is_stmt
           MVKL    .S2     _kernel_asm_size,B4

           MVKL    .S1     $C$SL13+0,A3
||         MVKH    .S2     _kernel_asm_size,B4

           MVKH    .S1     $C$SL13+0,A3
||         STW     .D2T2   B4,*+SP(8)        ; |105| 

$C$DW$66	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$66, DW_AT_low_pc(0x00)
	.dwattr $C$DW$66, DW_AT_name("_printf")
	.dwattr $C$DW$66, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(4)        ; |105| 

$C$RL26:   ; CALL OCCURS {_printf} {0}       ; |105| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 106,column 3,is_stmt
           MVKL    .S1     $C$SL14+0,A3

           MVKL    .S2     _kernel_ci_size,B4
||         MVKH    .S1     $C$SL14+0,A3

           MVKH    .S2     _kernel_ci_size,B4
||         STW     .D2T1   A3,*+SP(4)        ; |106| 

$C$DW$67	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$67, DW_AT_low_pc(0x00)
	.dwattr $C$DW$67, DW_AT_name("_printf")
	.dwattr $C$DW$67, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(8)        ; |106| 

$C$RL27:   ; CALL OCCURS {_printf} {0}       ; |106| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 107,column 3,is_stmt
           MVKL    .S1     $C$SL15+0,A3

           MVKL    .S2     _kernel_vec_size,B4
||         MVKH    .S1     $C$SL15+0,A3

           MVKH    .S2     _kernel_vec_size,B4
||         STW     .D2T1   A3,*+SP(4)        ; |107| 

$C$DW$68	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$68, DW_AT_low_pc(0x00)
	.dwattr $C$DW$68, DW_AT_name("_printf")
	.dwattr $C$DW$68, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(8)        ; |107| 

$C$RL28:   ; CALL OCCURS {_printf} {0}       ; |107| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 108,column 3,is_stmt
           MVKL    .S1     $C$SL2+0,A3
           MVKH    .S1     $C$SL2+0,A3
$C$DW$69	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$69, DW_AT_low_pc(0x00)
	.dwattr $C$DW$69, DW_AT_name("_printf")
	.dwattr $C$DW$69, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(4)        ; |108| 

$C$RL29:   ; CALL OCCURS {_printf} {0}       ; |108| 
           MVKL    .S2     $C$SL3+0,B4
           MVKH    .S2     $C$SL3+0,B4
$C$DW$70	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$70, DW_AT_low_pc(0x00)
	.dwattr $C$DW$70, DW_AT_name("_printf")
	.dwattr $C$DW$70, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |108| 

$C$RL30:   ; CALL OCCURS {_printf} {0}       ; |108| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 109,column 1,is_stmt
           LDW     .D2T2   *++SP(16),B3      ; |109| 
           NOP             4
	.dwcfi	cfa_offset, 0
	.dwcfi	restore_reg, 19
	.dwcfi	cfa_offset, 0
$C$DW$71	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$71, DW_AT_low_pc(0x00)
	.dwattr $C$DW$71, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |109| 
           ; BRANCH OCCURS {B3}              ; |109| 
	.dwattr $C$DW$57, DW_AT_TI_end_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c")
	.dwattr $C$DW$57, DW_AT_TI_end_line(0x6d)
	.dwattr $C$DW$57, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$57

	.sect	".text"
	.clink
	.global	_print_test_results

$C$DW$72	.dwtag  DW_TAG_subprogram, DW_AT_name("print_test_results")
	.dwattr $C$DW$72, DW_AT_low_pc(_print_test_results)
	.dwattr $C$DW$72, DW_AT_high_pc(0x00)
	.dwattr $C$DW$72, DW_AT_TI_symbol_name("_print_test_results")
	.dwattr $C$DW$72, DW_AT_external
	.dwattr $C$DW$72, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c")
	.dwattr $C$DW$72, DW_AT_TI_begin_line(0x71)
	.dwattr $C$DW$72, DW_AT_TI_begin_column(0x06)
	.dwattr $C$DW$72, DW_AT_TI_max_frame_size(0x18)
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 114,column 1,is_stmt,address _print_test_results

	.dwfde $C$DW$CIE, _print_test_results
$C$DW$73	.dwtag  DW_TAG_formal_parameter, DW_AT_name("code")
	.dwattr $C$DW$73, DW_AT_TI_symbol_name("_code")
	.dwattr $C$DW$73, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$73, DW_AT_location[DW_OP_reg4]

;******************************************************************************
;* FUNCTION NAME: print_test_results                                          *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,DP,SP,A16,A17,A18,A19,A20,A21,A22,A23,  *
;*                           A24,A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19, *
;*                           B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31  *
;*   Local Frame Size  : 8 Args + 8 Auto + 4 Save = 20 byte                   *
;******************************************************************************
_print_test_results:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           STW     .D2T2   B3,*SP--(24)      ; |114| 
	.dwcfi	cfa_offset, 24
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$74	.dwtag  DW_TAG_variable, DW_AT_name("code")
	.dwattr $C$DW$74, DW_AT_TI_symbol_name("_code")
	.dwattr $C$DW$74, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$74, DW_AT_location[DW_OP_breg31 12]
$C$DW$75	.dwtag  DW_TAG_variable, DW_AT_name("comma")
	.dwattr $C$DW$75, DW_AT_TI_symbol_name("_comma")
	.dwattr $C$DW$75, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$75, DW_AT_location[DW_OP_breg31 16]
           STW     .D2T1   A4,*+SP(12)       ; |114| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 115,column 7,is_stmt
           ZERO    .L2     B4                ; |115| 
           STW     .D2T2   B4,*+SP(16)       ; |115| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 118,column 3,is_stmt
           CMPEQ   .L2X    A4,1,B0           ; |118| 
   [!B0]   BNOP    .S1     $C$L1,4           ; |118| 
           MV      .L2X    A4,B4
           ; BRANCHCC OCCURS {$C$L1}         ; |118| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 14
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 119,column 5,is_stmt
           MVKL    .S2     $C$SL16+0,B4
           MVKH    .S2     $C$SL16+0,B4
$C$DW$76	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$76, DW_AT_low_pc(0x00)
	.dwattr $C$DW$76, DW_AT_name("_printf")
	.dwattr $C$DW$76, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |119| 

$C$RL31:   ; CALL OCCURS {_printf} {0}       ; |119| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 120,column 3,is_stmt
           BNOP    .S1     $C$L5,5           ; |120| 
           ; BRANCH OCCURS {$C$L5}           ; |120| 
;** --------------------------------------------------------------------------*
$C$L1:    
;          EXCLUSIVE CPU CYCLES: 7
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 121,column 8,is_stmt
           CMPEQ   .L2     B4,2,B0           ; |121| 
   [!B0]   BNOP    .S1     $C$L2,5           ; |121| 
           ; BRANCHCC OCCURS {$C$L2}         ; |121| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 20
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 122,column 5,is_stmt
           MVKL    .S2     $C$SL17+0,B4

           MVKH    .S2     $C$SL17+0,B4
||         MVKL    .S1     _seed,A3

           STW     .D2T2   B4,*+SP(4)        ; |122| 
||         MVKH    .S1     _seed,A3

           LDW     .D1T1   *A3,A3            ; |122| 
           NOP             4
$C$DW$77	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$77, DW_AT_low_pc(0x00)
	.dwattr $C$DW$77, DW_AT_name("_printf")
	.dwattr $C$DW$77, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T1   A3,*+SP(8)        ; |122| 

$C$RL32:   ; CALL OCCURS {_printf} {0}       ; |122| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 123,column 3,is_stmt
           BNOP    .S1     $C$L5,5           ; |123| 
           ; BRANCH OCCURS {$C$L5}           ; |123| 
;** --------------------------------------------------------------------------*
$C$L2:    
;          EXCLUSIVE CPU CYCLES: 7
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 124,column 8,is_stmt
           CMPEQ   .L2     B4,3,B0           ; |124| 
   [!B0]   BNOP    .S1     $C$L3,5           ; |124| 
           ; BRANCHCC OCCURS {$C$L3}         ; |124| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 14
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 125,column 5,is_stmt
           MVKL    .S2     $C$SL18+0,B4
           MVKH    .S2     $C$SL18+0,B4
$C$DW$78	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$78, DW_AT_low_pc(0x00)
	.dwattr $C$DW$78, DW_AT_name("_printf")
	.dwattr $C$DW$78, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |125| 

$C$RL33:   ; CALL OCCURS {_printf} {0}       ; |125| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 126,column 3,is_stmt
           BNOP    .S1     $C$L5,5           ; |126| 
           ; BRANCH OCCURS {$C$L5}           ; |126| 
;** --------------------------------------------------------------------------*
$C$L3:    
;          EXCLUSIVE CPU CYCLES: 7
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 127,column 8,is_stmt
           CMPEQ   .L2     B4,4,B0           ; |127| 
   [!B0]   BNOP    .S1     $C$L4,5           ; |127| 
           ; BRANCHCC OCCURS {$C$L4}         ; |127| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 14
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 128,column 5,is_stmt
           MVKL    .S2     $C$SL19+0,B4
           MVKH    .S2     $C$SL19+0,B4
$C$DW$79	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$79, DW_AT_low_pc(0x00)
	.dwattr $C$DW$79, DW_AT_name("_printf")
	.dwattr $C$DW$79, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |128| 

$C$RL34:   ; CALL OCCURS {_printf} {0}       ; |128| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 129,column 3,is_stmt
           BNOP    .S1     $C$L5,5           ; |129| 
           ; BRANCH OCCURS {$C$L5}           ; |129| 
;** --------------------------------------------------------------------------*
$C$L4:    
;          EXCLUSIVE CPU CYCLES: 14
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 131,column 5,is_stmt
           MVKL    .S2     $C$SL20+0,B5

           MVKH    .S2     $C$SL20+0,B5
||         STW     .D2T2   B4,*+SP(8)        ; |131| 

$C$DW$80	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$80, DW_AT_low_pc(0x00)
	.dwattr $C$DW$80, DW_AT_name("_printf")
	.dwattr $C$DW$80, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B5,*+SP(4)        ; |131| 

$C$RL35:   ; CALL OCCURS {_printf} {0}       ; |131| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 132,column 5,is_stmt
$C$DW$81	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$81, DW_AT_low_pc(0x00)
	.dwattr $C$DW$81, DW_AT_name("_exit")
	.dwattr $C$DW$81, DW_AT_TI_call

           CALLP   .S2     _exit,B3
||         MVK     .L1     0x2,A4            ; |132| 

$C$RL36:   ; CALL OCCURS {_exit} {0}         ; |132| 
;** --------------------------------------------------------------------------*
$C$L5:    
;          EXCLUSIVE CPU CYCLES: 13
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 136,column 3,is_stmt
           MVKL    .S1     _all_pass,A3
           MVKH    .S1     _all_pass,A3
           LDW     .D1T1   *A3,A0            ; |136| 
           NOP             4
   [!A0]   BNOP    .S1     $C$L7,5           ; |136| 
           ; BRANCHCC OCCURS {$C$L7}         ; |136| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 32
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 137,column 5,is_stmt
           MVKL    .S2     $C$SL21+0,B4
           MVKH    .S2     $C$SL21+0,B4
$C$DW$82	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$82, DW_AT_low_pc(0x00)
	.dwattr $C$DW$82, DW_AT_name("_printf")
	.dwattr $C$DW$82, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |137| 

$C$RL37:   ; CALL OCCURS {_printf} {0}       ; |137| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 140,column 5,is_stmt
$C$DW$83	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$83, DW_AT_low_pc(0x00)
	.dwattr $C$DW$83, DW_AT_name("_rand")
	.dwattr $C$DW$83, DW_AT_TI_call
           CALLP   .S2     _rand,B3
$C$RL38:   ; CALL OCCURS {_rand} {0}         ; |140| 
           MVKL    .S2     _seed,B5

           MVKH    .S2     _seed,B5
||         MV      .L2X    A4,B4             ; |140| 

           STW     .D2T2   B4,*B5            ; |140| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 141,column 5,is_stmt
           MVKL    .S2     $C$SL22+0,B4

           MVKL    .S1     $C$SL23+0,A4
||         MVKH    .S2     $C$SL22+0,B4

$C$DW$84	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$84, DW_AT_low_pc(0x00)
	.dwattr $C$DW$84, DW_AT_name("_fopen")
	.dwattr $C$DW$84, DW_AT_TI_call

           CALLP   .S2     _fopen,B3
||         MVKH    .S1     $C$SL23+0,A4

$C$RL39:   ; CALL OCCURS {_fopen} {0}        ; |141| 
           MV      .L1     A4,A0             ; |141| 
   [ A0]   BNOP    .S1     $C$L6,2           ; |141| 
           MVKL    .S1     _fp,A3
           MVKH    .S1     _fp,A3
           STW     .D1T1   A0,*A3            ; |141| 
           ; BRANCHCC OCCURS {$C$L6}         ; |141| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 15
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 142,column 7,is_stmt
           MVKL    .S1     $C$SL24+0,A3

           MVKL    .S2     $C$SL23+0,B4
||         MVKH    .S1     $C$SL24+0,A3

           MVKH    .S2     $C$SL23+0,B4
||         STW     .D2T1   A3,*+SP(4)        ; |142| 

$C$DW$85	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$85, DW_AT_low_pc(0x00)
	.dwattr $C$DW$85, DW_AT_name("_printf")
	.dwattr $C$DW$85, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(8)        ; |142| 

$C$RL40:   ; CALL OCCURS {_printf} {0}       ; |142| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 143,column 7,is_stmt
$C$DW$86	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$86, DW_AT_low_pc(0x00)
	.dwattr $C$DW$86, DW_AT_name("_exit")
	.dwattr $C$DW$86, DW_AT_TI_call

           CALLP   .S2     _exit,B3
||         MVK     .L1     0x1,A4            ; |143| 

$C$RL41:   ; CALL OCCURS {_exit} {0}         ; |143| 
;** --------------------------------------------------------------------------*
$C$L6:    
;          EXCLUSIVE CPU CYCLES: 30
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 145,column 5,is_stmt
           MVKL    .S2     $C$SL25+0,B5
           MVKL    .S2     _seed,B4
           MVKH    .S2     $C$SL25+0,B5

           STW     .D2T2   B5,*+SP(4)        ; |145| 
||         MVKH    .S2     _seed,B4

           LDW     .D2T2   *B4,B4            ; |145| 
           MVKL    .S2     _fp,B5
           MVKH    .S2     _fp,B5
           NOP             2
           STW     .D2T2   B4,*+SP(8)        ; |145| 
$C$DW$87	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$87, DW_AT_low_pc(0x00)
	.dwattr $C$DW$87, DW_AT_name("_fprintf")
	.dwattr $C$DW$87, DW_AT_TI_call

           CALLP   .S2     _fprintf,B3
||         LDW     .D2T1   *B5,A4            ; |145| 

$C$RL42:   ; CALL OCCURS {_fprintf} {0}      ; |145| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 146,column 5,is_stmt
           MVKL    .S1     _fp,A3
           MVKH    .S1     _fp,A3
$C$DW$88	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$88, DW_AT_low_pc(0x00)
	.dwattr $C$DW$88, DW_AT_name("_fclose")
	.dwattr $C$DW$88, DW_AT_TI_call

           CALLP   .S2     _fclose,B3
||         LDW     .D1T1   *A3,A4            ; |146| 

$C$RL43:   ; CALL OCCURS {_fclose} {0}       ; |146| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 147,column 3,is_stmt
           BNOP    .S1     $C$L15,5          ; |147| 
           ; BRANCH OCCURS {$C$L15}          ; |147| 
;** --------------------------------------------------------------------------*
$C$L7:    
;          EXCLUSIVE CPU CYCLES: 21
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 149,column 5,is_stmt
           MVKL    .S2     $C$SL26+0,B4
           MVKH    .S2     $C$SL26+0,B4
$C$DW$89	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$89, DW_AT_low_pc(0x00)
	.dwattr $C$DW$89, DW_AT_name("_printf")
	.dwattr $C$DW$89, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |149| 

$C$RL44:   ; CALL OCCURS {_printf} {0}       ; |149| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 151,column 5,is_stmt
           MVKL    .S1     _fcn_pass+4,A3
           MVKH    .S1     _fcn_pass+4,A3
           LDW     .D1T1   *A3,A0            ; |151| 
           NOP             4
   [ A0]   BNOP    .S1     $C$L8,5           ; |151| 
           ; BRANCHCC OCCURS {$C$L8}         ; |151| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 10
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 152,column 7,is_stmt
           MVKL    .S2     $C$SL27+0,B4
           MVKH    .S2     $C$SL27+0,B4
$C$DW$90	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$90, DW_AT_low_pc(0x00)
	.dwattr $C$DW$90, DW_AT_name("_printf")
	.dwattr $C$DW$90, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |152| 

$C$RL45:   ; CALL OCCURS {_printf} {0}       ; |152| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 153,column 7,is_stmt
           MVK     .L2     1,B4              ; |153| 
           STW     .D2T2   B4,*+SP(16)       ; |153| 
;** --------------------------------------------------------------------------*
$C$L8:    
;          EXCLUSIVE CPU CYCLES: 13
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 155,column 5,is_stmt
           MVKL    .S1     _fcn_pass+8,A3
           MVKH    .S1     _fcn_pass+8,A3
           LDW     .D1T1   *A3,A0            ; |155| 
           NOP             4
   [ A0]   BNOP    .S1     $C$L10,5          ; |155| 
           ; BRANCHCC OCCURS {$C$L10}        ; |155| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 156,column 7,is_stmt
           LDW     .D2T2   *+SP(16),B0       ; |156| 
           NOP             4
   [!B0]   BNOP    .S1     $C$L9,5           ; |156| 
           ; BRANCHCC OCCURS {$C$L9}         ; |156| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 8
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 156,column 18,is_stmt
           MVKL    .S2     $C$SL28+0,B4
           MVKH    .S2     $C$SL28+0,B4
$C$DW$91	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$91, DW_AT_low_pc(0x00)
	.dwattr $C$DW$91, DW_AT_name("_printf")
	.dwattr $C$DW$91, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |156| 

$C$RL46:   ; CALL OCCURS {_printf} {0}       ; |156| 
;** --------------------------------------------------------------------------*
$C$L9:    
;          EXCLUSIVE CPU CYCLES: 10
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 157,column 7,is_stmt
           MVKL    .S2     $C$SL29+0,B4
           MVKH    .S2     $C$SL29+0,B4
$C$DW$92	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$92, DW_AT_low_pc(0x00)
	.dwattr $C$DW$92, DW_AT_name("_printf")
	.dwattr $C$DW$92, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |157| 

$C$RL47:   ; CALL OCCURS {_printf} {0}       ; |157| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 158,column 7,is_stmt
           MVK     .L2     1,B4              ; |158| 
           STW     .D2T2   B4,*+SP(16)       ; |158| 
;** --------------------------------------------------------------------------*
$C$L10:    
;          EXCLUSIVE CPU CYCLES: 13
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 160,column 5,is_stmt
           MVKL    .S1     _fcn_pass+12,A3
           MVKH    .S1     _fcn_pass+12,A3
           LDW     .D1T1   *A3,A0            ; |160| 
           NOP             4
   [ A0]   BNOP    .S1     $C$L12,5          ; |160| 
           ; BRANCHCC OCCURS {$C$L12}        ; |160| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 161,column 7,is_stmt
           LDW     .D2T2   *+SP(16),B0       ; |161| 
           NOP             4
   [!B0]   BNOP    .S1     $C$L11,5          ; |161| 
           ; BRANCHCC OCCURS {$C$L11}        ; |161| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 8
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 161,column 18,is_stmt
           MVKL    .S2     $C$SL28+0,B4
           MVKH    .S2     $C$SL28+0,B4
$C$DW$93	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$93, DW_AT_low_pc(0x00)
	.dwattr $C$DW$93, DW_AT_name("_printf")
	.dwattr $C$DW$93, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |161| 

$C$RL48:   ; CALL OCCURS {_printf} {0}       ; |161| 
;** --------------------------------------------------------------------------*
$C$L11:    
;          EXCLUSIVE CPU CYCLES: 10
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 162,column 7,is_stmt
           MVKL    .S2     $C$SL30+0,B4
           MVKH    .S2     $C$SL30+0,B4
$C$DW$94	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$94, DW_AT_low_pc(0x00)
	.dwattr $C$DW$94, DW_AT_name("_printf")
	.dwattr $C$DW$94, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |162| 

$C$RL49:   ; CALL OCCURS {_printf} {0}       ; |162| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 163,column 7,is_stmt
           MVK     .L2     1,B4              ; |163| 
           STW     .D2T2   B4,*+SP(16)       ; |163| 
;** --------------------------------------------------------------------------*
$C$L12:    
;          EXCLUSIVE CPU CYCLES: 13
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 165,column 5,is_stmt
           MVKL    .S1     _fcn_pass+16,A3
           MVKH    .S1     _fcn_pass+16,A3
           LDW     .D1T1   *A3,A0            ; |165| 
           NOP             4
   [ A0]   BNOP    .S1     $C$L14,5          ; |165| 
           ; BRANCHCC OCCURS {$C$L14}        ; |165| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 166,column 7,is_stmt
           LDW     .D2T2   *+SP(16),B0       ; |166| 
           NOP             4
   [!B0]   BNOP    .S1     $C$L13,5          ; |166| 
           ; BRANCHCC OCCURS {$C$L13}        ; |166| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 8
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 166,column 18,is_stmt
           MVKL    .S2     $C$SL28+0,B4
           MVKH    .S2     $C$SL28+0,B4
$C$DW$95	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$95, DW_AT_low_pc(0x00)
	.dwattr $C$DW$95, DW_AT_name("_printf")
	.dwattr $C$DW$95, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |166| 

$C$RL50:   ; CALL OCCURS {_printf} {0}       ; |166| 
;** --------------------------------------------------------------------------*
$C$L13:    
;          EXCLUSIVE CPU CYCLES: 8
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 167,column 7,is_stmt
           MVKL    .S2     $C$SL31+0,B4
           MVKH    .S2     $C$SL31+0,B4
$C$DW$96	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$96, DW_AT_low_pc(0x00)
	.dwattr $C$DW$96, DW_AT_name("_printf")
	.dwattr $C$DW$96, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |167| 

$C$RL51:   ; CALL OCCURS {_printf} {0}       ; |167| 
;** --------------------------------------------------------------------------*
$C$L14:    
;          EXCLUSIVE CPU CYCLES: 8
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 169,column 5,is_stmt
           MVKL    .S2     $C$SL32+0,B4
           MVKH    .S2     $C$SL32+0,B4
$C$DW$97	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$97, DW_AT_low_pc(0x00)
	.dwattr $C$DW$97, DW_AT_name("_printf")
	.dwattr $C$DW$97, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |169| 

$C$RL52:   ; CALL OCCURS {_printf} {0}       ; |169| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c",line 171,column 1,is_stmt
;** --------------------------------------------------------------------------*
$C$L15:    
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *++SP(24),B3      ; |171| 
           NOP             4
	.dwcfi	cfa_offset, 0
	.dwcfi	restore_reg, 19
	.dwcfi	cfa_offset, 0
$C$DW$98	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$98, DW_AT_low_pc(0x00)
	.dwattr $C$DW$98, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |171| 
           ; BRANCH OCCURS {B3}              ; |171| 
	.dwattr $C$DW$72, DW_AT_TI_end_file("C:/nightlybuilds/mathlib/ti/mathlib/src/common/driver.c")
	.dwattr $C$DW$72, DW_AT_TI_end_line(0xab)
	.dwattr $C$DW$72, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$72

;******************************************************************************
;* STRINGS                                                                    *
;******************************************************************************
	.sect	".const:.string"
$C$SL1:	.string	10,10,10,10,0
$C$SL2:	.string	"----------------------------------------",0
$C$SL3:	.string	"----------------------------------------",10,0
$C$SL4:	.string	"Verification Results:  %s",10,0
$C$SL5:	.string	10,0
$C$SL6:	.string	"Cycle Profile:  %s",10,0
$C$SL7:	.string	"RTS:     %d cycles",10,0
$C$SL8:	.string	"ASM:     %d cycles",10,0
$C$SL9:	.string	"C:       %d cycles",10,0
$C$SL10:	.string	"Inline:  %d cycles",10,0
$C$SL11:	.string	"Vector:  %d cycles",10,0
$C$SL12:	.string	"Memory Profile:  %s",10,0
$C$SL13:	.string	"ASM:     %d bytes",10,0
$C$SL14:	.string	"C:       %d bytes",10,0
$C$SL15:	.string	"Vector:  %d bytes",10,0
$C$SL16:	.string	"Pre-defined Data: ",0
$C$SL17:	.string	"Random Data (seed = %d): ",0
$C$SL18:	.string	"Special Case Data: ",0
$C$SL19:	.string	"Extended Range Data: ",0
$C$SL20:	.string	"Undefined test code ",34,"%d",34,10,0
$C$SL21:	.string	" Passed",10,0
$C$SL22:	.string	"w",0
$C$SL23:	.string	"seedfile.txt",0
$C$SL24:	.string	"Error writing seed file ",34,"%s",34,10,0
$C$SL25:	.string	"%d",10,0
$C$SL26:	.string	" Failed (",0
$C$SL27:	.string	"ASM",0
$C$SL28:	.string	",",0
$C$SL29:	.string	"C",0
$C$SL30:	.string	"INL",0
$C$SL31:	.string	"VEC",0
$C$SL32:	.string	")",10,0
;*****************************************************************************
;* UNDEFINED EXTERNAL REFERENCES                                             *
;*****************************************************************************
	.global	_fclose
	.global	_fopen
	.global	_fprintf
	.global	_printf
	.global	_rand
	.global	_srand
	.global	_exit
	.global	_kernel_asm_size
	.global	_kernel_vec_size
	.global	_kernel_ci_size

;******************************************************************************
;* BUILD ATTRIBUTES                                                           *
;******************************************************************************
	.battr "TI", Tag_File, 1, Tag_ABI_stack_align_needed(0)
	.battr "TI", Tag_File, 1, Tag_ABI_stack_align_preserved(0)
	.battr "TI", Tag_File, 1, Tag_Tramps_Use_SOC(1)

;******************************************************************************
;* TYPE INFORMATION                                                           *
;******************************************************************************

$C$DW$T$20	.dwtag  DW_TAG_structure_type
	.dwattr $C$DW$T$20, DW_AT_byte_size(0x18)
$C$DW$99	.dwtag  DW_TAG_member
	.dwattr $C$DW$99, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$99, DW_AT_name("fd")
	.dwattr $C$DW$99, DW_AT_TI_symbol_name("_fd")
	.dwattr $C$DW$99, DW_AT_data_member_location[DW_OP_plus_uconst 0x0]
	.dwattr $C$DW$99, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$100	.dwtag  DW_TAG_member
	.dwattr $C$DW$100, DW_AT_type(*$C$DW$T$19)
	.dwattr $C$DW$100, DW_AT_name("buf")
	.dwattr $C$DW$100, DW_AT_TI_symbol_name("_buf")
	.dwattr $C$DW$100, DW_AT_data_member_location[DW_OP_plus_uconst 0x4]
	.dwattr $C$DW$100, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$101	.dwtag  DW_TAG_member
	.dwattr $C$DW$101, DW_AT_type(*$C$DW$T$19)
	.dwattr $C$DW$101, DW_AT_name("pos")
	.dwattr $C$DW$101, DW_AT_TI_symbol_name("_pos")
	.dwattr $C$DW$101, DW_AT_data_member_location[DW_OP_plus_uconst 0x8]
	.dwattr $C$DW$101, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$102	.dwtag  DW_TAG_member
	.dwattr $C$DW$102, DW_AT_type(*$C$DW$T$19)
	.dwattr $C$DW$102, DW_AT_name("bufend")
	.dwattr $C$DW$102, DW_AT_TI_symbol_name("_bufend")
	.dwattr $C$DW$102, DW_AT_data_member_location[DW_OP_plus_uconst 0xc]
	.dwattr $C$DW$102, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$103	.dwtag  DW_TAG_member
	.dwattr $C$DW$103, DW_AT_type(*$C$DW$T$19)
	.dwattr $C$DW$103, DW_AT_name("buff_stop")
	.dwattr $C$DW$103, DW_AT_TI_symbol_name("_buff_stop")
	.dwattr $C$DW$103, DW_AT_data_member_location[DW_OP_plus_uconst 0x10]
	.dwattr $C$DW$103, DW_AT_accessibility(DW_ACCESS_public)
$C$DW$104	.dwtag  DW_TAG_member
	.dwattr $C$DW$104, DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$104, DW_AT_name("flags")
	.dwattr $C$DW$104, DW_AT_TI_symbol_name("_flags")
	.dwattr $C$DW$104, DW_AT_data_member_location[DW_OP_plus_uconst 0x14]
	.dwattr $C$DW$104, DW_AT_accessibility(DW_ACCESS_public)
	.dwendtag $C$DW$T$20

$C$DW$T$21	.dwtag  DW_TAG_typedef, DW_AT_name("FILE")
	.dwattr $C$DW$T$21, DW_AT_type(*$C$DW$T$20)
	.dwattr $C$DW$T$21, DW_AT_language(DW_LANG_C)
$C$DW$T$22	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$22, DW_AT_type(*$C$DW$T$21)
	.dwattr $C$DW$T$22, DW_AT_address_class(0x20)
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
$C$DW$T$19	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$19, DW_AT_type(*$C$DW$T$6)
	.dwattr $C$DW$T$19, DW_AT_address_class(0x20)
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

$C$DW$T$42	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$42, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$T$42, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$42, DW_AT_byte_size(0x14)
$C$DW$105	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$105, DW_AT_upper_bound(0x04)
	.dwendtag $C$DW$T$42

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

$C$DW$T$44	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$44, DW_AT_type(*$C$DW$T$14)
	.dwattr $C$DW$T$44, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$44, DW_AT_byte_size(0x28)
$C$DW$106	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$106, DW_AT_upper_bound(0x04)
	.dwendtag $C$DW$T$44

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
$C$DW$T$18	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$18, DW_AT_encoding(DW_ATE_float)
	.dwattr $C$DW$T$18, DW_AT_name("long double")
	.dwattr $C$DW$T$18, DW_AT_byte_size(0x08)
$C$DW$T$23	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$23, DW_AT_encoding(DW_ATE_signed_char)
	.dwattr $C$DW$T$23, DW_AT_name("signed char")
	.dwattr $C$DW$T$23, DW_AT_byte_size(0x01)
$C$DW$T$24	.dwtag  DW_TAG_const_type
	.dwattr $C$DW$T$24, DW_AT_type(*$C$DW$T$23)
$C$DW$T$25	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$25, DW_AT_type(*$C$DW$T$24)
	.dwattr $C$DW$T$25, DW_AT_address_class(0x20)
$C$DW$T$32	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$32, DW_AT_type(*$C$DW$T$23)
	.dwattr $C$DW$T$32, DW_AT_address_class(0x20)
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

$C$DW$107	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A0")
	.dwattr $C$DW$107, DW_AT_location[DW_OP_reg0]
$C$DW$108	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A1")
	.dwattr $C$DW$108, DW_AT_location[DW_OP_reg1]
$C$DW$109	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A2")
	.dwattr $C$DW$109, DW_AT_location[DW_OP_reg2]
$C$DW$110	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A3")
	.dwattr $C$DW$110, DW_AT_location[DW_OP_reg3]
$C$DW$111	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A4")
	.dwattr $C$DW$111, DW_AT_location[DW_OP_reg4]
$C$DW$112	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A5")
	.dwattr $C$DW$112, DW_AT_location[DW_OP_reg5]
$C$DW$113	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A6")
	.dwattr $C$DW$113, DW_AT_location[DW_OP_reg6]
$C$DW$114	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A7")
	.dwattr $C$DW$114, DW_AT_location[DW_OP_reg7]
$C$DW$115	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A8")
	.dwattr $C$DW$115, DW_AT_location[DW_OP_reg8]
$C$DW$116	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A9")
	.dwattr $C$DW$116, DW_AT_location[DW_OP_reg9]
$C$DW$117	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A10")
	.dwattr $C$DW$117, DW_AT_location[DW_OP_reg10]
$C$DW$118	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A11")
	.dwattr $C$DW$118, DW_AT_location[DW_OP_reg11]
$C$DW$119	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A12")
	.dwattr $C$DW$119, DW_AT_location[DW_OP_reg12]
$C$DW$120	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A13")
	.dwattr $C$DW$120, DW_AT_location[DW_OP_reg13]
$C$DW$121	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A14")
	.dwattr $C$DW$121, DW_AT_location[DW_OP_reg14]
$C$DW$122	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A15")
	.dwattr $C$DW$122, DW_AT_location[DW_OP_reg15]
$C$DW$123	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B0")
	.dwattr $C$DW$123, DW_AT_location[DW_OP_reg16]
$C$DW$124	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B1")
	.dwattr $C$DW$124, DW_AT_location[DW_OP_reg17]
$C$DW$125	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B2")
	.dwattr $C$DW$125, DW_AT_location[DW_OP_reg18]
$C$DW$126	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B3")
	.dwattr $C$DW$126, DW_AT_location[DW_OP_reg19]
$C$DW$127	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B4")
	.dwattr $C$DW$127, DW_AT_location[DW_OP_reg20]
$C$DW$128	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B5")
	.dwattr $C$DW$128, DW_AT_location[DW_OP_reg21]
$C$DW$129	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B6")
	.dwattr $C$DW$129, DW_AT_location[DW_OP_reg22]
$C$DW$130	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B7")
	.dwattr $C$DW$130, DW_AT_location[DW_OP_reg23]
$C$DW$131	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B8")
	.dwattr $C$DW$131, DW_AT_location[DW_OP_reg24]
$C$DW$132	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B9")
	.dwattr $C$DW$132, DW_AT_location[DW_OP_reg25]
$C$DW$133	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B10")
	.dwattr $C$DW$133, DW_AT_location[DW_OP_reg26]
$C$DW$134	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B11")
	.dwattr $C$DW$134, DW_AT_location[DW_OP_reg27]
$C$DW$135	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B12")
	.dwattr $C$DW$135, DW_AT_location[DW_OP_reg28]
$C$DW$136	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B13")
	.dwattr $C$DW$136, DW_AT_location[DW_OP_reg29]
$C$DW$137	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DP")
	.dwattr $C$DW$137, DW_AT_location[DW_OP_reg30]
$C$DW$138	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SP")
	.dwattr $C$DW$138, DW_AT_location[DW_OP_reg31]
$C$DW$139	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FP")
	.dwattr $C$DW$139, DW_AT_location[DW_OP_regx 0x20]
$C$DW$140	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("PC")
	.dwattr $C$DW$140, DW_AT_location[DW_OP_regx 0x21]
$C$DW$141	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IRP")
	.dwattr $C$DW$141, DW_AT_location[DW_OP_regx 0x22]
$C$DW$142	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IFR")
	.dwattr $C$DW$142, DW_AT_location[DW_OP_regx 0x23]
$C$DW$143	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("NRP")
	.dwattr $C$DW$143, DW_AT_location[DW_OP_regx 0x24]
$C$DW$144	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A16")
	.dwattr $C$DW$144, DW_AT_location[DW_OP_regx 0x25]
$C$DW$145	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A17")
	.dwattr $C$DW$145, DW_AT_location[DW_OP_regx 0x26]
$C$DW$146	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A18")
	.dwattr $C$DW$146, DW_AT_location[DW_OP_regx 0x27]
$C$DW$147	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A19")
	.dwattr $C$DW$147, DW_AT_location[DW_OP_regx 0x28]
$C$DW$148	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A20")
	.dwattr $C$DW$148, DW_AT_location[DW_OP_regx 0x29]
$C$DW$149	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A21")
	.dwattr $C$DW$149, DW_AT_location[DW_OP_regx 0x2a]
$C$DW$150	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A22")
	.dwattr $C$DW$150, DW_AT_location[DW_OP_regx 0x2b]
$C$DW$151	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A23")
	.dwattr $C$DW$151, DW_AT_location[DW_OP_regx 0x2c]
$C$DW$152	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A24")
	.dwattr $C$DW$152, DW_AT_location[DW_OP_regx 0x2d]
$C$DW$153	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A25")
	.dwattr $C$DW$153, DW_AT_location[DW_OP_regx 0x2e]
$C$DW$154	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A26")
	.dwattr $C$DW$154, DW_AT_location[DW_OP_regx 0x2f]
$C$DW$155	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A27")
	.dwattr $C$DW$155, DW_AT_location[DW_OP_regx 0x30]
$C$DW$156	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A28")
	.dwattr $C$DW$156, DW_AT_location[DW_OP_regx 0x31]
$C$DW$157	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A29")
	.dwattr $C$DW$157, DW_AT_location[DW_OP_regx 0x32]
$C$DW$158	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A30")
	.dwattr $C$DW$158, DW_AT_location[DW_OP_regx 0x33]
$C$DW$159	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A31")
	.dwattr $C$DW$159, DW_AT_location[DW_OP_regx 0x34]
$C$DW$160	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B16")
	.dwattr $C$DW$160, DW_AT_location[DW_OP_regx 0x35]
$C$DW$161	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B17")
	.dwattr $C$DW$161, DW_AT_location[DW_OP_regx 0x36]
$C$DW$162	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B18")
	.dwattr $C$DW$162, DW_AT_location[DW_OP_regx 0x37]
$C$DW$163	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B19")
	.dwattr $C$DW$163, DW_AT_location[DW_OP_regx 0x38]
$C$DW$164	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B20")
	.dwattr $C$DW$164, DW_AT_location[DW_OP_regx 0x39]
$C$DW$165	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B21")
	.dwattr $C$DW$165, DW_AT_location[DW_OP_regx 0x3a]
$C$DW$166	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B22")
	.dwattr $C$DW$166, DW_AT_location[DW_OP_regx 0x3b]
$C$DW$167	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B23")
	.dwattr $C$DW$167, DW_AT_location[DW_OP_regx 0x3c]
$C$DW$168	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B24")
	.dwattr $C$DW$168, DW_AT_location[DW_OP_regx 0x3d]
$C$DW$169	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B25")
	.dwattr $C$DW$169, DW_AT_location[DW_OP_regx 0x3e]
$C$DW$170	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B26")
	.dwattr $C$DW$170, DW_AT_location[DW_OP_regx 0x3f]
$C$DW$171	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B27")
	.dwattr $C$DW$171, DW_AT_location[DW_OP_regx 0x40]
$C$DW$172	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B28")
	.dwattr $C$DW$172, DW_AT_location[DW_OP_regx 0x41]
$C$DW$173	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B29")
	.dwattr $C$DW$173, DW_AT_location[DW_OP_regx 0x42]
$C$DW$174	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B30")
	.dwattr $C$DW$174, DW_AT_location[DW_OP_regx 0x43]
$C$DW$175	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B31")
	.dwattr $C$DW$175, DW_AT_location[DW_OP_regx 0x44]
$C$DW$176	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("AMR")
	.dwattr $C$DW$176, DW_AT_location[DW_OP_regx 0x45]
$C$DW$177	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CSR")
	.dwattr $C$DW$177, DW_AT_location[DW_OP_regx 0x46]
$C$DW$178	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ISR")
	.dwattr $C$DW$178, DW_AT_location[DW_OP_regx 0x47]
$C$DW$179	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ICR")
	.dwattr $C$DW$179, DW_AT_location[DW_OP_regx 0x48]
$C$DW$180	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IER")
	.dwattr $C$DW$180, DW_AT_location[DW_OP_regx 0x49]
$C$DW$181	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ISTP")
	.dwattr $C$DW$181, DW_AT_location[DW_OP_regx 0x4a]
$C$DW$182	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IN")
	.dwattr $C$DW$182, DW_AT_location[DW_OP_regx 0x4b]
$C$DW$183	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("OUT")
	.dwattr $C$DW$183, DW_AT_location[DW_OP_regx 0x4c]
$C$DW$184	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ACR")
	.dwattr $C$DW$184, DW_AT_location[DW_OP_regx 0x4d]
$C$DW$185	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ADR")
	.dwattr $C$DW$185, DW_AT_location[DW_OP_regx 0x4e]
$C$DW$186	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FADCR")
	.dwattr $C$DW$186, DW_AT_location[DW_OP_regx 0x4f]
$C$DW$187	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FAUCR")
	.dwattr $C$DW$187, DW_AT_location[DW_OP_regx 0x50]
$C$DW$188	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FMCR")
	.dwattr $C$DW$188, DW_AT_location[DW_OP_regx 0x51]
$C$DW$189	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GFPGFR")
	.dwattr $C$DW$189, DW_AT_location[DW_OP_regx 0x52]
$C$DW$190	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DIER")
	.dwattr $C$DW$190, DW_AT_location[DW_OP_regx 0x53]
$C$DW$191	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("REP")
	.dwattr $C$DW$191, DW_AT_location[DW_OP_regx 0x54]
$C$DW$192	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSCL")
	.dwattr $C$DW$192, DW_AT_location[DW_OP_regx 0x55]
$C$DW$193	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSCH")
	.dwattr $C$DW$193, DW_AT_location[DW_OP_regx 0x56]
$C$DW$194	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ARP")
	.dwattr $C$DW$194, DW_AT_location[DW_OP_regx 0x57]
$C$DW$195	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ILC")
	.dwattr $C$DW$195, DW_AT_location[DW_OP_regx 0x58]
$C$DW$196	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RILC")
	.dwattr $C$DW$196, DW_AT_location[DW_OP_regx 0x59]
$C$DW$197	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DNUM")
	.dwattr $C$DW$197, DW_AT_location[DW_OP_regx 0x5a]
$C$DW$198	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SSR")
	.dwattr $C$DW$198, DW_AT_location[DW_OP_regx 0x5b]
$C$DW$199	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GPLYA")
	.dwattr $C$DW$199, DW_AT_location[DW_OP_regx 0x5c]
$C$DW$200	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GPLYB")
	.dwattr $C$DW$200, DW_AT_location[DW_OP_regx 0x5d]
$C$DW$201	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSR")
	.dwattr $C$DW$201, DW_AT_location[DW_OP_regx 0x5e]
$C$DW$202	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ITSR")
	.dwattr $C$DW$202, DW_AT_location[DW_OP_regx 0x5f]
$C$DW$203	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("NTSR")
	.dwattr $C$DW$203, DW_AT_location[DW_OP_regx 0x60]
$C$DW$204	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("EFR")
	.dwattr $C$DW$204, DW_AT_location[DW_OP_regx 0x61]
$C$DW$205	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ECR")
	.dwattr $C$DW$205, DW_AT_location[DW_OP_regx 0x62]
$C$DW$206	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IERR")
	.dwattr $C$DW$206, DW_AT_location[DW_OP_regx 0x63]
$C$DW$207	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DMSG")
	.dwattr $C$DW$207, DW_AT_location[DW_OP_regx 0x64]
$C$DW$208	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CMSG")
	.dwattr $C$DW$208, DW_AT_location[DW_OP_regx 0x65]
$C$DW$209	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_ADDR")
	.dwattr $C$DW$209, DW_AT_location[DW_OP_regx 0x66]
$C$DW$210	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_DATA")
	.dwattr $C$DW$210, DW_AT_location[DW_OP_regx 0x67]
$C$DW$211	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_CNTL")
	.dwattr $C$DW$211, DW_AT_location[DW_OP_regx 0x68]
$C$DW$212	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TCU_CNTL")
	.dwattr $C$DW$212, DW_AT_location[DW_OP_regx 0x69]
$C$DW$213	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_REC_CNTL")
	.dwattr $C$DW$213, DW_AT_location[DW_OP_regx 0x6a]
$C$DW$214	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_XMT_CNTL")
	.dwattr $C$DW$214, DW_AT_location[DW_OP_regx 0x6b]
$C$DW$215	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_CFG")
	.dwattr $C$DW$215, DW_AT_location[DW_OP_regx 0x6c]
$C$DW$216	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_RDATA")
	.dwattr $C$DW$216, DW_AT_location[DW_OP_regx 0x6d]
$C$DW$217	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_WDATA")
	.dwattr $C$DW$217, DW_AT_location[DW_OP_regx 0x6e]
$C$DW$218	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_RADDR")
	.dwattr $C$DW$218, DW_AT_location[DW_OP_regx 0x6f]
$C$DW$219	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_WADDR")
	.dwattr $C$DW$219, DW_AT_location[DW_OP_regx 0x70]
$C$DW$220	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("MFREG0")
	.dwattr $C$DW$220, DW_AT_location[DW_OP_regx 0x71]
$C$DW$221	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DBG_STAT")
	.dwattr $C$DW$221, DW_AT_location[DW_OP_regx 0x72]
$C$DW$222	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("BRK_EN")
	.dwattr $C$DW$222, DW_AT_location[DW_OP_regx 0x73]
$C$DW$223	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP0_CNT")
	.dwattr $C$DW$223, DW_AT_location[DW_OP_regx 0x74]
$C$DW$224	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP0")
	.dwattr $C$DW$224, DW_AT_location[DW_OP_regx 0x75]
$C$DW$225	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP1")
	.dwattr $C$DW$225, DW_AT_location[DW_OP_regx 0x76]
$C$DW$226	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP2")
	.dwattr $C$DW$226, DW_AT_location[DW_OP_regx 0x77]
$C$DW$227	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP3")
	.dwattr $C$DW$227, DW_AT_location[DW_OP_regx 0x78]
$C$DW$228	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("OVERLAY")
	.dwattr $C$DW$228, DW_AT_location[DW_OP_regx 0x79]
$C$DW$229	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("PC_PROF")
	.dwattr $C$DW$229, DW_AT_location[DW_OP_regx 0x7a]
$C$DW$230	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ATSR")
	.dwattr $C$DW$230, DW_AT_location[DW_OP_regx 0x7b]
$C$DW$231	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TRR")
	.dwattr $C$DW$231, DW_AT_location[DW_OP_regx 0x7c]
$C$DW$232	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TCRR")
	.dwattr $C$DW$232, DW_AT_location[DW_OP_regx 0x7d]
$C$DW$233	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DESR")
	.dwattr $C$DW$233, DW_AT_location[DW_OP_regx 0x7e]
$C$DW$234	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DETR")
	.dwattr $C$DW$234, DW_AT_location[DW_OP_regx 0x7f]
$C$DW$235	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CIE_RETA")
	.dwattr $C$DW$235, DW_AT_location[DW_OP_regx 0xe4]
	.dwendtag $C$DW$CU

