;******************************************************************************
;* TMS320C6x C/C++ Codegen                                          PC v7.4.2 *
;* Date/Time created: Tue May 17 03:15:34 2016                                *
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
	.word	0c0c88f0bh		; _a[0] @ 0
	.word	0c10f4415h		; _a[1] @ 32
	.word	041103c61h		; _a[2] @ 64
	.word	04105432ah		; _a[3] @ 96
	.word	03fdf7976h		; _a[4] @ 128
	.word	040e81574h		; _a[5] @ 160
	.word	0bf089013h		; _a[6] @ 192
	.word	0bf9a739dh		; _a[7] @ 224
	.word	0c05750feh		; _a[8] @ 256
	.word	0be9f123ch		; _a[9] @ 288
	.word	0c10b2fb9h		; _a[10] @ 320
	.word	041143c22h		; _a[11] @ 352
	.word	0404b1992h		; _a[12] @ 384
	.word	040e14267h		; _a[13] @ 416
	.word	0c10d76b9h		; _a[14] @ 448
	.word	0c11aa46eh		; _a[15] @ 480
	.word	0c1195bf9h		; _a[16] @ 512
	.word	0c1032356h		; _a[17] @ 544
	.word	0c117d3f9h		; _a[18] @ 576
	.word	0c0479718h		; _a[19] @ 608
	.word	0c0fd136bh		; _a[20] @ 640
	.word	03f9e315ch		; _a[21] @ 672
	.word	040a1260eh		; _a[22] @ 704
	.word	0410248eah		; _a[23] @ 736
	.word	040b7fc0fh		; _a[24] @ 768
	.word	040d47288h		; _a[25] @ 800
	.word	0c0a73791h		; _a[26] @ 832
	.word	040fe9e5fh		; _a[27] @ 864
	.word	0c0c2696eh		; _a[28] @ 896
	.word	0407a7dd2h		; _a[29] @ 928
	.word	040892e82h		; _a[30] @ 960
	.word	04086b386h		; _a[31] @ 992
	.word	0404ceb5eh		; _a[32] @ 1024
	.word	0c0d09585h		; _a[33] @ 1056
	.word	04010bd12h		; _a[34] @ 1088
	.word	04114ec1ch		; _a[35] @ 1120
	.word	0408e6755h		; _a[36] @ 1152
	.word	0be887722h		; _a[37] @ 1184
	.word	03ff3f4b7h		; _a[38] @ 1216
	.word	03e345961h		; _a[39] @ 1248
	.word	0c0db991dh		; _a[40] @ 1280
	.word	0c1043584h		; _a[41] @ 1312
	.word	03f4a301ah		; _a[42] @ 1344
	.word	0c0036602h		; _a[43] @ 1376
	.word	041117e53h		; _a[44] @ 1408
	.word	0403109d2h		; _a[45] @ 1440
	.word	03f8a6a33h		; _a[46] @ 1472
	.word	04101d280h		; _a[47] @ 1504
	.word	0c069574dh		; _a[48] @ 1536
	.word	0c114ec7dh		; _a[49] @ 1568
	.word	0410c35deh		; _a[50] @ 1600
	.word	03f08cf59h		; _a[51] @ 1632
	.word	04063613eh		; _a[52] @ 1664
	.word	0c01f0a39h		; _a[53] @ 1696
	.word	0c0a3ad1dh		; _a[54] @ 1728
	.word	041195b5fh		; _a[55] @ 1760
	.word	0405ce50ch		; _a[56] @ 1792
	.word	0411c0b7ah		; _a[57] @ 1824
	.word	0c0bb04aeh		; _a[58] @ 1856
	.word	0c0c4d891h		; _a[59] @ 1888
	.word	0c0d2e3f4h		; _a[60] @ 1920
	.word	0bfe54969h		; _a[61] @ 1952
	.word	040c8b3b2h		; _a[62] @ 1984
	.word	0c05ab649h		; _a[63] @ 2016
	.word	0c0a03c69h		; _a[64] @ 2048
	.word	0bfefb6d9h		; _a[65] @ 2080
	.word	0c09e20dbh		; _a[66] @ 2112
	.word	0409e12cah		; _a[67] @ 2144
	.word	0c0bbe202h		; _a[68] @ 2176
	.word	0411458cch		; _a[69] @ 2208
	.word	04108c21bh		; _a[70] @ 2240
	.word	0c1118a07h		; _a[71] @ 2272
	.word	03fabf9ffh		; _a[72] @ 2304
	.word	0c093ebb7h		; _a[73] @ 2336
	.word	0bef3d259h		; _a[74] @ 2368
	.word	0c1169c85h		; _a[75] @ 2400
	.word	0c108d85ch		; _a[76] @ 2432
	.word	0409b5c67h		; _a[77] @ 2464
	.word	04097d474h		; _a[78] @ 2496
	.word	0408c53deh		; _a[79] @ 2528
	.word	040583bdah		; _a[80] @ 2560
	.word	0c1171ac0h		; _a[81] @ 2592
	.word	040b21353h		; _a[82] @ 2624
	.word	0c0233db8h		; _a[83] @ 2656
	.word	03f548bc3h		; _a[84] @ 2688
	.word	041175d7ch		; _a[85] @ 2720
	.word	040f8f0aah		; _a[86] @ 2752
	.word	0bf828742h		; _a[87] @ 2784
	.word	0c1060d57h		; _a[88] @ 2816
	.word	0404e5167h		; _a[89] @ 2848
	.word	03fbbe6a3h		; _a[90] @ 2880
	.word	0402298a8h		; _a[91] @ 2912
	.word	040fdfadfh		; _a[92] @ 2944
	.word	0c021b015h		; _a[93] @ 2976
	.word	03f6b0ae5h		; _a[94] @ 3008
	.word	040c94dc8h		; _a[95] @ 3040
	.word	040d1ca61h		; _a[96] @ 3072
	.word	04025d1e3h		; _a[97] @ 3104
	.word	040a2e7c3h		; _a[98] @ 3136
	.word	0c0ea5a46h		; _a[99] @ 3168
$C$IR_1:	.set	400

	.sect	".cinit"
	.align	8
	.field  	$C$IR_2,32
	.field  	_b+0,32
	.word	041c80000h		; _b[0] @ 0
	.word	041c00000h		; _b[1] @ 32
	.word	041c80000h		; _b[2] @ 64
	.word	0c1c80000h		; _b[3] @ 96
	.word	000000000h		; _b[4] @ 128
	.word	041c80000h		; _b[5] @ 160
	.word	041c80000h		; _b[6] @ 192
	.word	041c80000h		; _b[7] @ 224
	.word	041c80000h		; _b[8] @ 256
	.word	041c80000h		; _b[9] @ 288
	.word	041c80000h		; _b[10] @ 320
	.word	041c00000h		; _b[11] @ 352
	.word	041c80000h		; _b[12] @ 384
	.word	0c1c80000h		; _b[13] @ 416
	.word	000000000h		; _b[14] @ 448
	.word	041c80000h		; _b[15] @ 480
	.word	041c80000h		; _b[16] @ 512
	.word	041c80000h		; _b[17] @ 544
	.word	041c80000h		; _b[18] @ 576
	.word	041c80000h		; _b[19] @ 608
	.word	041c80000h		; _b[20] @ 640
	.word	041c00000h		; _b[21] @ 672
	.word	041c80000h		; _b[22] @ 704
	.word	0c1c80000h		; _b[23] @ 736
	.word	000000000h		; _b[24] @ 768
	.word	041c80000h		; _b[25] @ 800
	.word	041c80000h		; _b[26] @ 832
	.word	041c80000h		; _b[27] @ 864
	.word	041c80000h		; _b[28] @ 896
	.word	041c80000h		; _b[29] @ 928
	.word	041c80000h		; _b[30] @ 960
	.word	041c00000h		; _b[31] @ 992
	.word	041c80000h		; _b[32] @ 1024
	.word	0c1c80000h		; _b[33] @ 1056
	.word	000000000h		; _b[34] @ 1088
	.word	041c80000h		; _b[35] @ 1120
	.word	041c80000h		; _b[36] @ 1152
	.word	041c80000h		; _b[37] @ 1184
	.word	041c80000h		; _b[38] @ 1216
	.word	041c80000h		; _b[39] @ 1248
	.word	041c80000h		; _b[40] @ 1280
	.word	041c00000h		; _b[41] @ 1312
	.word	041c80000h		; _b[42] @ 1344
	.word	0c1c80000h		; _b[43] @ 1376
	.word	000000000h		; _b[44] @ 1408
	.word	041c80000h		; _b[45] @ 1440
	.word	041c80000h		; _b[46] @ 1472
	.word	041c80000h		; _b[47] @ 1504
	.word	041c80000h		; _b[48] @ 1536
	.word	041c80000h		; _b[49] @ 1568
	.word	041c80000h		; _b[50] @ 1600
	.word	041c00000h		; _b[51] @ 1632
	.word	041c80000h		; _b[52] @ 1664
	.word	0c1c80000h		; _b[53] @ 1696
	.word	000000000h		; _b[54] @ 1728
	.word	041c80000h		; _b[55] @ 1760
	.word	041c80000h		; _b[56] @ 1792
	.word	041c80000h		; _b[57] @ 1824
	.word	041c80000h		; _b[58] @ 1856
	.word	041c80000h		; _b[59] @ 1888
	.word	041c80000h		; _b[60] @ 1920
	.word	041c00000h		; _b[61] @ 1952
	.word	041c80000h		; _b[62] @ 1984
	.word	0c1c80000h		; _b[63] @ 2016
	.word	000000000h		; _b[64] @ 2048
	.word	041c80000h		; _b[65] @ 2080
	.word	041c80000h		; _b[66] @ 2112
	.word	041c80000h		; _b[67] @ 2144
	.word	041c80000h		; _b[68] @ 2176
	.word	041c80000h		; _b[69] @ 2208
	.word	041c80000h		; _b[70] @ 2240
	.word	041c00000h		; _b[71] @ 2272
	.word	041c80000h		; _b[72] @ 2304
	.word	0c1c80000h		; _b[73] @ 2336
	.word	000000000h		; _b[74] @ 2368
	.word	041c80000h		; _b[75] @ 2400
	.word	041c80000h		; _b[76] @ 2432
	.word	041c80000h		; _b[77] @ 2464
	.word	041c80000h		; _b[78] @ 2496
	.word	041c80000h		; _b[79] @ 2528
	.word	041c80000h		; _b[80] @ 2560
	.word	041c00000h		; _b[81] @ 2592
	.word	041c80000h		; _b[82] @ 2624
	.word	0c1c80000h		; _b[83] @ 2656
	.word	000000000h		; _b[84] @ 2688
	.word	041c80000h		; _b[85] @ 2720
	.word	041c80000h		; _b[86] @ 2752
	.word	041c80000h		; _b[87] @ 2784
	.word	041c80000h		; _b[88] @ 2816
	.word	041c80000h		; _b[89] @ 2848
	.word	041c80000h		; _b[90] @ 2880
	.word	041c00000h		; _b[91] @ 2912
	.word	041c80000h		; _b[92] @ 2944
	.word	0c1c80000h		; _b[93] @ 2976
	.word	000000000h		; _b[94] @ 3008
	.word	041c80000h		; _b[95] @ 3040
	.word	041c80000h		; _b[96] @ 3072
	.word	041c80000h		; _b[97] @ 3104
	.word	041c80000h		; _b[98] @ 3136
	.word	041c80000h		; _b[99] @ 3168
$C$IR_2:	.set	400

	.global	_a
_a:	.usect	".far",400,8
	.global	_b
_b:	.usect	".far",400,8
;	C:\MATHLIB_Tools\CCSV5_5_0\ccsv5\tools\compiler\c6000_7.4.2\bin\opt6x.exe C:\\Users\\gtbldadm\\AppData\\Local\\Temp\\030208 C:\\Users\\gtbldadm\\AppData\\Local\\Temp\\0302010 
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
;*   Local Frame Size  : 8 Args + 36 Auto + 56 Save = 100 byte                *
;******************************************************************************
_main:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           STW     .D2T1   A11,*SP--(8)      ; |113| 
           STW     .D2T1   A10,*SP--(8)      ; |113| 
           STDW    .D2T2   B13:B12,*SP--     ; |113| 
           STDW    .D2T2   B11:B10,*SP--     ; |113| 
           STDW    .D2T1   A15:A14,*SP--     ; |113| 

           STDW    .D2T1   A13:A12,*SP--     ; |113| 
||         MVKL    .S1     $C$SL1+0,A4

           STW     .D2T2   B3,*SP--(56)      ; |113| 
||         CALLP   .S2     _driver_init,B3
||         MVKH    .S1     $C$SL1+0,A4

$C$RL0:    ; CALL OCCURS {_driver_init} {0}  ; |122| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 4

           MVKL    .S2     _b,B10
||         MVKL    .S1     _a,A12

           MVK     .S1     0x64,A11
||         MVKL    .S2     _output,B11

           MVKH    .S2     _b,B10
||         MVKH    .S1     _a,A12

           CALL    .S1     _powf             ; |131| 
||         MV      .L2     B10,B13
||         MV      .L1     A12,A13
||         MVKH    .S2     _output,B11
||         ZERO    .D2     B12

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L1:    
;          EXCLUSIVE CPU CYCLES: 5

           LDW     .D1T1   *A13++,A4         ; |131| 
||         LDW     .D2T2   *B13++,B4         ; |131| 

           ADDKPC  .S2     $C$RL1,B3,3       ; |131| 
$C$RL1:    ; CALL OCCURS {_powf} {0}         ; |131| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7

           LDW     .D2T2   *B11,B4           ; |131| 
||         SUB     .L1     A11,1,A0          ; |130| 
||         SUB     .S1     A11,1,A11         ; |130| 

   [ A0]   BNOP    .S1     $C$L1,3           ; |130| 

           ADD     .L2     B12,B4,B4         ; |131| 
||         ADD     .S2     4,B12,B12         ; |130| 

   [ A0]   CALL    .S1     _powf             ; |131| 
||         STW     .D2T1   A4,*B4            ; |131| 
|| [!A0]   ADD     .L2     4,B11,B4

           ; BRANCHCC OCCURS {$C$L1}         ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1

           CALL    .S2     _powsp            ; |134| 
||         ZERO    .L2     B12
||         MV      .L1X    B10,A11
||         MV      .D1     A12,A10
||         MVK     .S1     0x64,A13
||         STW     .D2T2   B4,*+SP(20)

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L2:    
;          EXCLUSIVE CPU CYCLES: 5

           LDW     .D1T1   *A12++,A4         ; |134| 
||         LDW     .D2T2   *B10++,B4         ; |134| 

           ADDKPC  .S2     $C$RL2,B3,3       ; |134| 
$C$RL2:    ; CALL OCCURS {_powsp} {0}        ; |134| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12
           LDW     .D2T2   *+SP(20),B4       ; |134| 
           SUB     .L1     A13,1,A0          ; |133| 
           SUB     .L1     A13,1,A13         ; |133| 
           NOP             2
           LDW     .D2T2   *B4,B4            ; |134| 
   [ A0]   BNOP    .S1     $C$L2,3           ; |133| 

           ADD     .L2     B12,B4,B4         ; |134| 
||         ADD     .S2     4,B12,B12         ; |133| 

   [ A0]   CALL    .S1     _powsp            ; |134| 
||         STW     .D2T1   A4,*B4            ; |134| 

           ; BRANCHCC OCCURS {$C$L2}         ; |133| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7
           LDW     .D2T2   *+SP(20),B4
           MV      .L2X    A11,B12
           MV      .L1     A10,A13
           MVK     .S1     0x64,A12
           ZERO    .L2     B10
           ADD     .L2     4,B4,B4

           STW     .D2T2   B4,*+SP(24)
||         CALL    .S1     _powsp_c          ; |137| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L3:    
;          EXCLUSIVE CPU CYCLES: 5

           LDW     .D1T1   *A13++,A4         ; |137| 
||         LDW     .D2T2   *B12++,B4         ; |137| 

           ADDKPC  .S2     $C$RL3,B3,3       ; |137| 
$C$RL3:    ; CALL OCCURS {_powsp_c} {0}      ; |137| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12

           LDW     .D2T2   *+SP(24),B4       ; |137| 
||         SUB     .L1     A12,1,A0          ; |136| 

           SUB     .L1     A12,1,A12         ; |136| 
   [!A0]   MVKL    .S2     0xbf000003,B9
   [!A0]   MVKL    .S1     0x3effffff,A17
   [!A0]   LDW     .D2T1   *+SP(24),A3
           LDW     .D2T2   *B4,B4            ; |137| 
   [ A0]   BNOP    .S1     $C$L3,3           ; |136| 

           ADD     .L2     B10,B4,B4         ; |137| 
||         ADD     .S2     4,B10,B10         ; |136| 

   [ A0]   CALL    .S1     _powsp_c          ; |137| 
||         STW     .D2T1   A4,*B4            ; |137| 
|| [!A0]   ZERO    .L2     B4

           ; BRANCHCC OCCURS {$C$L3}         ; |136| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 8

           MVKL    .S2     0x3f7ffffe,B17
||         MVKH    .S1     0x3effffff,A17
||         ZERO    .L1     A7
||         ADD     .D1     4,A3,A12
||         MV      .L2X    A11,B21

           MVK     .S2     0x64,B5
||         MVKL    .S1     0xfefa39ec,A4
||         LDW     .D1T1   *A12,A20
||         MV      .L1     A10,A21

           MVKH    .S2     0xbf000003,B9
||         MVKL    .S1     0xc2aeac50,A13
||         ADD     .L2X    1,A17,B13
||         ZERO    .L1     A22               ; |87| 
||         ZERO    .D1     A16               ; |87| 

           SET     .S2     B4,0x17,0x1e,B4
||         MVKL    .S1     0x3fe62e42,A5
||         MV      .L1X    B5,A26
||         ZERO    .D1     A6                ; |87| 

           MVKH    .S2     0x3f7ffffe,B17
||         ADD     .L2     -1,B4,B12
||         ADD     .D2     -3,B9,B4
||         MVKH    .S1     0xfefa39ec,A4

           ADD     .L2     2,B17,B10
||         STW     .D2T2   B4,*+SP(12)
||         MVKH    .S1     0xc2aeac50,A13

           MVKH    .S1     0x3fe62e42,A5
           SET     .S1     A7,0x14,0x1d,A7
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*
;*      Loop found in file               : C:/nightlybuilds/mathlib/ti/mathlib/src/powsp/powsp_d.c
;*      Loop source line                 : 139
;*      Loop opening brace source line   : 139
;*      Loop closing brace source line   : 141
;*      Known Minimum Trip Count         : 100                    
;*      Known Maximum Trip Count         : 100                    
;*      Known Max Trip Count Factor      : 100
;*      Loop Carried Dependency Bound(^) : 128
;*      Unpartitioned Resource Bound     : 17
;*      Partitioned Resource Bound(*)    : 18
;*      Resource Partition:
;*                                A-side   B-side
;*      .L units                     3        5     
;*      .S units                    17       16     
;*      .D units                     3        3     
;*      .M units                    12       11     
;*      .X cross paths              17       11     
;*      .T address paths             3        3     
;*      Long read paths              0        0     
;*      Long write paths             0        0     
;*      Logical  ops (.LS)           8       13     (.L or .S unit)
;*      Addition ops (.LSD)         23       15     (.L or .S or .D unit)
;*      Bound(.L .S .LS)            14       17     
;*      Bound(.L .S .D .LS .LSD)    18*      18*    
;*
;*      Disqualified loop: Loop carried dependency bound too large
;*----------------------------------------------------------------------------*
$C$L4:    
;          EXCLUSIVE CPU CYCLES: 5
           LDW     .D1T1   *A21++,A3         ; |87| 
           NOP             4
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 124

           ABSSP   .S1     A3,A17            ; |87| 
||         MVKL    .S2     0x3e436581,B4
||         LDW     .D2T2   *B21++,B30        ; |87| 

           SPDP    .S1     A17,A9:A8         ; |87| 
||         MVKH    .S2     0x3e436581,B4
||         LDW     .D2T2   *+SP(12),B29

           MVKL    .S2     0xbe804d66,B5

           RCPDP   .S1     A9:A8,A9:A8       ; |87| 
||         MVKH    .S2     0xbe804d66,B5

           MVKL    .S2     0xbf000003,B31

           CLR     .S1     A9,0,16,A23       ; |87| 
||         MVKH    .S2     0xbf000003,B31

           MPYSPDP .M1     A17,A23:A22,A9:A8 ; |87| 
||         MVKL    .S1     0xbe6bd0fc,A31
||         MVKL    .S2     0x41b8aa3b,B6

           MVKH    .S2     0x41b8aa3b,B6
           EXTU    .S1     A23,1,21,A30      ; |87| 
           MVKL    .S2     0x3d317000,B28
           MVKH    .S1     0xbe6bd0fc,A31
           MVKH    .S2     0x3d317000,B28
           MVK     .S1     0xfffffc01,A25
           FSUBDP  .L1     A9:A8,A7:A6,A19:A18 ; |87| 
           MVKL    .S2     0x3e2abce4,B27
           MVKL    .S1     0x3eaaa872,A29
           DPSP    .L1     A19:A18,A8        ; |87| 
           MVKH    .S2     0x3e2abce4,B27
           MVKH    .S1     0x3eaaa872,A29
           MVKL    .S2     _ti_math_jTable,B24
           MPYSP   .M1     A8,A8,A28         ; |87| 
           MPYSP   .M2X    B4,A8,B4          ; |87| 
           MVKL    .S1     _ti_math_logtable,A2
           MVKH    .S1     _ti_math_logtable,A2

           MPYSP   .M1     A28,A8,A25        ; |87| 
||         ADD     .L1     A25,A30,A8        ; |87| 

           FADDSP  .L2     B5,B4,B4          ; |87| 
||         MPYSP   .M1     A31,A28,A24       ; |87| 

           INTDP   .L1     A8,A9:A8          ; |87| 
           MPYSP   .M1     A28,A28,A27       ; |87| 
           MPYSP   .M1     A29,A25,A29       ; |87| 

           FADDSP  .L1X    A24,B4,A23        ; |87| 
||         EXTU    .S1     A23,12,29,A24     ; |87| 
||         MPYSP   .M2X    B31,A28,B4        ; |87| 

           LDDW    .D1T1   *+A2[A24],A25:A24 ; |87| 
           FMPYDP  .M1     A5:A4,A9:A8,A9:A8 ; |87| 
           MPYSP   .M1     A27,A23,A23       ; |87| 
           MVKL    .S2     _ti_math_kTable,B25
           FADDSP  .L2X    A29,B4,B4         ; |87| 
           FSUBDP  .L1     A25:A24,A9:A8,A9:A8 ; |87| 
           MVKH    .S2     _ti_math_jTable,B24
           FADDSP  .L2X    A23,B4,B4         ; |87| 
           FADDDP  .L1     A9:A8,A19:A18,A9:A8 ; |87| 
           MVKH    .S2     _ti_math_kTable,B25
           SPDP    .S2     B4,B5:B4          ; |87| 
           CMPGTSP .S1X    A17,B12,A1        ; |87| 
           FADDDP  .L2X    B5:B4,A9:A8,B5:B4 ; |87| 
           CMPEQSP .S1X    A17,B10,A0        ; |87| 
           MVK     .S2     0x72,B19
           DPSP    .L2     B5:B4,B20         ; |87| 
           MVKL    .S1     0x42b17218,A8
           MVKH    .S1     0x42b17218,A8
           MVKL    .S1     0x3effffff,A30
   [ A1]   MV      .L2X    A8,B20            ; |87| 
   [!A0]   MPYSP   .M2     B30,B20,B4        ; |87| 
           MVKL    .S1     0x3605fdf4,A31
           MVKH    .S1     0x3605fdf4,A31
   [ A0]   MV      .L2X    A22,B4            ; |87| 
           MPYSP   .M2     B6,B4,B6          ; |141| 
           MVKH    .S1     0x3effffff,A30
           SUB     .L1     A26,1,A2          ; |139| 
           MVKL    .S1     0x3f7ffffe,A28
           FADDSP  .L2     B13,B6,B7         ; |141| 
           SUB     .L1     A26,1,A26         ; |139| 
           MVKH    .S1     0x3f7ffffe,A28
           SPTRUNC .L2     B7,B9             ; |141| 
           MVKL    .S2     0x42b17218,B5
           MVKH    .S2     0x42b17218,B5
           CMPLTSP .S2     B6,B29,B8         ; |141| 
           SUB     .L2     B9,B8,B17         ; |141| 
           INTSP   .S2     B17,B6            ; |141| 
           ZERO    .L2     B20
           EXTU    .S2     B4,1,24,B22       ; |141| 
           CMPLTU  .L2     B22,B19,B0        ; |141| 
           MPYSP   .M2     B28,B6,B7         ; |141| 
           SPINT   .L2     B30,B22           ; |141| 
           MPYSP   .M1X    A31,B6,A8         ; |141| 
           CMPLTSP .S1X    B4,A13,A0         ; |141| 
           FSUBSP  .L2     B4,B7,B6          ; |141| 
   [!B0]   MV      .L1     A7,A17            ; |141| 
           INTSP   .L2     B22,B2            ; |141| 
           FSUBSP  .S2X    B6,A8,B6          ; |141| 
           ZERO    .L2     B29
           AND     .D2     1,B22,B1          ; |141| 
           MPYSP   .M2     B6,B6,B7          ; |141| 
           CMPEQSP .S2     B30,B2,B31        ; |141| 
           SUB     .L2     B9,B8,B23         ; |141| 
           EXTU    .S2     B23,28,30,B26     ; |141| 
           MPYSP   .M2     B7,B6,B16         ; |141| 
           AND     .L2     3,B17,B8          ; |141| 
           ZERO    .L2     B28
           MPYSP   .M1X    A30,B7,A9         ; |141| 
           MPYSP   .M2     B27,B16,B7        ; |141| 
           MPYSP   .M1X    A28,B6,A8         ; |141| 
           XOR     .L2     1,B31,B27         ; |141| 
           LDDW    .D2T2   *+B24[B8],B17:B16 ; |141| 

           ZERO    .L2     B26
||         FADDSP  .S2X    A9,B7,B9          ; |141| 
||         LDDW    .D2T2   *+B25[B26],B7:B6  ; |141| 

   [!B1]   MVK     .L1     0x1,A9            ; |141| 
   [ B1]   MVK     .L1     0xffffffff,A9     ; |141| 
           FADDSP  .L2X    B9,A8,B18         ; |141| 
   [ B0]   FADDSP  .L2     B10,B4,B18        ; |141| 

           FMPYDP  .M2     B17:B16,B7:B6,B7:B6 ; |141| 
|| [!B0]   EXTU    .S2     B23,16,20,B16     ; |141| 

   [!B0]   SPDP    .S2     B18,B19:B18       ; |141| 
           ZERO    .L2     B23

   [!B0]   FADDDP  .L2X    A17:A16,B19:B18,B19:B18 ; |141| 
|| [!B0]   SHL     .S2     B16,20,B24        ; |141| 

           SPDP    .S2X    A3,B9:B8          ; |141| 
||         ZERO    .L2     B17:B16           ; |141| 

   [!B0]   ADD     .D2     B24,B7,B7         ; |141| 

   [!B0]   FMPYDP  .M2     B7:B6,B19:B18,B7:B6 ; |141| 
||         CMPLTDP .S2     B9:B8,B17:B16,B8  ; |141| 

           ZERO    .L2     B24
           CMPEQSP .S2     B30,B2,B9         ; |141| 

           AND     .L2     B9,B8,B1          ; |141| 
||         CMPLTSP .S2X    A3,B29,B8         ; |141| 

   [!B0]   DPSP    .L2     B7:B6,B18         ; |141| 
|| [ B1]   MV      .L1     A9,A8             ; |141| 
|| [!B1]   MVK     .S1     0x1,A8            ; |141| 
|| [ A0]   ZERO    .D2     B7
||         CMPGTSP .S2     B4,B5,B2          ; |141| 

           INTSP   .L1     A8,A8             ; |141| 
||         ZERO    .L2     B6
||         AND     .D2     B27,B8,B9         ; |141| 
||         SET     .S2     B23,0x0,0x1e,B8

   [ B2]   MV      .L2     B12,B6            ; |141| 
||         CMPLTSP .S2     B30,B20,B17       ; |141| 
||         ZERO    .D2     B5

           XOR     .D2     1,B17,B25         ; |141| 
||         CMPEQSP .S2X    A3,B26,B4         ; |141| 

   [!A0]   MV      .L2     B18,B7            ; |141| 
||         MV      .S1X    B9,A0             ; |141| 
||         CMPLTSP .S2     B30,B28,B16       ; |141| 
||         ZERO    .D2     B23

           AND     .L2     B4,B25,B2         ; |141| 
||         AND     .S2     B4,B16,B1         ; |141| 
|| [!B2]   MV      .D2     B7,B6             ; |141| 

   [ A0]   MV      .L1X    B8,A8             ; |141| 
   [!A0]   MPYSP   .M1X    B6,A8,A8          ; |141| 
           SET     .S2     B24,0x17,0x1e,B4
   [ B2]   ZERO    .L2     B5
           CMPEQSP .S2     B30,B23,B0        ; |141| 
   [ A2]   B       .S1     $C$L4             ; |139| 
   [!B2]   MV      .L2X    A8,B5             ; |141| 
   [!B1]   MV      .L2     B5,B4             ; |141| 
   [ B0]   MV      .L1X    B10,A3            ; |141| 
   [!B0]   MV      .L1X    B4,A3             ; |141| 

   [!A2]   CALL    .S1     _powsp_v          ; |142| 
||         STW     .D1T1   A3,*A20++         ; |141| 

           ; BRANCHCC OCCURS {$C$L4}         ; |139| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           LDW     .D2T1   *+B11(16),A6      ; |142| 
           MVK     .S2     0x64,B6           ; |142| 
           MV      .L2X    A11,B4            ; |142| 
           MV      .L1     A10,A4            ; |142| 
           ADDKPC  .S2     $C$RL4,B3,0       ; |142| 
$C$RL4:    ; CALL OCCURS {_powsp_v} {0}      ; |142| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 46
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           LDW     .D2T1   *B11,A4           ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T2   *+B11(4),B4       ; |66| 
||         MVK     .S1     0x64,A6           ; |66| 
||         MVK     .L1     0x9,A8            ; |66| 

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
||         MVK     .S1     0x64,A6           ; |67| 
||         MVK     .L1     0x9,A8            ; |67| 

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
||         MVK     .S1     0x64,A6           ; |68| 
||         MVK     .L1     0x9,A8            ; |68| 

$C$RL7:    ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     _fcn_pass,B4
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     _fcn_pass,B4
           MVKH    .S2     0x3f1a36e2,B7

           ADD     .L1     4,A12,A10
||         STW     .D2T1   A4,*+B4(12)       ; |68| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B11,A4           ; |69| 
||         LDW     .D1T2   *A10,B4           ; |69| 
||         MVK     .S1     0x64,A6           ; |69| 
||         MVK     .L1     0x9,A8            ; |69| 

$C$RL8:    ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 14
           MVKL    .S2     _fcn_pass,B4
           MVKH    .S2     _fcn_pass,B4

           ADD     .L2     4,B4,B4
||         MVKL    .S2     _fcn_pass,B5

           LDW     .D2T2   *B4,B0            ; |72| 
           MVKH    .S2     _fcn_pass,B5
           STW     .D2T1   A4,*+B5(16)       ; |69| 
           STW     .D2T2   B4,*+SP(28)
           ADD     .L2     8,B5,B4

   [!B0]   MVKL    .S1     _all_pass,A4
|| [!B0]   B       .S2     $C$L5             ; |72| 
||         MV      .L2     B0,B1             ; guard predicate rewrite
|| [!B0]   ZERO    .L1     A3                ; |72| 

   [!B0]   CALL    .S2     _print_test_results ; |146| 
|| [!B0]   MVKH    .S1     _all_pass,A4
|| [ B0]   LDW     .D2T2   *B4,B0            ; |72| 

           STW     .D2T2   B4,*+SP(32)       ; |69| 
   [ B1]   ADD     .L2     4,B4,B4
           NOP             2
           ; BRANCHCC OCCURS {$C$L5}         ; |72| 
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
           CALL    .S1     _print_test_results ; |146| 
           MVKL    .S1     _all_pass,A4
           MVKH    .S1     _all_pass,A4
           NOP             2
;** --------------------------------------------------------------------------*
$C$L5:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A3,*A4            ; |72| 
||         MVK     .L1     0x1,A4            ; |146| 
||         ADDKPC  .S2     $C$RL9,B3,0       ; |146| 

$C$RL9:    ; CALL OCCURS {_print_test_results} {0}  ; |146| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 9
           MVKL    .S1     _a_sc,A3
           MVKH    .S1     _a_sc,A3
           ZERO    .L2     B11

           MV      .L2X    A3,B4             ; |154| 
||         MVKH    .S2     0xbf800000,B11
||         ZERO    .D2     B7

           STDW    .D2T2   B11:B10,*+B4(16)  ; |158| 
||         SET     .S2     B7,0x0,0x1e,B7
||         ZERO    .L1     A5

           ADD     .D2     B4,24,B7
||         ADD     .L2     1,B7,B9
||         MVKH    .S1     0xff800000,A5
||         ZERO    .L1     A4
||         ZERO    .S2     B8

           STDW    .D2T2   B9:B8,*B4         ; |154| 
||         SET     .S1     A4,0x17,0x1e,A4
||         MVKL    .S2     _b_sc,B5

           MVK     .L2     0x6,B4            ; |161| 
||         STDW    .D2T1   A5:A4,*+B4(8)     ; |156| 
||         MVKH    .S2     _b_sc,B5
||         MVK     .L1     0x5,A1            ; |160| 

           MVC     .S2     B4,RILC
||         MVK     .L2     5,B9              ; |161| 
||         STDW    .D2T2   B9:B8,*B5         ; |154| 
||         SUB     .L1     A1,1,A1           ; |160| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*
;*      Loop found in file               : C:/nightlybuilds/mathlib/ti/mathlib/src/powsp/powsp_d.c
;*      Loop source line                 : 161
;*      Loop opening brace source line   : 161
;*      Loop closing brace source line   : 165
;*      Known Minimum Trip Count         : 6                    
;*      Known Maximum Trip Count         : 6                    
;*      Known Max Trip Count Factor      : 6
;*      Loop Carried Dependency Bound(^) : 6
;*      Unpartitioned Resource Bound     : 2
;*      Partitioned Resource Bound(*)    : 4
;*      Resource Partition:
;*                                A-side   B-side
;*      .L units                     1        0     
;*      .S units                     1        0     
;*      .D units                     1        3     
;*      .M units                     0        0     
;*      .X cross paths               0        0     
;*      .T address paths             0        4*    
;*      Long read paths              0        0     
;*      Long write paths             0        0     
;*      Logical  ops (.LS)           0        0     (.L or .S unit)
;*      Addition ops (.LSD)          6        0     (.L or .S or .D unit)
;*      Bound(.L .S .LS)             1        0     
;*      Bound(.L .S .D .LS .LSD)     3        1     
;*
;*      Searching for software pipeline schedule at ...
;*         ii = 6  Schedule found with 2 iterations in parallel
;*
;*      Register Usage Table:
;*          +-----------------------------------------------------------------+
;*          |AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA|BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB|
;*          |00000000001111111111222222222233|00000000001111111111222222222233|
;*          |01234567890123456789012345678901|01234567890123456789012345678901|
;*          |--------------------------------+--------------------------------|
;*       0: |   ****                         |     ***                        |
;*       1: |   *****                        |     ***                        |
;*       2: |*  ****                         |    ****                        |
;*       3: |   *****                        |     ***                        |
;*       4: |   *****                        |    ****                        |
;*       5: |   *****                        |     ***                        |
;*          +-----------------------------------------------------------------+
;*
;*      Done
;*
;*      Loop will be splooped
;*      Collapsed epilog stages       : 0
;*      Collapsed prolog stages       : 0
;*      Minimum required memory pad   : 0 bytes
;*
;*      Minimum safe trip count       : 1
;*      Min. prof. trip count  (est.) : 2
;*
;*      Mem bank conflicts/iter(est.) : { min 0.000, est 0.000, max 0.000 }
;*      Mem bank perf. penalty (est.) : 0.0%
;*
;*
;*      Total cycles (est.)         : 6 + min_trip_cnt * 6 = 42        
;*----------------------------------------------------------------------------*
;*        SINGLE SCHEDULED ITERATION
;*
;*        $C$C167:
;*   0              ADD     .L1     A4,A3,A7          ; |163| 
;*   1              CMPLT   .L1     A7,6,A0           ; |163| 
;*   2      [!A0]   MVK     .S1     0xffffffe8,A7     ; |163| 
;*     ||   [ A0]   ZERO    .D1     A7                ; |163| 
;*   3              LDW     .D2T2   *B5++,B4          ; |162|  ^ 
;*     ||           ADD     .L1     A5,A7,A7          ; |163| 
;*   4              ADD     .L1     A6,A7,A7          ; |163| 
;*   5              LDW     .D1T2   *A7,B4            ; |163|  ^ 
;*     ||           ADD     .L1     1,A4,A4           ; |161| 
;*   6              NOP             2
;*   8              STW     .D2T2   B4,*B7++          ; |162|  ^ 
;*     ||           ADD     .L1     4,A5,A5           ; |161| 
;*   9              NOP             1
;*  10              STW     .D2T2   B4,*B6++          ; |163|  ^ 
;*     ||           SPBR            $C$C167
;*  11              NOP             1
;*  12              ; BRANCHCC OCCURS {$C$C167}       ; |161| 
;*----------------------------------------------------------------------------*
$C$L6:    ; PIPED LOOP PROLOG
;          EXCLUSIVE CPU CYCLES: 7

   [ A1]   SPLOOPD 6       ;12               ; SPRELOAD
||         STDW    .D2T1   A5:A4,*+B5(8)     ; |156| 
||         MVK     .L1     0x1,A3            ; |160| 
||         ZERO    .S1     A4                ; |161| 
||         MVC     .S2     B9,ILC

;** --------------------------------------------------------------------------*
$C$L7:    ; PIPED LOOP KERNEL
;          EXCLUSIVE CPU CYCLES: 6

           SPMASK          L2,D2
||         ADD     .D2     B5,24,B6
||         MVK     .L2     0x4,B8
||         ADD     .L1     A4,A3,A7          ; |163| (P) <0,0> 

           SPMASK          S1,S2,D2
||         STDW    .D2T2   B11:B10,*+B5(16)  ; |158| 
||         MV      .S1X    B5,A12            ; |154| 
||         MVKL    .S2     _a_sc,B5
||         CMPLT   .L1     A7,6,A0           ; |163| (P) <0,1> 

           SPMASK          L1,S2
||         MVKH    .S2     _a_sc,B5
||         MV      .L1X    B8,A5
|| [ A0]   ZERO    .D1     A7                ; |163| (P) <0,2> 
|| [!A0]   MVK     .S1     0xffffffe8,A7     ; |163| (P) <0,2> 

           SPMASK          S1
||         MV      .S1     A12,A6
||         LDW     .D2T2   *B5++,B4          ; |162| (P) <0,3>  ^ 
||         ADD     .L1     A5,A7,A7          ; |163| (P) <0,3> 

           ADD     .L1     A6,A7,A7          ; |163| (P) <0,4> 

           ADD     .L1     1,A4,A4           ; |161| (P) <0,5> 
||         LDW     .D1T2   *A7,B4            ; |163| (P) <0,5>  ^ 

           NOP             2

           STW     .D2T2   B4,*B7++          ; |162| <0,8>  ^ 
||         ADD     .L1     4,A5,A5           ; |161| <0,8> 

           NOP             1

           SPKERNEL 0,0
||         STW     .D2T2   B4,*B6++          ; |163| <0,10>  ^ 

;** --------------------------------------------------------------------------*
$C$L8:    ; PIPED LOOP EPILOG AND PROLOG
;          EXCLUSIVE CPU CYCLES: 9
           NOP             2

           SPMASKR         ;$C$L8
||         ADD     .S1     1,A3,A3           ; |160| 
||         ZERO    .D1     A4                ; |161| 

           ADD     .L2     4,B8,B8           ; |160| 
|| [ A1]   B       .S2     $C$L8             ; |160| 

           MV      .S1     A6,A12
||         MVKL    .S2     _a_sc,B5
||         SUB     .D1     A1,1,A1           ; |160| 

           MVKH    .S2     _a_sc,B5
||         MV      .L1X    B8,A5

           MV      .S1     A12,A6
||         MVK     .L2     0x6,B4            ; |161| 

           NOP             2
           ; BRANCHCC OCCURS {$C$L8}         ; |160| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7
           MVKL    .S1     _a_sc,A3
           MVK     .S1     0x24,A31          ; |168| 
           MVKH    .S1     _a_sc,A3

           SUB     .L1     A3,4,A11
||         STW     .D2T1   A31,*+SP(16)
||         SUB     .L2X    A12,4,B11

           LDW     .D1T1   *++A11,A3         ; |170| 
||         LDW     .D2T2   *++B11,B4         ; |170| 

           CALL    .S1     _pow              ; |170| 
           ZERO    .L1     A15
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains control code
;*----------------------------------------------------------------------------*
$C$L9:    
;          EXCLUSIVE CPU CYCLES: 4
           ADDKPC  .S2     $C$RL10,B3,1      ; |170| 

           SPDP    .S1     A3,A5:A4          ; |170| 
||         SPDP    .S2     B4,B5:B4          ; |170| 

           NOP             1
$C$RL10:   ; CALL OCCURS {_pow} {0}          ; |170| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 24
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           LDW     .D2T2   *B4,B4            ; |170| 
           DPSP    .L1     A5:A4,A3          ; |170| 
           ZERO    .L1     A5
           MVKH    .S1     0x38100000,A5
           NOP             1
           ADD     .L1X    A15,B4,A4         ; |170| 
           STW     .D1T1   A3,*A4            ; |170| 
           LDW     .D2T2   *B11,B4           ; |171| 
           MVKL    .S1     0xa639b,A4
           MVKH    .S1     0xa639b,A4
           NOP             2
           SPDP    .S2     B4,B7:B6          ; |171| 
           NOP             1
           CMPGTDP .S2X    B7:B6,A5:A4,B0    ; |171| 
           NOP             1

   [!B0]   B       .S1     $C$L10            ; |171| 
|| [!B0]   LDW     .D1T1   *A11,A3           ; |171| 

   [ B0]   CALL    .S1     _powsp            ; |171| 
|| [ B0]   LDW     .D1T1   *A11,A4           ; |171| 

   [!B0]   CALL    .S1     _pow              ; |171| 
           NOP             2
   [!B0]   SPDP    .S1     A3,A5:A4          ; |171| 
           ; BRANCHCC OCCURS {$C$L10}        ; |171| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1
           ADDKPC  .S2     $C$RL11,B3,0      ; |171| 
$C$RL11:   ; CALL OCCURS {_powsp} {0}        ; |171| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 6

           B       .S2     $C$L11            ; |171| 
||         MVKL    .S1     _output,A14

           SPDP    .S1     A4,A5:A4          ; |171| 
           NOP             1

           MVKH    .S1     _output,A14
||         DPSP    .L1     A5:A4,A3          ; |171| 

           LDW     .D1T1   *+A14(4),A4       ; |171| 
           NOP             1
           ; BRANCH OCCURS {$C$L11}          ; |171| 
;** --------------------------------------------------------------------------*
$C$L10:    
;          EXCLUSIVE CPU CYCLES: 2
           DADD    .L2     0,B7:B6,B5:B4     ; |171| 
           ADDKPC  .S2     $C$RL12,B3,0      ; |171| 
$C$RL12:   ; CALL OCCURS {_pow} {0}          ; |171| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 4
           MVKL    .S1     _output,A14
           MVKH    .S1     _output,A14

           DPSP    .L1     A5:A4,A3          ; |171| 
||         LDW     .D1T1   *+A14(4),A4       ; |171| 

           NOP             1
;** --------------------------------------------------------------------------*
$C$L11:    
;          EXCLUSIVE CPU CYCLES: 10
           NOP             3
           ADD     .L1     A15,A4,A4         ; |171| 

           STW     .D1T1   A3,*A4            ; |171| 
||         CALL    .S1     _powsp_c          ; |172| 

           LDW     .D1T1   *A11,A4           ; |172| 
||         LDW     .D2T2   *B11,B4           ; |172| 

           ADDKPC  .S2     $C$RL13,B3,3      ; |172| 
$C$RL13:   ; CALL OCCURS {_powsp_c} {0}      ; |172| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 140

           LDW     .D1T1   *+A14(8),A3       ; |172| 
||         MV      .L2X    A4,B4             ; |172| 
||         ZERO    .L1     A7
||         ZERO    .S1     A6                ; |87| 
||         MVKL    .S2     0x3e436581,B31
||         LDW     .D2T2   *+SP(12),B26

           ZERO    .L1     A23
           LDW     .D2T1   *+SP(16),A21      ; |197| 
           ZERO    .L1     A24
           MVKH    .S2     0x3e436581,B31
           ADD     .L1     A15,A3,A3         ; |172| 
           STW     .D1T2   B4,*A3            ; |172| 
           LDW     .D1T1   *A11,A3           ; |173| 
           SET     .S1     A7,0x14,0x1d,A7
           MVK     .S1     0xfffffc01,A30
           ZERO    .L1     A22
           MVKL    .S1     0xbe6bd0fc,A9
           ABSSP   .S1     A3,A8             ; |181| 
           SPDP    .S1     A8,A5:A4          ; |87| 
           MVKL    .S2     0xbe804d66,B30
           RCPDP   .S1     A5:A4,A5:A4       ; |87| 
           MVKH    .S2     0xbe804d66,B30

           CLR     .S1     A5,0,16,A17       ; |87| 
||         ZERO    .L1     A4                ; |87| 

           MV      .L1     A17,A5            ; |87| 
           MPYSPDP .M1     A8,A5:A4,A5:A4    ; |87| 
           MVKL    .S2     0xfefa39ec,B6
           MVKH    .S1     0xbe6bd0fc,A9
           MVKL    .S1     0x3eaaa872,A29
           MVKL    .S2     0x3fe62e42,B7
           MVKH    .S2     0xfefa39ec,B6
           MVKH    .S1     0x3eaaa872,A29
           FSUBDP  .L1     A5:A4,A7:A6,A7:A6 ; |87| 
           LDW     .D2T2   *B11,B28          ; |173| 
           MVKL    .S1     _ti_math_logtable,A28
           DPSP    .L1     A7:A6,A18         ; |87| 
           MVKH    .S2     0x3fe62e42,B7
           MVKH    .S1     _ti_math_logtable,A28
           EXTU    .S1     A17,1,21,A31      ; |87| 
           MPYSP   .M1     A18,A18,A16       ; |87| 
           MPYSP   .M2X    B31,A18,B5        ; |87| 
           ADD     .L1     A30,A31,A4        ; |87| 
           INTDP   .L1     A4,A5:A4          ; |87| 
           MPYSP   .M1     A9,A16,A19        ; |87| 

           FADDSP  .L2     B30,B5,B4         ; |87| 
||         MPYSP   .M1     A16,A18,A20       ; |87| 

           MVKL    .S2     0xbf000003,B29
           MVKH    .S2     0xbf000003,B29
           MPYSP   .M1     A16,A16,A18       ; |87| 

           FADDSP  .L1X    A19,B4,A17        ; |87| 
||         FMPYDP  .M2X    B7:B6,A5:A4,B5:B4 ; |87| 
||         EXTU    .S1     A17,12,29,A5      ; |87| 
||         MPYSP   .M1     A29,A20,A9        ; |87| 

           LDDW    .D1T1   *+A28[A5],A5:A4   ; |87| 
||         MPYSP   .M2X    B29,A16,B6        ; |87| 

           MVKL    .S1     0x3605fdf4,A27
           MPYSP   .M1     A18,A17,A16       ; |87| 
           MVKH    .S1     0x3605fdf4,A27
           FADDSP  .L2X    A9,B6,B6          ; |87| 
           FSUBDP  .L1X    A5:A4,B5:B4,A5:A4 ; |87| 
           MVKL    .S2     0x41b8aa3b,B27
           FADDSP  .L2X    A16,B6,B4         ; |87| 
           FADDDP  .L1     A5:A4,A7:A6,A5:A4 ; |87| 
           MVKH    .S2     0x41b8aa3b,B27
           SPDP    .S2     B4,B5:B4          ; |87| 
           MVKL    .S1     0x3effffff,A26
           FADDDP  .L2X    B5:B4,A5:A4,B5:B4 ; |87| 
           SPINT   .L2     B28,B18           ; |174| 
           MVKL    .S2     0x3d317000,B25
           DPSP    .L2     B5:B4,B4          ; |87| 
           MVKL    .S1     0x42b17218,A4
           MVKH    .S1     0x42b17218,A4
           CMPGTSP .S1X    A8,B12,A0         ; |89| 
   [ A0]   MV      .L2X    A4,B4             ; |90| 
           MPYSP   .M2     B28,B4,B6         ; |182| 
           MVKH    .S2     0x3d317000,B25
           MVKL    .S2     0x3e2abce4,B24
           CMPEQSP .S1X    A8,B10,A2         ; |183| 
   [ A2]   ZERO    .L2     B6
           MPYSP   .M2     B27,B6,B5         ; |121| 
           MVKH    .S2     0x3e2abce4,B24
           INTSP   .L2     B18,B1            ; |175| 
           MVKL    .S2     _ti_math_jTable,B22
           FADDSP  .L2     B13,B5,B4         ; |121| 
           MVKL    .S2     _ti_math_kTable,B2
           SPDP    .S2X    A3,B17:B16        ; |175| 
           SPTRUNC .L2     B4,B19            ; |121| 
           MVKH    .S2     _ti_math_jTable,B22
           MVKH    .S2     _ti_math_kTable,B2
           CMPLTSP .S2     B5,B26,B0         ; |123| 
   [ B0]   SUB     .L2     B19,1,B19         ; |124| 
           INTSP   .L2     B19,B5            ; |143| 
           MVKH    .S1     0x3effffff,A26
           MVKL    .S1     0x3f7ffffe,A25
           MVKH    .S1     0x3f7ffffe,A25
           MPYSP   .M2     B25,B5,B4         ; |143| 
           CMPEQSP .S2     B28,B1,B7         ; |175| 
           MPYSP   .M1X    A27,B5,A4         ; |143| 
           EXTU    .S2     B18,31,31,B29     ; |175| 
           FSUBSP  .L2     B6,B4,B4          ; |143| 
           EXTU    .S2     B6,1,24,B27       ; |146| 
           CMPLTSP .S1X    B6,A13,A0         ; |151| 
           FSUBSP  .L2X    B4,A4,B5          ; |143| 
           MVK     .S2     0x72,B26
           AND     .S2     3,B19,B23         ; |143| 
           MPYSP   .M2     B5,B5,B4          ; |143| 
           EXTU    .S2     B19,28,30,B21     ; |143| 
           EXTU    .S2     B19,16,20,B31     ; |143| 
           SHL     .S2     B31,20,B30        ; |143| 
           MPYSP   .M2     B4,B5,B8          ; |143| 
           ZERO    .L2     B25
           MPYSP   .M1X    A25,B5,A6         ; |143| 
           MPYSP   .M1X    A26,B4,A5         ; |143| 

           INTSP   .L2     B18,B22           ; |188| 
||         MPYSP   .M2     B24,B8,B4         ; |143| 
||         LDDW    .D2T2   *+B22[B23],B9:B8  ; |143| 

           MVKL    .S2     0x42b17218,B23
           ZERO    .L2     B24
           MVKH    .S2     0x42b17218,B23

           FADDSP  .L2X    A5,B4,B20         ; |143| 
||         LDDW    .D2T2   *+B2[B21],B5:B4   ; |143| 

           ZERO    .L2     B2
           ZERO    .L1     A5:A4             ; |175| 
           FADDSP  .L2X    B20,A6,B20        ; |143| 
           CMPLTDP .S2X    B17:B16,A5:A4,B21 ; |175| 

           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |143| 
||         SET     .S1     A24,0x14,0x1d,A5
||         ZERO    .L1     A4                ; |143| 

           SPDP    .S2     B20,B17:B16       ; |143| 
           ZERO    .L2     B20

           FADDDP  .L2X    A5:A4,B17:B16,B9:B8 ; |143| 
||         CMPLTSP .S2     B28,B25,B16       ; |193| 

           ADD     .L2     B30,B5,B5         ; |143| 
||         CMPLTSP .S2X    A3,B24,B17        ; |188| 

           CMPEQSP .S2     B28,B20,B1        ; |196| 

           FMPYDP  .M2     B5:B4,B9:B8,B5:B4 ; |143| 
||         AND     .L2     B7,B21,B8         ; |175| 

           AND     .L2     B29,B8,B0         ; |175| 
||         MVK     .S2     0x1,B8            ; |170| 

   [ B0]   ADD     .S2     -2,B8,B8
||         CMPLTU  .L2     B27,B26,B0        ; |146| 
||         ZERO    .D2     B26

           INTSP   .L2     B8,B19            ; |186| 
||         CMPLTSP .S2     B28,B26,B9        ; |192| 

           DPSP    .L2     B5:B4,B8          ; |143| 
||         CMPEQSP .S2X    A3,B2,B4          ; |192| 
||         XOR     .D2     1,B9,B31          ; |192| 
||         SET     .S1     A23,0x0,0x1e,A3
||         ADDAW   .D1     A14,3,A23

           LDW     .D1T1   *A23,A5           ; |173| 
||         AND     .L2     B4,B16,B2         ; |193| 

   [ B0]   FADDSP  .L2     B10,B6,B8         ; |147| 
           AND     .S2     B4,B31,B4         ; |192| 
           CMPGTSP .S2     B6,B23,B0         ; |156| 

   [ A0]   ZERO    .L2     B8
||         MV      .L1X    B4,A0             ; |188| 

   [ B0]   MV      .L2     B12,B8            ; |157| 
||         ADD     .L1     A15,A5,A4         ; |173| 
||         ADD     .S1     4,A15,A15         ; |174| 

           MPYSP   .M2     B8,B19,B5         ; |186| 
           CMPEQSP .S2     B28,B22,B8        ; |188| 
           XOR     .L2     1,B8,B7           ; |188| 
           AND     .L2     B7,B17,B0         ; |188| 

   [ B0]   MV      .L2X    A3,B5             ; |189| 
||         SET     .S1     A22,0x17,0x1e,A3

   [ A0]   ZERO    .L2     B5
||         SUB     .L1     A21,1,A0          ; |168| 

   [ A0]   B       .S1     $C$L9             ; |168| 
|| [ B2]   MV      .L2X    A3,B5             ; |193| 
||         STW     .D2T1   A0,*+SP(16)       ; |173| 

   [ B1]   MV      .L2     B10,B5            ; |197| 
|| [!A0]   MV      .S2X    A14,B11

           STW     .D1T2   B5,*A4            ; |173| 

   [!A0]   CALL    .S1     _powsp_v          ; |177| 
|| [ A0]   LDW     .D2T2   *++B11,B4         ; |170| 
|| [ A0]   LDW     .D1T1   *++A11,A3         ; |170| 

   [ A0]   CALL    .S1     _pow              ; |170| 
|| [!A0]   LDW     .D2T1   *+B11(16),A6      ; |177| 

   [!A0]   MVKL    .S1     _a_sc,A4
           ; BRANCHCC OCCURS {$C$L9}         ; |168| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 3
           MVK     .S2     0x24,B6           ; |177| 
           MV      .L2X    A12,B4            ; |177| 

           ADDKPC  .S2     $C$RL14,B3,0      ; |177| 
||         MVKH    .S1     _a_sc,A4

$C$RL14:   ; CALL OCCURS {_powsp_v} {0}      ; |177| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 46
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           LDW     .D2T1   *B11,A4           ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T2   *+B11(4),B4       ; |66| 
||         MVK     .S1     0x24,A6           ; |66| 
||         MVK     .L1     0x9,A8            ; |66| 

$C$RL15:   ; CALL OCCURS {_isequal} {0}      ; |66| 
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
||         MVK     .S1     0x24,A6           ; |67| 
||         MVK     .L1     0x9,A8            ; |67| 

$C$RL16:   ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     _fcn_pass,B5
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     _fcn_pass,B5
           MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B5(8)        ; |67| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A14,A4           ; |68| 
||         LDW     .D2T2   *+B11(12),B4      ; |68| 
||         MVK     .S1     0x24,A6           ; |68| 
||         MVK     .L1     0x9,A8            ; |68| 

$C$RL17:   ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     _fcn_pass,B4
           MVKL    .S2     0xeb1c432d,B6
           MVKL    .S2     0x3f1a36e2,B7
           MVKH    .S2     _fcn_pass,B4
           MVKH    .S2     0xeb1c432d,B6

           STW     .D2T1   A4,*+B4(12)       ; |68| 
||         LDW     .D1T2   *A10,B4           ; |69| 
||         MVKH    .S2     0x3f1a36e2,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A14,A4           ; |69| 
||         MVK     .S1     0x24,A6           ; |69| 
||         MVK     .L1     0x9,A8            ; |69| 

$C$RL18:   ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           LDW     .D2T1   *+SP(28),A12      ; |69| 
           LDW     .D2T1   *+SP(32),A3
           MVKL    .S2     _fcn_pass,B4
           MVKH    .S2     _fcn_pass,B4
           STW     .D2T1   A4,*+B4(16)       ; |69| 
           LDW     .D1T1   *A12,A0           ; |72| 
           STW     .D2T1   A3,*+SP(36)       ; |72| 
           NOP             3

   [!A0]   B       .S1     $C$L12            ; |72| 
|| [ A0]   MV      .L2X    A3,B4

   [!A0]   CALL    .S1     _print_test_results ; |181| 
|| [ A0]   LDW     .D2T2   *B4,B0            ; |72| 

   [!A0]   ZERO    .L1     A4                ; |72| 
   [!A0]   MVKL    .S1     _all_pass,A3
   [!A0]   MVKH    .S1     _all_pass,A3
   [ A0]   ADD     .L2     4,B4,B4
           ; BRANCHCC OCCURS {$C$L12}        ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 10
   [ B0]   LDW     .D2T2   *B4,B4            ; |72| 
           CMPEQ   .L1     A4,0,A3           ; |72| 
           XOR     .L1     1,A3,A3           ; |72| 
           MVK     .L1     0x1,A4            ; |72| 
   [!B0]   ZERO    .L1     A0                ; |72| 
   [ B0]   CMPEQ   .L2     B4,0,B4           ; |72| 
   [ B0]   XOR     .L2     1,B4,B4           ; |72| 
           NOP             1
   [ B0]   AND     .L1X    A3,B4,A0          ; |72| 
   [!A0]   ZERO    .L1     A4                ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           CALL    .S1     _print_test_results ; |181| 
           MVKL    .S1     _all_pass,A3
           MVKH    .S1     _all_pass,A3
           NOP             2
;** --------------------------------------------------------------------------*
$C$L12:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A4,*A3            ; |72| 
||         MVK     .L1     0x3,A4            ; |181| 
||         ADDKPC  .S2     $C$RL19,B3,0      ; |181| 

$C$RL19:   ; CALL OCCURS {_print_test_results} {0}  ; |181| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 15
           MVKL    .S1     _b_ext,A3
           MVKH    .S1     _b_ext,A3
           MVK     .S1     0xed,A31
           MV      .L2X    A3,B4             ; |185| 

           MV      .L2X    A3,B11            ; |185| 
||         MVKL    .S1     _a_ext,A3

           MVKH    .S1     _a_ext,A3
||         ZERO    .L1     A4

           SET     .S1     A4,0x17,0x17,A4
||         STW     .D2T1   A31,*+SP(16)      ; |184| 

           MV      .L2X    A3,B5             ; |184| 
||         STW     .D2T1   A4,*B4            ; |185| 

           STW     .D2T1   A4,*B5            ; |184| 
           LDW     .D2T2   *B11,B4           ; |187| 
           MV      .L1     A3,A11            ; |184| 
           LDW     .D1T1   *A11,A3           ; |187| 
           ZERO    .L1     A15
           NOP             1

           CALL    .S1     _pow              ; |187| 
||         SPDP    .S2     B4,B5:B4          ; |187| 

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains control code
;*----------------------------------------------------------------------------*
$C$L13:    
;          EXCLUSIVE CPU CYCLES: 5
           NOP             3
           SPDP    .S1     A3,A5:A4          ; |187| 
           ADDKPC  .S2     $C$RL20,B3,0      ; |187| 
$C$RL20:   ; CALL OCCURS {_pow} {0}          ; |187| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 23
           MV      .L2X    A14,B4            ; |187| 
           LDW     .D2T2   *B4,B4            ; |187| 
           DPSP    .L1     A5:A4,A3          ; |187| 
           ZERO    .L1     A5
           MVKH    .S1     0x38100000,A5
           NOP             1
           ADD     .L1X    A15,B4,A4         ; |187| 
           STW     .D1T1   A3,*A4            ; |187| 
           LDW     .D2T2   *B11,B4           ; |188| 
           MVKL    .S1     0xa639b,A4
           MVKH    .S1     0xa639b,A4
           NOP             2
           SPDP    .S2     B4,B7:B6          ; |188| 
           NOP             1
           CMPGTDP .S2X    B7:B6,A5:A4,B0    ; |188| 
           NOP             1

   [!B0]   B       .S1     $C$L14            ; |188| 
|| [!B0]   LDW     .D1T1   *A11,A3           ; |188| 

   [ B0]   CALL    .S1     _powsp            ; |188| 
|| [ B0]   LDW     .D1T1   *A11,A4           ; |188| 

   [!B0]   CALL    .S1     _pow              ; |188| 
           NOP             2
   [!B0]   SPDP    .S1     A3,A5:A4          ; |188| 
           ; BRANCHCC OCCURS {$C$L14}        ; |188| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1
           ADDKPC  .S2     $C$RL21,B3,0      ; |188| 
$C$RL21:   ; CALL OCCURS {_powsp} {0}        ; |188| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 6

           BNOP    .S2     $C$L15,1          ; |188| 
||         SPDP    .S1     A4,A5:A4          ; |188| 

           DPSP    .L1     A5:A4,A3          ; |188| 
||         LDW     .D1T1   *+A14(4),A4       ; |188| 

           NOP             3
           ; BRANCH OCCURS {$C$L15}          ; |188| 
;** --------------------------------------------------------------------------*
$C$L14:    
;          EXCLUSIVE CPU CYCLES: 2
           DADD    .L2     0,B7:B6,B5:B4     ; |188| 
           ADDKPC  .S2     $C$RL22,B3,0      ; |188| 
$C$RL22:   ; CALL OCCURS {_pow} {0}          ; |188| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 4

           DPSP    .L1     A5:A4,A3          ; |188| 
||         LDW     .D1T1   *+A14(4),A4       ; |188| 

           NOP             3
;** --------------------------------------------------------------------------*
$C$L15:    
;          EXCLUSIVE CPU CYCLES: 8
           NOP             1
           ADD     .L1     A15,A4,A4         ; |188| 

           STW     .D1T1   A3,*A4            ; |188| 
||         CALL    .S1     _powsp_c          ; |189| 

           LDW     .D1T1   *A11,A4           ; |189| 
||         LDW     .D2T2   *B11,B4           ; |189| 

           ADDKPC  .S2     $C$RL23,B3,3      ; |189| 
$C$RL23:   ; CALL OCCURS {_powsp_c} {0}      ; |189| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 155

           LDW     .D1T1   *+A14(8),A3       ; |189| 
||         ZERO    .L1     A9
||         ZERO    .S1     A8                ; |87| 
||         MVKL    .S2     0x3e436581,B4
||         LDW     .D2T2   *+SP(12),B28

           SET     .S1     A9,0x14,0x1d,A9
||         MVKH    .S2     0x3e436581,B4
||         ZERO    .L1     A21
||         ADDAW   .D1     A14,3,A1

           MVK     .S1     0xfffffc01,A30
||         MVKL    .S2     0xbe804d66,B31

           MVKL    .S1     0xbe6bd0fc,A29
||         MVKH    .S2     0xbe804d66,B31

           MVKH    .S1     0xbe6bd0fc,A29
||         MVKL    .S2     0x3fe62e42,B7

           ADD     .L1     A15,A3,A3         ; |189| 
||         MVKL    .S2     0xfefa39ec,B6
||         MVKL    .S1     0x3eaaa872,A28

           STW     .D1T1   A4,*A3            ; |189| 
||         MVKH    .S1     0x3eaaa872,A28
||         MVKH    .S2     0x3fe62e42,B7

           LDW     .D2T2   *B11,B19          ; |190| 
||         LDW     .D1T1   *A11,A3           ; |190| 

           MVKL    .S1     _ti_math_logtable,A25
           MVKH    .S2     0xfefa39ec,B6
           MVKH    .S1     _ti_math_logtable,A25
           MVKL    .S1     0x3605fdf4,A24
           ABSSP   .S1     A3,A16            ; |181| 
           SPDP    .S1     A16,A5:A4         ; |87| 
           MVKL    .S2     0xbf000003,B30
           RCPDP   .S1     A5:A4,A5:A4       ; |87| 
           MVKH    .S2     0xbf000003,B30

           ZERO    .L1     A4                ; |87| 
||         CLR     .S1     A5,0,16,A5        ; |87| 

           MPYSPDP .M1     A16,A5:A4,A7:A6   ; |87| 
           MVKH    .S1     0x3605fdf4,A24
           MVKL    .S2     0x41b8aa3b,B29
           MVKL    .S1     0x3effffff,A23
           MVKH    .S2     0x41b8aa3b,B29
           SPINT   .L2     B19,B20           ; |174| 
           MVKL    .S2     0x3d317000,B27
           FSUBDP  .L1     A7:A6,A9:A8,A9:A8 ; |87| 
           MVKH    .S2     0x3d317000,B27
           CMPGTSP .S2X    A16,B12,B0        ; |89| 
           DPSP    .L1     A9:A8,A18         ; |87| 
           MVKL    .S2     0x3e2abce4,B26
           MVKH    .S2     0x3e2abce4,B26
           EXTU    .S1     A5,1,21,A31       ; |87| 
           MPYSP   .M1     A18,A18,A17       ; |87| 
           MPYSP   .M2X    B4,A18,B5         ; |87| 
           ADD     .L1     A30,A31,A4        ; |87| 
           INTDP   .L1     A4,A7:A6          ; |87| 
           MPYSP   .M1     A29,A17,A19       ; |87| 

           FADDSP  .L2     B31,B5,B4         ; |87| 
||         MPYSP   .M1     A17,A18,A18       ; |87| 

           EXTU    .S1     A5,12,29,A27      ; |87| 
           LDDW    .D1T1   *+A25[A27],A5:A4  ; |87| 
           MPYSP   .M1     A17,A17,A20       ; |87| 

           FADDSP  .L1X    A19,B4,A7         ; |87| 
||         FMPYDP  .M2X    B7:B6,A7:A6,B5:B4 ; |87| 
||         MPYSP   .M1     A28,A18,A26       ; |87| 

           MPYSP   .M2X    B30,A17,B6        ; |87| 
           INTSP   .L1X    B20,A2            ; |175| 
           MPYSP   .M1     A20,A7,A7         ; |87| 
           MVKL    .S2     _ti_math_kTable,B24
           FADDSP  .L2X    A26,B6,B6         ; |87| 
           FSUBDP  .L1X    A5:A4,B5:B4,A5:A4 ; |87| 
           MVKL    .S2     _ti_math_jTable,B22
           FADDSP  .L2X    A7,B6,B4          ; |87| 
           FADDDP  .L1     A5:A4,A9:A8,A5:A4 ; |87| 
           MVKH    .S2     _ti_math_kTable,B24
           SPDP    .S2     B4,B5:B4          ; |87| 
           ZERO    .L1     A31
           FADDDP  .L2X    B5:B4,A5:A4,B5:B4 ; |87| 
           MVKH    .S2     _ti_math_jTable,B22
           CMPEQSP .S1X    A16,B10,A0        ; |183| 
           DPSP    .L2     B5:B4,B4          ; |87| 
           MVKL    .S1     0x42b17218,A4
           MVKH    .S1     0x42b17218,A4
           EXTU    .S2     B20,31,31,B1      ; |175| 
   [ B0]   MV      .L2X    A4,B4             ; |90| 
           MPYSP   .M2     B19,B4,B18        ; |182| 
           MVKH    .S1     0x3effffff,A23
           MVKL    .S1     0x3f7ffffe,A22
           MVKH    .S1     0x3f7ffffe,A22
   [ A0]   ZERO    .L2     B18
           MPYSP   .M2     B29,B18,B5        ; |121| 
           CMPEQSP .S1     A3,A31,A30        ; |192| 
           ZERO    .L1     A29
           CMPEQSP .S2X    B19,A2,B31        ; |175| 
           FADDSP  .L2     B13,B5,B4         ; |121| 
           LDW     .D2T1   *+SP(16),A19
           ZERO    .L1     A28
           SPTRUNC .L2     B4,B21            ; |121| 
           EXTU    .S2     B18,1,24,B30      ; |146| 
           CMPLTSP .S1X    B18,A13,A0        ; |151| 
           CMPLTSP .S2     B5,B28,B0         ; |123| 
   [ B0]   SUB     .L2     B21,1,B21         ; |124| 
           INTSP   .L2     B21,B5            ; |143| 
           MVK     .S2     0x72,B29
           ZERO    .L2     B28
           EXTU    .S2     B21,28,30,B25     ; |143| 
           MPYSP   .M2     B27,B5,B4         ; |143| 
           AND     .L2     3,B21,B23         ; |143| 
           MPYSP   .M1X    A24,B5,A4         ; |143| 
           LDDW    .D2T2   *+B24[B25],B9:B8  ; |143| 
           FSUBSP  .L2     B18,B4,B4         ; |143| 
           ZERO    .L2     B25
           EXTU    .S2     B21,16,20,B2      ; |143| 
           FSUBSP  .L2X    B4,A4,B5          ; |143| 
           LDDW    .D2T2   *+B22[B23],B17:B16 ; |143| 
           ZERO    .S2     B21
           MPYSP   .M2     B5,B5,B4          ; |143| 
           ZERO    .L2     B22
           MVKL    .S2     0x42b17218,B27
           MVKH    .S2     0x42b17218,B27
           MPYSP   .M2     B4,B5,B6          ; |143| 
           FMPYDP  .M2     B17:B16,B9:B8,B9:B8 ; |143| 
           SHL     .S2     B2,20,B16         ; |143| 
           MPYSP   .M1X    A23,B4,A5         ; |143| 
           MPYSP   .M2     B26,B6,B6         ; |143| 
           CMPLTSP .S2     B19,B22,B17       ; |193| 
           MPYSP   .M1X    A22,B5,A4         ; |143| 
           ADD     .L2     B16,B9,B9         ; |143| 
           FADDSP  .L2X    A5,B6,B6          ; |143| 
           INTSP   .L2     B20,B26           ; |188| 
           SET     .S1     A21,0x14,0x1d,A5
           FADDSP  .L2X    B6,A4,B4          ; |143| 
           SPDP    .S2X    A3,B7:B6          ; |175| 
           ZERO    .L1     A4                ; |143| 
           SPDP    .S2     B4,B5:B4          ; |143| 
           ZERO    .L2     B16

           ZERO    .L1     A5:A4             ; |175| 
||         FADDDP  .L2X    A5:A4,B5:B4,B5:B4 ; |143| 

           CMPEQSP .S2     B19,B26,B23       ; |188| 

           CMPLTDP .S2X    B7:B6,A5:A4,B6    ; |175| 
||         LDW     .D1T1   *A1,A5            ; |190| 

           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |143| 
||         XOR     .L2     1,B23,B9          ; |188| 

           CMPLTSP .S2     B19,B28,B8        ; |192| 

           XOR     .L2     1,B8,B24          ; |192| 
||         CMPLTSP .S2X    A3,B25,B7         ; |188| 
||         SET     .S1     A29,0x0,0x1e,A3

           AND     .L2     B9,B7,B2          ; |188| 
||         MVKL    .S2     0xc000cccc,B7

           DPSP    .L2     B5:B4,B6          ; |143| 
||         AND     .S2     B31,B6,B5         ; |175| 
||         ADD     .L1     A15,A5,A29        ; |190| 

           AND     .S2     B1,B5,B0          ; |175| 
||         MVK     .D2     0x1,B5            ; |170| 
||         CMPLTU  .L2     B30,B29,B1        ; |146| 

   [ B1]   FADDSP  .L2     B10,B18,B6        ; |147| 
|| [ B0]   ADD     .S2     -2,B5,B5

           INTSP   .L2     B5,B5             ; |186| 
           CMPGTSP .S2     B18,B27,B0        ; |156| 
   [ A0]   ZERO    .L2     B6
   [ B0]   MV      .S2     B12,B6            ; |157| 
           MPYSP   .M2     B6,B5,B5          ; |186| 
           AND     .L2X    A30,B17,B1        ; |193| 
           AND     .L1X    A30,B24,A0        ; |192| 
           CMPEQSP .S2     B19,B21,B0        ; |196| 

   [ B2]   MV      .L2X    A3,B5             ; |189| 
||         SET     .S1     A28,0x17,0x1e,A3

   [ A0]   ZERO    .L2     B5
   [ B1]   MV      .L2X    A3,B5             ; |193| 
   [ B0]   MV      .L2     B10,B5            ; |197| 
           STW     .D1T2   B5,*A29           ; |190| 

           LDW     .D1T1   *A11,A3           ; |191| 
||         LDW     .D2T2   *B11,B5           ; |192| 

           MVKH    .S2     0xc000cccc,B7
           MVKL    .S2     0xcccccccd,B6
           MVKH    .S2     0xcccccccd,B6
           MVKH    .S2     0xbfc00000,B16

           MPYSPDP .M1X    A3,B7:B6,A5:A4    ; |191| 
||         MPYSP   .M2     B16,B5,B4         ; |192| 

           ADD     .L1     4,A15,A15         ; |186| 
           SUB     .L1     A19,1,A0          ; |186| 
           NOP             1
           STW     .D2T2   B4,*++B11         ; |192| 
   [ A0]   LDW     .D2T2   *B11,B4           ; |187| 
   [!A0]   MV      .L2X    A14,B11           ; |194| 
           DPSP    .L1     A5:A4,A4          ; |191| 
   [ A0]   BNOP    .S1     $C$L13,1          ; |186| 
   [ A0]   SPDP    .S2     B4,B5:B4          ; |187| 
           STW     .D1T1   A4,*++A11         ; |191| 
   [ A0]   LDW     .D1T1   *A11,A3           ; |187| 

   [ A0]   CALL    .S1     _pow              ; |187| 
||         STW     .D2T1   A0,*+SP(16)       ; |191| 

           ; BRANCHCC OCCURS {$C$L13}        ; |186| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 6

           LDW     .D2T2   *+B11(16),B5      ; |194| 
||         CALL    .S1     _powsp_v          ; |194| 
||         MVKL    .S2     _b_ext,B4

           MVKH    .S2     _b_ext,B4
           MVKL    .S1     _a_ext,A4
           MVKH    .S1     _a_ext,A4
           MVK     .S2     0xed,B6           ; |194| 

           MV      .L1X    B5,A6             ; |194| 
||         ADDKPC  .S2     $C$RL24,B3,0      ; |194| 

$C$RL24:   ; CALL OCCURS {_powsp_v} {0}      ; |194| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 47
           LDW     .D2T2   *B11,B5           ; |66| 
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7
           MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T2   *+B11(4),B4       ; |66| 
||         MV      .L1X    B5,A4             ; |66| 
||         MVK     .S1     0xed,A6           ; |66| 
||         MVK     .D1     0x9,A8            ; |66| 

$C$RL25:   ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     _fcn_pass,B5
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     _fcn_pass,B5

           LDW     .D2T2   *+B11(8),B4       ; |67| 
||         MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B5(4)        ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B11,A4           ; |67| 
||         MVK     .S1     0xed,A6           ; |67| 
||         MVK     .L1     0x9,A8            ; |67| 

$C$RL26:   ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     _fcn_pass,B5
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     _fcn_pass,B5
           MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B5(8)        ; |67| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A14,A4           ; |68| 
||         LDW     .D2T2   *+B11(12),B4      ; |68| 
||         MVK     .S1     0xed,A6           ; |68| 
||         MVK     .L1     0x9,A8            ; |68| 

$C$RL27:   ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     _fcn_pass,B4
           MVKL    .S2     0xeb1c432d,B6
           MVKL    .S2     0x3f1a36e2,B7
           MVKH    .S2     _fcn_pass,B4

           STW     .D2T1   A4,*+B4(12)       ; |68| 
||         MVKH    .S2     0xeb1c432d,B6

           MV      .L1     A10,A3            ; |68| 
||         LDW     .D1T1   *A14,A4           ; |69| 
||         MVKH    .S2     0x3f1a36e2,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D1T2   *A3,B4            ; |69| 
||         MVK     .S1     0xed,A6           ; |69| 
||         MVK     .L1     0x9,A8            ; |69| 

$C$RL28:   ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11

           LDW     .D1T1   *A12,A0           ; |72| 
||         LDW     .D2T2   *+SP(36),B5

           MVKL    .S2     _fcn_pass,B4
           MVKH    .S2     _fcn_pass,B4
           STW     .D2T1   A4,*+B4(16)       ; |69| 
           NOP             1

   [!A0]   B       .S1     $C$L16            ; |72| 
|| [ A0]   LDW     .D2T2   *B5,B0            ; |72| 

   [!A0]   CALL    .S1     _print_test_results ; |198| 
   [ A0]   ADD     .L2     4,B5,B4
   [!A0]   ZERO    .L1     A4                ; |72| 
   [!A0]   MVKL    .S1     _all_pass,A3
   [!A0]   MVKH    .S1     _all_pass,A3
           ; BRANCHCC OCCURS {$C$L16}        ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 10
   [ B0]   LDW     .D2T2   *B4,B4            ; |72| 
           CMPEQ   .L1     A4,0,A3           ; |72| 
           XOR     .L1     1,A3,A3           ; |72| 
   [!B0]   ZERO    .L1     A0                ; |72| 
           MVK     .L1     0x1,A4            ; |72| 
   [ B0]   CMPEQ   .L2     B4,0,B4           ; |72| 
   [ B0]   XOR     .L2     1,B4,B4           ; |72| 
           NOP             1
   [ B0]   AND     .L1X    A3,B4,A0          ; |72| 
   [!A0]   ZERO    .L1     A4                ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           CALL    .S1     _print_test_results ; |198| 
           MVKL    .S1     _all_pass,A3
           MVKH    .S1     _all_pass,A3
           NOP             2
;** --------------------------------------------------------------------------*
$C$L16:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A4,*A3            ; |72| 
||         MVK     .L1     0x4,A4            ; |198| 
||         ADDKPC  .S2     $C$RL29,B3,0      ; |198| 

$C$RL29:   ; CALL OCCURS {_print_test_results} {0}  ; |198| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 14

           ZERO    .L2     B5                ; |114| 
||         MVKL    .S2     _cycle_counts+24,B7

           MVKH    .S2     _cycle_counts+24,B7
||         ZERO    .L2     B6                ; |114| 
||         ZERO    .D2     B4                ; |114| 

           MVC     .S2     B6,TSCL           ; |115| 
||         STDW    .D2T2   B5:B4,*B7         ; |114| 

           MVC     .S2     TSCL,B8           ; |116| 
           MVC     .S2     TSCH,B6           ; |116| 
           MVKL    .S1     _t_start,A3

           MVKH    .S1     _t_start,A3
||         MV      .L2     B6,B9             ; |116| 

           MVC     .S2     TSCL,B6           ; |117| 
||         STDW    .D1T2   B9:B8,*A3         ; |116| 

           MVC     .S2     TSCH,B16          ; |117| 

           CALL    .S2     _gimme_random     ; |208| 
||         MVKL    .S1     _t_offset,A4

           SUBU    .L2     B6,B8,B7:B6       ; |117| 
||         MVK     .S1     0x80,A11
||         ZERO    .L1     A12

           EXT     .S2     B7,24,24,B8       ; |117| 
||         MVKH    .S1     0x41a00000,A12
||         SUB     .L2     B16,B9,B31        ; |117| 

           ADD     .L2     B31,B8,B7         ; |117| 
||         ZERO    .S2     B4
||         MVKH    .S1     _t_offset,A4
||         STDW    .D2T2   B5:B4,*+SP(40)    ; |117| 

           MV      .L1     A12,A4            ; |208| 
||         STDW    .D1T2   B7:B6,*A4         ; |117| 
||         MVKH    .S2     0xc1200000,B4
||         MV      .S1X    B5,A15

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L17:    
;          EXCLUSIVE CPU CYCLES: 1
           ADDKPC  .S2     $C$RL30,B3,0      ; |208| 
$C$RL30:   ; CALL OCCURS {_gimme_random} {0}  ; |208| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 18
           MVKL    .S2     _input,B4
           MVKH    .S2     _input,B4
           NOP             1
           MV      .L1X    B4,A14            ; |208| 
           LDW     .D1T2   *A14,B4           ; |208| 
           NOP             4
           ADD     .L2X    A15,B4,B4         ; |208| 

           STW     .D2T1   A4,*B4            ; |208| 
||         ZERO    .L2     B4

           MVKH    .S2     0xc1200000,B4

           CALLP   .S2     _gimme_random,B3
||         MV      .L1     A12,A4            ; |209| 

$C$RL31:   ; CALL OCCURS {_gimme_random} {0}  ; |209| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 161
           LDW     .D1T1   *+A14(4),A3       ; |209| 
           NOP             4
           ADD     .L1     A15,A3,A3         ; |209| 

           STW     .D1T1   A4,*A3            ; |209| 
||         MVC     .S2     TSCL,B6           ; |122| 

           MVC     .S2     TSCH,B7           ; |122| 

           LDW     .D1T1   *A14,A3           ; |213| 
||         ZERO    .L2     B5
||         ZERO    .D2     B4                ; |87| 
||         MVKL    .S2     0x3e436581,B31
||         MVKL    .S1     0xbe6bd0fc,A31
||         SPINT   .L1     A4,A24            ; |174| 

           SET     .S2     B5,0x14,0x1d,B5
||         MVKH    .S1     0xbe6bd0fc,A31
||         LDW     .D2T2   *+SP(12),B27

           MVKH    .S2     0x3e436581,B31
||         MVK     .S1     0xfffffc01,A16

           MVKL    .S1     0x3eaaa872,A23
           MVKL    .S2     0xbe804d66,B30
           ADD     .L1     A15,A3,A3         ; |213| 
           LDW     .D1T1   *A3,A3            ; |213| 
           INTSP   .L1     A24,A2            ; |175| 
           MVKH    .S1     0x3eaaa872,A23
           MVKL    .S1     _ti_math_logtable,A28
           MVKH    .S1     _ti_math_logtable,A28
           ABSSP   .S1     A3,A5             ; |181| 
           SPDP    .S1     A5,A7:A6          ; |87| 
           MVKH    .S2     0xbe804d66,B30
           RCPDP   .S1     A7:A6,A7:A6       ; |87| 
           MVKL    .S2     0xbf000003,B29

           CLR     .S1     A7,0,16,A18       ; |87| 
||         ZERO    .L1     A6                ; |87| 

           MV      .L1     A18,A7            ; |87| 
           MPYSPDP .M1     A5,A7:A6,A7:A6    ; |87| 
           MVKH    .S2     0xbf000003,B29
           MVKL    .S2     0x41b8aa3b,B28
           MVKL    .S1     0x3fe62e42,A17
           MVKH    .S2     0x41b8aa3b,B28
           MVKH    .S1     0x3fe62e42,A17
           MVKL    .S1     0x3605fdf4,A27
           FSUBDP  .L1X    A7:A6,B5:B4,A9:A8 ; |87| 
           MVKL    .S2     0x3d317000,B26
           MVKH    .S2     0x3d317000,B26
           DPSP    .L1     A9:A8,A6          ; |87| 
           MVKL    .S2     0x3e2abce4,B25
           MVKH    .S2     0x3e2abce4,B25
           CMPGTSP .S2X    A5,B12,B0         ; |89| 
           MPYSP   .M1     A6,A6,A22         ; |87| 
           MPYSP   .M2X    B31,A6,B4         ; |87| 
           EXTU    .S1     A18,1,21,A30      ; |87| 
           EXTU    .S1     A18,12,29,A29     ; |87| 
           MPYSP   .M1     A31,A22,A19       ; |87| 

           FADDSP  .L2     B30,B4,B4         ; |87| 
||         MPYSP   .M1     A22,A6,A20        ; |87| 
||         ADD     .L1     A16,A30,A6        ; |87| 
||         MVKL    .S1     0xfefa39ec,A16

           INTDP   .L1     A6,A7:A6          ; |87| 
           MVKH    .S1     0xfefa39ec,A16
           MPYSP   .M1     A22,A22,A21       ; |87| 

           LDDW    .D1T1   *+A28[A29],A19:A18 ; |87| 
||         MPYSP   .M1     A23,A20,A23       ; |87| 
||         FADDSP  .L1X    A19,B4,A20        ; |87| 

           MPYSP   .M2X    B29,A22,B4        ; |87| 
           FMPYDP  .M1     A17:A16,A7:A6,A7:A6 ; |87| 
           MPYSP   .M1     A21,A20,A16       ; |87| 
           CMPEQSP .S2X    A5,B10,B2         ; |183| 
           FADDSP  .L2X    A23,B4,B4         ; |87| 
           FSUBDP  .L1     A19:A18,A7:A6,A7:A6 ; |87| 
           MVKL    .S2     _ti_math_kTable,B24
           FADDSP  .L2X    A16,B4,B4         ; |87| 
           FADDDP  .L1     A7:A6,A9:A8,A7:A6 ; |87| 
           MVKL    .S2     _ti_math_jTable,B1
           SPDP    .S2     B4,B5:B4          ; |87| 
           MVKH    .S1     0x3605fdf4,A27
           FADDDP  .L2X    B5:B4,A7:A6,B5:B4 ; |87| 
           MVKH    .S2     _ti_math_kTable,B24
           MVKH    .S2     _ti_math_jTable,B1
           DPSP    .L2     B5:B4,B4          ; |87| 
           MVKL    .S1     0x42b17218,A6
           MVKH    .S1     0x42b17218,A6
           MVKL    .S1     0x3effffff,A26
   [ B0]   MV      .L2X    A6,B4             ; |90| 
           MPYSP   .M2X    A4,B4,B20         ; |182| 
           MVKH    .S1     0x3effffff,A26
           MVKL    .S1     0x3f7ffffe,A25
           MVKH    .S1     0x3f7ffffe,A25
   [ B2]   ZERO    .L2     B20
           MPYSP   .M2     B28,B20,B5        ; |121| 
           EXTU    .S1     A24,31,31,A1      ; |175| 
           ZERO    .L2     B31
           ZERO    .L1     A30
           FADDSP  .L2     B13,B5,B4         ; |121| 
           ZERO    .L1     A29
           INTSP   .L2X    A24,B2            ; |188| 
           SPTRUNC .L2     B4,B21            ; |121| 
           EXTU    .S2     B20,1,24,B28      ; |146| 
           CMPLTSP .S2     B5,B27,B0         ; |123| 
           MVK     .S2     0x72,B27

   [ B0]   SUB     .L2     B21,1,B21         ; |124| 
||         ZERO    .S2     B0

           INTSP   .L2     B21,B5            ; |143| 
           EXTU    .S2     B21,28,30,B16     ; |143| 
           AND     .L2     3,B21,B23         ; |143| 
           LDDW    .D2T2   *+B24[B16],B17:B16 ; |143| 
           MPYSP   .M2     B26,B5,B4         ; |143| 
           ZERO    .L2     B24
           MPYSP   .M1X    A27,B5,A5         ; |143| 
           EXTU    .S2     B21,16,20,B29     ; |143| 
           FSUBSP  .L2     B20,B4,B4         ; |143| 
           LDDW    .D2T2   *+B1[B23],B19:B18 ; |143| 
           MVKL    .S2     _t_start,B23
           FSUBSP  .L2X    B4,A5,B8          ; |143| 
           MVKH    .S2     _t_start,B23
           MVKL    .S2     0x42b17218,B26
           MPYSP   .M2     B8,B8,B4          ; |143| 
           FMPYDP  .M2     B19:B18,B17:B16,B17:B16 ; |143| 
           MVKH    .S2     0x42b17218,B26
           SHL     .S2     B29,20,B18        ; |143| 
           MPYSP   .M2     B4,B8,B5          ; |143| 
           ADD     .L2     B18,B17,B17       ; |143| 
           MPYSP   .M1X    A25,B8,A5         ; |143| 
           MPYSP   .M1X    A26,B4,A6         ; |143| 
           MPYSP   .M2     B25,B5,B9         ; |143| 
           ZERO    .L2     B25
           SPDP    .S2X    A3,B5:B4          ; |175| 
           CMPLTSP .S1X    A4,B25,A31        ; |192| 
           FADDSP  .L2X    A6,B9,B22         ; |143| 
           SET     .S2     B31,0x14,0x1d,B9
           ZERO    .L1     A7:A6             ; |175| 

           FADDSP  .L2X    B22,A5,B8         ; |143| 
||         CMPEQSP .S1     A4,A2,A5          ; |175| 
||         ZERO    .S2     B22

           CMPLTDP .S2X    B5:B4,A7:A6,B30   ; |175| 
           CMPLTSP .S1X    B20,A13,A2        ; |151| 

           SPDP    .S2     B8,B5:B4          ; |143| 
||         ZERO    .L2     B8                ; |143| 

           AND     .L1X    A5,B30,A5         ; |175| 

           CMPLTSP .S2X    A3,B22,B31        ; |188| 
||         FADDDP  .L2     B9:B8,B5:B4,B5:B4 ; |143| 
||         AND     .L1     A1,A5,A0          ; |175| 

           MVK     .L1     0x1,A5            ; |170| 
   [ A0]   ADD     .L1     -2,A5,A5

           FMPYDP  .M2     B17:B16,B5:B4,B5:B4 ; |143| 
||         CMPLTSP .S2X    A4,B0,B17         ; |193| 
||         CMPLTU  .L2     B28,B27,B0        ; |146| 

           INTSP   .L2X    A5,B18            ; |186| 
           CMPEQSP .S2X    A4,B2,B16         ; |188| 
           XOR     .L2     1,B16,B21         ; |188| 

           DPSP    .L2     B5:B4,B5          ; |143| 
||         CMPEQSP .S2X    A3,B24,B4         ; |192| 

           SET     .S1     A30,0x0,0x1e,A3
   [ B0]   FADDSP  .L2     B10,B20,B5        ; |147| 
           AND     .S2     B4,B17,B1         ; |193| 
           CMPGTSP .S2     B20,B26,B0        ; |156| 
   [ A2]   ZERO    .L2     B5

   [ B0]   MV      .L2     B12,B5            ; |157| 
||         CMPEQSP .S2X    A4,B25,B0         ; |196| 
||         XOR     .L1     1,A31,A4          ; |192| 
||         MVKL    .S1     _output+12,A31

           MPYSP   .M2     B5,B18,B5         ; |186| 
||         MVKH    .S1     _output+12,A31

           AND     .L2X    B4,A4,B2          ; |192| 
||         AND     .S2     B21,B31,B4        ; |188| 
||         LDW     .D1T1   *A31,A30          ; |213| 

           STDW    .D2T2   B7:B6,*B23
           MV      .L1X    B4,A0             ; |193| 

   [ A0]   MV      .L2X    A3,B5             ; |189| 
||         SET     .S1     A29,0x17,0x1e,A3

   [ B2]   ZERO    .L2     B5

   [ B1]   MV      .L2X    A3,B5             ; |193| 
||         ADD     .L1     A15,A30,A29       ; |213| 

   [ B0]   MV      .L2     B10,B5            ; |197| 

           STW     .D1T2   B5,*A29           ; |213| 
||         MVC     .S2     TSCL,B8           ; |128| 

           MVC     .S2     TSCH,B9           ; |128| 

           MVKL    .S1     _t_offset,A3
||         SUB     .L1     A11,1,A0          ; |206| 
||         ADD     .D1     4,A15,A15         ; |206| 

           MVKH    .S1     _t_offset,A3
||         SUB     .L1     A11,1,A11         ; |206| 

           LDDW    .D1T1   *A3,A5:A4         ; |130| 
||         MVKL    .S1     _cycle_counts+24,A3

           MVKH    .S1     _cycle_counts+24,A3

           LDDW    .D1T1   *A3,A7:A6         ; |130| 
||         MVKL    .S1     _t_stop,A22

           MV      .L2X    A3,B11            ; |130| 
||         MVKH    .S1     _t_stop,A22

           STDW    .D1T2   B9:B8,*A22        ; |128| 
           ADDU    .L1X    A4,B6,A9:A8       ; |130| 

           ADD     .S1     A9,A5,A3          ; |130| 
||         SUBU    .L1X    B8,A8,A5:A4       ; |130| 

           EXT     .S1     A5,24,24,A28      ; |130| 
||         ADDU    .L1     A6,A4,A5:A4       ; |130| 

           ADD     .L2X    A3,B7,B4          ; |130| 
|| [ A0]   B       .S1     $C$L17            ; |206| 

   [ A0]   CALL    .S1     _gimme_random     ; |208| 
||         SUB     .L2     B9,B4,B4          ; |130| 

           ADD     .L1     A5,A7,A3          ; |130| 

           ADD     .L1X    B4,A28,A5         ; |130| 
|| [ A0]   ZERO    .L2     B4

           ADD     .L1     A3,A5,A5          ; |130| 
|| [ A0]   MVKH    .S2     0xc1200000,B4

   [!A0]   CALL    .S1     __fltllif         ; |135| 
||         STDW    .D2T1   A5:A4,*B11        ; |130| 
|| [ A0]   MV      .L1     A12,A4            ; |208| 

           ; BRANCHCC OCCURS {$C$L17}        ; |206| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 5
           LDDW    .D2T1   *B11,A5:A4        ; |135| 
           ADDKPC  .S2     $C$RL32,B3,3      ; |135| 
$C$RL32:   ; CALL OCCURS {__fltllif} {0}     ; |135| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 15
           ZERO    .L1     A11
           SET     .S1     A11,0x1a,0x1d,A11
           MPYSP   .M1     A11,A4,A3         ; |135| 
           ZERO    .L1     A13
           SET     .S1     A13,0x15,0x1d,A13
           ZERO    .L1     A12               ; |135| 
           SPDP    .S1     A3,A5:A4          ; |135| 
           NOP             2

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A13:A12,A5:A4,A5:A4 ; |135| 

$C$RL33:   ; CALL OCCURS {__fixdlli} {0}     ; |135| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           LDDW    .D2T2   *+SP(40),B5:B4
           STDW    .D2T1   A5:A4,*B11        ; |135| 
           SUBAW   .D2     B11,6,B10
           MV      .L2X    A12,B31           ; |114| 
           NOP             1

           STDW    .D2T2   B5:B4,*B10        ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B6           ; |116| 
           MVC     .S2     TSCH,B4           ; |116| 
           MVKL    .S1     _t_start,A15

           MVKH    .S1     _t_start,A15
||         MV      .L2     B4,B7             ; |116| 

           STDW    .D1T2   B7:B6,*A15        ; |116| 
||         MVC     .S2     TSCL,B4           ; |117| 

           MVC     .S2     TSCH,B8           ; |117| 

           SUBU    .L2     B4,B6,B5:B4       ; |117| 
||         MVKL    .S2     _t_offset,B13

           EXT     .S2     B5,24,24,B6       ; |117| 
||         SUB     .L2     B8,B7,B30         ; |117| 

           ADD     .L2     B30,B6,B5         ; |117| 
||         MVKH    .S2     _t_offset,B13

           STDW    .D2T2   B5:B4,*B13        ; |117| 
||         MV      .L2X    A14,B12
||         ADD     .L1     4,A14,A14
||         MVK     .S2     0x80,B11

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L18:    
;          EXCLUSIVE CPU CYCLES: 11

           LDW     .D2T2   *B12,B4           ; |221| 
||         LDW     .D1T1   *A14,A3           ; |221| 

           NOP             4

           ADD     .L2X    A12,B4,B4         ; |221| 
||         ADD     .L1     A12,A3,A3         ; |221| 
||         CALL    .S1     _powf             ; |221| 

           LDW     .D2T1   *B4,A4            ; |221| 
||         LDW     .D1T2   *A3,B4            ; |221| 

           ADDKPC  .S2     $C$RL34,B3,3      ; |221| 
$C$RL34:   ; CALL OCCURS {_powf} {0}         ; |221| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 9

           MVKL    .S1     _output,A3
||         SUB     .L1X    B11,1,A0          ; |220| 
||         SUB     .L2     B11,1,B11         ; |220| 

           MVKH    .S1     _output,A3
           LDW     .D1T1   *A3,A3            ; |221| 
   [ A0]   BNOP    .S1     $C$L18,3          ; |220| 

           ADD     .L1     A12,A3,A3         ; |221| 
||         ADD     .S1     4,A12,A12         ; |220| 

           STW     .D1T1   A4,*A3            ; |221| 
           ; BRANCHCC OCCURS {$C$L18}        ; |220| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           MVC     .S2     TSCL,B8           ; |128| 
           MVC     .S2     TSCH,B9           ; |128| 

           LDDW    .D1T1   *A15,A5:A4        ; |130| 
||         LDDW    .D2T2   *B13,B5:B4        ; |130| 

           LDDW    .D2T2   *B10,B17:B16      ; |130| 
           NOP             3
           ADDU    .L1X    B4,A4,A7:A6       ; |130| 
           ADD     .L1X    A7,B5,A4          ; |130| 

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
           MPYSP   .M1     A11,A4,A3         ; |130| 
           ZERO    .L1     A12               ; |130| 
           NOP             2
           SPDP    .S1     A3,A5:A4          ; |130| 
           NOP             1

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A13:A12,A5:A4,A5:A4 ; |130| 

$C$RL36:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 15
           LDDW    .D2T2   *+SP(40),B5:B4    ; |130| 
           MV      .L2X    A12,B31           ; |114| 
           STDW    .D2T1   A5:A4,*B10        ; |130| 
           NOP             2

           STDW    .D2T2   B5:B4,*+B10(8)    ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B8           ; |116| 
           MVC     .S2     TSCH,B4           ; |116| 
           MV      .L2     B4,B9             ; |116| 

           STDW    .D1T2   B9:B8,*A15        ; |116| 
||         MVC     .S2     TSCL,B4           ; |117| 

           MVC     .S2     TSCH,B6           ; |117| 
           SUBU    .L2     B4,B8,B5:B4       ; |117| 

           EXT     .S2     B5,24,24,B6       ; |117| 
||         SUB     .L2     B6,B9,B5          ; |117| 

           ADD     .L2     B5,B6,B5          ; |117| 
||         LDW     .D2T1   *+SP(20),A11      ; |117| 

           STDW    .D2T2   B5:B4,*B13        ; |117| 
||         MVK     .S2     0x80,B11

;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L19:    
;          EXCLUSIVE CPU CYCLES: 11

           LDW     .D2T2   *B12,B4           ; |229| 
||         LDW     .D1T1   *A14,A3           ; |229| 

           NOP             4

           ADD     .L2X    A12,B4,B4         ; |229| 
||         ADD     .L1     A12,A3,A3         ; |229| 
||         CALL    .S1     _powsp            ; |229| 

           LDW     .D2T1   *B4,A4            ; |229| 
||         LDW     .D1T2   *A3,B4            ; |229| 

           ADDKPC  .S2     $C$RL37,B3,3      ; |229| 
$C$RL37:   ; CALL OCCURS {_powsp} {0}        ; |229| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7

           LDW     .D1T1   *A11,A3           ; |229| 
||         SUB     .L1X    B11,1,A0          ; |228| 
||         SUB     .L2     B11,1,B11         ; |228| 

   [ A0]   BNOP    .S1     $C$L19,3          ; |228| 

           ADD     .L1     A12,A3,A3         ; |229| 
||         ADD     .S1     4,A12,A12         ; |228| 

           STW     .D1T1   A4,*A3            ; |229| 
           ; BRANCHCC OCCURS {$C$L19}        ; |228| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 17
           MVC     .S2     TSCL,B8           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           MV      .L1     A15,A11           ; |128| 
||         LDDW    .D2T2   *B13,B7:B6        ; |130| 

           LDDW    .D1T1   *A11,A5:A4        ; |130| 
           LDDW    .D2T2   *+B10(8),B17:B16  ; |130| 
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
||         ADDKPC  .S2     $C$RL38,B3,0      ; |130| 

$C$RL38:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 14
           ZERO    .L1     A3
           SET     .S1     A3,0x1a,0x1d,A3
           MPYSP   .M1     A3,A4,A3          ; |130| 
           ZERO    .L1     A12               ; |130| 
           NOP             2
           SPDP    .S1     A3,A5:A4          ; |130| 
           NOP             1

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A13:A12,A5:A4,A5:A4 ; |130| 

$C$RL39:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 20
           LDDW    .D2T2   *+SP(40),B5:B4
           ADDAD   .D2     B10,1,B11
           MV      .L2X    A12,B31           ; |114| 
           STDW    .D2T1   A5:A4,*B11        ; |130| 
           NOP             1

           STDW    .D2T2   B5:B4,*+B10(16)   ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B6           ; |116| 
           MVC     .S2     TSCH,B4           ; |116| 

           MV      .L1     A11,A12           ; |116| 
||         MV      .L2     B4,B7             ; |116| 

           STDW    .D1T2   B7:B6,*A12        ; |116| 
||         MVC     .S2     TSCL,B5           ; |117| 

           MVC     .S2     TSCH,B4           ; |117| 
           SUBU    .L2     B5,B6,B9:B8       ; |117| 

           EXT     .S2     B9,24,24,B5       ; |117| 
||         SUB     .L2     B4,B7,B4          ; |117| 

           ADD     .L2     B4,B5,B9          ; |117| 
           STDW    .D2T2   B9:B8,*B13        ; |117| 

           LDW     .D1T1   *A14,A3           ; |237| 
||         LDW     .D2T2   *B12,B4           ; |237| 

           MVK     .S2     0x80,B30
           LDW     .D2T1   *+SP(24),A11      ; |117| 
           STW     .D2T2   B30,*+SP(12)      ; |117| 
           ZERO    .L1     A15
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L20:    
;          EXCLUSIVE CPU CYCLES: 10
           NOP             4

           CALL    .S1     _powsp_c          ; |237| 
||         ADD     .L2X    A15,B4,B4         ; |237| 
||         ADD     .L1     A15,A3,A3         ; |237| 

           LDW     .D1T2   *A3,B4            ; |237| 
||         LDW     .D2T1   *B4,A4            ; |237| 

           NOP             3
           ADDKPC  .S2     $C$RL40,B3,0      ; |237| 
$C$RL40:   ; CALL OCCURS {_powsp_c} {0}      ; |237| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 12

           LDW     .D2T2   *+SP(12),B4       ; |237| 
||         LDW     .D1T1   *A11,A3           ; |237| 

           NOP             4

           SUB     .L1X    B4,1,A0           ; |236| 
||         SUB     .L2     B4,1,B4           ; |236| 
||         ADD     .S1     A15,A3,A3         ; |237| 
||         ADD     .D1     4,A15,A15         ; |236| 

   [ A0]   BNOP    .S1     $C$L20,3          ; |236| 
||         STW     .D2T2   B4,*+SP(12)       ; |237| 
||         STW     .D1T1   A4,*A3            ; |237| 

   [ A0]   LDW     .D1T1   *A14,A3           ; |237| 
   [ A0]   LDW     .D2T2   *B12,B4           ; |237| 
           ; BRANCHCC OCCURS {$C$L20}        ; |236| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           MVC     .S2     TSCL,B6           ; |128| 
           MVC     .S2     TSCH,B7           ; |128| 

           MV      .L1     A12,A11           ; |128| 
||         MV      .L2     B13,B4            ; |128| 

           LDDW    .D1T1   *A11,A5:A4        ; |130| 
||         LDDW    .D2T2   *B4,B5:B4         ; |130| 

           LDDW    .D2T2   *+B10(16),B17:B16 ; |130| 
           NOP             3
           ADDU    .L2X    B4,A4,B9:B8       ; |130| 

           ADD     .S2     B9,B5,B4          ; |130| 
||         SUBU    .L2     B6,B8,B9:B8       ; |130| 

           ADD     .L2X    B4,A5,B4          ; |130| 
||         MV      .S2     B8,B5             ; |130| 
||         CALL    .S1     __fltllif         ; |130| 

           ADDU    .L2     B16,B5,B9:B8      ; |130| 
||         EXT     .S2     B9,24,24,B5       ; |130| 
||         SUB     .D2     B7,B4,B4          ; |130| 

           ADD     .L2     B4,B5,B5          ; |130| 
||         ADD     .S2     B9,B17,B4         ; |130| 

           ADD     .L2     B4,B5,B9          ; |130| 
||         MVKL    .S2     _t_stop,B4

           MVKH    .S2     _t_stop,B4

           STDW    .D2T2   B7:B6,*B4         ; |128| 
||         DADD    .L1X    0,B9:B8,A5:A4     ; |130| 
||         ADDKPC  .S2     $C$RL41,B3,0      ; |130| 

$C$RL41:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 110
           ZERO    .L1     A3
           SET     .S1     A3,0x1a,0x1d,A3
           MPYSP   .M1     A3,A4,A3          ; |130| 
           ZERO    .L1     A12               ; |130| 
           NOP             3
           SPDP    .S2X    A3,B5:B4          ; |130| 
           NOP             2

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1X    A13:A12,B5:B4,A5:A4 ; |130| 

$C$RL42:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
           LDDW    .D2T2   *+SP(40),B5:B4
           ADDAD   .D2     B11,1,B13
           MV      .L2X    A12,B31           ; |114| 
           STDW    .D2T1   A5:A4,*B13        ; |130| 
           NOP             1

           STDW    .D2T2   B5:B4,*+B10(32)   ; |114| 
||         MVC     .S2     B31,TSCL          ; |115| 

           MVC     .S2     TSCL,B4           ; |116| 
           MVC     .S2     TSCH,B5           ; |116| 

           STDW    .D1T2   B5:B4,*A11        ; |116| 
||         MVC     .S2     TSCL,B6           ; |117| 

           MVC     .S2     TSCH,B8           ; |117| 
           MVKL    .S1     _t_offset,A3

           SUBU    .L2     B6,B4,B7:B6       ; |117| 
||         MVKH    .S1     _t_offset,A3

           EXT     .S2     B7,24,24,B7       ; |117| 
||         SUB     .L2     B8,B5,B5          ; |117| 

           MVKL    .S1     _output,A15
||         LDW     .D2T1   *B12,A4           ; |244| 
||         MV      .L2X    A3,B10            ; |117| 
||         ADD     .S2     B5,B7,B7          ; |117| 

           MVKH    .S1     _output,A15
||         MV      .L2X    A14,B9            ; |117| 
||         STDW    .D2T2   B7:B6,*B10        ; |117| 
||         MVK     .S2     0x80,B6           ; |244| 

           CALLP   .S2     _powsp_v,B3
||         LDW     .D1T1   *+A15(16),A6      ; |244| 
||         LDW     .D2T2   *B9,B4            ; |244| 

$C$RL43:   ; CALL OCCURS {_powsp_v} {0}      ; |244| 
           MVC     .S2     TSCL,B8           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 

           LDDW    .D1T1   *A11,A9:A8        ; |130| 
||         LDDW    .D2T2   *B10,B7:B6        ; |130| 

           MVK     .S1     16,A11
           ADD     .L1X    B13,A11,A11
           MVKL    .S1     _t_stop,A3

           LDDW    .D1T1   *A11,A17:A16      ; |130| 
||         MVKH    .S1     _t_stop,A3

           ADDU    .L1X    B6,A8,A5:A4       ; |130| 

           MV      .L2     B4,B9             ; |128| 
||         MV      .S2X    A3,B4             ; |128| 
||         ADD     .L1X    A5,B7,A3          ; |130| 

           SUBU    .L1X    B8,A4,A7:A6       ; |130| 

           ADD     .L1     A3,A9,A4          ; |130| 
||         EXT     .S1     A7,24,24,A3       ; |130| 

           SUB     .S1X    B9,A4,A6          ; |130| 
||         ADDU    .L1     A16,A6,A5:A4      ; |130| 

           ADD     .L1     A5,A17,A3         ; |130| 
||         ADD     .S1     A6,A3,A5          ; |130| 

           CALLP   .S2     __fltllif,B3
||         ADD     .L1     A3,A5,A5          ; |130| 
||         STDW    .D2T2   B9:B8,*B4         ; |128| 

$C$RL44:   ; CALL OCCURS {__fltllif} {0}     ; |130| 
           ZERO    .L1     A3
           SET     .S1     A3,0x1a,0x1d,A3
           MPYSP   .M1     A3,A4,A3          ; |130| 
           NOP             3
           SPDP    .S1     A3,A5:A4          ; |130| 
           NOP             1

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A13:A12,A5:A4,A5:A4 ; |130| 

$C$RL45:   ; CALL OCCURS {__fixdlli} {0}     ; |130| 
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6

           STDW    .D1T1   A5:A4,*A11        ; |130| 
||         MVKH    .S2     0x3f1a36e2,B7

           LDW     .D1T2   *+A15(4),B4       ; |66| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A15,A4           ; |66| 
||         MVK     .L1     0x9,A8            ; |66| 
||         MVK     .S1     0x80,A6           ; |66| 

$C$RL46:   ; CALL OCCURS {_isequal} {0}      ; |66| 

           MVKL    .S1     _fcn_pass,A3
||         MVKL    .S2     0xeb1c432d,B6

           MVKH    .S1     _fcn_pass,A3
||         MVKL    .S2     0x3f1a36e2,B7

           MVKH    .S2     0xeb1c432d,B6

           MV      .L1     A4,A5             ; |66| 
||         LDW     .D1T1   *A15,A4           ; |67| 
||         MV      .L2X    A3,B10            ; |66| 
||         MVKH    .S2     0x3f1a36e2,B7

           CALLP   .S2     _isequal,B3
||         LDW     .D1T2   *+A15(8),B4       ; |67| 
||         STW     .D2T1   A5,*+B10(4)       ; |66| 
||         MVK     .S1     0x80,A6           ; |67| 
||         MVK     .L1     0x9,A8            ; |67| 

$C$RL47:   ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B10(8)       ; |67| 
||         LDW     .D1T2   *+A15(12),B4      ; |68| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A15,A4           ; |68| 
||         MVK     .S1     0x80,A6           ; |68| 
||         MVK     .L1     0x9,A8            ; |68| 

$C$RL48:   ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     0x3f1a36e2,B7
           MVKL    .S2     0xeb1c432d,B6
           MVKH    .S2     0x3f1a36e2,B7

           STW     .D2T1   A4,*+B10(12)      ; |68| 
||         LDW     .D1T2   *A10,B4           ; |69| 
||         MVKH    .S2     0xeb1c432d,B6

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A15,A4           ; |69| 
||         MVK     .S1     0x80,A6           ; |69| 
||         MVK     .L1     0x9,A8            ; |69| 

$C$RL49:   ; CALL OCCURS {_isequal} {0}      ; |69| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 16
           LDW     .D2T2   *+SP(28),B4       ; |69| 
           STW     .D2T1   A4,*+B10(16)      ; |69| 
           NOP             3
           LDW     .D2T2   *B4,B0            ; |72| 
           LDW     .D2T2   *+SP(32),B4
           NOP             3

   [!B0]   B       .S1     $C$L21            ; |72| 
||         MV      .L2     B0,B1             ; guard predicate rewrite

   [ B0]   LDW     .D2T2   *B4,B0            ; |72| 
   [ B1]   ADD     .L2     4,B4,B4
           NOP             3
           ; BRANCHCC OCCURS {$C$L21}        ; |72| 
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
   [ A0]   B       .S1     $C$L22            ; |72| 
   [ A0]   CALL    .S1     _print_test_results ; |250| 
           MVK     .L1     0x1,A3            ; |72| 
   [ A0]   MVKL    .S1     _all_pass,A4
   [ A0]   MVKH    .S1     _all_pass,A4
           NOP             1
           ; BRANCHCC OCCURS {$C$L22}        ; |72| 
;** --------------------------------------------------------------------------*
$C$L21:    
;          EXCLUSIVE CPU CYCLES: 5
           CALL    .S1     _print_test_results ; |250| 
           MVKL    .S1     _all_pass,A4
           MVKH    .S1     _all_pass,A4
           MV      .L1     A12,A3
           NOP             1
;** --------------------------------------------------------------------------*
$C$L22:    
;          EXCLUSIVE CPU CYCLES: 1

           STW     .D1T1   A3,*A4            ; |72| 
||         MVK     .L1     0x2,A4            ; |250| 
||         ADDKPC  .S2     $C$RL50,B3,0      ; |250| 

$C$RL50:   ; CALL OCCURS {_print_test_results} {0}  ; |250| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 30
           MVKL    .S2     $C$SL2+0,B4
           MVKH    .S2     $C$SL2+0,B4

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |251| 

$C$RL51:   ; CALL OCCURS {_printf} {0}       ; |251| 
           MVKL    .S2     $C$SL3+0,B4
           MVKH    .S2     $C$SL3+0,B4

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |251| 

$C$RL52:   ; CALL OCCURS {_printf} {0}       ; |251| 
           MVKL    .S1     $C$SL1+0,A10
           MVKH    .S1     $C$SL1+0,A10

           CALLP   .S2     _print_profile_results,B3
||         MV      .L1     A10,A4            ; |258| 

$C$RL53:   ; CALL OCCURS {_print_profile_results} {0}  ; |258| 

           CALLP   .S2     _print_memory_results,B3
||         MV      .L1     A10,A4            ; |261| 

$C$RL54:   ; CALL OCCURS {_print_memory_results} {0}  ; |261| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *++SP(56),B3      ; |262| 
           LDDW    .D2T1   *++SP,A13:A12     ; |262| 
           LDDW    .D2T1   *++SP,A15:A14     ; |262| 
           LDDW    .D2T2   *++SP,B11:B10     ; |262| 
           LDDW    .D2T2   *++SP,B13:B12     ; |262| 

           LDW     .D2T1   *++SP(8),A10      ; |262| 
||         RET     .S2     B3                ; |262| 

           LDW     .D2T1   *++SP(8),A11      ; |262| 
           NOP             4
           ; BRANCH OCCURS {B3}              ; |262| 
;******************************************************************************
;* STRINGS                                                                    *
;******************************************************************************
	.sect	".const:.string"
$C$SL1:	.string	"powSP",0
$C$SL2:	.string	"----------------------------------------",0
$C$SL3:	.string	"----------------------------------------",10,0
;*****************************************************************************
;* UNDEFINED EXTERNAL REFERENCES                                             *
;*****************************************************************************
	.global	_printf
	.global	_pow
	.global	_powf
	.global	_driver_init
	.global	_print_profile_results
	.global	_print_memory_results
	.global	_print_test_results
	.global	_gimme_random
	.global	_isequal
	.global	_powsp
	.global	_powsp_c
	.global	_powsp_v
	.global	_ti_math_logtable
	.global	_ti_math_kTable
	.global	_ti_math_jTable
	.global	_t_start
	.global	_t_stop
	.global	_t_offset
	.global	_cycle_counts
	.global	_fcn_pass
	.global	_all_pass
	.global	_a_ext
	.global	_b_ext
	.global	_a_sc
	.global	_b_sc
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
