;******************************************************************************
;* TMS320C6x C/C++ Codegen                                          PC v7.4.2 *
;* Date/Time created: Tue May 17 03:26:18 2016                                *
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
	.word	0418cf297h		; _a[0] @ 0
	.word	042344956h		; _a[1] @ 32
	.word	0c29b829fh		; _a[2] @ 64
	.word	0421fcd8bh		; _a[3] @ 96
	.word	0c2c42d34h		; _a[4] @ 128
	.word	0c21925dch		; _a[5] @ 160
	.word	0c296fba2h		; _a[6] @ 192
	.word	0c269a74ah		; _a[7] @ 224
	.word	0426e4e13h		; _a[8] @ 256
	.word	0c22c2aa7h		; _a[9] @ 288
$C$IR_1:	.set	40

	.global	_a
_a:	.usect	".far",40,8
;	C:\MATHLIB_Tools\CCSV5_5_0\ccsv5\tools\compiler\c6000_7.4.2\bin\opt6x.exe C:\\Users\\gtbldadm\\AppData\\Local\\Temp\\017405 C:\\Users\\gtbldadm\\AppData\\Local\\Temp\\017407 
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
           STW     .D2T1   A11,*SP--(8)      ; |92| 
           STW     .D2T1   A10,*SP--(8)      ; |92| 
           STDW    .D2T2   B13:B12,*SP--     ; |92| 
           STDW    .D2T2   B11:B10,*SP--     ; |92| 
           STDW    .D2T1   A15:A14,*SP--     ; |92| 

           STDW    .D2T1   A13:A12,*SP--     ; |92| 
||         MVKL    .S1     $C$SL1+0,A4

           STW     .D2T2   B3,*SP--(80)      ; |92| 
||         CALLP   .S2     _driver_init,B3
||         MVKH    .S1     $C$SL1+0,A4

$C$RL0:    ; CALL OCCURS {_driver_init} {0}  ; |100| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 3

           MVK     .L2     0xa,B4
||         MVKL    .S1     _a,A11

           MVKH    .S1     _a,A11
||         MVKL    .S2     _output,B11

           CALL    .S1     _sinf             ; |109| 
||         MV      .L1     A11,A12
||         MV      .D1X    B4,A10
||         MVKH    .S2     _output,B11
||         ZERO    .L2     B10

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L1:    
;          EXCLUSIVE CPU CYCLES: 5
           LDW     .D1T1   *A12++,A4         ; |109| 
           ADDKPC  .S2     $C$RL1,B3,3       ; |109| 
$C$RL1:    ; CALL OCCURS {_sinf} {0}         ; |109| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7

           LDW     .D2T2   *B11,B4           ; |109| 
||         SUB     .L1     A10,1,A0          ; |108| 
||         SUB     .S1     A10,1,A10         ; |108| 

   [ A0]   BNOP    .S1     $C$L1,3           ; |108| 
|| [!A0]   MVK     .L2     0xa,B5

           ADD     .L2     B10,B4,B4         ; |109| 
||         ADD     .S2     4,B10,B10         ; |108| 

   [ A0]   CALL    .S1     _sinf             ; |109| 
||         STW     .D2T1   A4,*B4            ; |109| 
|| [!A0]   ADD     .L2     4,B11,B4

           ; BRANCHCC OCCURS {$C$L1}         ; |108| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1

           CALL    .S1     _sinsp            ; |112| 
||         ZERO    .L2     B10
||         MV      .L1X    B5,A10
||         MV      .D1     A11,A14
||         STW     .D2T2   B4,*+SP(32)

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L2:    
;          EXCLUSIVE CPU CYCLES: 5
           LDW     .D1T1   *A11++,A4         ; |112| 
           ADDKPC  .S2     $C$RL2,B3,3       ; |112| 
$C$RL2:    ; CALL OCCURS {_sinsp} {0}        ; |112| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           LDW     .D2T2   *+SP(32),B4       ; |112| 
           SUB     .L1     A10,1,A0          ; |111| 
           SUB     .L1     A10,1,A10         ; |111| 
           NOP             2
           LDW     .D2T2   *B4,B4            ; |112| 
   [ A0]   BNOP    .S1     $C$L2,3           ; |111| 

           ADD     .L2     B10,B4,B4         ; |112| 
||         ADD     .S2     4,B10,B10         ; |111| 

   [ A0]   CALL    .S1     _sinsp            ; |112| 
||         STW     .D2T1   A4,*B4            ; |112| 

           ; BRANCHCC OCCURS {$C$L2}         ; |111| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7
           LDW     .D2T2   *+SP(32),B4
           MVK     .L2     0xa,B5
           ZERO    .L2     B10
           MV      .L1     A14,A11
           MV      .L1X    B5,A10
           ADD     .L2     4,B4,B4

           STW     .D2T2   B4,*+SP(36)
||         CALL    .S1     _sinsp_c          ; |115| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L3:    
;          EXCLUSIVE CPU CYCLES: 5
           LDW     .D1T1   *A11++,A4         ; |115| 
           ADDKPC  .S2     $C$RL3,B3,3       ; |115| 
$C$RL3:    ; CALL OCCURS {_sinsp_c} {0}      ; |115| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           LDW     .D2T2   *+SP(36),B4       ; |115| 
           SUB     .L1     A10,1,A0          ; |114| 
           SUB     .L1     A10,1,A10         ; |114| 
           NOP             2
           LDW     .D2T2   *B4,B4            ; |115| 
   [ A0]   BNOP    .S1     $C$L3,3           ; |114| 

           ADD     .L2     B10,B4,B4         ; |115| 
||         ADD     .S2     4,B10,B10         ; |114| 

   [ A0]   CALL    .S1     _sinsp_c          ; |115| 
||         STW     .D2T1   A4,*B4            ; |115| 

           ; BRANCHCC OCCURS {$C$L3}         ; |114| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11

           LDW     .D2T2   *+SP(36),B4
||         MVKL    .S1     0x3ea2f983,A4
||         ZERO    .L1     A15
||         ZERO    .D1     A3
||         MVKL    .S2     0x3a7daa22,B13
||         ZERO    .L2     B12

           MVKL    .S1     0x3c08873e,A12
||         MVKH    .S2     0x3a7daa22,B13
||         ZERO    .L2     B6
||         ZERO    .L1     A5
||         MVK     .D1     9,A0
||         ZERO    .D2     B24

           MVKL    .S1     0x362e9c5b,A10
||         MVKL    .S2     0xbe2aaaa4,B5
||         STW     .D2T1   A5,*+SP(16)
||         MV      .L2X    A14,B8

           MVKH    .S1     0x3ea2f983,A4
||         MVKL    .S2     0xb94fb222,B31

           MVKH    .S1     0x3c08873e,A12
||         MVKH    .S2     0xbe2aaaa4,B5
||         STW     .D2T1   A4,*+SP(12)

           MVKH    .S1     0x362e9c5b,A10
||         MVKH    .S2     0xb94fb222,B31
||         ADD     .L2     4,B4,B10
||         MV      .L1X    B13,A4
||         MV      .D2X    A4,B25

           MVKH    .S1     0x40490000,A15
||         MVKH    .S2     0x49800000,B12
||         LDW     .D2T2   *B10,B9
||         MV      .L2X    A12,B18

           SET     .S1     A3,0x17,0x1d,A3
||         MVKH    .S2     0xbf800000,B6
||         MV      .L2X    A10,B16
||         MV      .D2     B5,B19

           STW     .D2T1   A3,*+SP(20)
||         DINT                              ; interrupts off
||         MV      .L2X    A15,B22

           MV      .L2X    A3,B21
||         MV      .S2     B12,B23
||         MV      .D2     B6,B20

           STW     .D2T2   B6,*+SP(24)
||         MV      .L2     B31,B17

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*
;*      Loop found in file               : C:/nightlybuilds/mathlib/ti/mathlib/src/sinsp/sinsp_d.c
;*      Loop source line                 : 117
;*      Loop opening brace source line   : 117
;*      Loop closing brace source line   : 119
;*      Known Minimum Trip Count         : 10                    
;*      Known Maximum Trip Count         : 10                    
;*      Known Max Trip Count Factor      : 10
;*      Loop Carried Dependency Bound(^) : 71
;*      Unpartitioned Resource Bound     : 5
;*      Partitioned Resource Bound(*)    : 9
;*      Resource Partition:
;*                                A-side   B-side
;*      .L units                     0        0     
;*      .S units                     1        2     
;*      .D units                     0        2     
;*      .M units                     1        9*    
;*      .X cross paths               1        1     
;*      .T address paths             0        2     
;*      Long read paths              0        0     
;*      Long write paths             0        0     
;*      Logical  ops (.LS)           0        8     (.L or .S unit)
;*      Addition ops (.LSD)          0        5     (.L or .S or .D unit)
;*      Bound(.L .S .LS)             1        5     
;*      Bound(.L .S .D .LS .LSD)     1        6     
;*
;*      Searching for software pipeline schedule at ...
;*         ii = 71 Schedule found with 1 iterations in parallel
;*
;*      Register Usage Table:
;*          +-----------------------------------------------------------------+
;*          |AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA|BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB|
;*          |00000000001111111111222222222233|00000000001111111111222222222233|
;*          |01234567890123456789012345678901|01234567890123456789012345678901|
;*          |--------------------------------+--------------------------------|
;*       0: |*   *                           |        **      **********      |
;*       1: |*   *                           |        **      **********      |
;*       2: |*   *                           |        **      **********      |
;*       3: |*   *                           |        **      **********      |
;*       4: |*   *                           |        **      **********      |
;*       5: |*   *                           |    *   **      **********      |
;*       6: |*   *                           |    **  **      **********      |
;*       7: |*   *                           |*   *   **      **********      |
;*       8: |*   *                           |    *   **      **********      |
;*       9: |*   *                           |    *   **      **********      |
;*      10: |*   *                           |    *   **      **********      |
;*      11: |*   *                           |    *   **      **********      |
;*      12: |*   *                           |    **  **      **********      |
;*      13: |*   *                           |    *   **      **********      |
;*      14: |*   *                           |    *   **      **********      |
;*      15: |*   *                           |    *   **      **********      |
;*      16: |*   *                           |    **  **      **********      |
;*      17: |*   *                           |    **  **      **********      |
;*      18: |*   *                           |    **  **      **********      |
;*      19: |*   *                           |    **  **      **********      |
;*      20: |*   *                           |    *** **      **********      |
;*      21: |*   *                           |    *** **      **********      |
;*      22: |*   *                           |    **  **      **********      |
;*      23: |*   *                           |    **  **      **********      |
;*      24: |*   *                           |    *** **      **********      |
;*      25: |*  **                           |     *  **      **********      |
;*      26: |*  **                           |     *  **      **********      |
;*      27: |*  **                           |    **  **      **********      |
;*      28: |*   *                           |     *  **      **********      |
;*      29: |*   *                           |     *  **      **********      |
;*      30: |*   *                           |     ** **      **********      |
;*      31: |*   *                           |     ** **      **********      |
;*      32: |*   *                           |     ** **      **********      |
;*      33: |*   *                           |     ** **      **********      |
;*      34: |*   *                           |     *****      **********      |
;*      35: |*   *                           |     *****      **********      |
;*      36: |*   *                           |     *****      **********      |
;*      37: |*   *                           |     *****      **********      |
;*      38: |*   *                           |    ******      **********      |
;*      39: |*   *                           |     *****      **********      |
;*      40: |*   *                           |     *****      **********      |
;*      41: |*   *                           |    ******      **********      |
;*      42: |*   *                           |     *****      **********      |
;*      43: |*   *                           |     *****      **********      |
;*      44: |*   *                           |     *****      **********      |
;*      45: |*   *                           |    ******      **********      |
;*      46: |*   *                           |     *****      **********      |
;*      47: |*   *                           |     *****      **********      |
;*      48: |*   *                           |    ******      **********      |
;*      49: |*   *                           |     *****      **********      |
;*      50: |*   *                           |     *****      **********      |
;*      51: |*   *                           |     *****      **********      |
;*      52: |*   *                           |    ******      **********      |
;*      53: |*   *                           |     *****      **********      |
;*      54: |*   *                           |     *****      **********      |
;*      55: |*   *                           |    ******      **********      |
;*      56: |*   *                           |     ** **      **********      |
;*      57: |*   *                           |     ** **      **********      |
;*      58: |*   *                           |     ** **      **********      |
;*      59: |*   *                           |    *** **      **********      |
;*      60: |*   *                           |     ** **      **********      |
;*      61: |*   *                           |     ** **      **********      |
;*      62: |*   *                           |     ** **      **********      |
;*      63: |*   *                           |    *** **      **********      |
;*      64: |*   *                           |     *  **      **********      |
;*      65: |*   *                           |*       **      **********      |
;*      66: |*   *                           |    **  **      **********      |
;*      67: |*   *                           |        **      **********      |
;*      68: |*   *                           |        **      **********      |
;*      69: |*   *                           |        **      **********      |
;*      70: |*   *                           |    *   **      **********      |
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
;*      Total cycles (est.)         : 0 + min_trip_cnt * 71 = 710        
;*----------------------------------------------------------------------------*
;*       SETUP CODE
;*
;*                  SUB             A0,1,A0
;*
;*        SINGLE SCHEDULED ITERATION
;*
;*        $C$C97:
;*   0              LDW     .D2T2   *B8++,B4          ; |84|  ^ 
;*   1              NOP             4
;*   5              ABSSP   .S2     B4,B5             ; |84|  ^ 
;*   6              CMPGTSP .S2     B5,B23,B0         ; |84|  ^ 
;*   7      [ B0]   MV      .L2     B24,B4            ; |84|  ^ 
;*   8              MPYSP   .M2     B25,B4,B5         ; |84|  ^ 
;*   9              NOP             3
;*  12              SPINT   .L2     B5,B5             ; |84|  ^ 
;*  13              NOP             3
;*  16              INTSP   .L2     B5,B6             ; |84|  ^ 
;*  17              NOP             3
;*  20              MPYSP   .M2     B22,B6,B6         ; |84|  ^ 
;*  21              MPYSP   .M1X    A4,B6,A3          ; |84| 
;*  22              NOP             2
;*  24              FSUBSP  .L2     B4,B6,B4          ; |84|  ^ 
;*  25              NOP             2
;*  27              FSUBSP  .L2X    B4,A3,B6          ; |84|  ^ 
;*  28              NOP             2
;*  30              MPYSP   .M2     B6,B6,B7          ; |84|  ^ 
;*  31              NOP             3
;*  34              MPYSP   .M2     B16,B7,B4         ; |118|  ^ 
;*  35              NOP             3
;*  38              FADDSP  .L2     B17,B4,B4         ; |118|  ^ 
;*  39              NOP             2
;*  41              MPYSP   .M2     B7,B4,B4          ; |118|  ^ 
;*  42              NOP             3
;*  45              FADDSP  .L2     B18,B4,B4         ; |118|  ^ 
;*  46              NOP             2
;*  48              MPYSP   .M2     B7,B4,B4          ; |118|  ^ 
;*  49              NOP             3
;*  52              FADDSP  .L2     B19,B4,B4         ; |118|  ^ 
;*  53              NOP             2
;*  55              MPYSP   .M2     B7,B4,B4          ; |118|  ^ 
;*  56              NOP             3
;*  59              MPYSP   .M2     B6,B4,B4          ; |118|  ^ 
;*  60              NOP             3
;*  63              FADDSP  .L2     B4,B6,B4          ; |118|  ^ 
;*  64              AND     .L2     1,B5,B0           ; |118| 
;*  65      [ B0]   MV      .S2     B20,B5            ; |118| 
;*     ||   [!B0]   MV      .D2     B21,B5            ; |118| 
;*     ||   [ A0]   BDEC    .S1     $C$C97,A0         ; |117| 
;*  66              MPYSP   .M2     B5,B4,B4          ; |118|  ^ 
;*  67              NOP             3
;*  70              STW     .D2T2   B4,*B9++          ; |118|  ^ 
;*  71              ; BRANCHCC OCCURS {$C$C97}        ; |117| 
;*----------------------------------------------------------------------------*
$C$L4:    ; PIPED LOOP PROLOG
;** --------------------------------------------------------------------------*
$C$L5:    ; PIPED LOOP KERNEL
;          EXCLUSIVE CPU CYCLES: 71
           LDW     .D2T2   *B8++,B4          ; |84| <0,0>  ^ 
           NOP             4
           ABSSP   .S2     B4,B5             ; |84| <0,5>  ^ 
           CMPGTSP .S2     B5,B23,B0         ; |84| <0,6>  ^ 
   [ B0]   MV      .L2     B24,B4            ; |84| <0,7>  ^ 
           MPYSP   .M2     B25,B4,B5         ; |84| <0,8>  ^ 
           NOP             3
           SPINT   .L2     B5,B5             ; |84| <0,12>  ^ 
           NOP             3
           INTSP   .L2     B5,B6             ; |84| <0,16>  ^ 
           NOP             3
           MPYSP   .M2     B22,B6,B6         ; |84| <0,20>  ^ 
           MPYSP   .M1X    A4,B6,A3          ; |84| <0,21> 
           NOP             2
           FSUBSP  .L2     B4,B6,B4          ; |84| <0,24>  ^ 
           NOP             2
           FSUBSP  .L2X    B4,A3,B6          ; |84| <0,27>  ^ 
           NOP             2
           MPYSP   .M2     B6,B6,B7          ; |84| <0,30>  ^ 
           NOP             3
           MPYSP   .M2     B16,B7,B4         ; |118| <0,34>  ^ 
           NOP             3
           FADDSP  .L2     B17,B4,B4         ; |118| <0,38>  ^ 
           NOP             2
           MPYSP   .M2     B7,B4,B4          ; |118| <0,41>  ^ 
           NOP             3
           FADDSP  .L2     B18,B4,B4         ; |118| <0,45>  ^ 
           NOP             2
           MPYSP   .M2     B7,B4,B4          ; |118| <0,48>  ^ 
           NOP             3
           FADDSP  .L2     B19,B4,B4         ; |118| <0,52>  ^ 
           NOP             2
           MPYSP   .M2     B7,B4,B4          ; |118| <0,55>  ^ 
           NOP             3
           MPYSP   .M2     B6,B4,B4          ; |118| <0,59>  ^ 
           NOP             3
           FADDSP  .L2     B4,B6,B4          ; |118| <0,63>  ^ 
           AND     .L2     1,B5,B0           ; |118| <0,64> 

   [ B0]   MV      .S2     B20,B5            ; |118| <0,65> 
|| [!B0]   MV      .D2     B21,B5            ; |118| <0,65> 
|| [ A0]   BDEC    .S1     $C$L5,A0          ; |117| <0,65> 

           MPYSP   .M2     B5,B4,B4          ; |118| <0,66>  ^ 
           NOP             3
           STW     .D2T2   B4,*B9++          ; |118| <0,70>  ^ 
;** --------------------------------------------------------------------------*
$C$L6:    ; PIPED LOOP EPILOG
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 10

           STW     .D2T2   B20,*+SP(24)
||         MV      .L1X    B18,A12

           STW     .D2T2   B21,*+SP(20)
||         MV      .L1X    B17,A11

           STW     .D2T2   B24,*+SP(16)
||         MV      .L1X    B16,A10

           MV      .L2X    A4,B13
||         MV      .L1X    B19,A13
||         STW     .D2T2   B25,*+SP(12)
||         RINT                              ; interrupts on

           CALLP   .S2     _sinsp_v,B3
||         LDW     .D2T2   *+B11(16),B4      ; |120| 
||         MV      .L1     A14,A4            ; |120| 
||         MVK     .S1     0xa,A6            ; |120| 
||         MV      .D1X    B22,A15
||         MV      .L2     B23,B12

$C$RL4:    ; CALL OCCURS {_sinsp_v} {0}      ; |120| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 48
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           LDW     .D2T2   *+B11(4),B4       ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B11,A4           ; |66| 
||         MVK     .L1     0xa,A6            ; |66| 
||         MVK     .S1     0x1,A8            ; |66| 

$C$RL5:    ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     _fcn_pass,B6
           MVKL    .S2     0x3f1a36e2,B7
           MVKH    .S2     _fcn_pass,B6
           MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B6(4)        ; |66| 
||         MVKL    .S2     0xeb1c432d,B6

           LDW     .D2T2   *+B11(8),B4       ; |67| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B11,A4           ; |67| 
||         MVK     .L1     0xa,A6            ; |67| 
||         MVK     .S1     0x1,A8            ; |67| 

$C$RL6:    ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     _fcn_pass,B6
           MVKL    .S2     0x3f1a36e2,B7
           MVKH    .S2     _fcn_pass,B6
           MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B6(8)        ; |67| 
||         MVKL    .S2     0xeb1c432d,B6

           MV      .L2     B11,B5
||         LDW     .D2T2   *+B11(12),B4      ; |68| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B5,A4            ; |68| 
||         MVK     .L1     0xa,A6            ; |68| 
||         MVK     .S1     0x1,A8            ; |68| 

$C$RL7:    ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     _fcn_pass,B4
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     _fcn_pass,B4
           MVKL    .S2     _output,B31
           MVKH    .S2     0x3f1a36e2,B7

           ADD     .L1X    4,B10,A3
||         STW     .D2T1   A4,*+B4(12)       ; |68| 
||         MVKH    .S2     0xeb1c432d,B6

           MVKH    .S2     _output,B31
||         STW     .D2T1   A3,*+SP(40)

           CALLP   .S2     _isequal,B3
||         LDW     .D1T2   *A3,B4            ; |69| 
||         LDW     .D2T1   *B31,A4           ; |69| 
||         MVK     .L1     0xa,A6            ; |69| 
||         MVK     .S1     0x1,A8            ; |69| 

$C$RL8:    ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 14

           MVKL    .S1     _fcn_pass,A3
||         MVKL    .S2     _fcn_pass,B5

           MVKH    .S1     _fcn_pass,A3
||         MVKH    .S2     _fcn_pass,B5

           ADD     .L1     4,A3,A3
           LDW     .D1T1   *A3,A0            ; |72| 
           STW     .D2T1   A4,*+B5(16)       ; |69| 
           ADD     .L2     8,B5,B4
           STW     .D2T2   B4,*+SP(48)       ; |69| 
           STW     .D2T1   A3,*+SP(44)

   [!A0]   B       .S1     $C$L7             ; |72| 
|| [ A0]   LDW     .D2T2   *B4,B0            ; |72| 

   [!A0]   CALL    .S1     _print_test_results ; |124| 
   [!A0]   ZERO    .L1     A3                ; |72| 
   [ A0]   ADD     .L2     4,B4,B4
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
           CALL    .S1     _print_test_results ; |124| 
           MVKL    .S1     _all_pass,A4
           MVKH    .S1     _all_pass,A4
           NOP             2
;** --------------------------------------------------------------------------*
$C$L7:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A3,*A4            ; |72| 
||         MVK     .L1     0x1,A4            ; |124| 
||         ADDKPC  .S2     $C$RL9,B3,0       ; |124| 

$C$RL9:    ; CALL OCCURS {_print_test_results} {0}  ; |124| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 8
           LDW     .D2T2   *+SP(16),B4       ; |134| 
           LDW     .D2T1   *+SP(20),A4       ; |136| 

           LDW     .D2T1   *+SP(24),A5       ; |132| 
||         MVKL    .S2     _a_sc,B6
||         ZERO    .L1     A7
||         MVK     .L2     0x7,B30

           STW     .D2T2   B30,*+SP(28)      ; |134| 
||         MVKH    .S1     0xff800000,A7
||         MVKH    .S2     _a_sc,B6
||         ZERO    .L1     A6
||         MVK     .L2     -1,B7             ; |137| 

           STW     .D2T2   B7,*+B6(24)       ; |137| 
||         MV      .L2     B6,B31            ; |132| 
||         SET     .S1     A6,0x17,0x1e,A6
||         ZERO    .S2     B5

           STDW    .D2T1   A7:A6,*+B31(8)    ; |134| 
||         MVKH    .S2     0x80000000,B5

           STDW    .D2T2   B5:B4,*B6         ; |132| 

           STDW    .D2T1   A5:A4,*+B6(16)    ; |136| 
||         MV      .L1X    B6,A14            ; |132| 
||         ZERO    .L2     B11

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
;          EXCLUSIVE CPU CYCLES: 7
           LDW     .D1T1   *A14,A3           ; |139| 
           CALL    .S1     _sin              ; |139| 
           ADDKPC  .S2     $C$RL10,B3,2      ; |139| 
           SPDP    .S1     A3,A5:A4          ; |139| 
           NOP             1
$C$RL10:   ; CALL OCCURS {_sin} {0}          ; |139| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 30
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T2   *B4,B4            ; |139| 
           DPSP    .L1     A5:A4,A3          ; |139| 
           NOP             3
           ADD     .L2     B11,B4,B4         ; |139| 
           STW     .D2T1   A3,*B4            ; |139| 

           CALLP   .S2     _sinsp,B3
||         LDW     .D1T1   *A14,A4           ; |140| 

$C$RL11:   ; CALL OCCURS {_sinsp} {0}        ; |140| 
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T2   *+B4(4),B4        ; |140| 
           NOP             4
           ADD     .L2     B11,B4,B4         ; |140| 
           STW     .D2T1   A4,*B4            ; |140| 

           CALLP   .S2     _sinsp_c,B3
||         LDW     .D1T1   *A14,A4           ; |141| 

$C$RL12:   ; CALL OCCURS {_sinsp_c} {0}      ; |141| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 88
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T2   *+B4(8),B4        ; |141| 
           LDW     .D2T1   *+SP(12),A31      ; |72| 
           LDW     .D2T2   *+SP(28),B5       ; |142| 
           LDW     .D2T2   *B10,B31          ; |142| 
           NOP             1
           ADD     .L2     B11,B4,B4         ; |141| 
           STW     .D2T1   A4,*B4            ; |141| 
           LDW     .D1T1   *A14++,A3         ; |69| 
           SUB     .L2     B5,1,B30          ; |138| 
           SUB     .L1X    B5,1,A2           ; |138| 
           STW     .D2T2   B30,*+SP(28)      ; |142| 
           NOP             1
           ABSSP   .S1     A3,A4             ; |71| 
           NOP             1
           CMPGTSP .S2X    A4,B12,B0         ; |71| 
   [ B0]   LDW     .D2T1   *+SP(16),A3       ; |71| 
           NOP             4
           MPYSP   .M1     A31,A3,A4         ; |77| 
           NOP             3
           SPINT   .L1     A4,A5             ; |77| 
           NOP             3
           INTSP   .L1     A5,A4             ; |77| 
           AND     .L1     1,A5,A0           ; |80| 
           NOP             2
           MPYSP   .M1     A15,A4,A6         ; |142| 
           MPYSP   .M2X    B13,A4,B4         ; |142| 
           NOP             2
           FSUBSP  .L1     A3,A6,A3          ; |142| 
           NOP             2
           FSUBSP  .L1X    A3,B4,A4          ; |142| 
           ADD     .L2     B11,B31,B4        ; |142| 
           ADD     .L2     4,B11,B11         ; |138| 
           MPYSP   .M1     A4,A4,A3          ; |142| 
   [!A2]   MVKL    .S2     _output,B11
   [!A2]   MVKH    .S2     _output,B11
           NOP             1
           MPYSP   .M1     A10,A3,A6         ; |142| 
           NOP             3
           FADDSP  .L1     A11,A6,A6         ; |142| 
           NOP             2
           MPYSP   .M1     A3,A6,A6          ; |142| 
           NOP             3
           FADDSP  .L1     A12,A6,A6         ; |142| 
           NOP             2
           MPYSP   .M1     A3,A6,A6          ; |142| 
           NOP             3
           FADDSP  .L1     A13,A6,A6         ; |142| 
           NOP             2
           MPYSP   .M1     A3,A6,A3          ; |142| 
           NOP             3
           MPYSP   .M1     A4,A3,A3          ; |142| 
           NOP             3

           FADDSP  .L1     A3,A4,A4          ; |142| 
|| [ A0]   LDW     .D2T1   *+SP(24),A3       ; |142| 

   [!A0]   LDW     .D2T1   *+SP(20),A3       ; |80| 
           NOP             3
   [ A2]   B       .S1     $C$L8             ; |138| 
           MPYSP   .M1     A3,A4,A3          ; |142| 
           NOP             3

   [!A2]   CALL    .S1     _sinsp_v          ; |144| 
||         STW     .D2T1   A3,*B4            ; |142| 

           ; BRANCHCC OCCURS {$C$L8}         ; |138| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           LDW     .D2T2   *+B11(16),B4      ; |144| 
           MVKL    .S1     _a_sc,A4
           MVKH    .S1     _a_sc,A4
           MVK     .L1     0x7,A6            ; |144| 
           ADDKPC  .S2     $C$RL13,B3,0      ; |144| 
$C$RL13:   ; CALL OCCURS {_sinsp_v} {0}      ; |144| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 46
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           LDW     .D2T1   *B11,A4           ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T2   *+B11(4),B4       ; |66| 
||         MVK     .L1     0x7,A6            ; |66| 
||         MVK     .S1     0x1,A8            ; |66| 

$C$RL14:   ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     _fcn_pass,B5
           MVKL    .S2     0x3f1a36e2,B7
           MVKH    .S2     _fcn_pass,B5
           MVKL    .S2     0xeb1c432d,B6

           LDW     .D2T2   *+B11(8),B4       ; |67| 
||         MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B5(4)        ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B11,A4           ; |67| 
||         MVK     .L1     0x7,A6            ; |67| 
||         MVK     .S1     0x1,A8            ; |67| 

$C$RL15:   ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     _fcn_pass,B5
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     _fcn_pass,B5
           MVKH    .S2     0x3f1a36e2,B7

           MV      .L1X    B11,A3            ; |67| 
||         STW     .D2T1   A4,*+B5(8)        ; |67| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A3,A4            ; |68| 
||         LDW     .D2T2   *+B11(12),B4      ; |68| 
||         MVK     .L1     0x7,A6            ; |68| 
||         MVK     .S1     0x1,A8            ; |68| 

$C$RL16:   ; CALL OCCURS {_isequal} {0}      ; |68| 

           LDW     .D2T1   *+SP(40),A3       ; |68| 
||         MVKL    .S2     _fcn_pass,B4

           MVKL    .S2     0xeb1c432d,B6
           MVKL    .S2     0x3f1a36e2,B7
           MVKH    .S2     _fcn_pass,B4

           STW     .D2T1   A4,*+B4(12)       ; |68| 
||         MVKH    .S2     0xeb1c432d,B6

           MV      .L2     B11,B31           ; |69| 
||         STW     .D2T1   A3,*+SP(52)       ; |68| 
||         MVKH    .S2     0x3f1a36e2,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D1T2   *A3,B4            ; |69| 
||         LDW     .D2T1   *B31,A4           ; |69| 
||         MVK     .L1     0x7,A6            ; |69| 
||         MVK     .S1     0x1,A8            ; |69| 

$C$RL17:   ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 17
           LDW     .D2T1   *+SP(44),A3       ; |69| 
           MVKL    .S2     _fcn_pass,B31
           MVKH    .S2     _fcn_pass,B31
           STW     .D2T1   A4,*+B31(16)      ; |69| 
           NOP             1

           MV      .L2X    A3,B4             ; |69| 
||         STW     .D2T1   A3,*+SP(56)       ; |69| 

           LDW     .D2T2   *B4,B0            ; |72| 
           LDW     .D2T1   *+SP(48),A3
           NOP             3

   [!B0]   B       .S1     $C$L9             ; |72| 
|| [!B0]   ZERO    .L1     A4                ; |72| 
||         MV      .L2     B0,B1             ; guard predicate rewrite

   [!B0]   CALL    .S2     _print_test_results ; |148| 
|| [ B0]   MV      .L2X    A3,B4
||         STW     .D2T1   A3,*+SP(60)       ; |72| 
|| [!B0]   MVKL    .S1     _all_pass,A3

   [!B0]   MVKH    .S1     _all_pass,A3
|| [ B0]   LDW     .D2T2   *B4,B0            ; |72| 

   [ B1]   ADD     .L2     4,B4,B4
           NOP             2
           ; BRANCHCC OCCURS {$C$L9}         ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
           CMPEQ   .L1     A4,0,A3           ; |72| 
   [ B0]   LDW     .D2T2   *B4,B4            ; |72| 
           XOR     .L1     1,A3,A3           ; |72| 
   [!B0]   ZERO    .L1     A0                ; |72| 
           MVK     .L1     0x1,A4            ; |72| 
           NOP             1
   [ B0]   CMPEQ   .L2     B4,0,B4           ; |72| 
   [ B0]   XOR     .L2     1,B4,B4           ; |72| 
           NOP             1
   [ B0]   AND     .L1X    A3,B4,A0          ; |72| 
   [!A0]   ZERO    .L1     A4                ; |72| 
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
;          EXCLUSIVE CPU CYCLES: 15
           MVKL    .S2     0xc000cccc,B4
           MVKL    .S2     0xcccccccd,B31
           MVK     .S2     0x81,B29
           MVKL    .S2     _a_ext,B5
           MVKH    .S2     0xc000cccc,B4

           MVKH    .S2     0xcccccccd,B31
||         STW     .D2T2   B29,*+SP(28)      ; |151| 

           MVKH    .S2     _a_ext,B5
||         ZERO    .L1     A4
||         STW     .D2T2   B4,*+SP(68)       ; |157| 

           MV      .L2     B5,B30            ; |151| 
||         SET     .S1     A4,0x17,0x17,A4
||         STW     .D2T2   B31,*+SP(64)      ; |157| 

           MV      .L1X    B5,A14            ; |151| 
||         STW     .D2T1   A4,*B30           ; |151| 

           LDW     .D1T1   *A14,A3           ; |153| 
           CALL    .S1     _sin              ; |153| 
           ZERO    .L2     B11
           NOP             2
           SPDP    .S1     A3,A5:A4          ; |153| 
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
;          EXCLUSIVE CPU CYCLES: 1
           ADDKPC  .S2     $C$RL19,B3,0      ; |153| 
$C$RL19:   ; CALL OCCURS {_sin} {0}          ; |153| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 30
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T1   *B4,A3            ; |153| 
           DPSP    .L1     A5:A4,A4          ; |153| 
           NOP             3
           ADD     .L1X    B11,A3,A3         ; |153| 
           STW     .D1T1   A4,*A3            ; |153| 

           CALLP   .S2     _sinsp,B3
||         LDW     .D1T1   *A14,A4           ; |154| 

$C$RL20:   ; CALL OCCURS {_sinsp} {0}        ; |154| 
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T2   *+B4(4),B4        ; |154| 
           NOP             4
           ADD     .L2     B11,B4,B4         ; |154| 
           STW     .D2T1   A4,*B4            ; |154| 

           CALLP   .S2     _sinsp_c,B3
||         LDW     .D1T1   *A14,A4           ; |155| 

$C$RL21:   ; CALL OCCURS {_sinsp_c} {0}      ; |155| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 113
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T2   *+B4(8),B4        ; |155| 
           LDW     .D2T1   *+SP(12),A31      ; |72| 
           LDW     .D2T2   *B10,B31          ; |156| 
           NOP             2
           ADD     .L2     B11,B4,B4         ; |155| 
           STW     .D2T1   A4,*B4            ; |155| 
           LDW     .D1T1   *A14,A4           ; |69| 
           NOP             4
           ABSSP   .S1     A4,A3             ; |71| 
           NOP             1
           CMPGTSP .S2X    A3,B12,B0         ; |71| 
   [ B0]   LDW     .D2T1   *+SP(16),A4       ; |71| 
           NOP             4
           MPYSP   .M1     A31,A4,A3         ; |77| 
           NOP             3
           SPINT   .L1     A3,A5             ; |77| 
           NOP             3
           INTSP   .L1     A5,A3             ; |77| 
           AND     .L1     1,A5,A0           ; |80| 
           NOP             2
           MPYSP   .M1     A15,A3,A6         ; |156| 
           MPYSP   .M2X    B13,A3,B4         ; |156| 
           NOP             2
           FSUBSP  .L1     A4,A6,A3          ; |156| 
           NOP             2
           FSUBSP  .L1X    A3,B4,A3          ; |156| 
           ADD     .L2     B11,B31,B4        ; |156| 
           ADD     .L2     4,B11,B11         ; |152| 
           MPYSP   .M1     A3,A3,A4          ; |156| 
           NOP             3
           MPYSP   .M1     A10,A4,A6         ; |156| 
           NOP             3
           FADDSP  .L1     A11,A6,A6         ; |156| 
           NOP             2
           MPYSP   .M1     A4,A6,A6          ; |156| 
           NOP             3
           FADDSP  .L1     A12,A6,A6         ; |156| 
           NOP             2
           MPYSP   .M1     A4,A6,A6          ; |156| 
           NOP             3
           FADDSP  .L1     A13,A6,A6         ; |156| 
           NOP             2
           MPYSP   .M1     A4,A6,A4          ; |156| 
           NOP             3
           MPYSP   .M1     A3,A4,A4          ; |156| 
           NOP             3

           FADDSP  .L1     A4,A3,A4          ; |156| 
|| [ A0]   LDW     .D2T1   *+SP(24),A3       ; |156| 

   [!A0]   LDW     .D2T1   *+SP(20),A3       ; |80| 
           NOP             4
           MPYSP   .M1     A3,A4,A3          ; |156| 
           NOP             3
           STW     .D2T1   A3,*B4            ; |156| 
           LDW     .D1T2   *A14,B6           ; |157| 
           LDDW    .D2T2   *+SP(64),B5:B4
           NOP             4
           MPYSPDP .M2     B6,B5:B4,B5:B4    ; |157| 
           NOP             6

           DPSP    .L2     B5:B4,B5          ; |157| 
||         LDW     .D2T2   *+SP(28),B4

           NOP             3
           STW     .D1T2   B5,*++A14         ; |157| 

           SUB     .L1X    B4,1,A0           ; |152| 
||         SUB     .L2     B4,1,B4           ; |152| 

   [ A0]   B       .S1     $C$L10            ; |152| 
|| [ A0]   LDW     .D1T1   *A14,A3           ; |153| 
|| [!A0]   MVKL    .S2     _output,B10
||         STW     .D2T2   B4,*+SP(28)       ; |157| 

   [ A0]   CALL    .S1     _sin              ; |153| 
   [!A0]   MVKH    .S2     _output,B10
   [!A0]   LDW     .D2T2   *+B10(16),B4      ; |159| 
   [!A0]   CALL    .S1     _sinsp_v          ; |159| 
           SPDP    .S1     A3,A5:A4          ; |153| 
           ; BRANCHCC OCCURS {$C$L10}        ; |152| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 4
           ADDKPC  .S2     $C$RL22,B3,0      ; |159| 
           MVKL    .S1     _a_ext,A4
           MVKH    .S1     _a_ext,A4
           MVK     .S1     0x81,A6           ; |159| 
$C$RL22:   ; CALL OCCURS {_sinsp_v} {0}      ; |159| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 43
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           LDW     .D2T1   *B10,A4           ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T2   *+B10(4),B4       ; |66| 
||         MVK     .S1     0x81,A6           ; |66| 
||         MVK     .L1     0x1,A8            ; |66| 

$C$RL23:   ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     _fcn_pass,B11
           MVKL    .S2     0x3f1a36e2,B7
           MVKH    .S2     _fcn_pass,B11
           MVKL    .S2     0xeb1c432d,B6

           LDW     .D2T2   *+B10(8),B4       ; |67| 
||         MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B11(4)       ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B10,A4           ; |67| 
||         MVK     .S1     0x81,A6           ; |67| 
||         MVK     .L1     0x1,A8            ; |67| 

$C$RL24:   ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           MV      .L1X    B10,A3            ; |67| 
||         STW     .D2T1   A4,*+B11(8)       ; |67| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A3,A4            ; |68| 
||         LDW     .D2T2   *+B10(12),B4      ; |68| 
||         MVK     .S1     0x81,A6           ; |68| 
||         MVK     .L1     0x1,A8            ; |68| 

$C$RL25:   ; CALL OCCURS {_isequal} {0}      ; |68| 
           LDW     .D2T1   *+SP(40),A3       ; |68| 
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           MV      .L2     B10,B4            ; |68| 
||         STW     .D2T1   A4,*+B11(12)      ; |68| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B4,A4            ; |69| 
||         LDW     .D1T2   *A3,B4            ; |69| 
||         MVK     .S1     0x81,A6           ; |69| 
||         MVK     .L1     0x1,A8            ; |69| 

$C$RL26:   ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           LDW     .D2T1   *+SP(44),A3       ; |69| 
           LDW     .D2T2   *+SP(48),B4
           STW     .D2T1   A4,*+B11(16)      ; |69| 
           NOP             2
           LDW     .D1T1   *A3,A0            ; |72| 
           NOP             4

   [!A0]   B       .S1     $C$L11            ; |72| 
|| [ A0]   LDW     .D2T2   *B4,B0            ; |72| 

   [!A0]   CALL    .S1     _print_test_results ; |163| 
   [ A0]   ADD     .L2     4,B4,B4
   [!A0]   ZERO    .L1     A3                ; |72| 
   [!A0]   MVKL    .S1     _all_pass,A4
   [!A0]   MVKH    .S1     _all_pass,A4
           ; BRANCHCC OCCURS {$C$L11}        ; |72| 
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
           CALL    .S1     _print_test_results ; |163| 
           MVKL    .S1     _all_pass,A4
           MVKH    .S1     _all_pass,A4
           NOP             2
;** --------------------------------------------------------------------------*
$C$L11:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A3,*A4            ; |72| 
||         MVK     .L1     0x4,A4            ; |163| 
||         ADDKPC  .S2     $C$RL27,B3,0      ; |163| 

$C$RL27:   ; CALL OCCURS {_print_test_results} {0}  ; |163| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 20
           ZERO    .L2     B7                ; |114| 

           ZERO    .L2     B6                ; |114| 
||         STW     .D2T2   B7,*+SP(64)       ; |114| 

           STW     .D2T2   B6,*+SP(68)       ; |114| 
           LDDW    .D2T2   *+SP(64),B7:B6    ; |114| 
           MVKL    .S2     _cycle_counts+24,B5
           MVKH    .S2     _cycle_counts+24,B5
           ZERO    .L2     B4                ; |114| 
           NOP             1

           STDW    .D2T2   B7:B6,*B5         ; |114| 
||         MVC     .S2     B4,TSCL           ; |115| 

           MVC     .S2     TSCL,B6           ; |116| 
           MVC     .S2     TSCH,B7           ; |116| 
           MVKL    .S1     _t_start,A3
           MVKH    .S1     _t_start,A3

           STDW    .D1T2   B7:B6,*A3         ; |116| 
||         MVC     .S2     TSCL,B4           ; |117| 

           MVC     .S2     TSCH,B8           ; |117| 

           ZERO    .L1     A4
||         CALL    .S2     _gimme_random     ; |173| 
||         MVKL    .S1     _t_offset,A3

           MVKH    .S1     0xc2c80000,A4
||         SUBU    .L2     B4,B6,B5:B4       ; |117| 
||         ZERO    .L1     A14
||         MVK     .S2     0x80,B30

           EXT     .S2     B5,24,24,B6       ; |117| 
||         SUB     .L2     B8,B7,B31         ; |117| 
||         MVKH    .S1     0x43480000,A14

           MV      .L2X    A4,B11
||         ADD     .S2     B31,B6,B5         ; |117| 
||         MVKH    .S1     _t_offset,A3
||         STW     .D2T2   B30,*+SP(28)      ; |117| 

           STDW    .D1T2   B5:B4,*A3         ; |117| 
||         MV      .L2     B11,B4            ; |173| 
||         MV      .L1     A14,A4            ; |173| 
||         ZERO    .S2     B10

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
;          EXCLUSIVE CPU CYCLES: 1
           ADDKPC  .S2     $C$RL28,B3,0      ; |173| 
$C$RL28:   ; CALL OCCURS {_gimme_random} {0}  ; |173| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 107
           MVKL    .S1     _input,A3
           MVKH    .S1     _input,A3
           LDW     .D1T1   *A3,A3            ; |173| 
           NOP             4
           ADD     .L1X    B10,A3,A3         ; |173| 

           STW     .D1T1   A4,*A3            ; |173| 
||         MVC     .S2     TSCL,B6           ; |122| 

           MVC     .S2     TSCH,B7           ; |122| 

           ABSSP   .S2X    A4,B4             ; |71| 
||         LDW     .D2T1   *+SP(12),A3       ; |72| 
||         MVKL    .S1     _t_start,A31

           CMPGTSP .S2     B4,B12,B0         ; |71| 
||         MVKH    .S1     _t_start,A31

   [ B0]   LDW     .D2T1   *+SP(16),A4       ; |71| 
||         MVKL    .S2     _output+12,B31
||         STDW    .D1T2   B7:B6,*A31

           MVKH    .S2     _output+12,B31
           NOP             3
           MPYSP   .M1     A3,A4,A3          ; |77| 
           NOP             3
           SPINT   .L1     A3,A5             ; |77| 
           NOP             3

           INTSP   .L1     A5,A3             ; |77| 
||         AND     .S1     1,A5,A0           ; |80| 

           NOP             3
           MPYSP   .M1     A15,A3,A6         ; |177| 
           MPYSP   .M2X    B13,A3,B4         ; |177| 
           NOP             2
           FSUBSP  .L1     A4,A6,A3          ; |177| 
           NOP             2

           FSUBSP  .L1X    A3,B4,A4          ; |177| 
||         LDW     .D2T2   *B31,B4           ; |177| 

           NOP             2
           MPYSP   .M1     A4,A4,A3          ; |177| 
           NOP             1
           ADD     .L2     B10,B4,B4         ; |177| 
           NOP             1
           MPYSP   .M1     A10,A3,A6         ; |177| 
           NOP             3
           FADDSP  .L1     A11,A6,A6         ; |177| 
           NOP             2
           MPYSP   .M1     A3,A6,A6          ; |177| 
           NOP             3
           FADDSP  .L1     A12,A6,A6         ; |177| 
           NOP             2
           MPYSP   .M1     A3,A6,A6          ; |177| 
           NOP             3
           FADDSP  .L1     A13,A6,A6         ; |177| 
           NOP             2
           MPYSP   .M1     A3,A6,A3          ; |177| 
           NOP             3
           MPYSP   .M1     A4,A3,A3          ; |177| 
           NOP             3

           FADDSP  .L1     A3,A4,A4          ; |177| 
|| [ A0]   LDW     .D2T1   *+SP(24),A3       ; |177| 

   [!A0]   LDW     .D2T1   *+SP(20),A3       ; |80| 
           NOP             4
           MPYSP   .M1     A3,A4,A4          ; |177| 
           NOP             3

           STW     .D2T1   A4,*B4            ; |177| 
||         MVC     .S2     TSCL,B8           ; |128| 

           MVC     .S2     TSCH,B9           ; |128| 

           MVKL    .S1     _t_offset,A3
||         MVKL    .S2     _t_stop,B29
||         ADD     .L2     4,B10,B10         ; |171| 

           MVKH    .S1     _t_offset,A3
||         MVKH    .S2     _t_stop,B29

           LDDW    .D1T1   *A3,A5:A4         ; |130| 
||         MVKL    .S1     _cycle_counts+24,A3
||         STDW    .D2T2   B9:B8,*B29        ; |128| 

           MVKH    .S1     _cycle_counts+24,A3
           LDDW    .D1T1   *A3,A7:A6         ; |130| 
           NOP             2
           ADDU    .L2X    A4,B6,B5:B4       ; |130| 

           ADD     .S2X    B5,A5,B6          ; |130| 
||         SUBU    .L2     B8,B4,B5:B4       ; |130| 

           ADD     .D2     B6,B7,B6          ; |130| 
||         EXT     .S2     B5,24,24,B7       ; |130| 
||         ADDU    .L2X    A6,B4,B5:B4       ; |130| 

           SUB     .L2     B9,B6,B6          ; |130| 
||         ADD     .S2X    B5,A7,B30         ; |130| 

           ADD     .L2     B6,B7,B7          ; |130| 
||         LDW     .D2T2   *+SP(28),B6       ; |130| 

           ADD     .L2     B30,B7,B5         ; |130| 
           STDW    .D1T2   B5:B4,*A3         ; |130| 
           NOP             2

           SUB     .L1X    B6,1,A0           ; |171| 
||         SUB     .L2     B6,1,B4           ; |171| 

           STW     .D2T2   B4,*+SP(28)       ; |130| 
|| [ A0]   MV      .L1     A14,A4            ; |173| 
|| [ A0]   MV      .L2     B11,B4            ; |173| 
|| [ A0]   B       .S1     $C$L12            ; |171| 
|| [!A0]   LDDW    .D1T1   *A3,A5:A4         ; |135| 

   [ A0]   CALL    .S1     _gimme_random     ; |173| 
   [!A0]   CALL    .S1     __fltllif         ; |135| 
           NOP             3
           ; BRANCHCC OCCURS {$C$L12}        ; |171| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 2
           ADDKPC  .S2     $C$RL29,B3,1      ; |135| 
$C$RL29:   ; CALL OCCURS {__fltllif} {0}     ; |135| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 15
           ZERO    .L1     A14
           SET     .S1     A14,0x1a,0x1d,A14
           MPYSP   .M1     A14,A4,A3         ; |135| 
           ZERO    .L1     A11
           SET     .S1     A11,0x15,0x1d,A11
           ZERO    .L1     A10               ; |135| 
           SPDP    .S1     A3,A5:A4          ; |135| 
           NOP             2

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A11:A10,A5:A4,A5:A4 ; |135| 

$C$RL30:   ; CALL OCCURS {__fixdlli} {0}     ; |135| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 19
           MVKL    .S1     _cycle_counts+24,A3

           LDDW    .D2T2   *+SP(64),B5:B4
||         MVKH    .S1     _cycle_counts+24,A3

           MV      .L2X    A10,B31           ; |114| 
           MV      .L2X    A3,B6
           STDW    .D2T1   A5:A4,*B6         ; |135| 
           SUBAW   .D2     B6,6,B10

           STDW    .D2T2   B5:B4,*B10        ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B6           ; |116| 
           MVC     .S2     TSCH,B7           ; |116| 
           MVKL    .S1     _t_start,A10
           MVKH    .S1     _t_start,A10

           STDW    .D1T2   B7:B6,*A10        ; |116| 
||         MVC     .S2     TSCL,B4           ; |117| 

           MVC     .S2     TSCH,B8           ; |117| 

           SUBU    .L2     B4,B6,B5:B4       ; |117| 
||         MVKL    .S2     _t_offset,B11

           EXT     .S2     B5,24,24,B6       ; |117| 
||         SUB     .L2     B8,B7,B30         ; |117| 

           ADD     .L2     B30,B6,B5         ; |117| 
||         MVKH    .S2     _t_offset,B11

           STDW    .D2T2   B5:B4,*B11        ; |117| 
||         MVK     .S2     0x80,B4

           MVKL    .S1     _input,A15

           MV      .L1X    B4,A13            ; |117| 
||         MVKH    .S1     _input,A15
||         ZERO    .D1     A12

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L13:    
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D1T1   *A15,A3           ; |185| 
           NOP             4

           ADD     .L1     A12,A3,A3         ; |185| 
||         CALL    .S1     _sinf             ; |185| 

           LDW     .D1T1   *A3,A4            ; |185| 
           ADDKPC  .S2     $C$RL31,B3,3      ; |185| 
$C$RL31:   ; CALL OCCURS {_sinf} {0}         ; |185| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 9

           MVKL    .S2     _output,B4
||         SUB     .L1     A13,1,A0          ; |184| 
||         SUB     .S1     A13,1,A13         ; |184| 

           MVKH    .S2     _output,B4
           LDW     .D2T2   *B4,B4            ; |185| 
   [ A0]   BNOP    .S1     $C$L13,3          ; |184| 

           ADD     .L1X    A12,B4,A3         ; |185| 
||         ADD     .S1     4,A12,A12         ; |184| 

           STW     .D1T1   A4,*A3            ; |185| 
           ; BRANCHCC OCCURS {$C$L13}        ; |184| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 18
           MVC     .S2     TSCL,B6           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           LDDW    .D1T1   *A10,A7:A6        ; |130| 
||         LDDW    .D2T2   *B11,B9:B8        ; |130| 

           MVKL    .S2     _t_stop,B12
           MV      .L2     B4,B7             ; |128| 
           MVKH    .S2     _t_stop,B12
           NOP             1
           ADDU    .L1X    B8,A6,A5:A4       ; |130| 

           ADD     .L1X    A5,B9,A3          ; |130| 
||         LDDW    .D2T2   *B10,B9:B8        ; |130| 

           ADD     .L1     A3,A7,A3          ; |130| 
||         SUBU    .L2X    B6,A4,B5:B4       ; |130| 

           EXT     .S2     B5,24,24,B16      ; |130| 
           SUB     .L2X    B7,A3,B31         ; |130| 
           CALL    .S1     __fltllif         ; |130| 

           ADDU    .L2     B8,B4,B5:B4       ; |130| 
||         ADD     .S2     B31,B16,B8        ; |130| 

           ADD     .L2     B5,B9,B5          ; |130| 
           ADD     .L2     B5,B8,B5          ; |130| 
           STDW    .D2T2   B7:B6,*B12        ; |128| 

           DADD    .L1X    0,B5:B4,A5:A4     ; |130| 
||         ADDKPC  .S2     $C$RL32,B3,0      ; |130| 

$C$RL32:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           MPYSP   .M1     A14,A4,A3         ; |130| 
           ZERO    .L1     A12               ; |130| 
           MV      .L1     A11,A13           ; |130| 
           NOP             1
           SPDP    .S1     A3,A5:A4          ; |130| 
           NOP             1

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A13:A12,A5:A4,A5:A4 ; |130| 

$C$RL33:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 17
           LDDW    .D2T2   *+SP(64),B5:B4    ; |130| 
           MV      .L2X    A12,B31           ; |114| 
           STDW    .D2T1   A5:A4,*B10        ; |130| 
           NOP             2

           STDW    .D2T2   B5:B4,*+B10(8)    ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B6           ; |116| 
           MVC     .S2     TSCH,B4           ; |116| 
           MV      .L2     B4,B7             ; |116| 

           STDW    .D1T2   B7:B6,*A10        ; |116| 
||         MVC     .S2     TSCL,B4           ; |117| 

           MVC     .S2     TSCH,B8           ; |117| 
           SUBU    .L2     B4,B6,B5:B4       ; |117| 

           EXT     .S2     B5,24,24,B6       ; |117| 
||         SUB     .L2     B8,B7,B30         ; |117| 

           ADD     .L2     B30,B6,B5         ; |117| 

           STDW    .D2T2   B5:B4,*B11        ; |117| 
||         MVK     .S2     0x80,B4

           LDW     .D2T2   *+SP(32),B13      ; |117| 
           MV      .L1X    B4,A11            ; |117| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L14:    
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D1T1   *A15,A3           ; |193| 
           NOP             4

           ADD     .L1     A12,A3,A3         ; |193| 
||         CALL    .S1     _sinsp            ; |193| 

           LDW     .D1T1   *A3,A4            ; |193| 
           ADDKPC  .S2     $C$RL34,B3,3      ; |193| 
$C$RL34:   ; CALL OCCURS {_sinsp} {0}        ; |193| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7

           LDW     .D2T2   *B13,B4           ; |193| 
||         SUB     .L1     A11,1,A0          ; |192| 
||         SUB     .S1     A11,1,A11         ; |192| 

   [ A0]   BNOP    .S1     $C$L14,3          ; |192| 

           ADD     .L1X    A12,B4,A3         ; |193| 
||         ADD     .S1     4,A12,A12         ; |192| 

           STW     .D1T1   A4,*A3            ; |193| 
           ; BRANCHCC OCCURS {$C$L14}        ; |192| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           MVC     .S2     TSCL,B8           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           LDDW    .D1T1   *A10,A5:A4        ; |130| 
||         LDDW    .D2T2   *B11,B7:B6        ; |130| 

           LDDW    .D2T2   *+B10(8),B17:B16  ; |130| 
           MV      .L2     B4,B9             ; |128| 
           NOP             2
           ADDU    .L1X    B6,A4,A7:A6       ; |130| 
           ADD     .L1X    A7,B7,A3          ; |130| 

           ADD     .L1     A3,A5,A3          ; |130| 
||         SUBU    .L2X    B8,A6,B5:B4       ; |130| 

           CALL    .S1     __fltllif         ; |130| 
||         ADDU    .L2     B16,B4,B7:B6      ; |130| 
||         EXT     .S2     B5,24,24,B5       ; |130| 

           SUB     .L2X    B9,A3,B4          ; |130| 
||         ADD     .S2     B7,B17,B31        ; |130| 

           ADD     .L2     B4,B5,B5          ; |130| 
||         MV      .S2     B12,B4            ; |130| 

           ADD     .L2     B31,B5,B7         ; |130| 
           STDW    .D2T2   B9:B8,*B4         ; |128| 

           DADD    .L1X    0,B7:B6,A5:A4     ; |130| 
||         ADDKPC  .S2     $C$RL35,B3,0      ; |130| 

$C$RL35:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           MPYSP   .M1     A14,A4,A3         ; |130| 
           ZERO    .L1     A12               ; |130| 
           NOP             2
           SPDP    .S1     A3,A5:A4          ; |130| 
           NOP             1

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A13:A12,A5:A4,A5:A4 ; |130| 

$C$RL36:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 17
           LDDW    .D2T2   *+SP(64),B5:B4
           ADDAD   .D2     B10,1,B12
           MV      .L2X    A12,B31           ; |114| 
           STDW    .D2T1   A5:A4,*B12        ; |130| 
           NOP             1

           STDW    .D2T2   B5:B4,*+B10(16)   ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B8           ; |116| 
           MVC     .S2     TSCH,B4           ; |116| 
           MV      .L2     B4,B9             ; |116| 

           STDW    .D1T2   B9:B8,*A10        ; |116| 
||         MVC     .S2     TSCL,B4           ; |117| 

           MVC     .S2     TSCH,B6           ; |117| 
           SUBU    .L2     B4,B8,B5:B4       ; |117| 

           EXT     .S2     B5,24,24,B6       ; |117| 
||         SUB     .L2     B6,B9,B5          ; |117| 

           ADD     .L2     B5,B6,B5          ; |117| 

           STDW    .D2T2   B5:B4,*B11        ; |117| 
||         MVK     .S2     0x80,B4

           LDW     .D2T2   *+SP(36),B13      ; |117| 
           MV      .L1X    B4,A11            ; |117| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L15:    
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D1T1   *A15,A3           ; |201| 
           NOP             4

           ADD     .L1     A12,A3,A3         ; |201| 
||         CALL    .S1     _sinsp_c          ; |201| 

           LDW     .D1T1   *A3,A4            ; |201| 
           ADDKPC  .S2     $C$RL37,B3,3      ; |201| 
$C$RL37:   ; CALL OCCURS {_sinsp_c} {0}      ; |201| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7

           LDW     .D2T2   *B13,B4           ; |201| 
||         SUB     .L1     A11,1,A0          ; |200| 
||         SUB     .S1     A11,1,A11         ; |200| 

   [ A0]   BNOP    .S1     $C$L15,3          ; |200| 

           ADD     .L1X    A12,B4,A3         ; |201| 
||         ADD     .S1     4,A12,A12         ; |200| 

           STW     .D1T1   A4,*A3            ; |201| 
           ; BRANCHCC OCCURS {$C$L15}        ; |200| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 15
           MVC     .S2     TSCL,B8           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           LDDW    .D1T1   *A10,A5:A4        ; |130| 
||         LDDW    .D2T2   *B11,B7:B6        ; |130| 

           LDDW    .D2T2   *+B10(16),B17:B16 ; |130| 
           MV      .L2     B4,B9             ; |128| 
           NOP             2
           ADDU    .L2X    B6,A4,B5:B4       ; |130| 

           ADD     .S2     B5,B7,B6          ; |130| 
||         SUBU    .L2     B8,B4,B5:B4       ; |130| 

           ADD     .L2X    B6,A5,B6          ; |130| 
||         CALL    .S1     __fltllif         ; |130| 
||         EXT     .S2     B5,24,24,B5       ; |130| 

           SUB     .S2     B9,B6,B4          ; |130| 
||         ADDU    .L2     B16,B4,B7:B6      ; |130| 

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
;          EXCLUSIVE CPU CYCLES: 108
           MPYSP   .M1     A14,A4,A3         ; |130| 
           ZERO    .L1     A12               ; |130| 
           NOP             3
           SPDP    .S2X    A3,B5:B4          ; |130| 
           NOP             2

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1X    A13:A12,B5:B4,A5:A4 ; |130| 

$C$RL39:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
           LDDW    .D2T2   *+SP(64),B5:B4
           ADDAD   .D2     B12,1,B13
           MV      .L2X    A12,B31           ; |114| 
           STDW    .D2T1   A5:A4,*B13        ; |130| 
           NOP             1

           STDW    .D2T2   B5:B4,*+B10(32)   ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B4           ; |116| 
           MVC     .S2     TSCH,B5           ; |116| 

           STDW    .D1T2   B5:B4,*A10        ; |116| 
||         MVC     .S2     TSCL,B6           ; |117| 

           MVC     .S2     TSCH,B8           ; |117| 
           SUBU    .L2     B6,B4,B7:B6       ; |117| 

           SUB     .L2     B8,B5,B4          ; |117| 
||         EXT     .S2     B7,24,24,B5       ; |117| 

           MVKL    .S1     _output,A11
||         ADD     .L2     B4,B5,B7          ; |117| 

           MVKH    .S1     _output,A11
||         MV      .L2X    A15,B9            ; |208| 
||         STDW    .D2T2   B7:B6,*B11        ; |117| 

           CALLP   .S2     _sinsp_v,B3
||         LDW     .D1T2   *+A11(16),B4      ; |208| 
||         LDW     .D2T1   *B9,A4            ; |208| 
||         MVK     .S1     0x80,A6           ; |208| 

$C$RL40:   ; CALL OCCURS {_sinsp_v} {0}      ; |208| 
           MVC     .S2     TSCL,B8           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           LDDW    .D1T1   *A10,A7:A6        ; |130| 
||         LDDW    .D2T2   *B11,B7:B6        ; |130| 

           MVK     .S1     16,A10
           ADD     .L1X    B13,A10,A10
           LDDW    .D1T1   *A10,A9:A8        ; |130| 
           MVKL    .S1     _t_stop,A3
           ADDU    .L1X    B6,A6,A5:A4       ; |130| 

           MVKH    .S1     _t_stop,A3
||         SUBU    .L1X    B8,A4,A17:A16     ; |130| 

           ADD     .L1X    A5,B7,A4          ; |130| 
||         MV      .L2     B4,B9             ; |128| 

           ADD     .L1     A4,A7,A4          ; |130| 
||         MV      .L2X    A3,B4             ; |128| 
||         MV      .D1     A16,A3            ; |130| 
||         EXT     .S1     A17,24,24,A6      ; |130| 

           SUB     .S1X    B9,A4,A3          ; |130| 
||         ADDU    .L1     A8,A3,A5:A4       ; |130| 

           ADD     .L1     A5,A9,A3          ; |130| 
||         ADD     .S1     A3,A6,A5          ; |130| 

           CALLP   .S2     __fltllif,B3
||         ADD     .L1     A3,A5,A5          ; |130| 
||         STDW    .D2T2   B9:B8,*B4         ; |128| 

$C$RL41:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
           MPYSP   .M1     A14,A4,A3         ; |130| 
           NOP             3
           SPDP    .S1     A3,A5:A4          ; |130| 
           NOP             1

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A13:A12,A5:A4,A5:A4 ; |130| 

$C$RL42:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6

           STDW    .D1T1   A5:A4,*A10        ; |130| 
||         MVKH    .S2     0x3f1a36e2,B7

           LDW     .D1T2   *+A11(4),B4       ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A11,A4           ; |66| 
||         MVK     .S1     0x80,A6           ; |66| 
||         MVK     .L1     0x1,A8            ; |66| 

$C$RL43:   ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKL    .S2     _fcn_pass,B10
           MVKH    .S2     0x3f1a36e2,B7
           MVKH    .S2     0xeb1c432d,B6

           MV      .L1     A4,A3             ; |66| 
||         LDW     .D1T1   *A11,A4           ; |67| 
||         MVKH    .S2     _fcn_pass,B10

           CALLP   .S2     _isequal,B3
||         LDW     .D1T2   *+A11(8),B4       ; |67| 
||         STW     .D2T1   A3,*+B10(4)       ; |66| 
||         MVK     .S1     0x80,A6           ; |67| 
||         MVK     .L1     0x1,A8            ; |67| 

$C$RL44:   ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B10(8)       ; |67| 
||         LDW     .D1T2   *+A11(12),B4      ; |68| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A11,A4           ; |68| 
||         MVK     .S1     0x80,A6           ; |68| 
||         MVK     .L1     0x1,A8            ; |68| 

$C$RL45:   ; CALL OCCURS {_isequal} {0}      ; |68| 
           LDW     .D2T1   *+SP(52),A3       ; |68| 
           MVKL    .S2     0xeb1c432d,B6
           MVKL    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B10(12)      ; |68| 
||         MVKH    .S2     0xeb1c432d,B6

           LDW     .D1T1   *A11,A4           ; |69| 
||         MVKH    .S2     0x3f1a36e2,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D1T2   *A3,B4            ; |69| 
||         MVK     .S1     0x80,A6           ; |69| 
||         MVK     .L1     0x1,A8            ; |69| 

$C$RL46:   ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           LDW     .D2T2   *+SP(56),B4       ; |69| 
           STW     .D2T1   A4,*+B10(16)      ; |69| 
           NOP             3
           LDW     .D2T2   *B4,B0            ; |72| 
           LDW     .D2T2   *+SP(60),B4
           NOP             3

   [!B0]   B       .S1     $C$L16            ; |72| 
||         MV      .L2     B0,B1             ; guard predicate rewrite

   [ B0]   LDW     .D2T2   *B4,B0            ; |72| 
   [ B1]   ADD     .L2     4,B4,B4
           NOP             3
           ; BRANCHCC OCCURS {$C$L16}        ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 15
   [ B0]   LDW     .D2T2   *B4,B4            ; |72| 
   [!B0]   MV      .L1     A12,A0            ; |72| 
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
           MV      .L1     A12,A3
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
$C$SL1:	.string	"sinSP",0
$C$SL2:	.string	"----------------------------------------",0
$C$SL3:	.string	"----------------------------------------",10,0
;*****************************************************************************
;* UNDEFINED EXTERNAL REFERENCES                                             *
;*****************************************************************************
	.global	_printf
	.global	_sin
	.global	_sinf
	.global	_driver_init
	.global	_print_profile_results
	.global	_print_memory_results
	.global	_print_test_results
	.global	_gimme_random
	.global	_isequal
	.global	_sinsp
	.global	_sinsp_c
	.global	_sinsp_v
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
	.global	__fltllif
	.global	__fixdlli

;******************************************************************************
;* BUILD ATTRIBUTES                                                           *
;******************************************************************************
	.battr "TI", Tag_File, 1, Tag_ABI_stack_align_needed(0)
	.battr "TI", Tag_File, 1, Tag_ABI_stack_align_preserved(0)
	.battr "TI", Tag_File, 1, Tag_Tramps_Use_SOC(1)
