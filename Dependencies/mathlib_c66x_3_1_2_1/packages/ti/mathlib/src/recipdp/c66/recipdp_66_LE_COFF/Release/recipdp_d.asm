;******************************************************************************
;* TMS320C6x C/C++ Codegen                                          PC v7.4.2 *
;* Date/Time created: Tue May 17 03:17:01 2016                                *
;******************************************************************************
	.compiler_opts --abi=coffabi --c64p_l1d_workaround=off --endian=little --hll_source=on --long_precision_bits=40 --mem_model:code=near --mem_model:const=data --mem_model:data=far --object_format=coff --silicon_version=6600 --symdebug:none 

;******************************************************************************
;* GLOBAL FILE PARAMETERS                                                     *
;*                                                                            *
;*   Architecture      : TMS320C66xx                                          *
;*   Optimization      : Enabled at level 3                                   *
;*   Optimizing for    : Speed                                                *
;*                       Based on options: -o3, no -ms                        *
;*   Endian            : Little                                               *
;*   Interrupt Thrshld : Disabled                                             *
;*   Data Access Model : Far                                                  *
;*   Pipelining        : Enabled                                              *
;*   Speculate Loads   : Enabled with threshold = 0                           *
;*   Memory Aliases    : Presume are aliases (pessimistic)                    *
;*   Debug Info        : No Debug Info                                        *
;*                                                                            *
;******************************************************************************

	.asg	A15, FP
	.asg	B14, DP
	.asg	B15, SP
	.global	$bss

;*****************************************************************************
;* CINIT RECORDS                                                             *
;*****************************************************************************
	.sect	".cinit"
	.align	8
	.field  	$C$IR_1,32
	.field  	_a+0,32
	.word	01585a343h,0c04f87b8h		; _a[0] @ 0
	.word	08467d92dh,0c055e6edh		; _a[1] @ 64
	.word	02fb27533h,0c04cf934h		; _a[2] @ 128
	.word	0463065f9h,0404ac024h		; _a[3] @ 192
	.word	03bfd0650h,0c0535d11h		; _a[4] @ 256
	.word	0ec41dd1ah,04036714ch		; _a[5] @ 320
	.word	044127e88h,0404bee82h		; _a[6] @ 384
	.word	0d9d3458dh,0404b5cc9h		; _a[7] @ 448
	.word	08867422eh,0c0456019h		; _a[8] @ 512
	.word	0cb31c21ah,0404f249eh		; _a[9] @ 576
$C$IR_1:	.set	80

	.global	_a
_a:	.usect	".far",80,8
;	C:\MATHLIB_Tools\CCSV5_5_0\ccsv5\tools\compiler\c6000_7.4.2\bin\opt6x.exe C:\\Users\\gtbldadm\\AppData\\Local\\Temp\\029767 C:\\Users\\gtbldadm\\AppData\\Local\\Temp\\029769 
	.sect	".text"
	.clink
	.global	_main

;******************************************************************************
;* FUNCTION NAME: main                                                        *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,   *
;*                           A15,B0,B1,B2,B3,B4,B5,B6,B7,B8,B9,B10,B11,B12,   *
;*                           B13,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24,A25,  *
;*                           A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20,B21, *
;*                           B22,B23,B24,B25,B26,B27,B28,B29,B30,B31          *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,   *
;*                           A15,B0,B1,B2,B3,B4,B5,B6,B7,B8,B9,B10,B11,B12,   *
;*                           B13,DP,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24,   *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Local Frame Size  : 8 Args + 60 Auto + 56 Save = 124 byte                *
;******************************************************************************
_main:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           STW     .D2T1   A11,*SP--(8)      ; |93| 
           STW     .D2T1   A10,*SP--(8)      ; |93| 
           STDW    .D2T2   B13:B12,*SP--     ; |93| 
           STDW    .D2T2   B11:B10,*SP--     ; |93| 
           STDW    .D2T1   A15:A14,*SP--     ; |93| 

           STDW    .D2T1   A13:A12,*SP--     ; |93| 
||         MVKL    .S1     $C$SL1+0,A4

           STW     .D2T2   B3,*SP--(80)      ; |93| 
||         CALLP   .S2     _driver_init,B3
||         MVKH    .S1     $C$SL1+0,A4

$C$RL0:    ; CALL OCCURS {_driver_init} {0}  ; |101| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 8

           MVKL    .S1     _a,A12
||         MVKL    .S2     _output,B4

           MVKH    .S1     _a,A12
||         MVKH    .S2     _output,B4

           LDW     .D2T1   *B4,A11
||         MVK     .L2     0xa,B4

           MV      .L2X    A12,B10
||         CALL    .S1     __divd            ; |110| 

           MV      .L1X    B4,A10
||         LDDW    .D2T2   *B10++,B5:B4      ; |110| 

           ZERO    .L1     A5
           SET     .S1     A5,0x14,0x1d,A5
           ZERO    .L1     A4                ; |110| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L1:    
;          EXCLUSIVE CPU CYCLES: 1
           ADDKPC  .S2     $C$RL1,B3,0       ; |110| 
$C$RL1:    ; CALL OCCURS {__divd} {0}        ; |110| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7

           SUB     .L1     A10,1,A0          ; |109| 
||         STDW    .D1T1   A5:A4,*A11++      ; |110| 
||         SUB     .S1     A10,1,A10         ; |109| 

   [ A0]   B       .S1     $C$L1             ; |109| 
|| [!A0]   MVKL    .S2     _output,B4
|| [ A0]   LDDW    .D2T2   *B10++,B5:B4      ; |110| 
|| [ A0]   ZERO    .L1     A5
|| [ A0]   ZERO    .D1     A4                ; |110| 
|| [!A0]   ZERO    .L2     B10

   [!A0]   MVK     .L1     0xa,A10
|| [!A0]   MV      .S1     A12,A11
|| [ A0]   CALL    .S2     __divd            ; |110| 

   [ A0]   SET     .S1     A5,0x14,0x1d,A5
   [!A0]   MVKH    .S2     _output,B4
   [!A0]   ADD     .L2     4,B4,B4
   [!A0]   STW     .D2T2   B4,*+SP(16)
           ; BRANCHCC OCCURS {$C$L1}         ; |109| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1
           CALL    .S1     _recipdp          ; |113| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L2:    
;          EXCLUSIVE CPU CYCLES: 5
           LDDW    .D1T1   *A11++,A5:A4      ; |113| 
           ADDKPC  .S2     $C$RL2,B3,3       ; |113| 
$C$RL2:    ; CALL OCCURS {_recipdp} {0}      ; |113| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           LDW     .D2T2   *+SP(16),B4       ; |113| 
           SUB     .L1     A10,1,A0          ; |112| 
           SUB     .L1     A10,1,A10         ; |112| 
           NOP             2
           LDW     .D2T2   *B4,B4            ; |113| 
   [ A0]   BNOP    .S1     $C$L2,3           ; |112| 

           ADD     .L2     B10,B4,B4         ; |113| 
||         ADD     .S2     8,B10,B10         ; |112| 

   [ A0]   CALL    .S1     _recipdp          ; |113| 
||         STDW    .D2T1   A5:A4,*B4         ; |113| 

           ; BRANCHCC OCCURS {$C$L2}         ; |112| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7
           LDW     .D2T2   *+SP(16),B4
           MV      .L1     A12,A10
           ZERO    .L2     B11
           MVK     .L1     0xa,A11
           NOP             1
           ADD     .L2     4,B4,B4

           STW     .D2T2   B4,*+SP(20)
||         CALL    .S1     _recipdp_c        ; |116| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L3:    
;          EXCLUSIVE CPU CYCLES: 5
           LDDW    .D1T1   *A10++,A5:A4      ; |116| 
           ADDKPC  .S2     $C$RL3,B3,3       ; |116| 
$C$RL3:    ; CALL OCCURS {_recipdp_c} {0}    ; |116| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           LDW     .D2T2   *+SP(20),B4       ; |116| 
           SUB     .L1     A11,1,A0          ; |115| 
           SUB     .L1     A11,1,A11         ; |115| 
   [!A0]   MVKL    .S2     0x7fefffff,B12
   [!A0]   MVKL    .S2     _output,B13
           LDW     .D2T2   *B4,B4            ; |116| 
   [ A0]   BNOP    .S1     $C$L3,3           ; |115| 

           ADD     .L2     B11,B4,B4         ; |116| 
||         ADD     .S2     8,B11,B11         ; |115| 

   [ A0]   CALL    .S1     _recipdp_c        ; |116| 
||         STDW    .D2T1   A5:A4,*B4         ; |116| 
|| [!A0]   ZERO    .L2     B4

           ; BRANCHCC OCCURS {$C$L3}         ; |115| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5

           MVKH    .S2     _output,B13
||         MVK     .L1     9,A0
||         ZERO    .L2     B8                ; |63| 
||         MVK     .D2     0xffffffff,B16    ; |63| 

           MVKH    .S2     0x7fefffff,B12
||         LDW     .D2T2   *+B13(12),B23
||         MV      .L2X    A12,B22

           SET     .S2     B4,0x1e,0x1e,B4
||         MV      .L2     B12,B17
||         ZERO    .D2     B18               ; |63| 

           MV      .L2     B4,B21
||         MV      .S2     B4,B19            ; |63| 
||         MV      .D2     B4,B9             ; |63| 

           DINT                              ; interrupts off
||         ZERO    .L2     B20               ; |63| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*
;*      Loop found in file               : C:/nightlybuilds/mathlib/ti/mathlib/src/recipdp/recipdp_d.c
;*      Loop source line                 : 118
;*      Loop opening brace source line   : 118
;*      Loop closing brace source line   : 120
;*      Known Minimum Trip Count         : 10                    
;*      Known Maximum Trip Count         : 10                    
;*      Known Max Trip Count Factor      : 10
;*      Loop Carried Dependency Bound(^) : 41
;*      Unpartitioned Resource Bound     : 4
;*      Partitioned Resource Bound(*)    : 6
;*      Resource Partition:
;*                                A-side   B-side
;*      .L units                     0        1     
;*      .S units                     1        5     
;*      .D units                     0        2     
;*      .M units                     0        6*    
;*      .X cross paths               0        0     
;*      .T address paths             0        2     
;*      Long read paths              0        0     
;*      Long write paths             0        0     
;*      Logical  ops (.LS)           0        3     (.L or .S unit)
;*      Addition ops (.LSD)          0        0     (.L or .S or .D unit)
;*      Bound(.L .S .LS)             1        5     
;*      Bound(.L .S .D .LS .LSD)     1        4     
;*
;*      Searching for software pipeline schedule at ...
;*         ii = 41 Schedule found with 1 iterations in parallel
;*
;*      Register Usage Table:
;*          +-----------------------------------------------------------------+
;*          |AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA|BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB|
;*          |00000000001111111111222222222233|00000000001111111111222222222233|
;*          |01234567890123456789012345678901|01234567890123456789012345678901|
;*          |--------------------------------+--------------------------------|
;*       0: |*                               |        **      ********        |
;*       1: |*                               |        **      ********        |
;*       2: |*                               |        **      ********        |
;*       3: |*                               |        **      ********        |
;*       4: |*                               |        **      ********        |
;*       5: |*                               |        **      **********      |
;*       6: |*                               |        **      **********      |
;*       7: |*                               |      ****      **********      |
;*       8: |*                               |      ****      **********      |
;*       9: |*                               |    ******      **********      |
;*      10: |*                               |    ******      **********      |
;*      11: |*                               |*   ******      **********      |
;*      12: |*                               |*     ****      **********      |
;*      13: |*                               |*     ****      **********      |
;*      14: |*                               |*   ******      **********      |
;*      15: |*                               |*       **      **********      |
;*      16: |*                               |*       **      **********      |
;*      17: |*                               |*       **      **********      |
;*      18: |*                               |*     ****      **********      |
;*      19: |*                               |*     ****      **********      |
;*      20: |*                               |*     ****      **********      |
;*      21: |*                               |*     ****      **********      |
;*      22: |*                               |*   ******      **********      |
;*      23: |*                               |*     ****      **********      |
;*      24: |*                               |*     ****      **********      |
;*      25: |*                               |*   ******      **********      |
;*      26: |*                               |*       **      **********      |
;*      27: |*                               |*       **      **********      |
;*      28: |*                               |*       **      **********      |
;*      29: |*                               |*     ****      **********      |
;*      30: |*                               |*     ****      ********        |
;*      31: |*                               |*     ****      ********        |
;*      32: |*                               |*     ****      ********        |
;*      33: |*                               |*   ******      ********        |
;*      34: |*                               |*     ****      ********        |
;*      35: |*                               |*     ****      ********        |
;*      36: |*                               |*   ******      ********        |
;*      37: |*                               |*       **      ********        |
;*      38: |*                               |*       **      ********        |
;*      39: |*                               |*       **      ********        |
;*      40: |*                               |    **  **      ********        |
;*          +-----------------------------------------------------------------+
;*
;*      Done
;*
;*      Collapsed epilog stages       : 0
;*      Collapsed prolog stages       : 0
;*
;*      Minimum safe trip count       : 1
;*
;*
;*      Mem bank conflicts/iter(est.) : { min 0.000, est 0.000, max 0.000 }
;*      Mem bank perf. penalty (est.) : 0.0%
;*
;*
;*      Total cycles (est.)         : 0 + min_trip_cnt * 41 = 410        
;*----------------------------------------------------------------------------*
;*       SETUP CODE
;*
;*                  SUB             A0,1,A0
;*
;*        SINGLE SCHEDULED ITERATION
;*
;*        $C$C81:
;*   0              LDDW    .D2T2   *B22++,B25:B24    ; |63|  ^ 
;*   1              NOP             4
;*   5              RCPDP   .S2     B25:B24,B7:B6     ; |63|  ^ 
;*   6              NOP             1
;*   7              FMPYDP  .M2     B7:B6,B25:B24,B5:B4 ; |63|  ^ 
;*     ||           ABSDP   .S2     B25:B24,B5:B4     ; |63| 
;*   8              NOP             1
;*   9              CMPGTDP .S2     B5:B4,B17:B16,B0  ; |63| 
;*  10              NOP             1
;*  11              FSUBDP  .L2     B21:B20,B5:B4,B5:B4 ; |63|  ^ 
;*  12              NOP             2
;*  14              FMPYDP  .M2     B7:B6,B5:B4,B7:B6 ; |63|  ^ 
;*  15              NOP             3
;*  18              FMPYDP  .M2     B7:B6,B25:B24,B5:B4 ; |63|  ^ 
;*  19              NOP             3
;*  22              FSUBDP  .L2     B19:B18,B5:B4,B5:B4 ; |63|  ^ 
;*  23              NOP             2
;*  25              FMPYDP  .M2     B7:B6,B5:B4,B7:B6 ; |63|  ^ 
;*  26              NOP             3
;*  29              FMPYDP  .M2     B7:B6,B25:B24,B5:B4 ; |63|  ^ 
;*  30              NOP             3
;*  33              FSUBDP  .L2     B9:B8,B5:B4,B5:B4 ; |63|  ^ 
;*  34              NOP             1
;*  35      [ A0]   BDEC    .S1     $C$C81,A0         ; |118| 
;*  36      [!B0]   FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |63|  ^ 
;*  37              NOP             2
;*  39      [ B0]   ZERO    .L2     B5:B4             ; |63| 
;*  40              STDW    .D2T2   B5:B4,*B23++      ; |63|  ^ 
;*  41              ; BRANCHCC OCCURS {$C$C81}        ; |118| 
;*----------------------------------------------------------------------------*
$C$L4:    ; PIPED LOOP PROLOG
;** --------------------------------------------------------------------------*
$C$L5:    ; PIPED LOOP KERNEL
;          EXCLUSIVE CPU CYCLES: 41
           LDDW    .D2T2   *B22++,B25:B24    ; |63| <0,0>  ^ 
           NOP             4
           RCPDP   .S2     B25:B24,B7:B6     ; |63| <0,5>  ^ 
           NOP             1

           ABSDP   .S2     B25:B24,B5:B4     ; |63| <0,7> 
||         FMPYDP  .M2     B7:B6,B25:B24,B5:B4 ; |63| <0,7>  ^ 

           NOP             1
           CMPGTDP .S2     B5:B4,B17:B16,B0  ; |63| <0,9> 
           NOP             1
           FSUBDP  .L2     B21:B20,B5:B4,B5:B4 ; |63| <0,11>  ^ 
           NOP             2
           FMPYDP  .M2     B7:B6,B5:B4,B7:B6 ; |63| <0,14>  ^ 
           NOP             3
           FMPYDP  .M2     B7:B6,B25:B24,B5:B4 ; |63| <0,18>  ^ 
           NOP             3
           FSUBDP  .L2     B19:B18,B5:B4,B5:B4 ; |63| <0,22>  ^ 
           NOP             2
           FMPYDP  .M2     B7:B6,B5:B4,B7:B6 ; |63| <0,25>  ^ 
           NOP             3
           FMPYDP  .M2     B7:B6,B25:B24,B5:B4 ; |63| <0,29>  ^ 
           NOP             3
           FSUBDP  .L2     B9:B8,B5:B4,B5:B4 ; |63| <0,33>  ^ 
           NOP             1
   [ A0]   BDEC    .S1     $C$L5,A0          ; |118| <0,35> 
   [!B0]   FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |63| <0,36>  ^ 
           NOP             2
   [ B0]   ZERO    .L2     B5:B4             ; |63| <0,39> 
           STDW    .D2T2   B5:B4,*B23++      ; |63| <0,40>  ^ 
;** --------------------------------------------------------------------------*
$C$L6:    ; PIPED LOOP EPILOG
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7

           MV      .L1X    B13,A10
||         RINT                              ; interrupts on
||         MV      .S1     A12,A4            ; |121| 

           CALLP   .S2     _recipdp_v,B3
||         LDW     .D1T2   *+A10(16),B4      ; |121| 
||         MVK     .L1     0xa,A6            ; |121| 
||         MV      .S1X    B9,A15
||         MV      .L2     B17,B12
||         MV      .D2     B21,B11

$C$RL4:    ; CALL OCCURS {_recipdp_v} {0}    ; |121| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 48
           MVKL    .S2     0x3ddb7cdf,B7
           MVKL    .S2     0xd9d7bdbb,B6
           MVKH    .S2     0x3ddb7cdf,B7

           LDW     .D1T2   *+A10(4),B4       ; |66| 
||         MVKH    .S2     0xd9d7bdbb,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A10,A4           ; |66| 
||         MVK     .L1     0xa,A6            ; |66| 
||         MVK     .S1     0x1,A8            ; |66| 

$C$RL5:    ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     _fcn_pass,B4
           MVKL    .S2     0xd9d7bdbb,B6
           MVKL    .S2     0x3ddb7cdf,B7
           MVKH    .S2     _fcn_pass,B4
           MVKH    .S2     0xd9d7bdbb,B6

           MV      .L1     A4,A3             ; |66| 
||         LDW     .D1T1   *A10,A4           ; |67| 
||         MVKH    .S2     0x3ddb7cdf,B7

           CALLP   .S2     _isequal,B3
||         STW     .D2T1   A3,*+B4(4)        ; |66| 
||         LDW     .D1T2   *+A10(8),B4       ; |67| 
||         MVK     .L1     0xa,A6            ; |67| 
||         MVK     .S1     0x1,A8            ; |67| 

$C$RL6:    ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     _fcn_pass,B4
           MVKL    .S2     0xd9d7bdbb,B6
           MVKL    .S2     0x3ddb7cdf,B7
           MVKH    .S2     _fcn_pass,B4
           MVKH    .S2     0xd9d7bdbb,B6

           MV      .L1X    B13,A3
||         STW     .D2T1   A4,*+B4(8)        ; |67| 
||         LDW     .D1T2   *+A10(12),B4      ; |68| 
||         MVKH    .S2     0x3ddb7cdf,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A3,A4            ; |68| 
||         MVK     .L1     0xa,A6            ; |68| 
||         MVK     .S1     0x1,A8            ; |68| 

$C$RL7:    ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     0xd9d7bdbb,B6

           LDW     .D2T2   *+SP(20),B5
||         MVKL    .S2     _fcn_pass,B31

           MVKL    .S2     0x3ddb7cdf,B7
           MVKH    .S2     _fcn_pass,B31
           MVKH    .S2     0xd9d7bdbb,B6

           STW     .D2T1   A4,*+B31(12)      ; |68| 
||         MVKH    .S2     0x3ddb7cdf,B7

           ADDAD   .D2     B5,1,B4
||         MVKL    .S2     _output,B5

           MVKH    .S2     _output,B5
||         LDW     .D2T2   *B4,B4            ; |69| 

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B5,A4            ; |69| 
||         MVK     .L1     0xa,A6            ; |69| 
||         MVK     .S1     0x1,A8            ; |69| 

$C$RL8:    ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 14

           MVKL    .S1     _fcn_pass,A3
||         MVKL    .S2     _fcn_pass,B5

           MVKH    .S1     _fcn_pass,A3
           ADD     .L1     4,A3,A3
           LDW     .D1T1   *A3,A0            ; |72| 
           MVKH    .S2     _fcn_pass,B5
           STW     .D2T1   A4,*+B5(16)       ; |69| 
           ADD     .L2     8,B5,B13
           STW     .D2T1   A3,*+SP(24)

   [!A0]   B       .S1     $C$L7             ; |72| 
|| [ A0]   LDW     .D2T2   *B13,B0           ; |72| 

   [!A0]   CALL    .S1     _print_test_results ; |125| 
   [!A0]   ZERO    .L1     A3                ; |72| 
   [ A0]   ADD     .L2     4,B13,B4
   [!A0]   MVKL    .S1     _all_pass,A4
   [!A0]   MVKH    .S1     _all_pass,A4
           ; BRANCHCC OCCURS {$C$L7}         ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 10
   [ B0]   LDW     .D2T2   *B4,B4            ; |72| 
           CMPEQ   .L1     A4,0,A3           ; |72| 
           XOR     .L1     1,A3,A3           ; |72| 
   [!B0]   ZERO    .L1     A0                ; |72| 
           NOP             1
   [ B0]   CMPEQ   .L2     B4,0,B4           ; |72| 
   [ B0]   XOR     .L2     1,B4,B4           ; |72| 
           NOP             1

   [ B0]   AND     .L1X    A3,B4,A0          ; |72| 
||         MVK     .S1     0x1,A3            ; |72| 

   [!A0]   ZERO    .L1     A3                ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           CALL    .S1     _print_test_results ; |125| 
           MVKL    .S1     _all_pass,A4
           MVKH    .S1     _all_pass,A4
           NOP             2
;** --------------------------------------------------------------------------*
$C$L7:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A3,*A4            ; |72| 
||         MVK     .L1     0x1,A4            ; |125| 
||         ADDKPC  .S2     $C$RL9,B3,0       ; |125| 

$C$RL9:    ; CALL OCCURS {_print_test_results} {0}  ; |125| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 17

           ZERO    .L1     A31               ; |136| 
||         ZERO    .S1     A3

           STW     .D2T1   A31,*+SP(48)      ; |137| 
||         SET     .S1     A3,0x14,0x1d,A3

           STW     .D2T1   A3,*+SP(52)       ; |136| 
||         ZERO    .L1     A5
||         MVKL    .S2     _a_sc,B4

           LDW     .D2T2   *+SP(20),B29      ; |132| 
||         MVKH    .S2     _a_sc,B4
||         MVKH    .S1     0xbff00000,A5
||         ZERO    .L1     A4                ; |137| 

           STDW    .D2T1   A5:A4,*+B4(40)    ; |137| 
           STW     .D2T2   B11,*+SP(44)      ; |63| 

           STW     .D2T2   B11,*+SP(60)      ; |63| 
||         ZERO    .L1     A29               ; |63| 

           STW     .D2T1   A29,*+SP(56)      ; |133| 

           LDDW    .D2T1   *+SP(48),A5:A4    ; |137| 
||         ZERO    .L1     A30               ; |63| 

           STW     .D2T1   A30,*+SP(40)      ; |135| 
||         ADD     .L2     1,B12,B9
||         MVK     .S2     0x6,B31

           STW     .D2T2   B31,*+SP(12)      ; |135| 
||         ZERO    .L2     B5
||         ZERO    .S2     B8                ; |134| 

           STDW    .D2T2   B9:B8,*+B4(16)    ; |134| 
||         MVKH    .S2     0xfff00000,B5
||         ZERO    .L2     B7
||         ZERO    .L1     A7:A6             ; |132| 

           STDW    .D2T1   A7:A6,*B4         ; |132| 
||         MV      .L1X    B4,A14            ; |132| 
||         MVKH    .S2     0x80000000,B7

           ZERO    .L2     B4                ; |135| 
||         STDW    .D2T1   A5:A4,*+B4(32)    ; |136| 
||         MV      .S2     B4,B6             ; |132| 

           ZERO    .L2     B6                ; |133| 
||         STDW    .D2T2   B5:B4,*+B6(24)    ; |135| 
||         MV      .S2X    A14,B30           ; |135| 

           STDW    .D2T2   B7:B6,*+B30(8)    ; |133| 
||         ADD     .L2     4,B29,B4
||         MV      .L1X    B11,A13           ; |63| 

           STW     .D2T2   B4,*+SP(28)       ; |133| 
||         ZERO    .L2     B11
||         MV      .L1X    B12,A11           ; |65| 
||         MVK     .S1     0xffffffff,A10    ; |65| 
||         ZERO    .D1     A12               ; |63| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L8:    
;          EXCLUSIVE CPU CYCLES: 6

           LDDW    .D1T1   *A14,A7:A6        ; |139| 
||         CALL    .S1     __divd            ; |139| 

           ZERO    .L1     A5
           SET     .S1     A5,0x14,0x1d,A5
           ADDKPC  .S2     $C$RL10,B3,1      ; |139| 

           MV      .L1     A12,A4            ; |139| 
||         DADD    .L2X    0,A7:A6,B5:B4     ; |139| 

$C$RL10:   ; CALL OCCURS {__divd} {0}        ; |139| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 30
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T2   *B4,B4            ; |139| 
           NOP             4
           ADD     .L2     B11,B4,B4         ; |139| 
           STDW    .D2T1   A5:A4,*B4         ; |139| 

           CALLP   .S2     _recipdp,B3
||         LDDW    .D1T1   *A14,A5:A4        ; |140| 

$C$RL11:   ; CALL OCCURS {_recipdp} {0}      ; |140| 
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T2   *+B4(4),B4        ; |140| 
           NOP             4
           ADD     .L2     B11,B4,B4         ; |140| 
           STDW    .D2T1   A5:A4,*B4         ; |140| 

           CALLP   .S2     _recipdp_c,B3
||         LDDW    .D1T1   *A14,A5:A4        ; |141| 

$C$RL12:   ; CALL OCCURS {_recipdp_c} {0}    ; |141| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 51
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T2   *+B4(8),B4        ; |141| 
           LDDW    .D2T1   *+SP(40),A17:A16
           LDW     .D2T2   *+SP(28),B5       ; |63| 
           NOP             2
           ADD     .L2     B11,B4,B4         ; |141| 
           STDW    .D2T1   A5:A4,*B4         ; |141| 
           LDDW    .D1T1   *A14++,A7:A6      ; |63| 
           LDW     .D2T2   *B5,B31           ; |142| 
           LDW     .D2T2   *+SP(12),B4       ; |66| 
           NOP             2
           RCPDP   .S1     A7:A6,A5:A4       ; |63| 
           ADD     .L2     B11,B31,B6        ; |142| 
           FMPYDP  .M1     A5:A4,A7:A6,A9:A8 ; |63| 
           ADD     .L2     8,B11,B11         ; |138| 
           SUB     .L2     B4,1,B30          ; |138| 
           SUB     .L1X    B4,1,A2           ; |138| 
           FSUBDP  .L1     A17:A16,A9:A8,A9:A8 ; |63| 
           STW     .D2T2   B30,*+SP(12)      ; |142| 
           NOP             1
           FMPYDP  .M1     A5:A4,A9:A8,A5:A4 ; |63| 
           NOP             3
           FMPYDP  .M1     A5:A4,A7:A6,A9:A8 ; |63| 
           NOP             3
           FSUBDP  .L1     A13:A12,A9:A8,A9:A8 ; |63| 
           NOP             2
           FMPYDP  .M1     A5:A4,A9:A8,A9:A8 ; |63| 
           LDDW    .D2T1   *+SP(56),A5:A4
           NOP             2
           FMPYDP  .M1     A9:A8,A7:A6,A17:A16 ; |63| 
           ABSDP   .S1     A7:A6,A7:A6       ; |65| 
           NOP             2
           FSUBDP  .L1     A5:A4,A17:A16,A5:A4 ; |63| 
           CMPGTDP .S1     A7:A6,A11:A10,A0  ; |65| 
           NOP             1

   [ A2]   BNOP    .S1     $C$L8,3           ; |138| 
||         FMPYDP  .M1     A9:A8,A5:A4,A5:A4 ; |63| 

   [ A0]   ZERO    .L1     A5:A4             ; |66| 

   [!A2]   CALL    .S1     _recipdp_v        ; |144| 
||         STDW    .D2T1   A5:A4,*B6         ; |142| 

           ; BRANCHCC OCCURS {$C$L8}         ; |138| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           LDW     .D2T2   *+B5(4),B4        ; |144| 
           MVKL    .S1     _a_sc,A4
           MVKH    .S1     _a_sc,A4
           MVK     .L1     0x6,A6            ; |144| 
           ADDKPC  .S2     $C$RL13,B3,0      ; |144| 
$C$RL13:   ; CALL OCCURS {_recipdp_v} {0}    ; |144| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 48
           MVKL    .S2     _output,B11
           MVKL    .S2     0xd9d7bdbb,B6
           MVKL    .S2     0x3ddb7cdf,B7
           MVKH    .S2     _output,B11
           MVKH    .S2     0xd9d7bdbb,B6

           LDW     .D2T1   *B11,A4           ; |66| 
||         MVKH    .S2     0x3ddb7cdf,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D2T2   *+B11(4),B4       ; |66| 
||         MVK     .L1     0x6,A6            ; |66| 
||         MVK     .S1     0x1,A8            ; |66| 

$C$RL14:   ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     _fcn_pass,B5
           MVKH    .S2     _fcn_pass,B5
           MVKL    .S2     0xd9d7bdbb,B6
           MVKL    .S2     0x3ddb7cdf,B7

           STW     .D2T1   A4,*+B5(4)        ; |66| 
||         MVKH    .S2     0xd9d7bdbb,B6

           LDW     .D2T2   *+B11(8),B4       ; |67| 
||         MVKH    .S2     0x3ddb7cdf,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B11,A4           ; |67| 
||         MVK     .L1     0x6,A6            ; |67| 
||         MVK     .S1     0x1,A8            ; |67| 

$C$RL15:   ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     _fcn_pass,B5
           MVKL    .S2     0xd9d7bdbb,B6
           MVKH    .S2     _fcn_pass,B5
           MVKL    .S2     0x3ddb7cdf,B7

           STW     .D2T1   A4,*+B5(8)        ; |67| 
||         MVKH    .S2     0xd9d7bdbb,B6

           LDW     .D2T2   *+B11(12),B4      ; |68| 
||         MVKH    .S2     0x3ddb7cdf,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B11,A4           ; |68| 
||         MVK     .L1     0x6,A6            ; |68| 
||         MVK     .S1     0x1,A8            ; |68| 

$C$RL16:   ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     _fcn_pass,B5
           MVKL    .S2     0x3ddb7cdf,B7
           MVKL    .S2     0xd9d7bdbb,B6
           MVKH    .S2     _fcn_pass,B5
           MVKH    .S2     0x3ddb7cdf,B7

           MV      .L1X    B11,A3            ; |69| 
||         STW     .D2T1   A4,*+B5(12)       ; |68| 
||         MVKH    .S2     0xd9d7bdbb,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A3,A4            ; |69| 
||         LDW     .D2T2   *+B11(16),B4      ; |69| 
||         MVK     .L1     0x6,A6            ; |69| 
||         MVK     .S1     0x1,A8            ; |69| 

$C$RL17:   ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           LDW     .D2T1   *+SP(24),A3       ; |69| 
           MVKL    .S2     _fcn_pass,B4
           STW     .D2T2   B13,*+SP(36)      ; |72| 
           MVKH    .S2     _fcn_pass,B4
           STW     .D2T1   A4,*+B4(16)       ; |69| 
           LDW     .D1T1   *A3,A0            ; |72| 
           STW     .D2T1   A3,*+SP(32)       ; |69| 
           NOP             3

   [!A0]   B       .S1     $C$L9             ; |72| 
|| [ A0]   MV      .L2     B13,B4

   [!A0]   CALL    .S1     _print_test_results ; |148| 
|| [ A0]   LDW     .D2T2   *B4,B0            ; |72| 

   [!A0]   MV      .L1     A12,A4
   [!A0]   MVKL    .S1     _all_pass,A3
   [ A0]   ADD     .L2     4,B4,B4
   [!A0]   MVKH    .S1     _all_pass,A3
           ; BRANCHCC OCCURS {$C$L9}         ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 10
   [ B0]   LDW     .D2T2   *B4,B4            ; |72| 
           CMPEQ   .L1     A4,0,A3           ; |72| 
           XOR     .L1     1,A3,A3           ; |72| 
           MVK     .L1     0x1,A4            ; |72| 
   [!B0]   MV      .L1     A12,A0            ; |72| 
   [ B0]   CMPEQ   .L2     B4,0,B4           ; |72| 
   [ B0]   XOR     .L2     1,B4,B4           ; |72| 
           NOP             1
   [ B0]   AND     .L1X    A3,B4,A0          ; |72| 
   [!A0]   MV      .L1     A12,A4
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           CALL    .S1     _print_test_results ; |148| 
           MVKL    .S1     _all_pass,A3
           MVKH    .S1     _all_pass,A3
           NOP             2
;** --------------------------------------------------------------------------*
$C$L9:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A4,*A3            ; |72| 
||         MVK     .L1     0x3,A4            ; |148| 
||         ADDKPC  .S2     $C$RL18,B3,0      ; |148| 

$C$RL18:   ; CALL OCCURS {_print_test_results} {0}  ; |148| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 10
           STW     .D2T1   A15,*+SP(60)      ; |63| 
           STW     .D2T1   A15,*+SP(44)      ; |63| 

           LDW     .D2T1   *+SP(28),A14      ; |157| 
||         MV      .L1     A12,A30           ; |63| 

           STW     .D2T1   A30,*+SP(40)      ; |63| 
||         MVKL    .S1     0xcccccccd,A31

           STW     .D2T1   A30,*+SP(56)      ; |151| 
||         MVKL    .S1     0xc000cccc,A3
||         MVK     .S2     0x777,B4

           STW     .D2T2   B4,*+SP(12)       ; |151| 
||         MVKH    .S1     0xcccccccd,A31
||         MVKL    .S2     _a_ext,B10

           STW     .D2T1   A31,*+SP(64)      ; |157| 
||         MVKH    .S2     _a_ext,B10
||         MVKH    .S1     0xc000cccc,A3
||         ZERO    .L1     A5

           STW     .D2T1   A3,*+SP(68)       ; |157| 
||         SET     .S1     A5,0x14,0x14,A5
||         MV      .L1     A12,A4            ; |63| 

           ZERO    .L1     A5
||         MV      .S1X    B10,A15           ; |151| 
||         STDW    .D2T1   A5:A4,*B10        ; |151| 
||         MV      .D1     A15,A13           ; |63| 

           CALL    .S2     __divd            ; |153| 
||         LDDW    .D1T1   *A15,A7:A6        ; |153| 
||         SET     .S1     A5,0x14,0x1d,A5
||         MV      .L1     A12,A4            ; |153| 
||         MV      .L2X    A12,B11           ; |151| 
||         MV      .D2     B11,B12           ; |63| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L10:    
;          EXCLUSIVE CPU CYCLES: 5
           ADDKPC  .S2     $C$RL19,B3,3      ; |153| 
           DADD    .L2X    0,A7:A6,B5:B4     ; |153| 
$C$RL19:   ; CALL OCCURS {__divd} {0}        ; |153| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 26
           LDW     .D2T2   *B12,B4           ; |153| 
           NOP             4
           ADD     .L2     B11,B4,B4         ; |153| 
           STDW    .D2T1   A5:A4,*B4         ; |153| 

           CALLP   .S2     _recipdp,B3
||         LDDW    .D1T1   *A15,A5:A4        ; |154| 

$C$RL20:   ; CALL OCCURS {_recipdp} {0}      ; |154| 
           LDW     .D2T2   *+B12(4),B4       ; |154| 
           NOP             4
           ADD     .L2     B11,B4,B4         ; |154| 
           STDW    .D2T1   A5:A4,*B4         ; |154| 

           CALLP   .S2     _recipdp_c,B3
||         LDDW    .D1T1   *A15,A5:A4        ; |155| 

$C$RL21:   ; CALL OCCURS {_recipdp_c} {0}    ; |155| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 61
           LDW     .D2T2   *+B12(8),B4       ; |155| 
           MV      .L2X    A14,B5            ; |156| 
           LDW     .D2T2   *B5,B31           ; |156| 
           NOP             2
           ADD     .L2     B11,B4,B4         ; |155| 
           STDW    .D2T1   A5:A4,*B4         ; |155| 
           LDDW    .D1T1   *A15,A7:A6        ; |63| 
           LDDW    .D2T1   *+SP(40),A5:A4
           ADD     .L2     B11,B31,B4        ; |156| 
           ADD     .L2     8,B11,B11         ; |152| 
           NOP             1
           RCPDP   .S1     A7:A6,A9:A8       ; |63| 
           NOP             1
           FMPYDP  .M1     A9:A8,A7:A6,A17:A16 ; |63| 
           NOP             3
           FSUBDP  .L1     A5:A4,A17:A16,A5:A4 ; |63| 
           NOP             2
           FMPYDP  .M1     A9:A8,A5:A4,A5:A4 ; |63| 
           NOP             3
           FMPYDP  .M1     A5:A4,A7:A6,A9:A8 ; |63| 
           NOP             3
           FSUBDP  .L1     A13:A12,A9:A8,A9:A8 ; |63| 
           NOP             2
           FMPYDP  .M1     A5:A4,A9:A8,A9:A8 ; |63| 
           LDDW    .D2T1   *+SP(56),A5:A4
           NOP             2
           FMPYDP  .M1     A9:A8,A7:A6,A17:A16 ; |63| 
           ABSDP   .S1     A7:A6,A7:A6       ; |65| 
           NOP             2
           FSUBDP  .L1     A5:A4,A17:A16,A5:A4 ; |63| 
           CMPGTDP .S1     A7:A6,A11:A10,A0  ; |65| 
           NOP             1
           FMPYDP  .M1     A9:A8,A5:A4,A5:A4 ; |63| 
           NOP             3
   [ A0]   ZERO    .L1     A5:A4             ; |66| 
           STDW    .D2T1   A5:A4,*B4         ; |156| 

           LDW     .D2T2   *+SP(12),B4
||         LDDW    .D1T1   *A15,A7:A6        ; |157| 

           LDDW    .D2T1   *+SP(64),A5:A4
           NOP             3

           SUB     .L1X    B4,1,A0           ; |152| 
||         SUB     .L2     B4,1,B4           ; |152| 

   [ A0]   BNOP    .S1     $C$L10,3          ; |152| 
||         FMPYDP  .M1     A5:A4,A7:A6,A5:A4 ; |157| 

   [!A0]   CALL    .S2     _recipdp_v        ; |159| 
||         STDW    .D1T1   A5:A4,*++A15      ; |157| 
||         STW     .D2T2   B4,*+SP(12)       ; |157| 
|| [ A0]   ZERO    .L1     A5
|| [ A0]   MV      .S1     A12,A4            ; |153| 

   [ A0]   CALL    .S2     __divd            ; |153| 
|| [!A0]   LDW     .D2T2   *+B5(4),B4        ; |159| 
|| [ A0]   LDDW    .D1T1   *A15,A7:A6        ; |153| 
|| [ A0]   SET     .S1     A5,0x14,0x1d,A5

           ; BRANCHCC OCCURS {$C$L10}        ; |152| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 4
           MV      .L1X    B10,A4
           MVK     .S1     0x777,A6          ; |159| 
           ADDKPC  .S2     $C$RL22,B3,1      ; |159| 
$C$RL22:   ; CALL OCCURS {_recipdp_v} {0}    ; |159| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 42
           MVKL    .S2     0x3ddb7cdf,B7
           MVKL    .S2     0xd9d7bdbb,B6
           MVKH    .S2     0x3ddb7cdf,B7

           LDW     .D2T1   *B12,A4           ; |66| 
||         MVKH    .S2     0xd9d7bdbb,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T2   *+B12(4),B4       ; |66| 
||         MVK     .S1     0x777,A6          ; |66| 
||         MVK     .L1     0x1,A8            ; |66| 

$C$RL23:   ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     _fcn_pass,B11
           MVKL    .S2     0x3ddb7cdf,B7
           MVKH    .S2     _fcn_pass,B11
           MVKL    .S2     0xd9d7bdbb,B6

           LDW     .D2T2   *+B12(8),B4       ; |67| 
||         MVKH    .S2     0x3ddb7cdf,B7

           STW     .D2T1   A4,*+B11(4)       ; |66| 
||         MVKH    .S2     0xd9d7bdbb,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B12,A4           ; |67| 
||         MVK     .S1     0x777,A6          ; |67| 
||         MVK     .L1     0x1,A8            ; |67| 

$C$RL24:   ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     0x3ddb7cdf,B7
           MVKL    .S2     0xd9d7bdbb,B6

           STW     .D2T1   A4,*+B11(8)       ; |67| 
||         MVKH    .S2     0x3ddb7cdf,B7

           LDW     .D2T2   *+B12(12),B4      ; |68| 
||         MVKH    .S2     0xd9d7bdbb,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B12,A4           ; |68| 
||         MVK     .S1     0x777,A6          ; |68| 
||         MVK     .L1     0x1,A8            ; |68| 

$C$RL25:   ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     0x3ddb7cdf,B7
           MVKL    .S2     0xd9d7bdbb,B6
           MVKH    .S2     0x3ddb7cdf,B7

           MV      .L1X    B12,A3            ; |159| 
||         STW     .D2T1   A4,*+B11(12)      ; |68| 
||         MVKH    .S2     0xd9d7bdbb,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A3,A4            ; |69| 
||         LDW     .D2T2   *+B12(16),B4      ; |69| 
||         MVK     .S1     0x777,A6          ; |69| 
||         MVK     .L1     0x1,A8            ; |69| 

$C$RL26:   ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           LDW     .D2T1   *+SP(24),A3       ; |69| 
           STW     .D2T1   A4,*+B11(16)      ; |69| 
           NOP             3
           LDW     .D1T1   *A3,A0            ; |72| 
           NOP             4

   [!A0]   B       .S1     $C$L11            ; |72| 
|| [ A0]   LDW     .D2T2   *B13,B0           ; |72| 

   [!A0]   CALL    .S1     _print_test_results ; |163| 
   [ A0]   ADD     .L2     4,B13,B4
   [!A0]   MV      .L1     A12,A4
   [!A0]   MVKL    .S1     _all_pass,A3
   [!A0]   MVKH    .S1     _all_pass,A3
           ; BRANCHCC OCCURS {$C$L11}        ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 10
   [ B0]   LDW     .D2T2   *B4,B4            ; |72| 
           CMPEQ   .L1     A4,0,A3           ; |72| 
           XOR     .L1     1,A3,A3           ; |72| 
   [!B0]   MV      .L1     A12,A0            ; |72| 
           MVK     .L1     0x1,A4            ; |72| 
   [ B0]   CMPEQ   .L2     B4,0,B4           ; |72| 
   [ B0]   XOR     .L2     1,B4,B4           ; |72| 
           NOP             1
   [ B0]   AND     .L1X    A3,B4,A0          ; |72| 
   [!A0]   MV      .L1     A12,A4
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           CALL    .S1     _print_test_results ; |163| 
           MVKL    .S1     _all_pass,A3
           MVKH    .S1     _all_pass,A3
           NOP             2
;** --------------------------------------------------------------------------*
$C$L11:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A4,*A3            ; |72| 
||         MVK     .L1     0x4,A4            ; |163| 
||         ADDKPC  .S2     $C$RL27,B3,0      ; |163| 

$C$RL27:   ; CALL OCCURS {_print_test_results} {0}  ; |163| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 22
           MV      .L2X    A12,B5            ; |114| 

           MV      .L2X    A12,B4            ; |114| 
||         STW     .D2T2   B5,*+SP(44)       ; |114| 

           STW     .D2T2   B4,*+SP(40)       ; |114| 
           LDDW    .D2T2   *+SP(40),B5:B4    ; |114| 
           MVKL    .S1     _cycle_counts+24,A3
           MVKH    .S1     _cycle_counts+24,A3
           MV      .L2X    A12,B6            ; |114| 
           NOP             1

           STDW    .D1T2   B5:B4,*A3         ; |114| 
||         MVC     .S2     B6,TSCL           ; |115| 

           MVC     .S2     TSCL,B4           ; |116| 
           MVC     .S2     TSCH,B5           ; |116| 
           MVKL    .S1     _t_start,A3
           MVKH    .S1     _t_start,A3

           STDW    .D1T2   B5:B4,*A3         ; |116| 
||         MVC     .S2     TSCL,B6           ; |117| 

           MVC     .S2     TSCH,B8           ; |117| 
           SUBU    .L2     B6,B4,B7:B6       ; |117| 

           EXT     .S2     B7,24,24,B5       ; |117| 
||         SUB     .L2     B8,B5,B7          ; |117| 
||         MVKL    .S1     _t_offset,A3

           ADD     .L2     B7,B5,B5          ; |117| 
||         MV      .S2     B6,B4             ; |117| 
||         MVKH    .S1     _t_offset,A3

           STDW    .D1T2   B5:B4,*A3         ; |117| 
||         MVKL    .S1     _input,A3

           MVKH    .S1     _input,A3
||         CALL    .S2     _gimme_random     ; |173| 

           LDW     .D1T1   *A3,A11           ; |173| 
||         ZERO    .L2     B5
||         ZERO    .L1     A5
||         MVK     .S1     0x80,A10

           MVKH    .S2     0xc0590000,B5
||         ZERO    .L2     B4                ; |173| 
||         MVKH    .S1     0x40690000,A5
||         MV      .D2X    A12,B11           ; |117| 
||         ZERO    .L1     A4                ; |173| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L12:    
;          EXCLUSIVE CPU CYCLES: 3
           ADDKPC  .S2     $C$RL28,B3,2      ; |173| 
$C$RL28:   ; CALL OCCURS {_gimme_random} {0}  ; |173| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 61
           ADD     .L1X    A11,B11,A3        ; |173| 

           STDW    .D1T1   A5:A4,*A3         ; |173| 
||         MVC     .S2     TSCL,B6           ; |122| 

           MVC     .S2     TSCH,B7           ; |122| 
           RCPDP   .S1     A5:A4,A7:A6       ; |63| 
           ZERO    .L1     A16               ; |63| 
           FMPYDP  .M1     A7:A6,A5:A4,A9:A8 ; |63| 
           ZERO    .L1     A17
           SET     .S1     A17,0x1e,0x1e,A17
           MVKL    .S2     _output+12,B8
           FSUBDP  .L1     A17:A16,A9:A8,A9:A8 ; |63| 
           ZERO    .L2     B18               ; |63| 
           ZERO    .L2     B4                ; |63| 
           FMPYDP  .M1     A7:A6,A9:A8,A7:A6 ; |63| 
           MV      .L2X    A11,B12           ; |65| 
           MVKH    .S2     _output+12,B8
           MV      .L2X    A17,B5            ; |63| 
           FMPYDP  .M1     A7:A6,A5:A4,A9:A8 ; |63| 
           ADD     .L1X    A11,B11,A3        ; |65| 
           LDW     .D2T2   *B8,B31           ; |177| 
           MVK     .L2     0xffffffff,B8     ; |65| 
           MVKL    .S2     0x7fefffff,B9
           FSUBDP  .L2X    B5:B4,A9:A8,B5:B4 ; |63| 
           MVKH    .S2     0x7fefffff,B9
           MVKL    .S2     _t_start,B10
           FMPYDP  .M2X    A7:A6,B5:B4,B17:B16 ; |63| 
           MV      .L2X    A17,B19           ; |63| 
           MVKH    .S2     _t_start,B10
           STDW    .D2T2   B7:B6,*B10
           FMPYDP  .M2X    B17:B16,A5:A4,B5:B4 ; |63| 
           LDDW    .D1T1   *A3,A5:A4         ; |65| 
           NOP             2
           FSUBDP  .L2     B19:B18,B5:B4,B5:B4 ; |63| 
           NOP             1
           ABSDP   .S1     A5:A4,A5:A4       ; |65| 
           FMPYDP  .M2     B17:B16,B5:B4,B5:B4 ; |63| 
           CMPGTDP .S1X    A5:A4,B9:B8,A0    ; |65| 
           ADD     .L2     B11,B31,B8        ; |177| 
           NOP             1
   [ A0]   ZERO    .L2     B5:B4             ; |66| 

           STDW    .D2T2   B5:B4,*B8         ; |177| 
||         MVC     .S2     TSCL,B8           ; |128| 

           MVC     .S2     TSCH,B9           ; |128| 

           MVKL    .S1     _t_offset,A3
||         SUB     .L1     A10,1,A0          ; |171| 
||         MVKL    .S2     _t_stop,B13
||         ADD     .L2     8,B11,B11         ; |171| 
||         SUB     .D1     A10,1,A10         ; |171| 

           MVKH    .S1     _t_offset,A3
||         MVKH    .S2     _t_stop,B13

           LDDW    .D1T1   *A3,A5:A4         ; |130| 
           STDW    .D2T2   B9:B8,*B13        ; |128| 
           MVKL    .S1     _cycle_counts+24,A31
           MVKH    .S1     _cycle_counts+24,A31
           MVKL    .S1     _cycle_counts+24,A12
           ADDU    .L2X    A4,B6,B5:B4       ; |130| 
           MVKH    .S1     _cycle_counts+24,A12

           ADD     .L1X    B5,A5,A3          ; |130| 
||         LDDW    .D1T1   *A31,A5:A4        ; |130| 
||         SUBU    .L2     B8,B4,B5:B4       ; |130| 

           EXT     .S2     B5,24,24,B30      ; |130| 
||         ADD     .L1X    A3,B7,A3          ; |130| 
|| [ A0]   ZERO    .L2     B5

   [ A0]   MVKH    .S2     0xc0590000,B5
           SUB     .L1X    B9,A3,A3          ; |130| 
   [ A0]   B       .S1     $C$L12            ; |171| 

           ADDU    .L1X    A4,B4,A7:A6       ; |130| 
|| [ A0]   ZERO    .L2     B4                ; |173| 

           ADD     .L1X    A3,B30,A4         ; |130| 
|| [ A0]   MVKL    .S1     _input,A3
||         ADD     .D1     A7,A5,A30         ; |130| 

   [ A0]   CALL    .S2     _gimme_random     ; |173| 
|| [ A0]   MVKH    .S1     _input,A3
||         ADD     .L1     A30,A4,A7         ; |130| 
|| [ A0]   ZERO    .D1     A5

   [ A0]   LDW     .D1T1   *A3,A11           ; |173| 
|| [ A0]   ZERO    .L1     A4                ; |173| 
|| [ A0]   MVKH    .S1     0x40690000,A5

   [!A0]   CALL    .S1     __fltllif         ; |135| 
||         STDW    .D1T1   A7:A6,*A12        ; |130| 

           ; BRANCHCC OCCURS {$C$L12}        ; |171| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           LDDW    .D1T1   *A12,A5:A4        ; |135| 
           ADDKPC  .S2     $C$RL29,B3,3      ; |135| 
$C$RL29:   ; CALL OCCURS {__fltllif} {0}     ; |135| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 15
           ZERO    .L1     A3
           SET     .S1     A3,0x1a,0x1d,A3
           MPYSP   .M1     A3,A4,A3          ; |135| 
           ZERO    .L1     A11
           SET     .S1     A11,0x15,0x1d,A11
           ZERO    .L1     A10               ; |135| 
           SPDP    .S1     A3,A5:A4          ; |135| 
           NOP             2

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A11:A10,A5:A4,A5:A4 ; |135| 

$C$RL30:   ; CALL OCCURS {__fixdlli} {0}     ; |135| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 17
           LDDW    .D2T2   *+SP(40),B5:B4
           MV      .L2X    A12,B6
           STDW    .D2T1   A5:A4,*B6         ; |135| 
           SUBAW   .D2     B6,6,B11
           MV      .L2X    A10,B31           ; |114| 

           STDW    .D2T2   B5:B4,*B11        ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B4           ; |116| 
           MVC     .S2     TSCH,B5           ; |116| 
           MV      .L1X    B4,A13            ; |116| 

           MV      .L1X    B5,A12            ; |116| 
||         MVC     .S2     TSCL,B5           ; |117| 

           MVC     .S2     TSCH,B4           ; |117| 

           SUBU    .L2X    B5,A13,B7:B6      ; |117| 
||         MVK     .S2     0x80,B30

           SUB     .L1X    B4,A12,A4         ; |117| 
||         EXT     .S2     B7,24,24,B4       ; |117| 
||         MVKL    .S1     _output,A3
||         LDW     .D2T1   *+SP(52),A5

           STW     .D2T2   B30,*+SP(12)      ; |117| 
||         MVKH    .S1     _output,A3
||         CALL    .S2     __divd            ; |185| 

           ADD     .L1X    A4,B4,A15         ; |117| 
||         LDDW    .D2T2   *B12++,B5:B4      ; |185| 
||         LDW     .D1T1   *A3,A10

           ZERO    .L1     A4                ; |185| 
           MV      .L1X    B6,A14            ; |117| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L13:    
;          EXCLUSIVE CPU CYCLES: 2
           ADDKPC  .S2     $C$RL31,B3,1      ; |185| 
$C$RL31:   ; CALL OCCURS {__divd} {0}        ; |185| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           LDW     .D2T2   *+SP(12),B4       ; |185| 
           STDW    .D1T1   A5:A4,*A10++      ; |185| 
           NOP             3

           SUB     .L1X    B4,1,A0           ; |184| 
||         SUB     .L2     B4,1,B4           ; |184| 

   [ A0]   B       .S1     $C$L13            ; |184| 
||         STW     .D2T2   B4,*+SP(12)       ; |185| 

   [ A0]   LDDW    .D2T2   *B12++,B5:B4      ; |185| 
   [ A0]   CALL    .S1     __divd            ; |185| 
   [ A0]   LDW     .D2T1   *+SP(52),A5
   [ A0]   ZERO    .L1     A4                ; |185| 
           NOP             1
           ; BRANCHCC OCCURS {$C$L13}        ; |184| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
           MVC     .S2     TSCL,B6           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           MV      .L2     B4,B7             ; |128| 
||         LDDW    .D2T2   *B11,B5:B4        ; |130| 
||         ADDU    .L1     A14,A13,A5:A4     ; |130| 

           ADD     .S1     A5,A15,A3         ; |130| 
||         SUBU    .L1X    B6,A4,A5:A4       ; |130| 

           ADD     .L1     A3,A12,A3         ; |130| 
||         EXT     .S1     A5,24,24,A31      ; |130| 

           CALL    .S1     __fltllif         ; |130| 
||         SUB     .L1X    B7,A3,A3          ; |130| 

           ADD     .L1     A3,A31,A3         ; |130| 

           ADDU    .L2X    B4,A4,B9:B8       ; |130| 
||         MV      .S2     B13,B4            ; |130| 

           ADD     .L2     B9,B5,B5          ; |130| 
||         STDW    .D2T2   B7:B6,*B4         ; |128| 

           MV      .L1X    B8,A4             ; |130| 

           ADD     .L1X    B5,A3,A5          ; |130| 
||         ADDKPC  .S2     $C$RL32,B3,0      ; |130| 

$C$RL32:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 14
           ZERO    .L1     A3
           SET     .S1     A3,0x1a,0x1d,A3
           MPYSP   .M1     A3,A4,A3          ; |130| 
           ZERO    .L1     A10               ; |130| 
           NOP             2
           SPDP    .S1     A3,A5:A4          ; |130| 
           NOP             1

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A11:A10,A5:A4,A5:A4 ; |130| 

$C$RL33:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 17
           LDDW    .D2T2   *+SP(40),B5:B4    ; |130| 
           ZERO    .L1     A12
           MV      .L2X    A10,B31           ; |114| 
           STDW    .D2T1   A5:A4,*B11        ; |130| 
           SET     .S1     A12,0x1a,0x1d,A12

           MVC     .S2     B31,TSCL          ; |115| 
||         STDW    .D2T2   B5:B4,*+B11(8)    ; |114| 

           MVC     .S2     TSCL,B6           ; |116| 
           MVC     .S2     TSCH,B4           ; |116| 

           MV      .L1X    B10,A14           ; |116| 
||         MV      .L2     B4,B7             ; |116| 

           STDW    .D1T2   B7:B6,*A14        ; |116| 
||         MVC     .S2     TSCL,B4           ; |117| 

           MVC     .S2     TSCH,B8           ; |117| 

           SUBU    .L2     B4,B6,B5:B4       ; |117| 
||         MVKL    .S2     _t_offset,B12

           EXT     .S2     B5,24,24,B5       ; |117| 
||         SUB     .L2     B8,B7,B6          ; |117| 

           ADD     .L2     B6,B5,B5          ; |117| 
||         MVKH    .S2     _t_offset,B12

           STDW    .D2T2   B5:B4,*B12        ; |117| 
||         MVK     .S2     0x80,B4

           MVKL    .S1     _input,A13

           MV      .L1X    B4,A15            ; |117| 
||         MVKH    .S1     _input,A13
||         LDW     .D2T2   *+SP(16),B13      ; |117| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L14:    
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D1T1   *A13,A3           ; |193| 
           NOP             4

           ADD     .L1     A10,A3,A3         ; |193| 
||         CALL    .S1     _recipdp          ; |193| 

           LDDW    .D1T1   *A3,A5:A4         ; |193| 
           ADDKPC  .S2     $C$RL34,B3,3      ; |193| 
$C$RL34:   ; CALL OCCURS {_recipdp} {0}      ; |193| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7

           LDW     .D2T2   *B13,B4           ; |193| 
||         SUB     .L1     A15,1,A0          ; |192| 
||         SUB     .S1     A15,1,A15         ; |192| 

   [ A0]   BNOP    .S1     $C$L14,3          ; |192| 

           ADD     .L1X    A10,B4,A3         ; |193| 
||         ADD     .S1     8,A10,A10         ; |192| 

           STDW    .D1T1   A5:A4,*A3         ; |193| 
           ; BRANCHCC OCCURS {$C$L14}        ; |192| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           MVC     .S2     TSCL,B8           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           LDDW    .D1T1   *A14,A5:A4        ; |130| 
||         LDDW    .D2T2   *B12,B7:B6        ; |130| 

           LDDW    .D2T2   *+B11(8),B17:B16  ; |130| 
           MV      .L2     B4,B9             ; |128| 
           NOP             2
           ADDU    .L1X    B6,A4,A7:A6       ; |130| 
           ADD     .L1X    A7,B7,A4          ; |130| 

           ADD     .L1     A4,A5,A3          ; |130| 
||         SUBU    .L2X    B8,A6,B5:B4       ; |130| 

           CALL    .S1     __fltllif         ; |130| 
||         ADDU    .L2     B16,B4,B7:B6      ; |130| 
||         EXT     .S2     B5,24,24,B5       ; |130| 

           SUB     .L2X    B9,A3,B4          ; |130| 
||         ADD     .S2     B7,B17,B31        ; |130| 

           ADD     .L2     B4,B5,B5          ; |130| 
||         MVKL    .S2     _t_stop,B4

           ADD     .L2     B31,B5,B7         ; |130| 
||         MVKH    .S2     _t_stop,B4

           STDW    .D2T2   B9:B8,*B4         ; |128| 

           DADD    .L1X    0,B7:B6,A5:A4     ; |130| 
||         ADDKPC  .S2     $C$RL35,B3,0      ; |130| 

$C$RL35:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           MPYSP   .M1     A12,A4,A3         ; |130| 
           ZERO    .L1     A10               ; |130| 
           NOP             2
           SPDP    .S1     A3,A5:A4          ; |130| 
           NOP             1

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A11:A10,A5:A4,A5:A4 ; |130| 

$C$RL36:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 17
           LDDW    .D2T2   *+SP(40),B5:B4
           ADDAD   .D2     B11,1,B13
           MV      .L2X    A10,B31           ; |114| 
           STDW    .D2T1   A5:A4,*B13        ; |130| 
           NOP             1

           STDW    .D2T2   B5:B4,*+B11(16)   ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B8           ; |116| 
           MVC     .S2     TSCH,B4           ; |116| 
           MV      .L2     B4,B9             ; |116| 

           STDW    .D1T2   B9:B8,*A14        ; |116| 
||         MVC     .S2     TSCL,B4           ; |117| 

           MVC     .S2     TSCH,B6           ; |117| 
           SUBU    .L2     B4,B8,B5:B4       ; |117| 

           EXT     .S2     B5,24,24,B5       ; |117| 
||         SUB     .L2     B6,B9,B6          ; |117| 

           ADD     .L2     B6,B5,B5          ; |117| 

           STDW    .D2T2   B5:B4,*B12        ; |117| 
||         MVK     .S2     0x80,B4

           LDW     .D2T2   *+SP(20),B10      ; |117| 
           MV      .L1X    B4,A15            ; |117| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L15:    
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D1T1   *A13,A3           ; |201| 
           NOP             4

           ADD     .L1     A10,A3,A3         ; |201| 
||         CALL    .S1     _recipdp_c        ; |201| 

           LDDW    .D1T1   *A3,A5:A4         ; |201| 
           ADDKPC  .S2     $C$RL37,B3,3      ; |201| 
$C$RL37:   ; CALL OCCURS {_recipdp_c} {0}    ; |201| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7

           LDW     .D2T2   *B10,B4           ; |201| 
||         SUB     .L1     A15,1,A0          ; |200| 
||         SUB     .S1     A15,1,A15         ; |200| 

   [ A0]   BNOP    .S1     $C$L15,3          ; |200| 

           ADD     .L1X    A10,B4,A3         ; |201| 
||         ADD     .S1     8,A10,A10         ; |200| 

           STDW    .D1T1   A5:A4,*A3         ; |201| 
           ; BRANCHCC OCCURS {$C$L15}        ; |200| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           MVC     .S2     TSCL,B8           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           MV      .L2     B12,B5            ; |128| 
||         LDDW    .D1T1   *A14,A5:A4        ; |130| 

           LDDW    .D2T2   *B5,B7:B6         ; |130| 
           LDDW    .D2T2   *+B11(16),B17:B16 ; |130| 
           MV      .L2     B4,B9             ; |128| 
           NOP             2
           ADDU    .L2X    B6,A4,B5:B4       ; |130| 

           ADD     .S2     B5,B7,B4          ; |130| 
||         SUBU    .L2     B8,B4,B7:B6       ; |130| 

           ADD     .L2X    B4,A5,B4          ; |130| 
||         MV      .S2     B6,B5             ; |130| 
||         CALL    .S1     __fltllif         ; |130| 

           SUB     .D2     B9,B4,B4          ; |130| 
||         ADDU    .L2     B16,B5,B7:B6      ; |130| 
||         EXT     .S2     B7,24,24,B5       ; |130| 

           ADD     .L2     B4,B5,B5          ; |130| 
||         ADD     .S2     B7,B17,B4         ; |130| 

           ADD     .L2     B4,B5,B7          ; |130| 
||         MVKL    .S2     _t_stop,B4

           MVKH    .S2     _t_stop,B4

           STDW    .D2T2   B9:B8,*B4         ; |128| 
||         DADD    .L1X    0,B7:B6,A5:A4     ; |130| 
||         ADDKPC  .S2     $C$RL38,B3,0      ; |130| 

$C$RL38:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 106
           MPYSP   .M1     A12,A4,A3         ; |130| 
           ZERO    .L1     A10               ; |130| 
           NOP             3
           SPDP    .S2X    A3,B5:B4          ; |130| 
           NOP             2

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1X    A11:A10,B5:B4,A5:A4 ; |130| 

$C$RL39:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
           LDDW    .D2T2   *+SP(40),B5:B4
           ADDAD   .D2     B13,1,B12
           MV      .L2X    A10,B31           ; |114| 
           STDW    .D2T1   A5:A4,*B12        ; |130| 
           NOP             1

           STDW    .D2T2   B5:B4,*+B11(32)   ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B4           ; |116| 
           MVC     .S2     TSCH,B5           ; |116| 

           STDW    .D1T2   B5:B4,*A14        ; |116| 
||         MVC     .S2     TSCL,B6           ; |117| 

           MVC     .S2     TSCH,B8           ; |117| 

           MV      .D2X    A13,B9            ; |208| 
||         SUBU    .L2     B6,B4,B7:B6       ; |117| 
||         MVKL    .S2     _t_offset,B11

           LDW     .D2T1   *B9,A4            ; |208| 
||         EXT     .S2     B7,24,24,B5       ; |117| 
||         SUB     .L2     B8,B5,B7          ; |117| 

           LDW     .D2T2   *+B10(8),B4       ; |208| 
||         ADD     .L2     B7,B5,B7          ; |117| 
||         MVKH    .S2     _t_offset,B11

           CALLP   .S2     _recipdp_v,B3
||         STDW    .D2T2   B7:B6,*B11        ; |117| 
||         MVK     .S1     0x80,A6           ; |208| 

$C$RL40:   ; CALL OCCURS {_recipdp_v} {0}    ; |208| 
           MVC     .S2     TSCL,B8           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           LDDW    .D1T1   *A14,A7:A6        ; |130| 
||         LDDW    .D2T2   *B11,B7:B6        ; |130| 

           MVK     .S1     16,A13
           ADD     .L1X    B12,A13,A13
           LDDW    .D1T1   *A13,A9:A8        ; |130| 
           MV      .L2     B4,B9             ; |128| 
           ADDU    .L1X    B6,A6,A5:A4       ; |130| 
           SUBU    .L1X    B8,A4,A17:A16     ; |130| 
           ADD     .L1X    A5,B7,A4          ; |130| 

           MV      .L1     A16,A3            ; |130| 
||         ADD     .D1     A4,A7,A4          ; |130| 
||         EXT     .S1     A17,24,24,A6      ; |130| 

           SUB     .S1X    B9,A4,A3          ; |130| 
||         ADDU    .L1     A8,A3,A5:A4       ; |130| 
||         MVKL    .S2     _t_stop,B4

           ADD     .L1     A5,A9,A3          ; |130| 
||         ADD     .S1     A3,A6,A5          ; |130| 
||         MVKH    .S2     _t_stop,B4

           CALLP   .S2     __fltllif,B3
||         ADD     .L1     A3,A5,A5          ; |130| 
||         STDW    .D2T2   B9:B8,*B4         ; |128| 

$C$RL41:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
           MPYSP   .M1     A12,A4,A3         ; |130| 
           NOP             3
           SPDP    .S1     A3,A5:A4          ; |130| 
           NOP             1

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A11:A10,A5:A4,A5:A4 ; |130| 

$C$RL42:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
           MVKL    .S2     0xd9d7bdbb,B6

           MVKL    .S1     _output,A10
||         MVKL    .S2     0x3ddb7cdf,B7

           MVKH    .S1     _output,A10
||         STDW    .D1T1   A5:A4,*A13        ; |130| 
||         MVKH    .S2     0xd9d7bdbb,B6

           LDW     .D1T1   *A10,A4           ; |66| 
||         MVKH    .S2     0x3ddb7cdf,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D1T2   *+A10(4),B4       ; |66| 
||         MVK     .S1     0x80,A6           ; |66| 
||         MVK     .L1     0x1,A8            ; |66| 

$C$RL43:   ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     0x3ddb7cdf,B7
           MVKL    .S2     0xd9d7bdbb,B6
           MVKL    .S2     _fcn_pass,B10
           MVKH    .S2     0x3ddb7cdf,B7
           MVKH    .S2     0xd9d7bdbb,B6

           MV      .L1     A4,A3             ; |66| 
||         LDW     .D1T1   *A10,A4           ; |67| 
||         MVKH    .S2     _fcn_pass,B10

           CALLP   .S2     _isequal,B3
||         LDW     .D1T2   *+A10(8),B4       ; |67| 
||         STW     .D2T1   A3,*+B10(4)       ; |66| 
||         MVK     .S1     0x80,A6           ; |67| 
||         MVK     .L1     0x1,A8            ; |67| 

$C$RL44:   ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     0xd9d7bdbb,B6
           MVKL    .S2     0x3ddb7cdf,B7

           MV      .L1     A4,A3             ; |67| 
||         MVKH    .S2     0xd9d7bdbb,B6

           LDW     .D1T2   *+A10(12),B4      ; |68| 
||         MVKH    .S2     0x3ddb7cdf,B7
||         STW     .D2T1   A3,*+B10(8)       ; |67| 

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A10,A4           ; |68| 
||         MVK     .S1     0x80,A6           ; |68| 
||         MVK     .L1     0x1,A8            ; |68| 

$C$RL45:   ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     0x3ddb7cdf,B7
           MVKL    .S2     0xd9d7bdbb,B6
           MVKH    .S2     0x3ddb7cdf,B7

           MV      .L1     A10,A3            ; |68| 
||         STW     .D2T1   A4,*+B10(12)      ; |68| 
||         LDW     .D1T2   *+A10(16),B4      ; |69| 
||         MVKH    .S2     0xd9d7bdbb,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A3,A4            ; |69| 
||         MVK     .S1     0x80,A6           ; |69| 
||         MVK     .L1     0x1,A8            ; |69| 

$C$RL46:   ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           LDW     .D2T1   *+SP(32),A3       ; |69| 
           LDW     .D2T2   *+SP(36),B4
           STW     .D2T1   A4,*+B10(16)      ; |69| 
           NOP             2
           LDW     .D1T1   *A3,A0            ; |72| 
           NOP             4

   [!A0]   BNOP    .S1     $C$L16,5          ; |72| 
|| [ A0]   LDW     .D2T2   *B4,B0            ; |72| 
|| [ A0]   ADD     .L2     4,B4,B4

           ; BRANCHCC OCCURS {$C$L16}        ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 15
   [ B0]   LDW     .D2T2   *B4,B4            ; |72| 
   [!B0]   ZERO    .L1     A0                ; |72| 
           CMPEQ   .L1     A4,0,A3           ; |72| 
           XOR     .L1     1,A3,A3           ; |72| 
           NOP             1
   [ B0]   CMPEQ   .L2     B4,0,B4           ; |72| 
   [ B0]   XOR     .L2     1,B4,B4           ; |72| 
           NOP             1
   [ B0]   AND     .L1X    A3,B4,A0          ; |72| 
   [ A0]   B       .S1     $C$L17            ; |72| 
   [ A0]   CALL    .S1     _print_test_results ; |214| 
           MVK     .L1     0x1,A3            ; |72| 
   [ A0]   MVKL    .S1     _all_pass,A4
   [ A0]   MVKH    .S1     _all_pass,A4
           NOP             1
           ; BRANCHCC OCCURS {$C$L17}        ; |72| 
;** --------------------------------------------------------------------------*
$C$L16:    
;          EXCLUSIVE CPU CYCLES: 5
           CALL    .S1     _print_test_results ; |214| 
           MVKL    .S1     _all_pass,A4
           MVKH    .S1     _all_pass,A4
           ZERO    .L1     A3                ; |72| 
           NOP             1
;** --------------------------------------------------------------------------*
$C$L17:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A3,*A4            ; |72| 
||         MVK     .L1     0x2,A4            ; |214| 
||         ADDKPC  .S2     $C$RL47,B3,0      ; |214| 

$C$RL47:   ; CALL OCCURS {_print_test_results} {0}  ; |214| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 30
           MVKL    .S2     $C$SL2+0,B4
           MVKH    .S2     $C$SL2+0,B4

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |215| 

$C$RL48:   ; CALL OCCURS {_printf} {0}       ; |215| 
           MVKL    .S2     $C$SL3+0,B4
           MVKH    .S2     $C$SL3+0,B4

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |215| 

$C$RL49:   ; CALL OCCURS {_printf} {0}       ; |215| 
           MVKL    .S1     $C$SL1+0,A10
           MVKH    .S1     $C$SL1+0,A10

           CALLP   .S2     _print_profile_results,B3
||         MV      .L1     A10,A4            ; |222| 

$C$RL50:   ; CALL OCCURS {_print_profile_results} {0}  ; |222| 

           CALLP   .S2     _print_memory_results,B3
||         MV      .L1     A10,A4            ; |225| 

$C$RL51:   ; CALL OCCURS {_print_memory_results} {0}  ; |225| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *++SP(80),B3      ; |226| 
           LDDW    .D2T1   *++SP,A13:A12     ; |226| 
           LDDW    .D2T1   *++SP,A15:A14     ; |226| 
           LDDW    .D2T2   *++SP,B11:B10     ; |226| 
           LDDW    .D2T2   *++SP,B13:B12     ; |226| 

           LDW     .D2T1   *++SP(8),A10      ; |226| 
||         RET     .S2     B3                ; |226| 

           LDW     .D2T1   *++SP(8),A11      ; |226| 
           NOP             4
           ; BRANCH OCCURS {B3}              ; |226| 
;******************************************************************************
;* STRINGS                                                                    *
;******************************************************************************
	.sect	".const:.string"
$C$SL1:	.string	"recipDP",0
$C$SL2:	.string	"----------------------------------------",0
$C$SL3:	.string	"----------------------------------------",10,0
;*****************************************************************************
;* UNDEFINED EXTERNAL REFERENCES                                             *
;*****************************************************************************
	.global	_printf
	.global	_driver_init
	.global	_print_profile_results
	.global	_print_memory_results
	.global	_print_test_results
	.global	_gimme_random
	.global	_isequal
	.global	_recipdp
	.global	_recipdp_c
	.global	_recipdp_v
	.global	_t_start
	.global	_t_stop
	.global	_t_offset
	.global	_cycle_counts
	.global	_fcn_pass
	.global	_all_pass
	.global	_a_ext
	.global	_a_sc
	.global	_output
	.global	_input
	.global	__divd
	.global	__fltllif
	.global	__fixdlli

;******************************************************************************
;* BUILD ATTRIBUTES                                                           *
;******************************************************************************
	.battr "TI", Tag_File, 1, Tag_ABI_stack_align_needed(0)
	.battr "TI", Tag_File, 1, Tag_ABI_stack_align_preserved(0)
	.battr "TI", Tag_File, 1, Tag_Tramps_Use_SOC(1)
