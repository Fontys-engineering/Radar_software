;******************************************************************************
;* TMS320C6x C/C++ Codegen                                          PC v7.4.2 *
;* Date/Time created: Tue May 17 02:35:44 2016                                *
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
	.dwattr $C$DW$CU, DW_AT_name("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$CU, DW_AT_producer("TMS320C6x C/C++ Codegen PC v7.4.2 Copyright (c) 1996-2012 Texas Instruments Incorporated")
	.dwattr $C$DW$CU, DW_AT_TI_version(0x01)
	.dwattr $C$DW$CU, DW_AT_comp_dir("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug")
;*****************************************************************************
;* CINIT RECORDS                                                             *
;*****************************************************************************
	.sect	".cinit"
	.align	8
	.field  	$C$IR_1,32
	.field  	_a+0,32
	.word	0412af0e3h,05be02646h		; _a[0] @ 0
	.word	04121197ah,020423b0eh		; _a[1] @ 64
	.word	040fdd968h,066ce505bh		; _a[2] @ 128
	.word	0411dd368h,0bb561764h		; _a[3] @ 192
	.word	040ed382dh,09b258ad4h		; _a[4] @ 256
	.word	04121d565h,09d3b8af1h		; _a[5] @ 320
	.word	04116dda3h,0357bd563h		; _a[6] @ 384
	.word	0412bd534h,0dfabcffdh		; _a[7] @ 448
	.word	04129b83fh,09ea2edd6h		; _a[8] @ 512
	.word	0411316d3h,0f33d2956h		; _a[9] @ 576
	.word	0412616dbh,0dea31975h		; _a[10] @ 640
	.word	04128999fh,0442a927bh		; _a[11] @ 704
	.word	040ff1162h,0f4b223d4h		; _a[12] @ 768
	.word	0412c7afbh,048aef7d0h		; _a[13] @ 832
	.word	041172716h,02e682152h		; _a[14] @ 896
	.word	041027270h,024213a0ch		; _a[15] @ 960
	.word	041197540h,038de59bdh		; _a[16] @ 1024
	.word	041093db2h,07a42ff59h		; _a[17] @ 1088
	.word	041008b60h,005c96031h		; _a[18] @ 1152
	.word	0411ec7d7h,05caf84bdh		; _a[19] @ 1216
	.word	041283555h,09a7f221fh		; _a[20] @ 1280
	.word	0411eac9bh,0d3cf04b5h		; _a[21] @ 1344
	.word	04116eaf8h,039f9c122h		; _a[22] @ 1408
	.word	040e6e078h,0a6c4fb47h		; _a[23] @ 1472
	.word	0412d5011h,03822eae7h		; _a[24] @ 1536
	.word	04127c24ah,0bfeef292h		; _a[25] @ 1600
	.word	040f71020h,09f912a5fh		; _a[26] @ 1664
	.word	0411f93ddh,0ca0f4d7bh		; _a[27] @ 1728
	.word	0410697d4h,0dd928fbah		; _a[28] @ 1792
	.word	04122381ah,0409807c9h		; _a[29] @ 1856
	.word	0412a26cbh,0117bc141h		; _a[30] @ 1920
	.word	04110c1a9h,047531048h		; _a[31] @ 1984
	.word	0411513c1h,03d9fb898h		; _a[32] @ 2048
	.word	040f7c32ah,0c0d5984dh		; _a[33] @ 2112
	.word	04129bddbh,098b47263h		; _a[34] @ 2176
	.word	0410ff548h,08969a0aeh		; _a[35] @ 2240
	.word	0412b9a46h,006488758h		; _a[36] @ 2304
	.word	040d6a8f9h,006eb412ch		; _a[37] @ 2368
	.word	04128c59dh,0be2003ach		; _a[38] @ 2432
	.word	040d69449h,04b40db92h		; _a[39] @ 2496
	.word	0412c62c5h,0e4d6eb34h		; _a[40] @ 2560
	.word	0412575b2h,02aca361ah		; _a[41] @ 2624
	.word	040fe2defh,06a712282h		; _a[42] @ 2688
	.word	04124bc73h,0b2c9696bh		; _a[43] @ 2752
	.word	040f240cfh,03d4e0978h		; _a[44] @ 2816
	.word	040f30c52h,0d6f2a5a4h		; _a[45] @ 2880
	.word	04126276dh,04f937238h		; _a[46] @ 2944
	.word	0412d4cc8h,0403d3ca1h		; _a[47] @ 3008
	.word	041220aeeh,06b23eac1h		; _a[48] @ 3072
	.word	041243069h,03dc66326h		; _a[49] @ 3136
	.word	04104bdf4h,0775cb5e3h		; _a[50] @ 3200
	.word	04106d07ch,093161a1eh		; _a[51] @ 3264
	.word	041131ff5h,03d990951h		; _a[52] @ 3328
	.word	04121564bh,073e56473h		; _a[53] @ 3392
	.word	041220d47h,08c2a7e59h		; _a[54] @ 3456
	.word	04110086bh,01d64f2c9h		; _a[55] @ 3520
	.word	04124d9ech,0fcdb6a1fh		; _a[56] @ 3584
	.word	0411f4735h,0387d4e09h		; _a[57] @ 3648
	.word	0412631a5h,05dbb6aa5h		; _a[58] @ 3712
	.word	04125ad41h,05f922963h		; _a[59] @ 3776
	.word	04126368dh,07feab368h		; _a[60] @ 3840
	.word	041283cbfh,013f65dc0h		; _a[61] @ 3904
	.word	0412ba7f5h,0c21bd8a4h		; _a[62] @ 3968
	.word	04122db3ch,058c2140ch		; _a[63] @ 4032
	.word	040f23957h,06f2ae07eh		; _a[64] @ 4096
	.word	04113223bh,0b2319e08h		; _a[65] @ 4160
	.word	040e2d284h,01f05ea25h		; _a[66] @ 4224
	.word	04124ed79h,0a346a352h		; _a[67] @ 4288
	.word	0412cad97h,09c435844h		; _a[68] @ 4352
	.word	041212e6dh,059ad2136h		; _a[69] @ 4416
	.word	04123ba2eh,0d7dd68fch		; _a[70] @ 4480
	.word	041084c18h,0412a5f4fh		; _a[71] @ 4544
	.word	041001909h,0911cb03ah		; _a[72] @ 4608
	.word	041230cd6h,0ee15d123h		; _a[73] @ 4672
	.word	041298aa9h,01ae865b0h		; _a[74] @ 4736
	.word	040e15eefh,00b3b67b5h		; _a[75] @ 4800
	.word	040ed7e79h,08183c36eh		; _a[76] @ 4864
	.word	04105633eh,05f865d7dh		; _a[77] @ 4928
	.word	0411326bfh,036c8853fh		; _a[78] @ 4992
	.word	04124ad32h,00ec980e8h		; _a[79] @ 5056
	.word	04125f43eh,0c2aa0522h		; _a[80] @ 5120
	.word	0411db3d4h,0cf0f1daah		; _a[81] @ 5184
	.word	0411689beh,028c4d80ch		; _a[82] @ 5248
	.word	041076d08h,090e0a84ch		; _a[83] @ 5312
	.word	0412577bfh,062055e0eh		; _a[84] @ 5376
	.word	0411a39cdh,01b8179e8h		; _a[85] @ 5440
	.word	0412b3befh,0efac710dh		; _a[86] @ 5504
	.word	0410d19ddh,0aa0dd5ach		; _a[87] @ 5568
	.word	041086339h,0415aebdfh		; _a[88] @ 5632
	.word	0411c1878h,0f8ac555eh		; _a[89] @ 5696
	.word	04116bd96h,02a353c22h		; _a[90] @ 5760
	.word	040f3c7c4h,05a908a26h		; _a[91] @ 5824
	.word	0412e4754h,0936c2d50h		; _a[92] @ 5888
	.word	04123426bh,0380614edh		; _a[93] @ 5952
	.word	0412abd5eh,023a73995h		; _a[94] @ 6016
	.word	041211d7eh,080f58cdfh		; _a[95] @ 6080
	.word	0411b2e12h,0e8e1932dh		; _a[96] @ 6144
	.word	0412228b9h,0a6f7d30bh		; _a[97] @ 6208
	.word	040f4f620h,094cf4a56h		; _a[98] @ 6272
	.word	040ff0d6eh,06e9f4fbch		; _a[99] @ 6336
	.word	041203e1dh,04aedb4a4h		; _a[100] @ 6400
	.word	04121e2c6h,0f32a355eh		; _a[101] @ 6464
	.word	0410f6359h,09083f69ah		; _a[102] @ 6528
	.word	04128d96eh,07ee9325eh		; _a[103] @ 6592
	.word	04128dc5eh,0ad5e071ch		; _a[104] @ 6656
	.word	0411b02fch,0231b65b7h		; _a[105] @ 6720
	.word	0410e86bch,0b7215c71h		; _a[106] @ 6784
	.word	0410fd030h,0fef72134h		; _a[107] @ 6848
	.word	0411b47a9h,0c94b167fh		; _a[108] @ 6912
	.word	041222bafh,0c0512c42h		; _a[109] @ 6976
	.word	0411f9324h,0a5044509h		; _a[110] @ 7040
	.word	04124ed13h,04a7c2515h		; _a[111] @ 7104
	.word	041058158h,0bebbe29ah		; _a[112] @ 7168
	.word	040ed87f3h,0a6f5a2afh		; _a[113] @ 7232
	.word	040fa6616h,09db7d69ch		; _a[114] @ 7296
	.word	0412bc170h,0b8770d7eh		; _a[115] @ 7360
	.word	041155a32h,04cf0d18fh		; _a[116] @ 7424
	.word	0411bf2d0h,0180f0557h		; _a[117] @ 7488
	.word	041240945h,01f78897fh		; _a[118] @ 7552
	.word	04115082bh,069d44490h		; _a[119] @ 7616
	.word	0412b5ab0h,0565f3af9h		; _a[120] @ 7680
	.word	0412e6a58h,0e860b7d1h		; _a[121] @ 7744
	.word	041183cbfh,0fcfe6c82h		; _a[122] @ 7808
	.word	040580f5dh,0ea2b8265h		; _a[123] @ 7872
	.word	0411545a8h,0b738609ah		; _a[124] @ 7936
	.word	0411d6d9ah,08263736dh		; _a[125] @ 8000
	.word	041032985h,0d5c34b44h		; _a[126] @ 8064
	.word	0411cedc7h,0a32d905ch		; _a[127] @ 8128
	.word	04129812dh,048bdf315h		; _a[128] @ 8192
	.word	041129a29h,038776816h		; _a[129] @ 8256
	.word	04115f77ah,036283177h		; _a[130] @ 8320
	.word	0412d6f9fh,0cab1df0bh		; _a[131] @ 8384
	.word	040f0ab74h,0dd9b3652h		; _a[132] @ 8448
	.word	0411c807fh,0eb1cb542h		; _a[133] @ 8512
	.word	040cc3746h,0358298cch		; _a[134] @ 8576
	.word	0410ea824h,0889019beh		; _a[135] @ 8640
	.word	04116058dh,013570245h		; _a[136] @ 8704
	.word	041207ebah,05f3962f8h		; _a[137] @ 8768
	.word	0412662dfh,07784a292h		; _a[138] @ 8832
	.word	04126032bh,0de42e481h		; _a[139] @ 8896
	.word	04122f0bah,0a9843584h		; _a[140] @ 8960
	.word	04125c376h,029e55f6bh		; _a[141] @ 9024
	.word	04125d212h,056059063h		; _a[142] @ 9088
	.word	04115aedeh,06f709b74h		; _a[143] @ 9152
	.word	04114828dh,0472cf95dh		; _a[144] @ 9216
	.word	041271e58h,02c63d669h		; _a[145] @ 9280
	.word	04127393ah,0a8c63503h		; _a[146] @ 9344
	.word	04125ad0eh,01bb27533h		; _a[147] @ 9408
	.word	04122c68ch,039424448h		; _a[148] @ 9472
	.word	040de77b6h,04f9350aah		; _a[149] @ 9536
	.word	0410f12b9h,056818ee1h		; _a[150] @ 9600
	.word	0412d2addh,086d34ec7h		; _a[151] @ 9664
	.word	041009132h,08800218eh		; _a[152] @ 9728
	.word	0411e8200h,0e4cb0b2ch		; _a[153] @ 9792
	.word	041065c4eh,01dc69220h		; _a[154] @ 9856
	.word	04129d164h,09e73c199h		; _a[155] @ 9920
	.word	04105e20ch,07add6678h		; _a[156] @ 9984
	.word	04125e94ch,02ca08cd3h		; _a[157] @ 10048
	.word	040fd155fh,046177136h		; _a[158] @ 10112
	.word	0412673fah,0d08bc317h		; _a[159] @ 10176
	.word	04122b1d8h,09eb862b2h		; _a[160] @ 10240
	.word	041292d49h,0ebcbdc0ch		; _a[161] @ 10304
	.word	0410cb5f4h,05ccdd93ch		; _a[162] @ 10368
	.word	041215912h,08cb62698h		; _a[163] @ 10432
	.word	0412bca10h,0375d38c0h		; _a[164] @ 10496
	.word	04121963fh,07c47c668h		; _a[165] @ 10560
	.word	041125a31h,0abfd5022h		; _a[166] @ 10624
	.word	0412abe7fh,0a8e9e93dh		; _a[167] @ 10688
	.word	04100ba0bh,099d3b0edh		; _a[168] @ 10752
	.word	04111db71h,08f901df0h		; _a[169] @ 10816
	.word	04124335eh,0db7a94f2h		; _a[170] @ 10880
	.word	0412869a2h,063604692h		; _a[171] @ 10944
	.word	0412cd115h,0275455c3h		; _a[172] @ 11008
	.word	0412bac92h,0f44a2fcfh		; _a[173] @ 11072
	.word	041112c55h,0bb38d60ah		; _a[174] @ 11136
	.word	040e9d28ch,0bb729cd6h		; _a[175] @ 11200
	.word	04121bdbfh,0e795266dh		; _a[176] @ 11264
	.word	0412cf1f6h,081c81400h		; _a[177] @ 11328
	.word	041258dd1h,055e6d084h		; _a[178] @ 11392
	.word	0412d71a7h,09ec9e8edh		; _a[179] @ 11456
	.word	0412b096bh,0887ecc8fh		; _a[180] @ 11520
	.word	0411fbf65h,024b6dbcah		; _a[181] @ 11584
	.word	040efab9fh,0985a3429h		; _a[182] @ 11648
	.word	0411b02a0h,0c486cebch		; _a[183] @ 11712
	.word	0411215f1h,09c87f603h		; _a[184] @ 11776
	.word	04114c8b2h,0bb6df902h		; _a[185] @ 11840
	.word	040f8c07fh,00bcabed6h		; _a[186] @ 11904
	.word	0412e4faeh,088fce091h		; _a[187] @ 11968
	.word	04129daddh,09732c478h		; _a[188] @ 12032
	.word	04125c370h,0782c17b6h		; _a[189] @ 12096
	.word	0410045f7h,0a624dd2fh		; _a[190] @ 12160
	.word	041261c92h,0fa3cb817h		; _a[191] @ 12224
	.word	041035079h,0f824b9f4h		; _a[192] @ 12288
	.word	041204d21h,0427dd94ah		; _a[193] @ 12352
	.word	040f5106fh,060e42b8eh		; _a[194] @ 12416
	.word	041135db2h,0a67f9e46h		; _a[195] @ 12480
	.word	0411b30a1h,09e395663h		; _a[196] @ 12544
	.word	04111d92bh,038aae445h		; _a[197] @ 12608
	.word	0411d3709h,026e8f953h		; _a[198] @ 12672
	.word	040f1a205h,0aa1130aah		; _a[199] @ 12736
	.word	04120292bh,0c7b2788eh		; _a[200] @ 12800
$C$IR_1:	.set	1608


$C$DW$1	.dwtag  DW_TAG_subprogram, DW_AT_name("_extu")
	.dwattr $C$DW$1, DW_AT_TI_symbol_name("__extu")
	.dwattr $C$DW$1, DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$1, DW_AT_declaration
	.dwattr $C$DW$1, DW_AT_external
$C$DW$2	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$2, DW_AT_type(*$C$DW$T$11)
$C$DW$3	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$3, DW_AT_type(*$C$DW$T$11)
$C$DW$4	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$4, DW_AT_type(*$C$DW$T$11)
	.dwendtag $C$DW$1


$C$DW$5	.dwtag  DW_TAG_subprogram, DW_AT_name("_clr")
	.dwattr $C$DW$5, DW_AT_TI_symbol_name("__clr")
	.dwattr $C$DW$5, DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$5, DW_AT_declaration
	.dwattr $C$DW$5, DW_AT_external
$C$DW$6	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$6, DW_AT_type(*$C$DW$T$11)
$C$DW$7	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$7, DW_AT_type(*$C$DW$T$11)
$C$DW$8	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$8, DW_AT_type(*$C$DW$T$11)
	.dwendtag $C$DW$5


$C$DW$9	.dwtag  DW_TAG_subprogram, DW_AT_name("_fabs")
	.dwattr $C$DW$9, DW_AT_TI_symbol_name("__fabs")
	.dwattr $C$DW$9, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$9, DW_AT_declaration
	.dwattr $C$DW$9, DW_AT_external
$C$DW$10	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$10, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$9


$C$DW$11	.dwtag  DW_TAG_subprogram, DW_AT_name("_rcpdp")
	.dwattr $C$DW$11, DW_AT_TI_symbol_name("__rcpdp")
	.dwattr $C$DW$11, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$11, DW_AT_declaration
	.dwattr $C$DW$11, DW_AT_external
$C$DW$12	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$12, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$11


$C$DW$13	.dwtag  DW_TAG_subprogram, DW_AT_name("_rsqrdp")
	.dwattr $C$DW$13, DW_AT_TI_symbol_name("__rsqrdp")
	.dwattr $C$DW$13, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$13, DW_AT_declaration
	.dwattr $C$DW$13, DW_AT_external
$C$DW$14	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$14, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$13


$C$DW$15	.dwtag  DW_TAG_subprogram, DW_AT_name("_hi")
	.dwattr $C$DW$15, DW_AT_TI_symbol_name("__hi")
	.dwattr $C$DW$15, DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$15, DW_AT_declaration
	.dwattr $C$DW$15, DW_AT_external
$C$DW$16	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$16, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$15


$C$DW$17	.dwtag  DW_TAG_subprogram, DW_AT_name("_lo")
	.dwattr $C$DW$17, DW_AT_TI_symbol_name("__lo")
	.dwattr $C$DW$17, DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$17, DW_AT_declaration
	.dwattr $C$DW$17, DW_AT_external
$C$DW$18	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$18, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$17


$C$DW$19	.dwtag  DW_TAG_subprogram, DW_AT_name("_itod")
	.dwattr $C$DW$19, DW_AT_TI_symbol_name("__itod")
	.dwattr $C$DW$19, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$19, DW_AT_declaration
	.dwattr $C$DW$19, DW_AT_external
$C$DW$20	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$20, DW_AT_type(*$C$DW$T$11)
$C$DW$21	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$21, DW_AT_type(*$C$DW$T$11)
	.dwendtag $C$DW$19


$C$DW$22	.dwtag  DW_TAG_subprogram, DW_AT_name("_itoll")
	.dwattr $C$DW$22, DW_AT_TI_symbol_name("__itoll")
	.dwattr $C$DW$22, DW_AT_type(*$C$DW$T$14)
	.dwattr $C$DW$22, DW_AT_declaration
	.dwattr $C$DW$22, DW_AT_external
$C$DW$23	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$23, DW_AT_type(*$C$DW$T$11)
$C$DW$24	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$24, DW_AT_type(*$C$DW$T$11)
	.dwendtag $C$DW$22


$C$DW$25	.dwtag  DW_TAG_subprogram, DW_AT_name("_lltod")
	.dwattr $C$DW$25, DW_AT_TI_symbol_name("__lltod")
	.dwattr $C$DW$25, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$25, DW_AT_declaration
	.dwattr $C$DW$25, DW_AT_external
$C$DW$26	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$26, DW_AT_type(*$C$DW$T$14)
	.dwendtag $C$DW$25


$C$DW$27	.dwtag  DW_TAG_subprogram, DW_AT_name("printf")
	.dwattr $C$DW$27, DW_AT_TI_symbol_name("_printf")
	.dwattr $C$DW$27, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$27, DW_AT_declaration
	.dwattr $C$DW$27, DW_AT_external
$C$DW$28	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$28, DW_AT_type(*$C$DW$T$38)
$C$DW$29	.dwtag  DW_TAG_unspecified_parameters
	.dwendtag $C$DW$27


$C$DW$30	.dwtag  DW_TAG_subprogram, DW_AT_name("asinh")
	.dwattr $C$DW$30, DW_AT_TI_symbol_name("_asinh")
	.dwattr $C$DW$30, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$30, DW_AT_declaration
	.dwattr $C$DW$30, DW_AT_external
$C$DW$31	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$31, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$30


$C$DW$32	.dwtag  DW_TAG_subprogram, DW_AT_name("driver_init")
	.dwattr $C$DW$32, DW_AT_TI_symbol_name("_driver_init")
	.dwattr $C$DW$32, DW_AT_declaration
	.dwattr $C$DW$32, DW_AT_external
$C$DW$33	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$33, DW_AT_type(*$C$DW$T$26)
	.dwendtag $C$DW$32


$C$DW$34	.dwtag  DW_TAG_subprogram, DW_AT_name("print_profile_results")
	.dwattr $C$DW$34, DW_AT_TI_symbol_name("_print_profile_results")
	.dwattr $C$DW$34, DW_AT_declaration
	.dwattr $C$DW$34, DW_AT_external
$C$DW$35	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$35, DW_AT_type(*$C$DW$T$26)
	.dwendtag $C$DW$34


$C$DW$36	.dwtag  DW_TAG_subprogram, DW_AT_name("print_memory_results")
	.dwattr $C$DW$36, DW_AT_TI_symbol_name("_print_memory_results")
	.dwattr $C$DW$36, DW_AT_declaration
	.dwattr $C$DW$36, DW_AT_external
$C$DW$37	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$37, DW_AT_type(*$C$DW$T$26)
	.dwendtag $C$DW$36


$C$DW$38	.dwtag  DW_TAG_subprogram, DW_AT_name("print_test_results")
	.dwattr $C$DW$38, DW_AT_TI_symbol_name("_print_test_results")
	.dwattr $C$DW$38, DW_AT_declaration
	.dwattr $C$DW$38, DW_AT_external
$C$DW$39	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$39, DW_AT_type(*$C$DW$T$10)
	.dwendtag $C$DW$38


$C$DW$40	.dwtag  DW_TAG_subprogram, DW_AT_name("gimme_random")
	.dwattr $C$DW$40, DW_AT_TI_symbol_name("_gimme_random")
	.dwattr $C$DW$40, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$40, DW_AT_declaration
	.dwattr $C$DW$40, DW_AT_external
$C$DW$41	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$41, DW_AT_type(*$C$DW$T$17)
$C$DW$42	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$42, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$40


$C$DW$43	.dwtag  DW_TAG_subprogram, DW_AT_name("isequal")
	.dwattr $C$DW$43, DW_AT_TI_symbol_name("_isequal")
	.dwattr $C$DW$43, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$43, DW_AT_declaration
	.dwattr $C$DW$43, DW_AT_external
$C$DW$44	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$44, DW_AT_type(*$C$DW$T$33)
$C$DW$45	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$45, DW_AT_type(*$C$DW$T$33)
$C$DW$46	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$46, DW_AT_type(*$C$DW$T$10)
$C$DW$47	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$47, DW_AT_type(*$C$DW$T$17)
$C$DW$48	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$48, DW_AT_type(*$C$DW$T$10)
	.dwendtag $C$DW$43


$C$DW$49	.dwtag  DW_TAG_subprogram, DW_AT_name("asinhdp_c")
	.dwattr $C$DW$49, DW_AT_TI_symbol_name("_asinhdp_c")
	.dwattr $C$DW$49, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$49, DW_AT_declaration
	.dwattr $C$DW$49, DW_AT_external
$C$DW$50	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$50, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$49


$C$DW$51	.dwtag  DW_TAG_subprogram, DW_AT_name("asinhdp")
	.dwattr $C$DW$51, DW_AT_TI_symbol_name("_asinhdp")
	.dwattr $C$DW$51, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$51, DW_AT_declaration
	.dwattr $C$DW$51, DW_AT_external
$C$DW$52	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$52, DW_AT_type(*$C$DW$T$17)
	.dwendtag $C$DW$51


$C$DW$53	.dwtag  DW_TAG_subprogram, DW_AT_name("asinhdp_v")
	.dwattr $C$DW$53, DW_AT_TI_symbol_name("_asinhdp_v")
	.dwattr $C$DW$53, DW_AT_declaration
	.dwattr $C$DW$53, DW_AT_external
$C$DW$54	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$54, DW_AT_type(*$C$DW$T$32)
$C$DW$55	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$55, DW_AT_type(*$C$DW$T$33)
$C$DW$56	.dwtag  DW_TAG_formal_parameter
	.dwattr $C$DW$56, DW_AT_type(*$C$DW$T$10)
	.dwendtag $C$DW$53

$C$DW$57	.dwtag  DW_TAG_variable, DW_AT_name("t_start")
	.dwattr $C$DW$57, DW_AT_TI_symbol_name("_t_start")
	.dwattr $C$DW$57, DW_AT_type(*$C$DW$T$14)
	.dwattr $C$DW$57, DW_AT_declaration
	.dwattr $C$DW$57, DW_AT_external
$C$DW$58	.dwtag  DW_TAG_variable, DW_AT_name("t_stop")
	.dwattr $C$DW$58, DW_AT_TI_symbol_name("_t_stop")
	.dwattr $C$DW$58, DW_AT_type(*$C$DW$T$14)
	.dwattr $C$DW$58, DW_AT_declaration
	.dwattr $C$DW$58, DW_AT_external
$C$DW$59	.dwtag  DW_TAG_variable, DW_AT_name("t_offset")
	.dwattr $C$DW$59, DW_AT_TI_symbol_name("_t_offset")
	.dwattr $C$DW$59, DW_AT_type(*$C$DW$T$14)
	.dwattr $C$DW$59, DW_AT_declaration
	.dwattr $C$DW$59, DW_AT_external
$C$DW$60	.dwtag  DW_TAG_variable, DW_AT_name("cycle_counts")
	.dwattr $C$DW$60, DW_AT_TI_symbol_name("_cycle_counts")
	.dwattr $C$DW$60, DW_AT_type(*$C$DW$T$49)
	.dwattr $C$DW$60, DW_AT_declaration
	.dwattr $C$DW$60, DW_AT_external
$C$DW$61	.dwtag  DW_TAG_variable, DW_AT_name("fcn_pass")
	.dwattr $C$DW$61, DW_AT_TI_symbol_name("_fcn_pass")
	.dwattr $C$DW$61, DW_AT_type(*$C$DW$T$43)
	.dwattr $C$DW$61, DW_AT_declaration
	.dwattr $C$DW$61, DW_AT_external
$C$DW$62	.dwtag  DW_TAG_variable, DW_AT_name("all_pass")
	.dwattr $C$DW$62, DW_AT_TI_symbol_name("_all_pass")
	.dwattr $C$DW$62, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$62, DW_AT_declaration
	.dwattr $C$DW$62, DW_AT_external
$C$DW$63	.dwtag  DW_TAG_variable, DW_AT_name("a_ext")
	.dwattr $C$DW$63, DW_AT_TI_symbol_name("_a_ext")
	.dwattr $C$DW$63, DW_AT_type(*$C$DW$T$61)
	.dwattr $C$DW$63, DW_AT_declaration
	.dwattr $C$DW$63, DW_AT_external
$C$DW$64	.dwtag  DW_TAG_variable, DW_AT_name("a_sc")
	.dwattr $C$DW$64, DW_AT_TI_symbol_name("_a_sc")
	.dwattr $C$DW$64, DW_AT_type(*$C$DW$T$61)
	.dwattr $C$DW$64, DW_AT_declaration
	.dwattr $C$DW$64, DW_AT_external
$C$DW$65	.dwtag  DW_TAG_variable, DW_AT_name("output")
	.dwattr $C$DW$65, DW_AT_TI_symbol_name("_output")
	.dwattr $C$DW$65, DW_AT_type(*$C$DW$T$58)
	.dwattr $C$DW$65, DW_AT_declaration
	.dwattr $C$DW$65, DW_AT_external
$C$DW$66	.dwtag  DW_TAG_variable, DW_AT_name("input")
	.dwattr $C$DW$66, DW_AT_TI_symbol_name("_input")
	.dwattr $C$DW$66, DW_AT_type(*$C$DW$T$59)
	.dwattr $C$DW$66, DW_AT_declaration
	.dwattr $C$DW$66, DW_AT_external
	.global	_a
_a:	.usect	".far",1608,8
$C$DW$67	.dwtag  DW_TAG_variable, DW_AT_name("a")
	.dwattr $C$DW$67, DW_AT_TI_symbol_name("_a")
	.dwattr $C$DW$67, DW_AT_location[DW_OP_addr _a]
	.dwattr $C$DW$67, DW_AT_type(*$C$DW$T$63)
	.dwattr $C$DW$67, DW_AT_external
;	C:\MATHLIB_Tools\CCSV5_5_0\ccsv5\tools\compiler\c6000_7.4.2\bin\acp6x.exe -@C:\\Users\\gtbldadm\\AppData\\Local\\Temp\\0234018 
	.sect	".text"
	.clink

$C$DW$68	.dwtag  DW_TAG_subprogram, DW_AT_name("profile_init")
	.dwattr $C$DW$68, DW_AT_low_pc(_profile_init)
	.dwattr $C$DW$68, DW_AT_high_pc(0x00)
	.dwattr $C$DW$68, DW_AT_TI_symbol_name("_profile_init")
	.dwattr $C$DW$68, DW_AT_TI_begin_file("../../../../../../../ti/mathlib/src/common/driver.h")
	.dwattr $C$DW$68, DW_AT_TI_begin_line(0x70)
	.dwattr $C$DW$68, DW_AT_TI_begin_column(0x14)
	.dwattr $C$DW$68, DW_AT_TI_max_frame_size(0x08)
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 112,column 44,is_stmt,address _profile_init

	.dwfde $C$DW$CIE, _profile_init
$C$DW$69	.dwtag  DW_TAG_formal_parameter, DW_AT_name("code")
	.dwattr $C$DW$69, DW_AT_TI_symbol_name("_code")
	.dwattr $C$DW$69, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$69, DW_AT_location[DW_OP_reg4]

;******************************************************************************
;* FUNCTION NAME: profile_init                                                *
;*                                                                            *
;*   Regs Modified     : A3,A4,A5,A6,A7,B4,B5,SP                              *
;*   Regs Used         : A3,A4,A5,A6,A7,B3,B4,B5,SP                           *
;*   Local Frame Size  : 0 Args + 4 Auto + 0 Save = 4 byte                    *
;******************************************************************************
_profile_init:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 30
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           SUB     .L2     SP,8,SP           ; |112| 
	.dwcfi	cfa_offset, 8
$C$DW$70	.dwtag  DW_TAG_variable, DW_AT_name("code")
	.dwattr $C$DW$70, DW_AT_TI_symbol_name("_code")
	.dwattr $C$DW$70, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$70, DW_AT_location[DW_OP_breg31 4]
           STW     .D2T1   A4,*+SP(4)        ; |112| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 114,column 3,is_stmt
           MVKL    .S2     _cycle_counts,B5

           MVKH    .S2     _cycle_counts,B5
||         MV      .L2X    A4,B4
||         ZERO    .L1     A4                ; |114| 
||         ZERO    .S1     A5                ; |114| 

           STDW    .D2T1   A5:A4,*+B5[B4]    ; |114| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 115,column 3,is_stmt
           MV      .L2X    A4,B4
           MVC     .S2     B4,TSCL           ; |115| 
           NOP             1
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 116,column 3,is_stmt
           MVC     .S2     TSCL,B4           ; |116| 
           MVC     .S2     TSCH,B5           ; |116| 
           MVKL    .S1     _t_start,A3
           MVKH    .S1     _t_start,A3
           STDW    .D1T2   B5:B4,*A3         ; |116| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 117,column 3,is_stmt
           LDDW    .D1T1   *A3,A7:A6         ; |117| 
           NOP             3
           MVC     .S2     TSCL,B4           ; |117| 
           MVC     .S2     TSCH,B5           ; |117| 

           SUBU    .L1X    B4,A6,A5:A4       ; |117| 
||         SUB     .L2X    B5,A7,B4          ; |117| 
||         MVKL    .S1     _t_offset,A3

           EXT     .S1     A5,24,24,A5       ; |117| 

           ADD     .L1X    B4,A5,A5          ; |117| 
||         MVKH    .S1     _t_offset,A3

           STDW    .D1T1   A5:A4,*A3         ; |117| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 118,column 1,is_stmt
           ADD     .L2     8,SP,SP           ; |118| 
	.dwcfi	cfa_offset, 0
	.dwcfi	cfa_offset, 0
$C$DW$71	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$71, DW_AT_low_pc(0x00)
	.dwattr $C$DW$71, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |118| 
           ; BRANCH OCCURS {B3}              ; |118| 
	.dwattr $C$DW$68, DW_AT_TI_end_file("../../../../../../../ti/mathlib/src/common/driver.h")
	.dwattr $C$DW$68, DW_AT_TI_end_line(0x76)
	.dwattr $C$DW$68, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$68

	.sect	".text"
	.clink

$C$DW$72	.dwtag  DW_TAG_subprogram, DW_AT_name("profile_start")
	.dwattr $C$DW$72, DW_AT_low_pc(_profile_start)
	.dwattr $C$DW$72, DW_AT_high_pc(0x00)
	.dwattr $C$DW$72, DW_AT_TI_symbol_name("_profile_start")
	.dwattr $C$DW$72, DW_AT_TI_begin_file("../../../../../../../ti/mathlib/src/common/driver.h")
	.dwattr $C$DW$72, DW_AT_TI_begin_line(0x78)
	.dwattr $C$DW$72, DW_AT_TI_begin_column(0x14)
	.dwattr $C$DW$72, DW_AT_TI_max_frame_size(0x00)
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 120,column 41,is_stmt,address _profile_start

	.dwfde $C$DW$CIE, _profile_start

;******************************************************************************
;* FUNCTION NAME: profile_start                                               *
;*                                                                            *
;*   Regs Modified     : B4,B5,B6,B7                                          *
;*   Regs Used         : B3,B4,B5,B6,B7                                       *
;*   Local Frame Size  : 0 Args + 0 Auto + 0 Save = 0 byte                    *
;******************************************************************************
_profile_start:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 122,column 3,is_stmt
           MVC     .S2     TSCL,B6           ; |122| 
           MVC     .S2     TSCH,B4           ; |122| 
           MVKL    .S2     _t_start,B5

           MVKH    .S2     _t_start,B5
||         MV      .L2     B4,B7             ; |122| 

           STDW    .D2T2   B7:B6,*B5         ; |122| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 124,column 1,is_stmt
	.dwcfi	cfa_offset, 0
$C$DW$73	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$73, DW_AT_low_pc(0x00)
	.dwattr $C$DW$73, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |124| 
           ; BRANCH OCCURS {B3}              ; |124| 
	.dwattr $C$DW$72, DW_AT_TI_end_file("../../../../../../../ti/mathlib/src/common/driver.h")
	.dwattr $C$DW$72, DW_AT_TI_end_line(0x7c)
	.dwattr $C$DW$72, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$72

	.sect	".text"
	.clink

$C$DW$74	.dwtag  DW_TAG_subprogram, DW_AT_name("profile_update")
	.dwattr $C$DW$74, DW_AT_low_pc(_profile_update)
	.dwattr $C$DW$74, DW_AT_high_pc(0x00)
	.dwattr $C$DW$74, DW_AT_TI_symbol_name("_profile_update")
	.dwattr $C$DW$74, DW_AT_TI_begin_file("../../../../../../../ti/mathlib/src/common/driver.h")
	.dwattr $C$DW$74, DW_AT_TI_begin_line(0x7e)
	.dwattr $C$DW$74, DW_AT_TI_begin_column(0x14)
	.dwattr $C$DW$74, DW_AT_TI_max_frame_size(0x08)
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 126,column 46,is_stmt,address _profile_update

	.dwfde $C$DW$CIE, _profile_update
$C$DW$75	.dwtag  DW_TAG_formal_parameter, DW_AT_name("code")
	.dwattr $C$DW$75, DW_AT_TI_symbol_name("_code")
	.dwattr $C$DW$75, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$75, DW_AT_location[DW_OP_reg4]

;******************************************************************************
;* FUNCTION NAME: profile_update                                              *
;*                                                                            *
;*   Regs Modified     : A3,A4,A5,A6,A7,B4,B5,B6,B7,B8,B9,SP                  *
;*   Regs Used         : A3,A4,A5,A6,A7,B3,B4,B5,B6,B7,B8,B9,SP               *
;*   Local Frame Size  : 0 Args + 4 Auto + 0 Save = 4 byte                    *
;******************************************************************************
_profile_update:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 32
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           SUB     .L2     SP,8,SP           ; |126| 
	.dwcfi	cfa_offset, 8
$C$DW$76	.dwtag  DW_TAG_variable, DW_AT_name("code")
	.dwattr $C$DW$76, DW_AT_TI_symbol_name("_code")
	.dwattr $C$DW$76, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$76, DW_AT_location[DW_OP_breg31 4]
           STW     .D2T1   A4,*+SP(4)        ; |126| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 128,column 3,is_stmt
           MVC     .S2     TSCL,B6           ; |128| 
           MVC     .S2     TSCH,B4           ; |128| 
           MVKL    .S1     _t_stop,A3

           MVKH    .S1     _t_stop,A3
||         MV      .L2     B4,B7             ; |128| 

           STDW    .D1T2   B7:B6,*A3         ; |128| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 130,column 3,is_stmt

           MV      .L2X    A3,B5
||         MVKL    .S1     _t_start,A3

           MVKH    .S1     _t_start,A3

           LDDW    .D1T1   *A3,A5:A4         ; |130| 
||         LDDW    .D2T2   *B5,B7:B6         ; |130| 

           LDW     .D2T2   *+SP(4),B4        ; |130| 
           MVKL    .S2     _t_offset,B8
           MVKH    .S2     _t_offset,B8

           MVKL    .S2     _cycle_counts,B5
||         LDDW    .D2T2   *B8,B9:B8         ; |130| 

           MVKH    .S2     _cycle_counts,B5
||         SUBU    .L1X    B6,A4,A7:A6       ; |130| 

           ADDAD   .D2     B5,B4,B6          ; |130| 
||         EXT     .S1     A7,24,24,A4       ; |130| 

           LDDW    .D2T2   *B6,B5:B4         ; |130| 
||         SUB     .L2X    B7,A5,B7          ; |130| 

           ADD     .L2X    B7,A4,B7          ; |130| 
||         MV      .L1     A6,A3             ; |130| 

           SUB     .L2     B7,B9,B7          ; |130| 
||         SUBU    .L1X    A3,B8,A5:A4       ; |130| 

           EXT     .S1     A5,24,24,A3       ; |130| 
           ADD     .L1X    B7,A3,A6          ; |130| 
           ADDU    .L1X    B4,A4,A5:A4       ; |130| 
           ADD     .L1X    A5,B5,A3          ; |130| 
           ADD     .L1     A3,A6,A5          ; |130| 
           STDW    .D2T1   A5:A4,*B6         ; |130| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 131,column 1,is_stmt
           ADD     .L2     8,SP,SP           ; |131| 
	.dwcfi	cfa_offset, 0
	.dwcfi	cfa_offset, 0
$C$DW$77	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$77, DW_AT_low_pc(0x00)
	.dwattr $C$DW$77, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |131| 
           ; BRANCH OCCURS {B3}              ; |131| 
	.dwattr $C$DW$74, DW_AT_TI_end_file("../../../../../../../ti/mathlib/src/common/driver.h")
	.dwattr $C$DW$74, DW_AT_TI_end_line(0x83)
	.dwattr $C$DW$74, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$74

	.sect	".text"
	.clink

$C$DW$78	.dwtag  DW_TAG_subprogram, DW_AT_name("profile_avg")
	.dwattr $C$DW$78, DW_AT_low_pc(_profile_avg)
	.dwattr $C$DW$78, DW_AT_high_pc(0x00)
	.dwattr $C$DW$78, DW_AT_TI_symbol_name("_profile_avg")
	.dwattr $C$DW$78, DW_AT_TI_begin_file("../../../../../../../ti/mathlib/src/common/driver.h")
	.dwattr $C$DW$78, DW_AT_TI_begin_line(0x85)
	.dwattr $C$DW$78, DW_AT_TI_begin_column(0x14)
	.dwattr $C$DW$78, DW_AT_TI_max_frame_size(0x10)
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 133,column 50,is_stmt,address _profile_avg

	.dwfde $C$DW$CIE, _profile_avg
$C$DW$79	.dwtag  DW_TAG_formal_parameter, DW_AT_name("code")
	.dwattr $C$DW$79, DW_AT_TI_symbol_name("_code")
	.dwattr $C$DW$79, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$79, DW_AT_location[DW_OP_reg4]
$C$DW$80	.dwtag  DW_TAG_formal_parameter, DW_AT_name("n")
	.dwattr $C$DW$80, DW_AT_TI_symbol_name("_n")
	.dwattr $C$DW$80, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$80, DW_AT_location[DW_OP_reg20]

;******************************************************************************
;* FUNCTION NAME: profile_avg                                                 *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,DP,SP,A16,A17,A18,A19,A20,A21,A22,A23,  *
;*                           A24,A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19, *
;*                           B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31  *
;*   Local Frame Size  : 0 Args + 8 Auto + 4 Save = 12 byte                   *
;******************************************************************************
_profile_avg:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 53
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           STW     .D2T2   B3,*SP--(16)      ; |133| 
	.dwcfi	cfa_offset, 16
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$81	.dwtag  DW_TAG_variable, DW_AT_name("code")
	.dwattr $C$DW$81, DW_AT_TI_symbol_name("_code")
	.dwattr $C$DW$81, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$81, DW_AT_location[DW_OP_breg31 4]
$C$DW$82	.dwtag  DW_TAG_variable, DW_AT_name("n")
	.dwattr $C$DW$82, DW_AT_TI_symbol_name("_n")
	.dwattr $C$DW$82, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$82, DW_AT_location[DW_OP_breg31 8]
           STW     .D2T2   B4,*+SP(8)        ; |133| 
           STW     .D2T1   A4,*+SP(4)        ; |133| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 135,column 3,is_stmt
           MVKL    .S2     _cycle_counts,B4

           MVKH    .S2     _cycle_counts,B4
||         MV      .L2X    A4,B5

           LDDW    .D2T2   *+B4[B5],B5:B4    ; |135| 
           NOP             4
$C$DW$83	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$83, DW_AT_low_pc(0x00)
	.dwattr $C$DW$83, DW_AT_name("__fltllif")
	.dwattr $C$DW$83, DW_AT_TI_call

           CALLP   .S2     __fltllif,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |135| 

$C$RL1:    ; CALL OCCURS {__fltllif} {0}     ; |135| 
           LDW     .D2T2   *+SP(8),B4        ; |135| 
           NOP             4
$C$DW$84	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$84, DW_AT_low_pc(0x00)
	.dwattr $C$DW$84, DW_AT_name("__divf")
	.dwattr $C$DW$84, DW_AT_TI_call

           CALLP   .S2     __divf,B3
||         INTSP   .L2     B4,B4             ; |135| 

$C$RL2:    ; CALL OCCURS {__divf} {0}        ; |135| 
           SPDP    .S1     A4,A5:A4          ; |135| 
           ZERO    .L1     A7

           SET     .S1     A7,0x15,0x1d,A7
||         ZERO    .L1     A6                ; |135| 

$C$DW$85	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$85, DW_AT_low_pc(0x00)
	.dwattr $C$DW$85, DW_AT_name("__fixdlli")
	.dwattr $C$DW$85, DW_AT_TI_call

           CALLP   .S2     __fixdlli,B3
||         FADDDP  .L1     A7:A6,A5:A4,A5:A4 ; |135| 

$C$RL3:    ; CALL OCCURS {__fixdlli} {0}     ; |135| 
           LDW     .D2T2   *+SP(4),B4        ; |135| 
           MVKL    .S2     _cycle_counts,B5
           MVKH    .S2     _cycle_counts,B5
           NOP             2
           STDW    .D2T1   A5:A4,*+B5[B4]    ; |135| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/driver.h",line 136,column 1,is_stmt
           LDW     .D2T2   *++SP(16),B3      ; |136| 
           NOP             4
	.dwcfi	cfa_offset, 0
	.dwcfi	restore_reg, 19
	.dwcfi	cfa_offset, 0
$C$DW$86	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$86, DW_AT_low_pc(0x00)
	.dwattr $C$DW$86, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |136| 
           ; BRANCH OCCURS {B3}              ; |136| 
	.dwattr $C$DW$78, DW_AT_TI_end_file("../../../../../../../ti/mathlib/src/common/driver.h")
	.dwattr $C$DW$78, DW_AT_TI_end_line(0x88)
	.dwattr $C$DW$78, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$78

	.sect	".text"
	.clink

$C$DW$87	.dwtag  DW_TAG_subprogram, DW_AT_name("allequal")
	.dwattr $C$DW$87, DW_AT_low_pc(_allequal)
	.dwattr $C$DW$87, DW_AT_high_pc(0x00)
	.dwattr $C$DW$87, DW_AT_TI_symbol_name("_allequal")
	.dwattr $C$DW$87, DW_AT_TI_begin_file("../../../../../../../ti/mathlib/src/common/drvdp.h")
	.dwattr $C$DW$87, DW_AT_TI_begin_line(0x3e)
	.dwattr $C$DW$87, DW_AT_TI_begin_column(0x14)
	.dwattr $C$DW$87, DW_AT_TI_max_frame_size(0x18)
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/drvdp.h",line 63,column 1,is_stmt,address _allequal

	.dwfde $C$DW$CIE, _allequal
$C$DW$88	.dwtag  DW_TAG_formal_parameter, DW_AT_name("size")
	.dwattr $C$DW$88, DW_AT_TI_symbol_name("_size")
	.dwattr $C$DW$88, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$88, DW_AT_location[DW_OP_reg4]
$C$DW$89	.dwtag  DW_TAG_formal_parameter, DW_AT_name("tol")
	.dwattr $C$DW$89, DW_AT_TI_symbol_name("_tol")
	.dwattr $C$DW$89, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$89, DW_AT_location[DW_OP_reg20]
$C$DW$90	.dwtag  DW_TAG_formal_parameter, DW_AT_name("req_flags")
	.dwattr $C$DW$90, DW_AT_TI_symbol_name("_req_flags")
	.dwattr $C$DW$90, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$90, DW_AT_location[DW_OP_reg6]

;******************************************************************************
;* FUNCTION NAME: allequal                                                    *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP,A16,A17,A18,A19,A20,A21,A22,A23,A24, *
;*                           A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19,B20, *
;*                           B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,DP,SP,A16,A17,A18,A19,A20,A21,A22,A23,  *
;*                           A24,A25,A26,A27,A28,A29,A30,A31,B16,B17,B18,B19, *
;*                           B20,B21,B22,B23,B24,B25,B26,B27,B28,B29,B30,B31  *
;*   Local Frame Size  : 0 Args + 16 Auto + 4 Save = 20 byte                  *
;******************************************************************************
_allequal:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 73
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           STW     .D2T2   B3,*SP--(24)      ; |63| 
	.dwcfi	cfa_offset, 24
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$91	.dwtag  DW_TAG_variable, DW_AT_name("size")
	.dwattr $C$DW$91, DW_AT_TI_symbol_name("_size")
	.dwattr $C$DW$91, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$91, DW_AT_location[DW_OP_breg31 4]
$C$DW$92	.dwtag  DW_TAG_variable, DW_AT_name("tol")
	.dwattr $C$DW$92, DW_AT_TI_symbol_name("_tol")
	.dwattr $C$DW$92, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$92, DW_AT_location[DW_OP_breg31 8]
$C$DW$93	.dwtag  DW_TAG_variable, DW_AT_name("req_flags")
	.dwattr $C$DW$93, DW_AT_TI_symbol_name("_req_flags")
	.dwattr $C$DW$93, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$93, DW_AT_location[DW_OP_breg31 16]
           STDW    .D2T2   B5:B4,*+SP(8)     ; |63| 

           MV      .L2X    A4,B6             ; |63| 
||         STW     .D2T1   A6,*+SP(16)       ; |63| 

           STW     .D2T2   B6,*+SP(4)        ; |63| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/drvdp.h",line 66,column 3,is_stmt
           MVKL    .S1     _output+4,A3
           MVKH    .S1     _output+4,A3

           ADD     .L1     -4,A3,A4
||         LDW     .D1T2   *A3,B4            ; |66| 
||         MV      .S1     A6,A8             ; |66| 
||         LDW     .D2T1   *+SP(4),A6        ; |66| 

$C$DW$94	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$94, DW_AT_low_pc(0x00)
	.dwattr $C$DW$94, DW_AT_name("_isequal")
	.dwattr $C$DW$94, DW_AT_TI_call

           CALLP   .S2     _isequal,B3
||         LDW     .D1T1   *A4,A4            ; |66| 
||         LDDW    .D2T2   *+SP(8),B7:B6     ; |66| 

$C$RL4:    ; CALL OCCURS {_isequal} {0}      ; |66| 
           MVKL    .S2     _fcn_pass+4,B5

           MVKH    .S2     _fcn_pass+4,B5
||         MV      .L2X    A4,B4             ; |66| 

           STW     .D2T2   B4,*B5            ; |66| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/drvdp.h",line 67,column 3,is_stmt
           MVKL    .S1     _output,A4
           MVKH    .S1     _output,A4

           ADDAD   .D1     A4,1,A3
||         LDW     .D2T1   *+SP(16),A8       ; |67| 

           LDW     .D1T1   *A4,A4            ; |67| 
||         LDDW    .D2T2   *+SP(8),B7:B6     ; |67| 

$C$DW$95	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$95, DW_AT_low_pc(0x00)
	.dwattr $C$DW$95, DW_AT_name("_isequal")
	.dwattr $C$DW$95, DW_AT_TI_call

           CALLP   .S2     _isequal,B3
||         LDW     .D1T2   *A3,B4            ; |67| 
||         LDW     .D2T1   *+SP(4),A6        ; |67| 

$C$RL5:    ; CALL OCCURS {_isequal} {0}      ; |67| 
           MVKL    .S2     _fcn_pass+8,B5

           MVKH    .S2     _fcn_pass+8,B5
||         MV      .L2X    A4,B4             ; |67| 

           STW     .D2T2   B4,*B5            ; |67| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/drvdp.h",line 68,column 3,is_stmt
           MVKL    .S2     _output,B4

           MVKH    .S2     _output,B4
||         LDW     .D2T1   *+SP(4),A6        ; |68| 

           MVK     .S1     12,A3
||         LDW     .D2T1   *+SP(16),A8       ; |68| 

           ADD     .L1X    B4,A3,A3
||         LDDW    .D2T2   *+SP(8),B7:B6     ; |68| 

$C$DW$96	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$96, DW_AT_low_pc(0x00)
	.dwattr $C$DW$96, DW_AT_name("_isequal")
	.dwattr $C$DW$96, DW_AT_TI_call

           CALLP   .S2     _isequal,B3
||         LDW     .D2T1   *B4,A4            ; |68| 
||         LDW     .D1T2   *A3,B4            ; |68| 

$C$RL6:    ; CALL OCCURS {_isequal} {0}      ; |68| 
           MVKL    .S2     _fcn_pass+12,B5

           MVKH    .S2     _fcn_pass+12,B5
||         MV      .L2X    A4,B4             ; |68| 

           STW     .D2T2   B4,*B5            ; |68| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/drvdp.h",line 69,column 3,is_stmt
           MVKL    .S2     _output,B4
           MVKH    .S2     _output,B4
           MVK     .S1     16,A3
           ADD     .L1X    B4,A3,A3
           LDW     .D1T1   *A3,A3            ; |69| 
           LDW     .D2T1   *B4,A4            ; |69| 
           LDW     .D2T1   *+SP(4),A6        ; |69| 
           LDW     .D2T1   *+SP(16),A8       ; |69| 
           LDDW    .D2T2   *+SP(8),B7:B6     ; |69| 
$C$DW$97	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$97, DW_AT_low_pc(0x04)
	.dwattr $C$DW$97, DW_AT_name("_isequal")
	.dwattr $C$DW$97, DW_AT_TI_call

           MV      .L2X    A3,B4             ; |69| 
||         CALLP   .S2     _isequal,B3

$C$RL7:    ; CALL OCCURS {_isequal} {0}      ; |69| 
           MVKL    .S1     _fcn_pass+16,A3
           MVKH    .S1     _fcn_pass+16,A3
           STW     .D1T1   A4,*A3            ; |69| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/drvdp.h",line 72,column 3,is_stmt
           MVKL    .S1     _fcn_pass+4,A3
           MVKH    .S1     _fcn_pass+4,A3
           LDW     .D1T1   *A3,A0            ; |72| 
           ADD     .L2X    4,A3,B4
           ZERO    .L1     A3                ; |72| 
           NOP             2
   [!A0]   BNOP    .S1     $C$L1,5           ; |72| 
           ; BRANCHCC OCCURS {$C$L1}         ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *B4,B0            ; |72| 
           ADD     .L2     4,B4,B4
           NOP             3
   [!B0]   BNOP    .S1     $C$L1,5           ; |72| 
           ; BRANCHCC OCCURS {$C$L1}         ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *B4,B0            ; |72| 
           ADD     .L2     4,B4,B4
           NOP             3
   [!B0]   BNOP    .S1     $C$L1,5           ; |72| 
           ; BRANCHCC OCCURS {$C$L1}         ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
           LDW     .D2T2   *B4,B0            ; |72| 
           NOP             4
   [!B0]   BNOP    .S1     $C$L1,5           ; |72| 
           ; BRANCHCC OCCURS {$C$L1}         ; |72| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 1
           MVK     .L1     0x1,A3            ; |72| 
;** --------------------------------------------------------------------------*
$C$L1:    
;          EXCLUSIVE CPU CYCLES: 14
           MVKL    .S1     _all_pass,A4
           MVKH    .S1     _all_pass,A4
           STW     .D1T1   A3,*A4            ; |72| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/common/drvdp.h",line 74,column 1,is_stmt
           LDW     .D2T2   *++SP(24),B3      ; |74| 
           NOP             4
	.dwcfi	cfa_offset, 0
	.dwcfi	restore_reg, 19
	.dwcfi	cfa_offset, 0
$C$DW$98	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$98, DW_AT_low_pc(0x00)
	.dwattr $C$DW$98, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |74| 
           ; BRANCH OCCURS {B3}              ; |74| 
	.dwattr $C$DW$87, DW_AT_TI_end_file("../../../../../../../ti/mathlib/src/common/drvdp.h")
	.dwattr $C$DW$87, DW_AT_TI_end_line(0x4a)
	.dwattr $C$DW$87, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$87

	.sect	".text:optci"
	.clink

$C$DW$99	.dwtag  DW_TAG_subprogram, DW_AT_name("divdp_asinhdp_i")
	.dwattr $C$DW$99, DW_AT_low_pc(_divdp_asinhdp_i)
	.dwattr $C$DW$99, DW_AT_high_pc(0x00)
	.dwattr $C$DW$99, DW_AT_TI_symbol_name("_divdp_asinhdp_i")
	.dwattr $C$DW$99, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$99, DW_AT_TI_begin_file("../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h")
	.dwattr $C$DW$99, DW_AT_TI_begin_line(0x3e)
	.dwattr $C$DW$99, DW_AT_TI_begin_column(0x16)
	.dwattr $C$DW$99, DW_AT_TI_max_frame_size(0x40)
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 63,column 1,is_stmt,address _divdp_asinhdp_i

	.dwfde $C$DW$CIE, _divdp_asinhdp_i
$C$DW$100	.dwtag  DW_TAG_formal_parameter, DW_AT_name("a")
	.dwattr $C$DW$100, DW_AT_TI_symbol_name("_a")
	.dwattr $C$DW$100, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$100, DW_AT_location[DW_OP_reg4]
$C$DW$101	.dwtag  DW_TAG_formal_parameter, DW_AT_name("b")
	.dwattr $C$DW$101, DW_AT_TI_symbol_name("_b")
	.dwattr $C$DW$101, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$101, DW_AT_location[DW_OP_reg20]

;******************************************************************************
;* FUNCTION NAME: divdp_asinhdp_i                                             *
;*                                                                            *
;*   Regs Modified     : A4,A5,B0,B4,B5,B6,B7,B8,B9,SP                        *
;*   Regs Used         : A4,A5,B0,B3,B4,B5,B6,B7,B8,B9,SP                     *
;*   Local Frame Size  : 0 Args + 60 Auto + 0 Save = 60 byte                  *
;******************************************************************************
_divdp_asinhdp_i:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 92
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           ADDK    .S2     -64,SP            ; |63| 
	.dwcfi	cfa_offset, 64
$C$DW$102	.dwtag  DW_TAG_variable, DW_AT_name("a")
	.dwattr $C$DW$102, DW_AT_TI_symbol_name("_a")
	.dwattr $C$DW$102, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$102, DW_AT_location[DW_OP_breg31 8]
$C$DW$103	.dwtag  DW_TAG_variable, DW_AT_name("b")
	.dwattr $C$DW$103, DW_AT_TI_symbol_name("_b")
	.dwattr $C$DW$103, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$103, DW_AT_location[DW_OP_breg31 16]
$C$DW$104	.dwtag  DW_TAG_variable, DW_AT_name("y")
	.dwattr $C$DW$104, DW_AT_TI_symbol_name("_y")
	.dwattr $C$DW$104, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$104, DW_AT_location[DW_OP_breg31 24]
$C$DW$105	.dwtag  DW_TAG_variable, DW_AT_name("x1")
	.dwattr $C$DW$105, DW_AT_TI_symbol_name("_x1")
	.dwattr $C$DW$105, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$105, DW_AT_location[DW_OP_breg31 32]
$C$DW$106	.dwtag  DW_TAG_variable, DW_AT_name("x2")
	.dwattr $C$DW$106, DW_AT_TI_symbol_name("_x2")
	.dwattr $C$DW$106, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$106, DW_AT_location[DW_OP_breg31 40]
$C$DW$107	.dwtag  DW_TAG_variable, DW_AT_name("x3")
	.dwattr $C$DW$107, DW_AT_TI_symbol_name("_x3")
	.dwattr $C$DW$107, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$107, DW_AT_location[DW_OP_breg31 48]
$C$DW$108	.dwtag  DW_TAG_variable, DW_AT_name("two")
	.dwattr $C$DW$108, DW_AT_TI_symbol_name("_two")
	.dwattr $C$DW$108, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$108, DW_AT_location[DW_OP_breg31 56]

           STDW    .D2T1   A5:A4,*+SP(8)     ; |63| 
||         DADD    .L1X    0,B5:B4,A5:A4     ; |63| 

           STDW    .D2T1   A5:A4,*+SP(16)    ; |63| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 65,column 17,is_stmt
           ZERO    .L2     B5

           SET     .S2     B5,0x1e,0x1e,B5
||         ZERO    .L2     B4                ; |65| 

           STDW    .D2T2   B5:B4,*+SP(56)    ; |65| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 67,column 3,is_stmt
           LDDW    .D2T2   *+SP(16),B5:B4    ; |67| 
           NOP             4
           RCPDP   .S2     B5:B4,B5:B4       ; |67| 
           NOP             1
           STDW    .D2T2   B5:B4,*+SP(32)    ; |67| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 68,column 3,is_stmt
           LDDW    .D2T1   *+SP(32),A5:A4    ; |68| 
           LDDW    .D2T2   *+SP(16),B5:B4    ; |68| 
           LDDW    .D2T2   *+SP(56),B7:B6    ; |68| 
           NOP             3
           FMPYDP  .M1X    A5:A4,B5:B4,A5:A4 ; |68| 
           LDDW    .D2T2   *+SP(32),B5:B4    ; |68| 
           NOP             3
           FSUBDP  .L2X    B7:B6,A5:A4,B7:B6 ; |68| 
           NOP             2
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |68| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(32)    ; |68| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 69,column 3,is_stmt
           LDDW    .D2T2   *+SP(16),B5:B4    ; |69| 
           LDDW    .D2T2   *+SP(32),B7:B6    ; |69| 
           LDDW    .D2T2   *+SP(56),B9:B8    ; |69| 
           NOP             3
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |69| 
           NOP             3
           FSUBDP  .L2     B9:B8,B5:B4,B5:B4 ; |69| 
           NOP             2
           FMPYDP  .M2     B5:B4,B7:B6,B5:B4 ; |69| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(40)    ; |69| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 70,column 3,is_stmt
           LDDW    .D2T2   *+SP(16),B5:B4    ; |70| 
           LDDW    .D2T2   *+SP(40),B7:B6    ; |70| 
           NOP             4
           FMPYDP  .M2     B7:B6,B5:B4,B7:B6 ; |70| 
           LDDW    .D2T2   *+SP(40),B5:B4    ; |70| 
           NOP             2
           FSUBDP  .L2     B9:B8,B7:B6,B7:B6 ; |70| 
           NOP             2
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |70| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(48)    ; |70| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 71,column 3,is_stmt
           LDDW    .D2T1   *+SP(8),A5:A4     ; |71| 
           NOP             4
           FMPYDP  .M1X    A5:A4,B5:B4,A5:A4 ; |71| 
           NOP             3
           STDW    .D2T1   A5:A4,*+SP(24)    ; |71| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 73,column 3,is_stmt
           LDDW    .D2T2   *+SP(8),B7:B6     ; |73| 
           ZERO    .L2     B5:B4             ; |73| 
           NOP             3
           CMPEQDP .S2     B7:B6,B5:B4,B0    ; |73| 
           NOP             1
   [!B0]   BNOP    .S1     $C$L2,5           ; |73| 
           ; BRANCHCC OCCURS {$C$L2}         ; |73| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 2
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 74,column 5,is_stmt
           ZERO    .L2     B5:B4             ; |74| 
           STDW    .D2T2   B5:B4,*+SP(24)    ; |74| 
;** --------------------------------------------------------------------------*
$C$L2:    
;          EXCLUSIVE CPU CYCLES: 12
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 77,column 3,is_stmt
           LDDW    .D2T1   *+SP(24),A5:A4    ; |77| 
           NOP             4
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 78,column 1,is_stmt
           ADDK    .S2     64,SP             ; |78| 
	.dwcfi	cfa_offset, 0
	.dwcfi	cfa_offset, 0
$C$DW$109	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$109, DW_AT_low_pc(0x00)
	.dwattr $C$DW$109, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |78| 
           ; BRANCH OCCURS {B3}              ; |78| 
	.dwattr $C$DW$99, DW_AT_TI_end_file("../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h")
	.dwattr $C$DW$99, DW_AT_TI_end_line(0x4e)
	.dwattr $C$DW$99, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$99

	.sect	".text:optci"
	.clink

$C$DW$110	.dwtag  DW_TAG_subprogram, DW_AT_name("logdp_asinhdp_i")
	.dwattr $C$DW$110, DW_AT_low_pc(_logdp_asinhdp_i)
	.dwattr $C$DW$110, DW_AT_high_pc(0x00)
	.dwattr $C$DW$110, DW_AT_TI_symbol_name("_logdp_asinhdp_i")
	.dwattr $C$DW$110, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$110, DW_AT_TI_begin_file("../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h")
	.dwattr $C$DW$110, DW_AT_TI_begin_line(0x59)
	.dwattr $C$DW$110, DW_AT_TI_begin_column(0x16)
	.dwattr $C$DW$110, DW_AT_TI_max_frame_size(0xe0)
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 90,column 1,is_stmt,address _logdp_asinhdp_i

	.dwfde $C$DW$CIE, _logdp_asinhdp_i
$C$DW$111	.dwtag  DW_TAG_formal_parameter, DW_AT_name("x")
	.dwattr $C$DW$111, DW_AT_TI_symbol_name("_x")
	.dwattr $C$DW$111, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$111, DW_AT_location[DW_OP_reg4]

;******************************************************************************
;* FUNCTION NAME: logdp_asinhdp_i                                             *
;*                                                                            *
;*   Regs Modified     : A0,A4,A5,A6,A7,A8,A9,B0,B3,B4,B5,B6,B7,B8,B9,SP      *
;*   Regs Used         : A0,A4,A5,A6,A7,A8,A9,B0,B3,B4,B5,B6,B7,B8,B9,SP      *
;*   Local Frame Size  : 0 Args + 216 Auto + 4 Save = 220 byte                *
;******************************************************************************
_logdp_asinhdp_i:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 91
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           ADDK    .S2     -224,SP           ; |90| 
	.dwcfi	cfa_offset, 224
           STW     .D2T2   B3,*+SP(224)      ; |90| 
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$112	.dwtag  DW_TAG_variable, DW_AT_name("x")
	.dwattr $C$DW$112, DW_AT_TI_symbol_name("_x")
	.dwattr $C$DW$112, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$112, DW_AT_location[DW_OP_breg31 8]
$C$DW$113	.dwtag  DW_TAG_variable, DW_AT_name("Half")
	.dwattr $C$DW$113, DW_AT_TI_symbol_name("_Half")
	.dwattr $C$DW$113, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$113, DW_AT_location[DW_OP_breg31 16]
$C$DW$114	.dwtag  DW_TAG_variable, DW_AT_name("srHalf")
	.dwattr $C$DW$114, DW_AT_TI_symbol_name("_srHalf")
	.dwattr $C$DW$114, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$114, DW_AT_location[DW_OP_breg31 24]
$C$DW$115	.dwtag  DW_TAG_variable, DW_AT_name("a0")
	.dwattr $C$DW$115, DW_AT_TI_symbol_name("_a0")
	.dwattr $C$DW$115, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$115, DW_AT_location[DW_OP_breg31 32]
$C$DW$116	.dwtag  DW_TAG_variable, DW_AT_name("a1")
	.dwattr $C$DW$116, DW_AT_TI_symbol_name("_a1")
	.dwattr $C$DW$116, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$116, DW_AT_location[DW_OP_breg31 40]
$C$DW$117	.dwtag  DW_TAG_variable, DW_AT_name("a2")
	.dwattr $C$DW$117, DW_AT_TI_symbol_name("_a2")
	.dwattr $C$DW$117, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$117, DW_AT_location[DW_OP_breg31 48]
$C$DW$118	.dwtag  DW_TAG_variable, DW_AT_name("b0")
	.dwattr $C$DW$118, DW_AT_TI_symbol_name("_b0")
	.dwattr $C$DW$118, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$118, DW_AT_location[DW_OP_breg31 56]
$C$DW$119	.dwtag  DW_TAG_variable, DW_AT_name("b1")
	.dwattr $C$DW$119, DW_AT_TI_symbol_name("_b1")
	.dwattr $C$DW$119, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$119, DW_AT_location[DW_OP_breg31 64]
$C$DW$120	.dwtag  DW_TAG_variable, DW_AT_name("b2")
	.dwattr $C$DW$120, DW_AT_TI_symbol_name("_b2")
	.dwattr $C$DW$120, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$120, DW_AT_location[DW_OP_breg31 72]
$C$DW$121	.dwtag  DW_TAG_variable, DW_AT_name("c1")
	.dwattr $C$DW$121, DW_AT_TI_symbol_name("_c1")
	.dwattr $C$DW$121, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$121, DW_AT_location[DW_OP_breg31 80]
$C$DW$122	.dwtag  DW_TAG_variable, DW_AT_name("c2")
	.dwattr $C$DW$122, DW_AT_TI_symbol_name("_c2")
	.dwattr $C$DW$122, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$122, DW_AT_location[DW_OP_breg31 88]
$C$DW$123	.dwtag  DW_TAG_variable, DW_AT_name("max")
	.dwattr $C$DW$123, DW_AT_TI_symbol_name("_max")
	.dwattr $C$DW$123, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$123, DW_AT_location[DW_OP_breg31 96]
$C$DW$124	.dwtag  DW_TAG_variable, DW_AT_name("W")
	.dwattr $C$DW$124, DW_AT_TI_symbol_name("_W")
	.dwattr $C$DW$124, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$124, DW_AT_location[DW_OP_breg31 104]
$C$DW$125	.dwtag  DW_TAG_variable, DW_AT_name("X")
	.dwattr $C$DW$125, DW_AT_TI_symbol_name("_X")
	.dwattr $C$DW$125, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$125, DW_AT_location[DW_OP_breg31 112]
$C$DW$126	.dwtag  DW_TAG_variable, DW_AT_name("Y")
	.dwattr $C$DW$126, DW_AT_TI_symbol_name("_Y")
	.dwattr $C$DW$126, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$126, DW_AT_location[DW_OP_breg31 120]
$C$DW$127	.dwtag  DW_TAG_variable, DW_AT_name("Z")
	.dwattr $C$DW$127, DW_AT_TI_symbol_name("_Z")
	.dwattr $C$DW$127, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$127, DW_AT_location[DW_OP_breg31 128]
$C$DW$128	.dwtag  DW_TAG_variable, DW_AT_name("zn")
	.dwattr $C$DW$128, DW_AT_TI_symbol_name("_zn")
	.dwattr $C$DW$128, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$128, DW_AT_location[DW_OP_breg31 136]
$C$DW$129	.dwtag  DW_TAG_variable, DW_AT_name("zd")
	.dwattr $C$DW$129, DW_AT_TI_symbol_name("_zd")
	.dwattr $C$DW$129, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$129, DW_AT_location[DW_OP_breg31 144]
$C$DW$130	.dwtag  DW_TAG_variable, DW_AT_name("zn_2")
	.dwattr $C$DW$130, DW_AT_TI_symbol_name("_zn_2")
	.dwattr $C$DW$130, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$130, DW_AT_location[DW_OP_breg31 152]
$C$DW$131	.dwtag  DW_TAG_variable, DW_AT_name("zd_2")
	.dwattr $C$DW$131, DW_AT_TI_symbol_name("_zd_2")
	.dwattr $C$DW$131, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$131, DW_AT_location[DW_OP_breg31 160]
$C$DW$132	.dwtag  DW_TAG_variable, DW_AT_name("Rz")
	.dwattr $C$DW$132, DW_AT_TI_symbol_name("_Rz")
	.dwattr $C$DW$132, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$132, DW_AT_location[DW_OP_breg31 168]
$C$DW$133	.dwtag  DW_TAG_variable, DW_AT_name("Sa")
	.dwattr $C$DW$133, DW_AT_TI_symbol_name("_Sa")
	.dwattr $C$DW$133, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$133, DW_AT_location[DW_OP_breg31 176]
$C$DW$134	.dwtag  DW_TAG_variable, DW_AT_name("Bd")
	.dwattr $C$DW$134, DW_AT_TI_symbol_name("_Bd")
	.dwattr $C$DW$134, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$134, DW_AT_location[DW_OP_breg31 184]
$C$DW$135	.dwtag  DW_TAG_variable, DW_AT_name("Cn")
	.dwattr $C$DW$135, DW_AT_TI_symbol_name("_Cn")
	.dwattr $C$DW$135, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$135, DW_AT_location[DW_OP_breg31 192]
$C$DW$136	.dwtag  DW_TAG_variable, DW_AT_name("Da")
	.dwattr $C$DW$136, DW_AT_TI_symbol_name("_Da")
	.dwattr $C$DW$136, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$136, DW_AT_location[DW_OP_breg31 200]
$C$DW$137	.dwtag  DW_TAG_variable, DW_AT_name("upper")
	.dwattr $C$DW$137, DW_AT_TI_symbol_name("_upper")
	.dwattr $C$DW$137, DW_AT_type(*$C$DW$T$11)
	.dwattr $C$DW$137, DW_AT_location[DW_OP_breg31 208]
$C$DW$138	.dwtag  DW_TAG_variable, DW_AT_name("N")
	.dwattr $C$DW$138, DW_AT_TI_symbol_name("_N")
	.dwattr $C$DW$138, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$138, DW_AT_location[DW_OP_breg31 212]
$C$DW$139	.dwtag  DW_TAG_variable, DW_AT_name("exp_")
	.dwattr $C$DW$139, DW_AT_TI_symbol_name("_exp_")
	.dwattr $C$DW$139, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$139, DW_AT_location[DW_OP_breg31 216]
           STDW    .D2T1   A5:A4,*+SP(8)     ; |90| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 91,column 16,is_stmt
           ZERO    .L1     A5

           SET     .S1     A5,0x15,0x1d,A5
||         ZERO    .L1     A4                ; |91| 

           STDW    .D2T1   A5:A4,*+SP(16)    ; |91| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 92,column 16,is_stmt
           MVKL    .S2     0x3fe6a09e,B5
           MVKL    .S2     0x667f3bcd,B4
           MVKH    .S2     0x3fe6a09e,B5
           MVKH    .S2     0x667f3bcd,B4
           STDW    .D2T2   B5:B4,*+SP(24)    ; |92| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 93,column 16,is_stmt
           MVKL    .S1     0xc05007ff,A5
           MVKL    .S1     0x12b3b59a,A4
           MVKH    .S1     0xc05007ff,A5
           MVKH    .S1     0x12b3b59a,A4
           STDW    .D2T1   A5:A4,*+SP(32)    ; |93| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 94,column 16,is_stmt
           MVKL    .S2     0x4030624a,B5
           MVKL    .S2     0x2016afed,B4
           MVKH    .S2     0x4030624a,B5
           MVKH    .S2     0x2016afed,B4
           STDW    .D2T2   B5:B4,*+SP(40)    ; |94| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 95,column 16,is_stmt
           MVKL    .S1     0xbfe94415,A5
           MVKL    .S1     0xb356bd29,A4
           MVKH    .S1     0xbfe94415,A5
           MVKH    .S1     0xb356bd29,A4
           STDW    .D2T1   A5:A4,*+SP(48)    ; |95| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 96,column 16,is_stmt
           MVKL    .S2     0xc0880bfe,B5
           MVKL    .S2     0x9c0d9077,B4
           MVKH    .S2     0xc0880bfe,B5
           MVKH    .S2     0x9c0d9077,B4
           STDW    .D2T2   B5:B4,*+SP(56)    ; |96| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 97,column 16,is_stmt
           MVKL    .S1     0x40738083,A5
           MVKL    .S1     0xfa15267e,A4
           MVKH    .S1     0x40738083,A5
           MVKH    .S1     0xfa15267e,A4
           STDW    .D2T1   A5:A4,*+SP(64)    ; |97| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 98,column 16,is_stmt
           MVKL    .S1     0xc041d580,A5
           MVKL    .S1     0x4b67ce0f,A4
           MVKH    .S1     0xc041d580,A5
           MVKH    .S1     0x4b67ce0f,A4
           STDW    .D2T1   A5:A4,*+SP(72)    ; |98| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 99,column 16,is_stmt
           MVKL    .S2     0x3fe63000,B5

           MVKH    .S2     0x3fe63000,B5
||         ZERO    .L2     B4                ; |99| 

           STDW    .D2T2   B5:B4,*+SP(80)    ; |99| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 100,column 16,is_stmt
           MVKL    .S1     0xbf2bd010,A5
           MVKL    .S1     0x5c610ca8,A4
           MVKH    .S1     0xbf2bd010,A5
           MVKH    .S1     0x5c610ca8,A4
           STDW    .D2T1   A5:A4,*+SP(88)    ; |100| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 101,column 16,is_stmt
           MVKL    .S2     0x40862e42,B5
           MVKL    .S2     0xfefa39ef,B4
           MVKH    .S2     0x40862e42,B5
           MVKH    .S2     0xfefa39ef,B4
           STDW    .D2T2   B5:B4,*+SP(96)    ; |101| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 109,column 3,is_stmt
           LDDW    .D2T2   *+SP(8),B5:B4     ; |109| 
           NOP             4
           STDW    .D2T2   B5:B4,*+SP(120)   ; |109| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 110,column 3,is_stmt
           EXTU    .S2     B5,1,21,B4        ; |110| 
           STW     .D2T2   B4,*+SP(216)      ; |110| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 111,column 3,is_stmt
           ADDK    .S2     -1022,B4          ; |111| 
           STW     .D2T2   B4,*+SP(212)      ; |111| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 114,column 3,is_stmt
           LDDW    .D2T2   *+SP(120),B5:B4   ; |114| 
           NOP             4
           CLR     .S2     B5,20,31,B4       ; |114| 
           STW     .D2T2   B4,*+SP(208)      ; |114| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 115,column 3,is_stmt
           SET     .S2     B4,21,29,B4       ; |115| 
           STW     .D2T2   B4,*+SP(208)      ; |115| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 116,column 3,is_stmt
           LDDW    .D2T2   *+SP(120),B5:B4   ; |116| 
           LDW     .D2T2   *+SP(208),B5      ; |116| 
           NOP             4
           STDW    .D2T2   B5:B4,*+SP(128)   ; |116| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 118,column 3,is_stmt
           LDW     .D2T2   *+SP(216),B0      ; |118| 
           NOP             4
   [ B0]   BNOP    .S1     $C$L3,5           ; |118| 
           ; BRANCHCC OCCURS {$C$L3}         ; |118| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 2
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 119,column 5,is_stmt
           ZERO    .L2     B5:B4             ; |119| 
           STDW    .D2T2   B5:B4,*+SP(128)   ; |119| 
;** --------------------------------------------------------------------------*
$C$L3:    
;          EXCLUSIVE CPU CYCLES: 64
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 122,column 3,is_stmt
           LDDW    .D2T2   *+SP(16),B5:B4    ; |122| 
           LDDW    .D2T2   *+SP(128),B7:B6   ; |122| 
           LDDW    .D2T2   *+SP(16),B9:B8    ; |122| 
           NOP             3
           FSUBDP  .L2     B7:B6,B5:B4,B5:B4 ; |122| 
           NOP             2
           FSUBDP  .L2     B5:B4,B9:B8,B5:B4 ; |122| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(136)   ; |122| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 123,column 3,is_stmt
           LDDW    .D2T2   *+SP(128),B5:B4   ; |123| 
           LDDW    .D2T2   *+SP(16),B7:B6    ; |123| 
           NOP             4
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |123| 
           NOP             3
           FADDDP  .L2     B9:B8,B5:B4,B5:B4 ; |123| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(144)   ; |123| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 125,column 3,is_stmt
           LDDW    .D2T2   *+SP(16),B5:B4    ; |125| 
           LDDW    .D2T2   *+SP(128),B7:B6   ; |125| 
           NOP             4
           FSUBDP  .L2     B7:B6,B5:B4,B5:B4 ; |125| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(152)   ; |125| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 126,column 3,is_stmt
           LDDW    .D2T2   *+SP(16),B7:B6    ; |126| 
           NOP             4
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |126| 
           NOP             3
           FADDDP  .L2     B9:B8,B5:B4,B5:B4 ; |126| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(160)   ; |126| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 128,column 3,is_stmt
           LDDW    .D2T1   *+SP(128),A5:A4   ; |128| 
           LDDW    .D2T2   *+SP(24),B5:B4    ; |128| 
           NOP             4
           CMPGTDP .S1X    A5:A4,B5:B4,A0    ; |128| 
           NOP             1
   [ A0]   BNOP    .S1     $C$L4,5           ; |128| 
           ; BRANCHCC OCCURS {$C$L4}         ; |128| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 19
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 129,column 5,is_stmt
           LDDW    .D2T2   *+SP(152),B5:B4   ; |129| 
           NOP             4
           STDW    .D2T2   B5:B4,*+SP(136)   ; |129| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 130,column 5,is_stmt
           LDDW    .D2T2   *+SP(160),B5:B4   ; |130| 
           NOP             4
           STDW    .D2T2   B5:B4,*+SP(144)   ; |130| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 131,column 5,is_stmt
           LDW     .D2T2   *+SP(212),B4      ; |131| 
           NOP             4
           SUB     .L2     B4,1,B4           ; |131| 
           STW     .D2T2   B4,*+SP(212)      ; |131| 
;** --------------------------------------------------------------------------*
$C$L4:    
;          EXCLUSIVE CPU CYCLES: 135
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 134,column 3,is_stmt
           LDDW    .D2T1   *+SP(136),A5:A4   ; |134| 
$C$DW$140	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$140, DW_AT_low_pc(0x00)
	.dwattr $C$DW$140, DW_AT_name("_divdp_asinhdp_i")
	.dwattr $C$DW$140, DW_AT_TI_call

           CALLP   .S2     _divdp_asinhdp_i,B3
||         LDDW    .D2T2   *+SP(144),B5:B4   ; |134| 

$C$RL8:    ; CALL OCCURS {_divdp_asinhdp_i} {0}  ; |134| 
           STDW    .D2T1   A5:A4,*+SP(112)   ; |134| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 135,column 3,is_stmt
           LDDW    .D2T2   *+SP(112),B7:B6   ; |135| 
           LDDW    .D2T2   *+SP(112),B5:B4   ; |135| 
           NOP             4
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |135| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(104)   ; |135| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 136,column 3,is_stmt
           LDDW    .D2T2   *+SP(72),B7:B6    ; |136| 
           LDDW    .D2T2   *+SP(104),B9:B8   ; |136| 
           NOP             3
           FADDDP  .L2     B7:B6,B5:B4,B5:B4 ; |136| 
           LDDW    .D2T2   *+SP(64),B7:B6    ; |136| 
           NOP             1
           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |136| 
           NOP             3
           FADDDP  .L2     B7:B6,B5:B4,B5:B4 ; |136| 
           LDDW    .D2T2   *+SP(56),B7:B6    ; |136| 
           NOP             1
           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |136| 
           NOP             3
           FADDDP  .L2     B7:B6,B5:B4,B5:B4 ; |136| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(184)   ; |136| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 137,column 3,is_stmt
           LDDW    .D2T2   *+SP(104),B5:B4   ; |137| 
           LDDW    .D2T2   *+SP(48),B9:B8    ; |137| 
           LDDW    .D2T2   *+SP(40),B7:B6    ; |137| 
           NOP             3
           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |137| 
           LDDW    .D2T2   *+SP(104),B9:B8   ; |137| 
           NOP             2
           FADDDP  .L2     B7:B6,B5:B4,B5:B4 ; |137| 
           LDDW    .D2T2   *+SP(32),B7:B6    ; |137| 
           NOP             1
           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |137| 
           NOP             3
           FADDDP  .L2     B7:B6,B5:B4,B5:B4 ; |137| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(192)   ; |137| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 138,column 3,is_stmt
           LDDW    .D2T2   *+SP(184),B5:B4   ; |138| 
$C$DW$141	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$141, DW_AT_low_pc(0x00)
	.dwattr $C$DW$141, DW_AT_name("_divdp_asinhdp_i")
	.dwattr $C$DW$141, DW_AT_TI_call

           CALLP   .S2     _divdp_asinhdp_i,B3
||         LDDW    .D2T1   *+SP(192),A5:A4   ; |138| 

$C$RL9:    ; CALL OCCURS {_divdp_asinhdp_i} {0}  ; |138| 
           LDDW    .D2T2   *+SP(104),B5:B4   ; |138| 
           NOP             4
           FMPYDP  .M2X    A5:A4,B5:B4,B5:B4 ; |138| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(168)   ; |138| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 139,column 3,is_stmt
           LDDW    .D2T2   *+SP(112),B7:B6   ; |139| 
           LDDW    .D2T2   *+SP(168),B9:B8   ; |139| 
           LDDW    .D2T2   *+SP(112),B5:B4   ; |139| 
           NOP             3
           FMPYDP  .M2     B9:B8,B7:B6,B7:B6 ; |139| 
           NOP             3
           FADDDP  .L2     B7:B6,B5:B4,B5:B4 ; |139| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(176)   ; |139| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 140,column 3,is_stmt
           LDW     .D2T2   *+SP(212),B4      ; |140| 
           NOP             4
           INTDP   .L2     B4,B5:B4          ; |140| 
           NOP             4
           STDW    .D2T2   B5:B4,*+SP(192)   ; |140| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 141,column 3,is_stmt
           LDDW    .D2T1   *+SP(88),A7:A6    ; |141| 
           LDDW    .D2T2   *+SP(192),B9:B8   ; |141| 
           LDDW    .D2T2   *+SP(192),B7:B6   ; |141| 
           LDDW    .D2T1   *+SP(80),A5:A4    ; |141| 
           LDDW    .D2T2   *+SP(176),B5:B4   ; |141| 
           NOP             1
           FMPYDP  .M1X    A7:A6,B9:B8,A9:A8 ; |141| 
           NOP             2
           FMPYDP  .M1X    A5:A4,B7:B6,A7:A6 ; |141| 
           FADDDP  .L1X    B5:B4,A9:A8,A5:A4 ; |141| 
           NOP             2
           FADDDP  .L1     A7:A6,A5:A4,A5:A4 ; |141| 
           NOP             2
           STDW    .D2T1   A5:A4,*+SP(200)   ; |141| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 143,column 3,is_stmt
           LDDW    .D2T2   *+SP(8),B5:B4     ; |143| 
           MVKL    .S2     0x7fefffff,B7
           MVKH    .S2     0x7fefffff,B7
           MVK     .L2     0xffffffff,B6     ; |143| 
           NOP             1
           CMPGTDP .S2     B5:B4,B7:B6,B0    ; |143| 
           NOP             1
   [!B0]   BNOP    .S1     $C$L5,5           ; |143| 
           ; BRANCHCC OCCURS {$C$L5}         ; |143| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 6
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 144,column 5,is_stmt
           LDDW    .D2T2   *+SP(96),B5:B4    ; |144| 
           NOP             4
           STDW    .D2T2   B5:B4,*+SP(200)   ; |144| 
;** --------------------------------------------------------------------------*
$C$L5:    
;          EXCLUSIVE CPU CYCLES: 17
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 147,column 3,is_stmt
           LDDW    .D2T1   *+SP(200),A5:A4   ; |147| 
           NOP             4
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 148,column 1,is_stmt
           LDW     .D2T2   *+SP(224),B3      ; |148| 
           NOP             4
	.dwcfi	restore_reg, 19
           ADDK    .S2     224,SP            ; |148| 
	.dwcfi	cfa_offset, 0
	.dwcfi	cfa_offset, 0
$C$DW$142	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$142, DW_AT_low_pc(0x00)
	.dwattr $C$DW$142, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |148| 
           ; BRANCH OCCURS {B3}              ; |148| 
	.dwattr $C$DW$110, DW_AT_TI_end_file("../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h")
	.dwattr $C$DW$110, DW_AT_TI_end_line(0x94)
	.dwattr $C$DW$110, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$110

	.sect	".text:optci"
	.clink

$C$DW$143	.dwtag  DW_TAG_subprogram, DW_AT_name("sqrtdp_asinhdp_i")
	.dwattr $C$DW$143, DW_AT_low_pc(_sqrtdp_asinhdp_i)
	.dwattr $C$DW$143, DW_AT_high_pc(0x00)
	.dwattr $C$DW$143, DW_AT_TI_symbol_name("_sqrtdp_asinhdp_i")
	.dwattr $C$DW$143, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$143, DW_AT_TI_begin_file("../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h")
	.dwattr $C$DW$143, DW_AT_TI_begin_line(0xa1)
	.dwattr $C$DW$143, DW_AT_TI_begin_column(0x16)
	.dwattr $C$DW$143, DW_AT_TI_max_frame_size(0x48)
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 162,column 1,is_stmt,address _sqrtdp_asinhdp_i

	.dwfde $C$DW$CIE, _sqrtdp_asinhdp_i
$C$DW$144	.dwtag  DW_TAG_formal_parameter, DW_AT_name("z")
	.dwattr $C$DW$144, DW_AT_TI_symbol_name("_z")
	.dwattr $C$DW$144, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$144, DW_AT_location[DW_OP_reg4]
$C$DW$145	.dwtag  DW_TAG_formal_parameter, DW_AT_name("x")
	.dwattr $C$DW$145, DW_AT_TI_symbol_name("_x")
	.dwattr $C$DW$145, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$145, DW_AT_location[DW_OP_reg20]

;******************************************************************************
;* FUNCTION NAME: sqrtdp_asinhdp_i                                            *
;*                                                                            *
;*   Regs Modified     : A4,A5,B0,B4,B5,B6,B7,B8,B9,SP,B16,B17                *
;*   Regs Used         : A4,A5,B0,B3,B4,B5,B6,B7,B8,B9,SP,B16,B17             *
;*   Local Frame Size  : 0 Args + 68 Auto + 0 Save = 68 byte                  *
;******************************************************************************
_sqrtdp_asinhdp_i:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 115
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           ADDK    .S2     -72,SP            ; |162| 
	.dwcfi	cfa_offset, 72
$C$DW$146	.dwtag  DW_TAG_variable, DW_AT_name("z")
	.dwattr $C$DW$146, DW_AT_TI_symbol_name("_z")
	.dwattr $C$DW$146, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$146, DW_AT_location[DW_OP_breg31 8]
$C$DW$147	.dwtag  DW_TAG_variable, DW_AT_name("x")
	.dwattr $C$DW$147, DW_AT_TI_symbol_name("_x")
	.dwattr $C$DW$147, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$147, DW_AT_location[DW_OP_breg31 16]
$C$DW$148	.dwtag  DW_TAG_variable, DW_AT_name("half")
	.dwattr $C$DW$148, DW_AT_TI_symbol_name("_half")
	.dwattr $C$DW$148, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$148, DW_AT_location[DW_OP_breg31 24]
$C$DW$149	.dwtag  DW_TAG_variable, DW_AT_name("OneP5")
	.dwattr $C$DW$149, DW_AT_TI_symbol_name("_OneP5")
	.dwattr $C$DW$149, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$149, DW_AT_location[DW_OP_breg31 32]
$C$DW$150	.dwtag  DW_TAG_variable, DW_AT_name("x0")
	.dwattr $C$DW$150, DW_AT_TI_symbol_name("_x0")
	.dwattr $C$DW$150, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$150, DW_AT_location[DW_OP_breg31 40]
$C$DW$151	.dwtag  DW_TAG_variable, DW_AT_name("x1")
	.dwattr $C$DW$151, DW_AT_TI_symbol_name("_x1")
	.dwattr $C$DW$151, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$151, DW_AT_location[DW_OP_breg31 48]
$C$DW$152	.dwtag  DW_TAG_variable, DW_AT_name("x2")
	.dwattr $C$DW$152, DW_AT_TI_symbol_name("_x2")
	.dwattr $C$DW$152, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$152, DW_AT_location[DW_OP_breg31 56]
$C$DW$153	.dwtag  DW_TAG_variable, DW_AT_name("res")
	.dwattr $C$DW$153, DW_AT_TI_symbol_name("_res")
	.dwattr $C$DW$153, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$153, DW_AT_location[DW_OP_breg31 64]
           STDW    .D2T2   B5:B4,*+SP(16)    ; |162| 
           STDW    .D2T1   A5:A4,*+SP(8)     ; |162| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 164,column 17,is_stmt
           ZERO    .L1     A5

           SET     .S1     A5,0x15,0x1d,A5
||         ZERO    .L1     A4                ; |164| 

           STDW    .D2T1   A5:A4,*+SP(24)    ; |164| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 165,column 17,is_stmt
           ZERO    .L2     B5

           SET     .S2     B5,0x13,0x1d,B5
||         ZERO    .L2     B4                ; |165| 

           STDW    .D2T2   B5:B4,*+SP(32)    ; |165| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 169,column 3,is_stmt
           LDDW    .D2T2   *+SP(8),B5:B4     ; |169| 
           NOP             4
           RSQRDP  .S2     B5:B4,B5:B4       ; |169| 
           NOP             1
           STDW    .D2T2   B5:B4,*+SP(40)    ; |169| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 171,column 3,is_stmt
           LDDW    .D2T2   *+SP(40),B9:B8    ; |171| 
           LDDW    .D2T2   *+SP(8),B5:B4     ; |171| 
           LDDW    .D2T2   *+SP(40),B7:B6    ; |171| 
           NOP             3
           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |171| 
           LDDW    .D2T2   *+SP(24),B9:B8    ; |171| 
           NOP             2
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |171| 
           LDDW    .D2T2   *+SP(32),B7:B6    ; |171| 
           NOP             2
           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |171| 
           LDDW    .D2T2   *+SP(40),B9:B8    ; |171| 
           NOP             2
           FSUBDP  .L2     B7:B6,B5:B4,B5:B4 ; |171| 
           NOP             2
           FMPYDP  .M2     B5:B4,B9:B8,B5:B4 ; |171| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(48)    ; |171| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 172,column 3,is_stmt
           LDDW    .D2T2   *+SP(8),B5:B4     ; |172| 
           LDDW    .D2T2   *+SP(48),B7:B6    ; |172| 
           LDDW    .D2T2   *+SP(48),B9:B8    ; |172| 
           NOP             3
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |172| 
           LDDW    .D2T2   *+SP(32),B7:B6    ; |172| 
           NOP             2
           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |172| 
           NOP             4
           FMPYDP  .M1X    A5:A4,B5:B4,A5:A4 ; |172| 
           LDDW    .D2T2   *+SP(48),B5:B4    ; |172| 
           NOP             3
           FSUBDP  .L2X    B7:B6,A5:A4,B7:B6 ; |172| 
           NOP             2
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |172| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(56)    ; |172| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 173,column 3,is_stmt
           LDDW    .D2T2   *+SP(8),B5:B4     ; |173| 
           LDDW    .D2T2   *+SP(56),B7:B6    ; |173| 
           LDDW    .D2T2   *+SP(56),B9:B8    ; |173| 
           LDDW    .D2T1   *+SP(56),A5:A4    ; |173| 
           NOP             2
           FMPYDP  .M2     B7:B6,B5:B4,B7:B6 ; |173| 
           LDDW    .D2T2   *+SP(24),B5:B4    ; |173| 
           NOP             2
           FMPYDP  .M2     B9:B8,B7:B6,B17:B16 ; |173| 
           LDDW    .D2T2   *+SP(8),B9:B8     ; |173| 
           LDDW    .D2T2   *+SP(32),B7:B6    ; |173| 
           NOP             1
           FMPYDP  .M2     B5:B4,B17:B16,B17:B16 ; |173| 
           NOP             2
           FMPYDP  .M2X    A5:A4,B9:B8,B5:B4 ; |173| 
           FSUBDP  .L2     B7:B6,B17:B16,B7:B6 ; |173| 
           NOP             2
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |173| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(64)    ; |173| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 175,column 3,is_stmt
           LDDW    .D2T2   *+SP(16),B5:B4    ; |175| 
           LDDW    .D2T2   *+SP(16),B9:B8    ; |175| 
           LDDW    .D2T2   *+SP(8),B7:B6     ; |175| 
           NOP             3
           FMPYDP  .M2     B9:B8,B5:B4,B5:B4 ; |175| 
           NOP             3
           CMPEQDP .S2     B7:B6,B5:B4,B0    ; |175| 
           NOP             1
   [!B0]   BNOP    .S1     $C$L6,5           ; |175| 
           ; BRANCHCC OCCURS {$C$L6}         ; |175| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 6
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 176,column 5,is_stmt
           LDDW    .D2T2   *+SP(16),B5:B4    ; |176| 
           NOP             4
           STDW    .D2T2   B5:B4,*+SP(64)    ; |176| 
;** --------------------------------------------------------------------------*
$C$L6:    
;          EXCLUSIVE CPU CYCLES: 12
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 179,column 3,is_stmt
           LDDW    .D2T1   *+SP(64),A5:A4    ; |179| 
           NOP             4
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 180,column 1,is_stmt
           ADDK    .S2     72,SP             ; |180| 
	.dwcfi	cfa_offset, 0
	.dwcfi	cfa_offset, 0
$C$DW$154	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$154, DW_AT_low_pc(0x00)
	.dwattr $C$DW$154, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |180| 
           ; BRANCH OCCURS {B3}              ; |180| 
	.dwattr $C$DW$143, DW_AT_TI_end_file("../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h")
	.dwattr $C$DW$143, DW_AT_TI_end_line(0xb4)
	.dwattr $C$DW$143, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$143

	.sect	".text:optci"
	.clink

$C$DW$155	.dwtag  DW_TAG_subprogram, DW_AT_name("asinhdp_i")
	.dwattr $C$DW$155, DW_AT_low_pc(_asinhdp_i)
	.dwattr $C$DW$155, DW_AT_high_pc(0x00)
	.dwattr $C$DW$155, DW_AT_TI_symbol_name("_asinhdp_i")
	.dwattr $C$DW$155, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$155, DW_AT_TI_begin_file("../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h")
	.dwattr $C$DW$155, DW_AT_TI_begin_line(0xca)
	.dwattr $C$DW$155, DW_AT_TI_begin_column(0x16)
	.dwattr $C$DW$155, DW_AT_TI_max_frame_size(0xa8)
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 203,column 1,is_stmt,address _asinhdp_i

	.dwfde $C$DW$CIE, _asinhdp_i
$C$DW$156	.dwtag  DW_TAG_formal_parameter, DW_AT_name("x")
	.dwattr $C$DW$156, DW_AT_TI_symbol_name("_x")
	.dwattr $C$DW$156, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$156, DW_AT_location[DW_OP_reg4]

;******************************************************************************
;* FUNCTION NAME: asinhdp_i                                                   *
;*                                                                            *
;*   Regs Modified     : A0,A3,A4,A5,A6,A7,A8,A9,B0,B3,B4,B5,B6,B7,B8,B9,SP,  *
;*                           B16,B17,B18,B19,B20,B21,B22,B23                  *
;*   Regs Used         : A0,A3,A4,A5,A6,A7,A8,A9,B0,B3,B4,B5,B6,B7,B8,B9,SP,  *
;*                           B16,B17,B18,B19,B20,B21,B22,B23                  *
;*   Local Frame Size  : 0 Args + 164 Auto + 4 Save = 168 byte                *
;******************************************************************************
_asinhdp_i:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 65
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           ADDK    .S2     -168,SP           ; |203| 
	.dwcfi	cfa_offset, 168
           STW     .D2T2   B3,*+SP(168)      ; |203| 
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$157	.dwtag  DW_TAG_variable, DW_AT_name("x")
	.dwattr $C$DW$157, DW_AT_TI_symbol_name("_x")
	.dwattr $C$DW$157, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$157, DW_AT_location[DW_OP_breg31 8]
$C$DW$158	.dwtag  DW_TAG_variable, DW_AT_name("ln2")
	.dwattr $C$DW$158, DW_AT_TI_symbol_name("_ln2")
	.dwattr $C$DW$158, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$158, DW_AT_location[DW_OP_breg31 16]
$C$DW$159	.dwtag  DW_TAG_variable, DW_AT_name("pol_bound")
	.dwattr $C$DW$159, DW_AT_TI_symbol_name("_pol_bound")
	.dwattr $C$DW$159, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$159, DW_AT_location[DW_OP_breg31 24]
$C$DW$160	.dwtag  DW_TAG_variable, DW_AT_name("half")
	.dwattr $C$DW$160, DW_AT_TI_symbol_name("_half")
	.dwattr $C$DW$160, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$160, DW_AT_location[DW_OP_breg31 32]
$C$DW$161	.dwtag  DW_TAG_variable, DW_AT_name("sign")
	.dwattr $C$DW$161, DW_AT_TI_symbol_name("_sign")
	.dwattr $C$DW$161, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$161, DW_AT_location[DW_OP_breg31 40]
$C$DW$162	.dwtag  DW_TAG_variable, DW_AT_name("c2")
	.dwattr $C$DW$162, DW_AT_TI_symbol_name("_c2")
	.dwattr $C$DW$162, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$162, DW_AT_location[DW_OP_breg31 48]
$C$DW$163	.dwtag  DW_TAG_variable, DW_AT_name("c4")
	.dwattr $C$DW$163, DW_AT_TI_symbol_name("_c4")
	.dwattr $C$DW$163, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$163, DW_AT_location[DW_OP_breg31 56]
$C$DW$164	.dwtag  DW_TAG_variable, DW_AT_name("c6")
	.dwattr $C$DW$164, DW_AT_TI_symbol_name("_c6")
	.dwattr $C$DW$164, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$164, DW_AT_location[DW_OP_breg31 64]
$C$DW$165	.dwtag  DW_TAG_variable, DW_AT_name("c8")
	.dwattr $C$DW$165, DW_AT_TI_symbol_name("_c8")
	.dwattr $C$DW$165, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$165, DW_AT_location[DW_OP_breg31 72]
$C$DW$166	.dwtag  DW_TAG_variable, DW_AT_name("c10")
	.dwattr $C$DW$166, DW_AT_TI_symbol_name("_c10")
	.dwattr $C$DW$166, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$166, DW_AT_location[DW_OP_breg31 80]
$C$DW$167	.dwtag  DW_TAG_variable, DW_AT_name("res")
	.dwattr $C$DW$167, DW_AT_TI_symbol_name("_res")
	.dwattr $C$DW$167, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$167, DW_AT_location[DW_OP_breg31 88]
$C$DW$168	.dwtag  DW_TAG_variable, DW_AT_name("sqrt_")
	.dwattr $C$DW$168, DW_AT_TI_symbol_name("_sqrt_")
	.dwattr $C$DW$168, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$168, DW_AT_location[DW_OP_breg31 96]
$C$DW$169	.dwtag  DW_TAG_variable, DW_AT_name("pol")
	.dwattr $C$DW$169, DW_AT_TI_symbol_name("_pol")
	.dwattr $C$DW$169, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$169, DW_AT_location[DW_OP_breg31 104]
$C$DW$170	.dwtag  DW_TAG_variable, DW_AT_name("x2")
	.dwattr $C$DW$170, DW_AT_TI_symbol_name("_x2")
	.dwattr $C$DW$170, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$170, DW_AT_location[DW_OP_breg31 112]
$C$DW$171	.dwtag  DW_TAG_variable, DW_AT_name("x4")
	.dwattr $C$DW$171, DW_AT_TI_symbol_name("_x4")
	.dwattr $C$DW$171, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$171, DW_AT_location[DW_OP_breg31 120]
$C$DW$172	.dwtag  DW_TAG_variable, DW_AT_name("x6")
	.dwattr $C$DW$172, DW_AT_TI_symbol_name("_x6")
	.dwattr $C$DW$172, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$172, DW_AT_location[DW_OP_breg31 128]
$C$DW$173	.dwtag  DW_TAG_variable, DW_AT_name("x8")
	.dwattr $C$DW$173, DW_AT_TI_symbol_name("_x8")
	.dwattr $C$DW$173, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$173, DW_AT_location[DW_OP_breg31 136]
$C$DW$174	.dwtag  DW_TAG_variable, DW_AT_name("x10")
	.dwattr $C$DW$174, DW_AT_TI_symbol_name("_x10")
	.dwattr $C$DW$174, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$174, DW_AT_location[DW_OP_breg31 144]
$C$DW$175	.dwtag  DW_TAG_variable, DW_AT_name("x_abs")
	.dwattr $C$DW$175, DW_AT_TI_symbol_name("_x_abs")
	.dwattr $C$DW$175, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$175, DW_AT_location[DW_OP_breg31 152]
$C$DW$176	.dwtag  DW_TAG_variable, DW_AT_name("temp")
	.dwattr $C$DW$176, DW_AT_TI_symbol_name("_temp")
	.dwattr $C$DW$176, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$176, DW_AT_location[DW_OP_breg31 160]
           STDW    .D2T1   A5:A4,*+SP(8)     ; |203| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 204,column 16,is_stmt
           MVKL    .S1     0x3fe62e42,A5
           MVKL    .S1     0xfefa39ec,A4
           MVKH    .S1     0x3fe62e42,A5
           MVKH    .S1     0xfefa39ec,A4
           STDW    .D2T1   A5:A4,*+SP(16)    ; |204| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 205,column 16,is_stmt
           MVKL    .S2     0x3fd33333,B5
           MVKL    .S2     0x33333333,B4
           MVKH    .S2     0x3fd33333,B5
           MVKH    .S2     0x33333333,B4
           STDW    .D2T2   B5:B4,*+SP(24)    ; |205| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 206,column 16,is_stmt
           ZERO    .L1     A5

           SET     .S1     A5,0x15,0x1d,A5
||         ZERO    .L1     A4                ; |206| 

           STDW    .D2T1   A5:A4,*+SP(32)    ; |206| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 207,column 10,is_stmt
           ZERO    .L2     B5

           SET     .S2     B5,0x14,0x1d,B5
||         ZERO    .L2     B4                ; |207| 

           STDW    .D2T2   B5:B4,*+SP(40)    ; |207| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 210,column 16,is_stmt
           MVKL    .S1     0xbfc55555,A5
           MVKL    .S1     0x8d02093f,A4
           MVKH    .S1     0xbfc55555,A5
           MVKH    .S1     0x8d02093f,A4
           STDW    .D2T1   A5:A4,*+SP(48)    ; |210| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 211,column 16,is_stmt
           MVKL    .S2     0x3fb3335d,B5
           MVKL    .S2     0x9b04cad8,B4
           MVKH    .S2     0x3fb3335d,B5
           MVKH    .S2     0x9b04cad8,B4
           STDW    .D2T2   B5:B4,*+SP(56)    ; |211| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 212,column 16,is_stmt
           MVKL    .S1     0xbfa6e4ec,A5
           MVKL    .S1     0x93a79b50,A4
           MVKH    .S1     0xbfa6e4ec,A5
           MVKH    .S1     0x93a79b50,A4
           STDW    .D2T1   A5:A4,*+SP(64)    ; |212| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 213,column 16,is_stmt
           MVKL    .S1     0x3f9fd78f,A5
           MVKL    .S1     0x2884c094,A4
           MVKH    .S1     0x3f9fd78f,A5
           MVKH    .S1     0x2884c094,A4
           STDW    .D2T1   A5:A4,*+SP(72)    ; |213| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 214,column 16,is_stmt
           MVKL    .S2     0xbf978498,B5
           MVKL    .S2     0x92c0541a,B4
           MVKH    .S2     0xbf978498,B5
           MVKH    .S2     0x92c0541a,B4
           STDW    .D2T2   B5:B4,*+SP(80)    ; |214| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 220,column 3,is_stmt
           LDDW    .D2T2   *+SP(8),B5:B4     ; |220| 
           NOP             4
           ABSDP   .S2     B5:B4,B5:B4       ; |220| 
           NOP             1
           STDW    .D2T2   B5:B4,*+SP(152)   ; |220| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 222,column 3,is_stmt
           LDDW    .D2T2   *+SP(8),B7:B6     ; |222| 
           ZERO    .L2     B5:B4             ; |222| 
           NOP             3
           CMPLTDP .S2     B7:B6,B5:B4,B0    ; |222| 
           NOP             1
   [!B0]   BNOP    .S1     $C$L7,5           ; |222| 
           ; BRANCHCC OCCURS {$C$L7}         ; |222| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 7
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 223,column 5,is_stmt
           LDDW    .D2T2   *+SP(40),B5:B4    ; |223| 
           ZERO    .L1     A3                ; |223| 
           SET     .S1     A3,31,31,A3       ; |223| 
           NOP             2
           XOR     .L2X    B5,A3,B5          ; |223| 
           STDW    .D2T2   B5:B4,*+SP(40)    ; |223| 
;** --------------------------------------------------------------------------*
$C$L7:    
;          EXCLUSIVE CPU CYCLES: 152
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 227,column 3,is_stmt
           LDDW    .D2T2   *+SP(8),B5:B4     ; |227| 
           NOP             4
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |227| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(112)   ; |227| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 228,column 3,is_stmt
           LDDW    .D2T2   *+SP(112),B7:B6   ; |228| 
           NOP             4
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |228| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(120)   ; |228| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 229,column 3,is_stmt
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |229| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(128)   ; |229| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 230,column 3,is_stmt
           LDDW    .D2T2   *+SP(120),B5:B4   ; |230| 
           LDDW    .D2T2   *+SP(120),B7:B6   ; |230| 
           NOP             4
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |230| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(136)   ; |230| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 231,column 3,is_stmt
           LDDW    .D2T2   *+SP(120),B5:B4   ; |231| 
           LDDW    .D2T2   *+SP(128),B7:B6   ; |231| 
           NOP             4
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |231| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(144)   ; |231| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 233,column 3,is_stmt
           LDDW    .D2T2   *+SP(112),B5:B4   ; |233| 
           ZERO    .L2     B7
           SET     .S2     B7,0x14,0x1d,B7
           ZERO    .L2     B6                ; |233| 
           NOP             1
           FADDDP  .L2     B7:B6,B5:B4,B5:B4 ; |233| 
           NOP             3
$C$DW$177	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$177, DW_AT_low_pc(0x00)
	.dwattr $C$DW$177, DW_AT_name("_sqrtdp_asinhdp_i")
	.dwattr $C$DW$177, DW_AT_TI_call

           CALLP   .S2     _sqrtdp_asinhdp_i,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |233| 
||         LDDW    .D2T2   *+SP(152),B5:B4   ; |233| 

$C$RL10:   ; CALL OCCURS {_sqrtdp_asinhdp_i} {0}  ; |233| 
           STDW    .D2T1   A5:A4,*+SP(96)    ; |233| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 237,column 3,is_stmt
           LDDW    .D2T2   *+SP(48),B23:B22  ; |237| 
           LDDW    .D2T2   *+SP(120),B19:B18 ; |237| 
           LDDW    .D2T2   *+SP(112),B17:B16 ; |237| 
           LDDW    .D2T2   *+SP(128),B9:B8   ; |237| 
           LDDW    .D2T1   *+SP(64),A5:A4    ; |237| 
           LDDW    .D2T2   *+SP(72),B7:B6    ; |237| 
           LDDW    .D2T2   *+SP(136),B5:B4   ; |237| 
           LDDW    .D2T2   *+SP(56),B21:B20  ; |237| 
           FMPYDP  .M2     B23:B22,B17:B16,B17:B16 ; |237| 
           FMPYDP  .M1X    A5:A4,B9:B8,A5:A4 ; |237| 
           NOP             1
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |237| 
           FMPYDP  .M2     B21:B20,B19:B18,B9:B8 ; |237| 
           NOP             3

           FADDDP  .L2X    B5:B4,A5:A4,B7:B6 ; |237| 
||         FADDDP  .S2     B9:B8,B17:B16,B5:B4 ; |237| 

           NOP             2
           FADDDP  .L2     B7:B6,B5:B4,B5:B4 ; |237| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(104)   ; |237| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 238,column 3,is_stmt
           LDDW    .D2T2   *+SP(80),B5:B4    ; |238| 
           LDDW    .D2T2   *+SP(144),B7:B6   ; |238| 
           LDDW    .D2T2   *+SP(104),B9:B8   ; |238| 
           NOP             3
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |238| 
           LDDW    .D2T2   *+SP(8),B7:B6     ; |238| 
           NOP             2
           FADDDP  .L2     B5:B4,B9:B8,B5:B4 ; |238| 
           LDDW    .D2T2   *+SP(8),B9:B8     ; |238| 
           NOP             1
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |238| 
           NOP             3
           FADDDP  .L2     B9:B8,B5:B4,B5:B4 ; |238| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(88)    ; |238| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 243,column 3,is_stmt
           LDDW    .D2T2   *+SP(152),B5:B4   ; |243| 
           LDDW    .D2T2   *+SP(32),B17:B16  ; |243| 
           LDDW    .D2T2   *+SP(32),B7:B6    ; |243| 
           LDDW    .D2T2   *+SP(96),B9:B8    ; |243| 
           NOP             3
           FMPYDP  .M2     B7:B6,B5:B4,B5:B4 ; |243| 
           FMPYDP  .M2     B17:B16,B9:B8,B7:B6 ; |243| 
           NOP             3
           FADDDP  .L2     B7:B6,B5:B4,B5:B4 ; |243| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(160)   ; |243| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 244,column 3,is_stmt
$C$DW$178	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$178, DW_AT_low_pc(0x00)
	.dwattr $C$DW$178, DW_AT_name("_logdp_asinhdp_i")
	.dwattr $C$DW$178, DW_AT_TI_call

           CALLP   .S2     _logdp_asinhdp_i,B3
||         LDDW    .D2T1   *+SP(160),A5:A4   ; |244| 

$C$RL11:   ; CALL OCCURS {_logdp_asinhdp_i} {0}  ; |244| 
           LDDW    .D2T2   *+SP(16),B5:B4    ; |244| 
           NOP             4
           FADDDP  .L2X    B5:B4,A5:A4,B5:B4 ; |244| 
           NOP             2
           STDW    .D2T2   B5:B4,*+SP(160)   ; |244| 
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 247,column 3,is_stmt
           LDDW    .D2T2   *+SP(24),B5:B4    ; |247| 
           LDDW    .D2T2   *+SP(152),B7:B6   ; |247| 
           NOP             4
           CMPGTDP .S2     B7:B6,B5:B4,B0    ; |247| 
           NOP             1
   [!B0]   BNOP    .S1     $C$L8,5           ; |247| 
           ; BRANCHCC OCCURS {$C$L8}         ; |247| 
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 11
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 248,column 5,is_stmt
           LDDW    .D2T2   *+SP(160),B7:B6   ; |248| 
           LDDW    .D2T2   *+SP(40),B5:B4    ; |248| 
           NOP             4
           FMPYDP  .M2     B5:B4,B7:B6,B5:B4 ; |248| 
           NOP             3
           STDW    .D2T2   B5:B4,*+SP(88)    ; |248| 
;** --------------------------------------------------------------------------*
$C$L8:    
;          EXCLUSIVE CPU CYCLES: 17
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 251,column 3,is_stmt
           LDDW    .D2T1   *+SP(88),A5:A4    ; |251| 
           NOP             4
	.dwpsn	file "../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h",line 252,column 1,is_stmt
           LDW     .D2T2   *+SP(168),B3      ; |252| 
           NOP             4
	.dwcfi	restore_reg, 19
           ADDK    .S2     168,SP            ; |252| 
	.dwcfi	cfa_offset, 0
	.dwcfi	cfa_offset, 0
$C$DW$179	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$179, DW_AT_low_pc(0x00)
	.dwattr $C$DW$179, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |252| 
           ; BRANCH OCCURS {B3}              ; |252| 
	.dwattr $C$DW$155, DW_AT_TI_end_file("../../../../../../../ti/mathlib/src/asinhdp/c66/asinhdp_i.h")
	.dwattr $C$DW$155, DW_AT_TI_end_line(0xfc)
	.dwattr $C$DW$155, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$155

	.sect	".text"
	.clink
	.global	_main

$C$DW$180	.dwtag  DW_TAG_subprogram, DW_AT_name("main")
	.dwattr $C$DW$180, DW_AT_low_pc(_main)
	.dwattr $C$DW$180, DW_AT_high_pc(0x00)
	.dwattr $C$DW$180, DW_AT_TI_symbol_name("_main")
	.dwattr $C$DW$180, DW_AT_external
	.dwattr $C$DW$180, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$180, DW_AT_TI_begin_line(0x8e)
	.dwattr $C$DW$180, DW_AT_TI_begin_column(0x06)
	.dwattr $C$DW$180, DW_AT_TI_max_frame_size(0x10)
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 143,column 1,is_stmt,address _main

	.dwfde $C$DW$CIE, _main

;******************************************************************************
;* FUNCTION NAME: main                                                        *
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
_main:
;** --------------------------------------------------------------------------*
;          EXCLUSIVE CPU CYCLES: 18
	.dwcfi	cfa_offset, 0
	.dwcfi	save_reg_to_reg, 228, 19
           STW     .D2T2   B3,*SP--(16)      ; |143| 
	.dwcfi	cfa_offset, 16
	.dwcfi	save_reg_to_mem, 19, 0
$C$DW$181	.dwtag  DW_TAG_variable, DW_AT_name("i")
	.dwattr $C$DW$181, DW_AT_TI_symbol_name("_i")
	.dwattr $C$DW$181, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$181, DW_AT_location[DW_OP_breg31 12]
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 151,column 3,is_stmt
           MVKL    .S1     $C$SL1+0,A4
$C$DW$182	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$182, DW_AT_low_pc(0x00)
	.dwattr $C$DW$182, DW_AT_name("_driver_init")
	.dwattr $C$DW$182, DW_AT_TI_call

           CALLP   .S2     _driver_init,B3
||         MVKH    .S1     $C$SL1+0,A4

$C$RL12:   ; CALL OCCURS {_driver_init} {0}  ; |151| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 159,column 8,is_stmt
           ZERO    .L2     B4                ; |159| 
           STW     .D2T2   B4,*+SP(12)       ; |159| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 159,column 15,is_stmt
           MVK     .S2     201,B5            ; |159| 
           CMPLT   .L2     B4,B5,B0          ; |159| 
   [!B0]   BNOP    .S1     $C$L10,5          ; |159| 
           ; BRANCHCC OCCURS {$C$L10}        ; |159| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L9:    
$C$DW$L$_main$2$B:
;          EXCLUSIVE CPU CYCLES: 36
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 160,column 5,is_stmt
           MVKL    .S2     _a,B5
           MVKH    .S2     _a,B5
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |160| 
           NOP             4
$C$DW$183	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$183, DW_AT_low_pc(0x00)
	.dwattr $C$DW$183, DW_AT_name("_asinh")
	.dwattr $C$DW$183, DW_AT_TI_call

           CALLP   .S2     _asinh,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |160| 

$C$RL13:   ; CALL OCCURS {_asinh} {0}        ; |160| 
           MVKL    .S1     _output,A6

           MVKH    .S1     _output,A6
||         LDW     .D2T1   *+SP(12),A3       ; |160| 

           LDW     .D1T1   *A6,A6            ; |160| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |160| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 159,column 32,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |159| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |159| 
           STW     .D2T2   B4,*+SP(12)       ; |159| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 159,column 15,is_stmt
           MVK     .S2     201,B5            ; |159| 
           CMPLT   .L2     B4,B5,B0          ; |159| 
   [ B0]   BNOP    .S1     $C$L9,5           ; |159| 
           ; BRANCHCC OCCURS {$C$L9}         ; |159| 
$C$DW$L$_main$2$E:
;** --------------------------------------------------------------------------*
$C$L10:    
;          EXCLUSIVE CPU CYCLES: 9
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 162,column 8,is_stmt
           ZERO    .L2     B4                ; |162| 
           STW     .D2T2   B4,*+SP(12)       ; |162| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 162,column 15,is_stmt
           CMPLT   .L2     B4,B5,B0          ; |162| 
   [!B0]   BNOP    .S1     $C$L12,5          ; |162| 
           ; BRANCHCC OCCURS {$C$L12}        ; |162| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L11:    
$C$DW$L$_main$4$B:
;          EXCLUSIVE CPU CYCLES: 36
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 163,column 5,is_stmt
           MVKL    .S2     _a,B5
           MVKH    .S2     _a,B5
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |163| 
           NOP             4
$C$DW$184	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$184, DW_AT_low_pc(0x00)
	.dwattr $C$DW$184, DW_AT_name("_asinhdp")
	.dwattr $C$DW$184, DW_AT_TI_call

           CALLP   .S2     _asinhdp,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |163| 

$C$RL14:   ; CALL OCCURS {_asinhdp} {0}      ; |163| 
           MVKL    .S1     _output+4,A6

           MVKH    .S1     _output+4,A6
||         LDW     .D2T1   *+SP(12),A3       ; |163| 

           LDW     .D1T1   *A6,A6            ; |163| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |163| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 162,column 32,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |162| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |162| 
           STW     .D2T2   B4,*+SP(12)       ; |162| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 162,column 15,is_stmt
           MVK     .S2     201,B5            ; |162| 
           CMPLT   .L2     B4,B5,B0          ; |162| 
   [ B0]   BNOP    .S1     $C$L11,5          ; |162| 
           ; BRANCHCC OCCURS {$C$L11}        ; |162| 
$C$DW$L$_main$4$E:
;** --------------------------------------------------------------------------*
$C$L12:    
;          EXCLUSIVE CPU CYCLES: 9
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 165,column 8,is_stmt
           ZERO    .L2     B4                ; |165| 
           STW     .D2T2   B4,*+SP(12)       ; |165| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 165,column 15,is_stmt
           CMPLT   .L2     B4,B5,B0          ; |165| 
   [!B0]   BNOP    .S1     $C$L14,5          ; |165| 
           ; BRANCHCC OCCURS {$C$L14}        ; |165| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L13:    
$C$DW$L$_main$6$B:
;          EXCLUSIVE CPU CYCLES: 36
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 166,column 5,is_stmt
           MVKL    .S2     _a,B5
           MVKH    .S2     _a,B5
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |166| 
           NOP             4
$C$DW$185	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$185, DW_AT_low_pc(0x00)
	.dwattr $C$DW$185, DW_AT_name("_asinhdp_c")
	.dwattr $C$DW$185, DW_AT_TI_call

           CALLP   .S2     _asinhdp_c,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |166| 

$C$RL15:   ; CALL OCCURS {_asinhdp_c} {0}    ; |166| 
           MVKL    .S1     _output+8,A6

           MVKH    .S1     _output+8,A6
||         LDW     .D2T1   *+SP(12),A3       ; |166| 

           LDW     .D1T1   *A6,A6            ; |166| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |166| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 165,column 31,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |165| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |165| 
           STW     .D2T2   B4,*+SP(12)       ; |165| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 165,column 15,is_stmt
           MVK     .S2     201,B5            ; |165| 
           CMPLT   .L2     B4,B5,B0          ; |165| 
   [ B0]   BNOP    .S1     $C$L13,5          ; |165| 
           ; BRANCHCC OCCURS {$C$L13}        ; |165| 
$C$DW$L$_main$6$E:
;** --------------------------------------------------------------------------*
$C$L14:    
;          EXCLUSIVE CPU CYCLES: 9
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 168,column 8,is_stmt
           ZERO    .L2     B4                ; |168| 
           STW     .D2T2   B4,*+SP(12)       ; |168| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 168,column 15,is_stmt
           CMPLT   .L2     B4,B5,B0          ; |168| 
   [!B0]   BNOP    .S1     $C$L16,5          ; |168| 
           ; BRANCHCC OCCURS {$C$L16}        ; |168| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L15:    
$C$DW$L$_main$8$B:
;          EXCLUSIVE CPU CYCLES: 36
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 169,column 5,is_stmt
           MVKL    .S2     _a,B5
           MVKH    .S2     _a,B5
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |169| 
           NOP             4
$C$DW$186	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$186, DW_AT_low_pc(0x00)
	.dwattr $C$DW$186, DW_AT_name("_asinhdp_i")
	.dwattr $C$DW$186, DW_AT_TI_call

           CALLP   .S2     _asinhdp_i,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |169| 

$C$RL16:   ; CALL OCCURS {_asinhdp_i} {0}    ; |169| 
           MVKL    .S1     _output+12,A6

           MVKH    .S1     _output+12,A6
||         LDW     .D2T1   *+SP(12),A3       ; |169| 

           LDW     .D1T1   *A6,A6            ; |169| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |169| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 168,column 32,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |168| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |168| 
           STW     .D2T2   B4,*+SP(12)       ; |168| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 168,column 15,is_stmt
           MVK     .S2     201,B5            ; |168| 
           CMPLT   .L2     B4,B5,B0          ; |168| 
   [ B0]   BNOP    .S1     $C$L15,5          ; |168| 
           ; BRANCHCC OCCURS {$C$L15}        ; |168| 
$C$DW$L$_main$8$E:
;** --------------------------------------------------------------------------*
$C$L16:    
;          EXCLUSIVE CPU CYCLES: 58
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 171,column 3,is_stmt
           MVKL    .S2     _a,B4

           MVKH    .S2     _a,B4
||         MVKL    .S1     _output+16,A3

           MVKH    .S1     _output+16,A3
$C$DW$187	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$187, DW_AT_low_pc(0x00)
	.dwattr $C$DW$187, DW_AT_name("_asinhdp_v")
	.dwattr $C$DW$187, DW_AT_TI_call

           CALLP   .S2     _asinhdp_v,B3
||         MV      .L1X    B4,A4             ; |171| 
||         LDW     .D1T2   *A3,B4            ; |171| 
||         MVK     .S1     0xc9,A6           ; |171| 

$C$RL17:   ; CALL OCCURS {_asinhdp_v} {0}    ; |171| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 174,column 3,is_stmt
           MVKL    .S2     0x3ddb7cdf,B5
           MVKL    .S2     0xd9d7bdbb,B4
           MVKH    .S2     0x3ddb7cdf,B5
           MVKH    .S2     0xd9d7bdbb,B4
$C$DW$188	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$188, DW_AT_low_pc(0x00)
	.dwattr $C$DW$188, DW_AT_name("_allequal")
	.dwattr $C$DW$188, DW_AT_TI_call

           CALLP   .S2     _allequal,B3
||         MVK     .S1     0xc9,A4           ; |174| 
||         MVK     .L1     0x9,A6            ; |174| 

$C$RL18:   ; CALL OCCURS {_allequal} {0}     ; |174| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 175,column 3,is_stmt
$C$DW$189	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$189, DW_AT_low_pc(0x00)
	.dwattr $C$DW$189, DW_AT_name("_print_test_results")
	.dwattr $C$DW$189, DW_AT_TI_call

           CALLP   .S2     _print_test_results,B3
||         MVK     .L1     0x1,A4            ; |175| 

$C$RL19:   ; CALL OCCURS {_print_test_results} {0}  ; |175| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 182,column 3,is_stmt
           MVKL    .S2     _a_sc,B6

           MVKH    .S2     _a_sc,B6
||         ZERO    .L2     B5:B4             ; |182| 

           STDW    .D2T2   B5:B4,*B6         ; |182| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 183,column 3,is_stmt
           ZERO    .L1     A5

           MVKH    .S1     0x80000000,A5
||         ZERO    .L1     A4                ; |183| 
||         ADDAD   .D2     B6,1,B4

           STDW    .D2T1   A5:A4,*B4         ; |183| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 184,column 3,is_stmt
           ZERO    .L1     A5

           SET     .S1     A5,0x14,0x1e,A5
||         ADDAD   .D2     B6,2,B4

           STDW    .D2T1   A5:A4,*B4         ; |184| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 185,column 3,is_stmt
           ZERO    .L1     A5

           MVKH    .S1     0xfff00000,A5
||         ADDAD   .D2     B6,3,B4

           STDW    .D2T1   A5:A4,*B4         ; |185| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 186,column 3,is_stmt
           ZERO    .L1     A5

           SET     .S1     A5,0x14,0x1d,A5
||         ADDAD   .D2     B6,4,B4

           STDW    .D2T1   A5:A4,*B4         ; |186| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 187,column 3,is_stmt
           ZERO    .L1     A5

           MVKH    .S1     0xbff00000,A5
||         ADDAD   .D2     B6,5,B4

           STDW    .D2T1   A5:A4,*B4         ; |187| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 188,column 3,is_stmt
           ZERO    .L1     A5

           SET     .S1     A5,0x0,0x1e,A5
||         ADDAD   .D2     B6,6,B4
||         MVK     .L1     0xffffffff,A4     ; |188| 

           STDW    .D2T1   A5:A4,*B4         ; |188| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 189,column 3,is_stmt
           MV      .L2X    A4,B5

           ADDAD   .D2     B4,1,B6
||         MV      .L2X    A4,B4

           STDW    .D2T2   B5:B4,*B6         ; |189| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 191,column 8,is_stmt
           ZERO    .L2     B4                ; |191| 
           STW     .D2T2   B4,*+SP(12)       ; |191| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 191,column 15,is_stmt
           CMPLT   .L2     B4,8,B0           ; |191| 
   [!B0]   BNOP    .S1     $C$L21,5          ; |191| 
           ; BRANCHCC OCCURS {$C$L21}        ; |191| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains control code
;*----------------------------------------------------------------------------*
$C$L17:    
$C$DW$L$_main$10$B:
;          EXCLUSIVE CPU CYCLES: 7
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 193,column 5,is_stmt
           CMPEQ   .L2     B4,2,B0           ; |193| 
   [ B0]   BNOP    .S1     $C$L18,5          ; |193| 
           ; BRANCHCC OCCURS {$C$L18}        ; |193| 
$C$DW$L$_main$10$E:
;** --------------------------------------------------------------------------*
$C$DW$L$_main$11$B:
;          EXCLUSIVE CPU CYCLES: 7
           CMPEQ   .L2     B4,3,B0           ; |193| 
   [!B0]   BNOP    .S1     $C$L19,5          ; |193| 
           ; BRANCHCC OCCURS {$C$L19}        ; |193| 
$C$DW$L$_main$11$E:
;** --------------------------------------------------------------------------*
$C$L18:    
$C$DW$L$_main$12$B:
;          EXCLUSIVE CPU CYCLES: 19
           MVKL    .S2     _a_sc,B5
           MVKH    .S2     _a_sc,B5
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |193| 
           NOP             4
$C$DW$190	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$190, DW_AT_low_pc(0x00)
	.dwattr $C$DW$190, DW_AT_name("_asinhdp_c")
	.dwattr $C$DW$190, DW_AT_TI_call

           CALLP   .S2     _asinhdp_c,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |193| 

$C$RL20:   ; CALL OCCURS {_asinhdp_c} {0}    ; |193| 
           BNOP    .S1     $C$L20,5          ; |193| 
           ; BRANCH OCCURS {$C$L20}          ; |193| 
$C$DW$L$_main$12$E:
;** --------------------------------------------------------------------------*
$C$L19:    
$C$DW$L$_main$13$B:
;          EXCLUSIVE CPU CYCLES: 13
           MVKL    .S2     _a_sc,B5
           MVKH    .S2     _a_sc,B5
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |193| 
           NOP             4
$C$DW$191	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$191, DW_AT_low_pc(0x00)
	.dwattr $C$DW$191, DW_AT_name("_asinh")
	.dwattr $C$DW$191, DW_AT_TI_call

           CALLP   .S2     _asinh,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |193| 

$C$RL21:   ; CALL OCCURS {_asinh} {0}        ; |193| 
$C$DW$L$_main$13$E:
;** --------------------------------------------------------------------------*
$C$L20:    
$C$DW$L$_main$14$B:
;          EXCLUSIVE CPU CYCLES: 84
           MVKL    .S1     _output,A6

           MVKH    .S1     _output,A6
||         LDW     .D2T1   *+SP(12),A3       ; |193| 

           LDW     .D1T1   *A6,A6            ; |193| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |193| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 194,column 5,is_stmt
           LDW     .D2T1   *+SP(12),A3       ; |194| 
           MVKL    .S1     _a_sc,A4
           MVKH    .S1     _a_sc,A4
           NOP             2
$C$DW$192	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$192, DW_AT_low_pc(0x00)
	.dwattr $C$DW$192, DW_AT_name("_asinhdp")
	.dwattr $C$DW$192, DW_AT_TI_call

           CALLP   .S2     _asinhdp,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |194| 

$C$RL22:   ; CALL OCCURS {_asinhdp} {0}      ; |194| 

           DADD    .L2X    0,A5:A4,B5:B4     ; |194| 
||         MVKL    .S1     _output+4,A4

           MVKH    .S1     _output+4,A4
||         LDW     .D2T1   *+SP(12),A3       ; |194| 

           LDW     .D1T1   *A4,A4            ; |194| 
           NOP             4
           STDW    .D1T2   B5:B4,*+A4[A3]    ; |194| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 195,column 5,is_stmt
           LDW     .D2T1   *+SP(12),A3       ; |195| 
           MVKL    .S1     _a_sc,A4
           MVKH    .S1     _a_sc,A4
           NOP             2
$C$DW$193	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$193, DW_AT_low_pc(0x00)
	.dwattr $C$DW$193, DW_AT_name("_asinhdp_c")
	.dwattr $C$DW$193, DW_AT_TI_call

           CALLP   .S2     _asinhdp_c,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |195| 

$C$RL23:   ; CALL OCCURS {_asinhdp_c} {0}    ; |195| 
           MVKL    .S2     _output+8,B4

           MVKH    .S2     _output+8,B4
||         LDW     .D2T2   *+SP(12),B5       ; |195| 

           LDW     .D2T2   *B4,B4            ; |195| 
           DADD    .L2X    0,A5:A4,B7:B6     ; |195| 
           NOP             3
           STDW    .D2T2   B7:B6,*+B4[B5]    ; |195| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 196,column 5,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |196| 
           MVKL    .S2     _a_sc,B5
           MVKH    .S2     _a_sc,B5
           NOP             2
           LDDW    .D2T2   *+B5[B4],B5:B4    ; |196| 
           NOP             4
$C$DW$194	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$194, DW_AT_low_pc(0x00)
	.dwattr $C$DW$194, DW_AT_name("_asinhdp_i")
	.dwattr $C$DW$194, DW_AT_TI_call

           CALLP   .S2     _asinhdp_i,B3
||         DADD    .L1X    0,B5:B4,A5:A4     ; |196| 

$C$RL24:   ; CALL OCCURS {_asinhdp_i} {0}    ; |196| 
           MVKL    .S1     _output+12,A6

           MVKH    .S1     _output+12,A6
||         LDW     .D2T1   *+SP(12),A3       ; |196| 

           LDW     .D1T1   *A6,A6            ; |196| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |196| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 191,column 35,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |191| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |191| 
           STW     .D2T2   B4,*+SP(12)       ; |191| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 191,column 15,is_stmt
           CMPLT   .L2     B4,8,B0           ; |191| 
   [ B0]   BNOP    .S1     $C$L17,5          ; |191| 
           ; BRANCHCC OCCURS {$C$L17}        ; |191| 
$C$DW$L$_main$14$E:
;** --------------------------------------------------------------------------*
$C$L21:    
;          EXCLUSIVE CPU CYCLES: 38
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 198,column 3,is_stmt
           MVKL    .S2     _a_sc,B4

           MVKH    .S2     _a_sc,B4
||         MVKL    .S1     _output+16,A3

           MVKH    .S1     _output+16,A3
$C$DW$195	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$195, DW_AT_low_pc(0x00)
	.dwattr $C$DW$195, DW_AT_name("_asinhdp_v")
	.dwattr $C$DW$195, DW_AT_TI_call

           CALLP   .S2     _asinhdp_v,B3
||         MV      .L1X    B4,A4             ; |198| 
||         LDW     .D1T2   *A3,B4            ; |198| 
||         MVK     .S1     0x8,A6            ; |198| 

$C$RL25:   ; CALL OCCURS {_asinhdp_v} {0}    ; |198| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 201,column 3,is_stmt
           MVKL    .S2     0x3ddb7cdf,B5
           MVKL    .S2     0xd9d7bdbb,B4
           MVKH    .S2     0x3ddb7cdf,B5
           MVKH    .S2     0xd9d7bdbb,B4
$C$DW$196	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$196, DW_AT_low_pc(0x00)
	.dwattr $C$DW$196, DW_AT_name("_allequal")
	.dwattr $C$DW$196, DW_AT_TI_call

           CALLP   .S2     _allequal,B3
||         MVK     .L1     0x8,A4            ; |201| 
||         MVK     .S1     0x9,A6            ; |201| 

$C$RL26:   ; CALL OCCURS {_allequal} {0}     ; |201| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 202,column 3,is_stmt
$C$DW$197	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$197, DW_AT_low_pc(0x00)
	.dwattr $C$DW$197, DW_AT_name("_print_test_results")
	.dwattr $C$DW$197, DW_AT_TI_call

           CALLP   .S2     _print_test_results,B3
||         MVK     .L1     0x3,A4            ; |202| 

$C$RL27:   ; CALL OCCURS {_print_test_results} {0}  ; |202| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 205,column 3,is_stmt

           ZERO    .L1     A5
||         MVKL    .S2     _a_ext,B4

           MVKH    .S2     _a_ext,B4
||         SET     .S1     A5,0x14,0x14,A5
||         ZERO    .L1     A4                ; |205| 

           STDW    .D2T1   A5:A4,*B4         ; |205| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 206,column 8,is_stmt
           MV      .L2X    A4,B4
           STW     .D2T2   B4,*+SP(12)       ; |206| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 206,column 15,is_stmt
           MVK     .S2     1911,B5           ; |206| 
           CMPLT   .L2     B4,B5,B0          ; |206| 
   [!B0]   BNOP    .S1     $C$L23,5          ; |206| 
           ; BRANCHCC OCCURS {$C$L23}        ; |206| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L22:    
$C$DW$L$_main$16$B:
;          EXCLUSIVE CPU CYCLES: 103
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 207,column 5,is_stmt
           MVKL    .S1     _a_ext,A4

           MVKH    .S1     _a_ext,A4
||         MV      .L1X    B4,A3

$C$DW$198	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$198, DW_AT_low_pc(0x00)
	.dwattr $C$DW$198, DW_AT_name("_asinh")
	.dwattr $C$DW$198, DW_AT_TI_call

           CALLP   .S2     _asinh,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |207| 

$C$RL28:   ; CALL OCCURS {_asinh} {0}        ; |207| 
           MVKL    .S1     _output,A6

           MVKH    .S1     _output,A6
||         LDW     .D2T1   *+SP(12),A3       ; |207| 

           LDW     .D1T1   *A6,A6            ; |207| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |207| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 208,column 5,is_stmt
           LDW     .D2T1   *+SP(12),A3       ; |208| 
           MVKL    .S1     _a_ext,A4
           MVKH    .S1     _a_ext,A4
           NOP             2
$C$DW$199	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$199, DW_AT_low_pc(0x00)
	.dwattr $C$DW$199, DW_AT_name("_asinhdp")
	.dwattr $C$DW$199, DW_AT_TI_call

           CALLP   .S2     _asinhdp,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |208| 

$C$RL29:   ; CALL OCCURS {_asinhdp} {0}      ; |208| 
           MVKL    .S1     _output+4,A6

           MVKH    .S1     _output+4,A6
||         LDW     .D2T1   *+SP(12),A3       ; |208| 

           LDW     .D1T1   *A6,A6            ; |208| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |208| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 209,column 5,is_stmt
           LDW     .D2T1   *+SP(12),A3       ; |209| 
           MVKL    .S1     _a_ext,A4
           MVKH    .S1     _a_ext,A4
           NOP             2
$C$DW$200	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$200, DW_AT_low_pc(0x00)
	.dwattr $C$DW$200, DW_AT_name("_asinhdp_c")
	.dwattr $C$DW$200, DW_AT_TI_call

           CALLP   .S2     _asinhdp_c,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |209| 

$C$RL30:   ; CALL OCCURS {_asinhdp_c} {0}    ; |209| 
           MVKL    .S1     _output+8,A6

           MVKH    .S1     _output+8,A6
||         LDW     .D2T1   *+SP(12),A3       ; |209| 

           LDW     .D1T1   *A6,A6            ; |209| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |209| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 210,column 5,is_stmt
           LDW     .D2T1   *+SP(12),A3       ; |210| 
           MVKL    .S1     _a_ext,A4
           MVKH    .S1     _a_ext,A4
           NOP             2
$C$DW$201	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$201, DW_AT_low_pc(0x00)
	.dwattr $C$DW$201, DW_AT_name("_asinhdp_i")
	.dwattr $C$DW$201, DW_AT_TI_call

           CALLP   .S2     _asinhdp_i,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |210| 

$C$RL31:   ; CALL OCCURS {_asinhdp_i} {0}    ; |210| 
           MVKL    .S1     _output+12,A6

           MVKH    .S1     _output+12,A6
||         LDW     .D2T1   *+SP(12),A3       ; |210| 

           LDW     .D1T1   *A6,A6            ; |210| 
           NOP             4
           STDW    .D1T1   A5:A4,*+A6[A3]    ; |210| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 211,column 5,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |211| 
           MVKL    .S2     _a_ext,B5
           MVKH    .S2     _a_ext,B5
           MVKL    .S1     0xc000cccc,A5
           MVKL    .S1     0xcccccccd,A4
           LDDW    .D2T2   *+B5[B4],B7:B6    ; |211| 
           MVKH    .S1     0xc000cccc,A5
           MVKH    .S1     0xcccccccd,A4
           MV      .L1X    B4,A3             ; |211| 
           MVK     .L1     8,A6              ; |211| 
           FMPYDP  .M1X    A5:A4,B7:B6,A5:A4 ; |211| 
           ADDAD   .D1     A6,A3,A3          ; |211| 
           ADD     .L1X    B5,A3,A3          ; |211| 
           NOP             1
           STDW    .D1T1   A5:A4,*A3         ; |211| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 206,column 36,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |206| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |206| 
           STW     .D2T2   B4,*+SP(12)       ; |206| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 206,column 15,is_stmt
           MVK     .S2     1911,B5           ; |206| 
           CMPLT   .L2     B4,B5,B0          ; |206| 
   [ B0]   BNOP    .S1     $C$L22,5          ; |206| 
           ; BRANCHCC OCCURS {$C$L22}        ; |206| 
$C$DW$L$_main$16$E:
;** --------------------------------------------------------------------------*
$C$L23:    
;          EXCLUSIVE CPU CYCLES: 41
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 213,column 3,is_stmt
           MVKL    .S2     _a_ext,B4

           MVKH    .S2     _a_ext,B4
||         MVKL    .S1     _output+16,A3

           MVKH    .S1     _output+16,A3
$C$DW$202	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$202, DW_AT_low_pc(0x00)
	.dwattr $C$DW$202, DW_AT_name("_asinhdp_v")
	.dwattr $C$DW$202, DW_AT_TI_call

           CALLP   .S2     _asinhdp_v,B3
||         MV      .L1X    B4,A4             ; |213| 
||         LDW     .D1T2   *A3,B4            ; |213| 
||         MVK     .S1     0x777,A6          ; |213| 

$C$RL32:   ; CALL OCCURS {_asinhdp_v} {0}    ; |213| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 218,column 3,is_stmt
           MVKL    .S2     0x3ddb7cdf,B5
           MVKL    .S2     0xd9d7bdbb,B4
           MVKH    .S2     0x3ddb7cdf,B5
           MVKH    .S2     0xd9d7bdbb,B4
$C$DW$203	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$203, DW_AT_low_pc(0x00)
	.dwattr $C$DW$203, DW_AT_name("_allequal")
	.dwattr $C$DW$203, DW_AT_TI_call

           CALLP   .S2     _allequal,B3
||         MVK     .S1     0x777,A4          ; |218| 
||         MVK     .L1     0x9,A6            ; |218| 

$C$RL33:   ; CALL OCCURS {_allequal} {0}     ; |218| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 219,column 3,is_stmt
$C$DW$204	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$204, DW_AT_low_pc(0x00)
	.dwattr $C$DW$204, DW_AT_name("_print_test_results")
	.dwattr $C$DW$204, DW_AT_TI_call

           CALLP   .S2     _print_test_results,B3
||         MVK     .L1     0x4,A4            ; |219| 

$C$RL34:   ; CALL OCCURS {_print_test_results} {0}  ; |219| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 226,column 3,is_stmt
$C$DW$205	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$205, DW_AT_low_pc(0x00)
	.dwattr $C$DW$205, DW_AT_name("_profile_init")
	.dwattr $C$DW$205, DW_AT_TI_call

           CALLP   .S2     _profile_init,B3
||         MVK     .L1     0x3,A4            ; |226| 

$C$RL35:   ; CALL OCCURS {_profile_init} {0}  ; |226| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 227,column 8,is_stmt
           ZERO    .L2     B4                ; |227| 
           STW     .D2T2   B4,*+SP(12)       ; |227| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 227,column 15,is_stmt
           MVK     .S2     128,B5            ; |227| 
           CMPLT   .L2     B4,B5,B0          ; |227| 
   [!B0]   BNOP    .S1     $C$L25,5          ; |227| 
           ; BRANCHCC OCCURS {$C$L25}        ; |227| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L24:    
$C$DW$L$_main$18$B:
;          EXCLUSIVE CPU CYCLES: 64
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 229,column 5,is_stmt
           MVKL    .S2     0xc0b38800,B5

           MVKL    .S1     0x412e8482,A5
||         MVKH    .S2     0xc0b38800,B5

$C$DW$206	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$206, DW_AT_low_pc(0x00)
	.dwattr $C$DW$206, DW_AT_name("_gimme_random")
	.dwattr $C$DW$206, DW_AT_TI_call

           CALLP   .S2     _gimme_random,B3
||         MVKH    .S1     0x412e8482,A5
||         ZERO    .L1     A4                ; |229| 
||         ZERO    .L2     B4                ; |229| 

$C$RL36:   ; CALL OCCURS {_gimme_random} {0}  ; |229| 
           MVKL    .S2     _input,B5

           MVKH    .S2     _input,B5
||         LDW     .D2T2   *+SP(12),B4       ; |229| 

           LDW     .D2T2   *B5,B5            ; |229| 
           NOP             4
           STDW    .D2T1   A5:A4,*+B5[B4]    ; |229| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 232,column 5,is_stmt
$C$DW$207	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$207, DW_AT_low_pc(0x00)
	.dwattr $C$DW$207, DW_AT_name("_profile_start")
	.dwattr $C$DW$207, DW_AT_TI_call
           CALLP   .S2     _profile_start,B3
$C$RL37:   ; CALL OCCURS {_profile_start} {0}  ; |232| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 233,column 5,is_stmt
           MVKL    .S1     _input,A4

           MVKH    .S1     _input,A4
||         LDW     .D2T1   *+SP(12),A3       ; |233| 

           LDW     .D1T1   *A4,A4            ; |233| 
           NOP             4
$C$DW$208	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$208, DW_AT_low_pc(0x00)
	.dwattr $C$DW$208, DW_AT_name("_asinhdp_i")
	.dwattr $C$DW$208, DW_AT_TI_call

           CALLP   .S2     _asinhdp_i,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |233| 

$C$RL38:   ; CALL OCCURS {_asinhdp_i} {0}    ; |233| 
           MVKL    .S2     _output+12,B5

           MVKH    .S2     _output+12,B5
||         LDW     .D2T2   *+SP(12),B4       ; |233| 

           LDW     .D2T2   *B5,B5            ; |233| 
           NOP             4
           STDW    .D2T1   A5:A4,*+B5[B4]    ; |233| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 234,column 5,is_stmt
$C$DW$209	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$209, DW_AT_low_pc(0x00)
	.dwattr $C$DW$209, DW_AT_name("_profile_update")
	.dwattr $C$DW$209, DW_AT_TI_call

           CALLP   .S2     _profile_update,B3
||         MVK     .L1     0x3,A4            ; |234| 

$C$RL39:   ; CALL OCCURS {_profile_update} {0}  ; |234| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 227,column 29,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |227| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |227| 
           STW     .D2T2   B4,*+SP(12)       ; |227| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 227,column 15,is_stmt
           MVK     .S1     128,A3            ; |227| 
           CMPLT   .L1X    B4,A3,A0          ; |227| 
   [ A0]   BNOP    .S1     $C$L24,5          ; |227| 
           ; BRANCHCC OCCURS {$C$L24}        ; |227| 
$C$DW$L$_main$18$E:
;** --------------------------------------------------------------------------*
$C$L25:    
;          EXCLUSIVE CPU CYCLES: 23
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 236,column 3,is_stmt
           MVK     .S2     0x80,B4           ; |236| 
$C$DW$210	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$210, DW_AT_low_pc(0x00)
	.dwattr $C$DW$210, DW_AT_name("_profile_avg")
	.dwattr $C$DW$210, DW_AT_TI_call

           CALLP   .S2     _profile_avg,B3
||         MVK     .L1     0x3,A4            ; |236| 

$C$RL40:   ; CALL OCCURS {_profile_avg} {0}  ; |236| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 239,column 3,is_stmt
$C$DW$211	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$211, DW_AT_low_pc(0x00)
	.dwattr $C$DW$211, DW_AT_name("_profile_init")
	.dwattr $C$DW$211, DW_AT_TI_call

           CALLP   .S2     _profile_init,B3
||         ZERO    .L1     A4                ; |239| 

$C$RL41:   ; CALL OCCURS {_profile_init} {0}  ; |239| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 240,column 8,is_stmt
           ZERO    .L2     B4                ; |240| 
           STW     .D2T2   B4,*+SP(12)       ; |240| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 240,column 15,is_stmt
           MVK     .S2     128,B5            ; |240| 
           CMPLT   .L2     B4,B5,B0          ; |240| 
   [!B0]   BNOP    .S1     $C$L27,5          ; |240| 
           ; BRANCHCC OCCURS {$C$L27}        ; |240| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L26:    
$C$DW$L$_main$20$B:
;          EXCLUSIVE CPU CYCLES: 48
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 241,column 5,is_stmt
$C$DW$212	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$212, DW_AT_low_pc(0x00)
	.dwattr $C$DW$212, DW_AT_name("_profile_start")
	.dwattr $C$DW$212, DW_AT_TI_call
           CALLP   .S2     _profile_start,B3
$C$RL42:   ; CALL OCCURS {_profile_start} {0}  ; |241| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 242,column 5,is_stmt
           MVKL    .S1     _input,A4

           MVKH    .S1     _input,A4
||         LDW     .D2T1   *+SP(12),A3       ; |242| 

           LDW     .D1T1   *A4,A4            ; |242| 
           NOP             4
$C$DW$213	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$213, DW_AT_low_pc(0x00)
	.dwattr $C$DW$213, DW_AT_name("_asinh")
	.dwattr $C$DW$213, DW_AT_TI_call

           CALLP   .S2     _asinh,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |242| 

$C$RL43:   ; CALL OCCURS {_asinh} {0}        ; |242| 
           MVKL    .S2     _output,B5

           MVKH    .S2     _output,B5
||         LDW     .D2T2   *+SP(12),B4       ; |242| 

           LDW     .D2T2   *B5,B5            ; |242| 
           NOP             4
           STDW    .D2T1   A5:A4,*+B5[B4]    ; |242| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 243,column 5,is_stmt
$C$DW$214	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$214, DW_AT_low_pc(0x00)
	.dwattr $C$DW$214, DW_AT_name("_profile_update")
	.dwattr $C$DW$214, DW_AT_TI_call

           CALLP   .S2     _profile_update,B3
||         ZERO    .L1     A4                ; |243| 

$C$RL44:   ; CALL OCCURS {_profile_update} {0}  ; |243| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 240,column 29,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |240| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |240| 
           STW     .D2T2   B4,*+SP(12)       ; |240| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 240,column 15,is_stmt
           MVK     .S2     128,B5            ; |240| 
           CMPLT   .L2     B4,B5,B0          ; |240| 
   [ B0]   BNOP    .S1     $C$L26,5          ; |240| 
           ; BRANCHCC OCCURS {$C$L26}        ; |240| 
$C$DW$L$_main$20$E:
;** --------------------------------------------------------------------------*
$C$L27:    
;          EXCLUSIVE CPU CYCLES: 23
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 245,column 3,is_stmt
           MVK     .S2     0x80,B4           ; |245| 
$C$DW$215	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$215, DW_AT_low_pc(0x00)
	.dwattr $C$DW$215, DW_AT_name("_profile_avg")
	.dwattr $C$DW$215, DW_AT_TI_call

           CALLP   .S2     _profile_avg,B3
||         ZERO    .L1     A4                ; |245| 

$C$RL45:   ; CALL OCCURS {_profile_avg} {0}  ; |245| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 248,column 3,is_stmt
$C$DW$216	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$216, DW_AT_low_pc(0x00)
	.dwattr $C$DW$216, DW_AT_name("_profile_init")
	.dwattr $C$DW$216, DW_AT_TI_call

           CALLP   .S2     _profile_init,B3
||         MVK     .L1     0x1,A4            ; |248| 

$C$RL46:   ; CALL OCCURS {_profile_init} {0}  ; |248| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 249,column 8,is_stmt
           ZERO    .L2     B4                ; |249| 
           STW     .D2T2   B4,*+SP(12)       ; |249| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 249,column 15,is_stmt
           MVK     .S2     128,B5            ; |249| 
           CMPLT   .L2     B4,B5,B0          ; |249| 
   [!B0]   BNOP    .S1     $C$L29,5          ; |249| 
           ; BRANCHCC OCCURS {$C$L29}        ; |249| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L28:    
$C$DW$L$_main$22$B:
;          EXCLUSIVE CPU CYCLES: 48
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 250,column 5,is_stmt
$C$DW$217	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$217, DW_AT_low_pc(0x00)
	.dwattr $C$DW$217, DW_AT_name("_profile_start")
	.dwattr $C$DW$217, DW_AT_TI_call
           CALLP   .S2     _profile_start,B3
$C$RL47:   ; CALL OCCURS {_profile_start} {0}  ; |250| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 251,column 5,is_stmt
           MVKL    .S1     _input,A4

           MVKH    .S1     _input,A4
||         LDW     .D2T1   *+SP(12),A3       ; |251| 

           LDW     .D1T1   *A4,A4            ; |251| 
           NOP             4
$C$DW$218	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$218, DW_AT_low_pc(0x00)
	.dwattr $C$DW$218, DW_AT_name("_asinhdp")
	.dwattr $C$DW$218, DW_AT_TI_call

           CALLP   .S2     _asinhdp,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |251| 

$C$RL48:   ; CALL OCCURS {_asinhdp} {0}      ; |251| 
           MVKL    .S2     _output+4,B5

           MVKH    .S2     _output+4,B5
||         LDW     .D2T2   *+SP(12),B4       ; |251| 

           LDW     .D2T2   *B5,B5            ; |251| 
           NOP             4
           STDW    .D2T1   A5:A4,*+B5[B4]    ; |251| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 252,column 5,is_stmt
$C$DW$219	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$219, DW_AT_low_pc(0x00)
	.dwattr $C$DW$219, DW_AT_name("_profile_update")
	.dwattr $C$DW$219, DW_AT_TI_call

           CALLP   .S2     _profile_update,B3
||         MVK     .L1     0x1,A4            ; |252| 

$C$RL49:   ; CALL OCCURS {_profile_update} {0}  ; |252| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 249,column 29,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |249| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |249| 
           STW     .D2T2   B4,*+SP(12)       ; |249| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 249,column 15,is_stmt
           MVK     .S2     128,B5            ; |249| 
           CMPLT   .L2     B4,B5,B0          ; |249| 
   [ B0]   BNOP    .S1     $C$L28,5          ; |249| 
           ; BRANCHCC OCCURS {$C$L28}        ; |249| 
$C$DW$L$_main$22$E:
;** --------------------------------------------------------------------------*
$C$L29:    
;          EXCLUSIVE CPU CYCLES: 50
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 254,column 3,is_stmt
           MVK     .S2     0x80,B4           ; |254| 
$C$DW$220	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$220, DW_AT_low_pc(0x00)
	.dwattr $C$DW$220, DW_AT_name("_profile_avg")
	.dwattr $C$DW$220, DW_AT_TI_call

           CALLP   .S2     _profile_avg,B3
||         MVK     .L1     0x1,A4            ; |254| 

$C$RL50:   ; CALL OCCURS {_profile_avg} {0}  ; |254| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 257,column 3,is_stmt
$C$DW$221	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$221, DW_AT_low_pc(0x00)
	.dwattr $C$DW$221, DW_AT_name("_profile_init")
	.dwattr $C$DW$221, DW_AT_TI_call

           CALLP   .S2     _profile_init,B3
||         MVK     .L1     0x4,A4            ; |257| 

$C$RL51:   ; CALL OCCURS {_profile_init} {0}  ; |257| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 258,column 3,is_stmt

           MVKL    .S1     _output+16,A3
||         MVKL    .S2     _input,B4

           MVKH    .S1     _output+16,A3
||         MVKH    .S2     _input,B4

$C$DW$222	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$222, DW_AT_low_pc(0x00)
	.dwattr $C$DW$222, DW_AT_name("_asinhdp_v")
	.dwattr $C$DW$222, DW_AT_TI_call

           CALLP   .S2     _asinhdp_v,B3
||         LDW     .D2T1   *B4,A4            ; |258| 
||         LDW     .D1T2   *A3,B4            ; |258| 
||         MVK     .S1     0x80,A6           ; |258| 

$C$RL52:   ; CALL OCCURS {_asinhdp_v} {0}    ; |258| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 259,column 3,is_stmt
$C$DW$223	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$223, DW_AT_low_pc(0x00)
	.dwattr $C$DW$223, DW_AT_name("_profile_update")
	.dwattr $C$DW$223, DW_AT_TI_call

           CALLP   .S2     _profile_update,B3
||         MVK     .L1     0x4,A4            ; |259| 

$C$RL53:   ; CALL OCCURS {_profile_update} {0}  ; |259| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 260,column 3,is_stmt
           MVK     .S2     0x80,B4           ; |260| 
$C$DW$224	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$224, DW_AT_low_pc(0x00)
	.dwattr $C$DW$224, DW_AT_name("_profile_avg")
	.dwattr $C$DW$224, DW_AT_TI_call

           CALLP   .S2     _profile_avg,B3
||         MVK     .L1     0x4,A4            ; |260| 

$C$RL54:   ; CALL OCCURS {_profile_avg} {0}  ; |260| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 263,column 3,is_stmt
$C$DW$225	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$225, DW_AT_low_pc(0x00)
	.dwattr $C$DW$225, DW_AT_name("_profile_init")
	.dwattr $C$DW$225, DW_AT_TI_call

           CALLP   .S2     _profile_init,B3
||         MVK     .L1     0x2,A4            ; |263| 

$C$RL55:   ; CALL OCCURS {_profile_init} {0}  ; |263| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 264,column 8,is_stmt
           ZERO    .L2     B4                ; |264| 
           STW     .D2T2   B4,*+SP(12)       ; |264| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 264,column 15,is_stmt
           MVK     .S2     128,B5            ; |264| 
           CMPLT   .L2     B4,B5,B0          ; |264| 
   [!B0]   BNOP    .S1     $C$L31,5          ; |264| 
           ; BRANCHCC OCCURS {$C$L31}        ; |264| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: Software pipelining disabled
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*      Disqualified loop: Loop contains a call
;*      Disqualified loop: Loop contains non-pipelinable instructions
;*----------------------------------------------------------------------------*
$C$L30:    
$C$DW$L$_main$24$B:
;          EXCLUSIVE CPU CYCLES: 48
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 265,column 5,is_stmt
$C$DW$226	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$226, DW_AT_low_pc(0x00)
	.dwattr $C$DW$226, DW_AT_name("_profile_start")
	.dwattr $C$DW$226, DW_AT_TI_call
           CALLP   .S2     _profile_start,B3
$C$RL56:   ; CALL OCCURS {_profile_start} {0}  ; |265| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 266,column 5,is_stmt
           MVKL    .S1     _input,A4

           MVKH    .S1     _input,A4
||         LDW     .D2T1   *+SP(12),A3       ; |266| 

           LDW     .D1T1   *A4,A4            ; |266| 
           NOP             4
$C$DW$227	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$227, DW_AT_low_pc(0x00)
	.dwattr $C$DW$227, DW_AT_name("_asinhdp_c")
	.dwattr $C$DW$227, DW_AT_TI_call

           CALLP   .S2     _asinhdp_c,B3
||         LDDW    .D1T1   *+A4[A3],A5:A4    ; |266| 

$C$RL57:   ; CALL OCCURS {_asinhdp_c} {0}    ; |266| 
           MVKL    .S2     _output+8,B4

           MVKH    .S2     _output+8,B4
||         LDW     .D2T2   *+SP(12),B5       ; |266| 

           LDW     .D2T2   *B4,B4            ; |266| 
           NOP             4
           STDW    .D2T1   A5:A4,*+B4[B5]    ; |266| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 267,column 5,is_stmt
$C$DW$228	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$228, DW_AT_low_pc(0x00)
	.dwattr $C$DW$228, DW_AT_name("_profile_update")
	.dwattr $C$DW$228, DW_AT_TI_call

           CALLP   .S2     _profile_update,B3
||         MVK     .L1     0x2,A4            ; |267| 

$C$RL58:   ; CALL OCCURS {_profile_update} {0}  ; |267| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 264,column 29,is_stmt
           LDW     .D2T2   *+SP(12),B4       ; |264| 
           NOP             4
           ADD     .L2     1,B4,B4           ; |264| 
           STW     .D2T2   B4,*+SP(12)       ; |264| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 264,column 15,is_stmt
           MVK     .S2     128,B5            ; |264| 
           CMPLT   .L2     B4,B5,B0          ; |264| 
   [ B0]   BNOP    .S1     $C$L30,5          ; |264| 
           ; BRANCHCC OCCURS {$C$L30}        ; |264| 
$C$DW$L$_main$24$E:
;** --------------------------------------------------------------------------*
$C$L31:    
;          EXCLUSIVE CPU CYCLES: 64
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 269,column 3,is_stmt
           MVK     .S2     0x80,B4           ; |269| 
$C$DW$229	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$229, DW_AT_low_pc(0x00)
	.dwattr $C$DW$229, DW_AT_name("_profile_avg")
	.dwattr $C$DW$229, DW_AT_TI_call

           CALLP   .S2     _profile_avg,B3
||         MVK     .L1     0x2,A4            ; |269| 

$C$RL59:   ; CALL OCCURS {_profile_avg} {0}  ; |269| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 272,column 3,is_stmt
           MVKL    .S2     0x3ddb7cdf,B5
           MVKL    .S2     0xd9d7bdbb,B4
           MVKH    .S2     0x3ddb7cdf,B5
           MVKH    .S2     0xd9d7bdbb,B4
$C$DW$230	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$230, DW_AT_low_pc(0x00)
	.dwattr $C$DW$230, DW_AT_name("_allequal")
	.dwattr $C$DW$230, DW_AT_TI_call

           CALLP   .S2     _allequal,B3
||         MVK     .L1     0x9,A6            ; |272| 
||         MVK     .S1     0x80,A4           ; |272| 

$C$RL60:   ; CALL OCCURS {_allequal} {0}     ; |272| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 273,column 3,is_stmt
$C$DW$231	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$231, DW_AT_low_pc(0x00)
	.dwattr $C$DW$231, DW_AT_name("_print_test_results")
	.dwattr $C$DW$231, DW_AT_TI_call

           CALLP   .S2     _print_test_results,B3
||         MVK     .L1     0x2,A4            ; |273| 

$C$RL61:   ; CALL OCCURS {_print_test_results} {0}  ; |273| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 274,column 3,is_stmt
           MVKL    .S2     $C$SL2+0,B4
           MVKH    .S2     $C$SL2+0,B4
$C$DW$232	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$232, DW_AT_low_pc(0x00)
	.dwattr $C$DW$232, DW_AT_name("_printf")
	.dwattr $C$DW$232, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |274| 

$C$RL62:   ; CALL OCCURS {_printf} {0}       ; |274| 
           MVKL    .S2     $C$SL3+0,B4
           MVKH    .S2     $C$SL3+0,B4
$C$DW$233	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$233, DW_AT_low_pc(0x00)
	.dwattr $C$DW$233, DW_AT_name("_printf")
	.dwattr $C$DW$233, DW_AT_TI_call

           CALLP   .S2     _printf,B3
||         STW     .D2T2   B4,*+SP(4)        ; |274| 

$C$RL63:   ; CALL OCCURS {_printf} {0}       ; |274| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 281,column 3,is_stmt
           MVKL    .S1     $C$SL1+0,A4
$C$DW$234	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$234, DW_AT_low_pc(0x00)
	.dwattr $C$DW$234, DW_AT_name("_print_profile_results")
	.dwattr $C$DW$234, DW_AT_TI_call

           CALLP   .S2     _print_profile_results,B3
||         MVKH    .S1     $C$SL1+0,A4

$C$RL64:   ; CALL OCCURS {_print_profile_results} {0}  ; |281| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 284,column 3,is_stmt
           MVKL    .S1     $C$SL1+0,A4
$C$DW$235	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$235, DW_AT_low_pc(0x00)
	.dwattr $C$DW$235, DW_AT_name("_print_memory_results")
	.dwattr $C$DW$235, DW_AT_TI_call

           CALLP   .S2     _print_memory_results,B3
||         MVKH    .S1     $C$SL1+0,A4

$C$RL65:   ; CALL OCCURS {_print_memory_results} {0}  ; |284| 
	.dwpsn	file "C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c",line 285,column 1,is_stmt
           LDW     .D2T2   *++SP(16),B3      ; |285| 
           NOP             4
	.dwcfi	cfa_offset, 0
	.dwcfi	restore_reg, 19
	.dwcfi	cfa_offset, 0
$C$DW$236	.dwtag  DW_TAG_TI_branch
	.dwattr $C$DW$236, DW_AT_low_pc(0x00)
	.dwattr $C$DW$236, DW_AT_TI_return
           RETNOP  .S2     B3,5              ; |285| 
           ; BRANCH OCCURS {B3}              ; |285| 

$C$DW$237	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$237, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L30:1:1463466944")
	.dwattr $C$DW$237, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$237, DW_AT_TI_begin_line(0x108)
	.dwattr $C$DW$237, DW_AT_TI_end_line(0x10c)
$C$DW$238	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$238, DW_AT_low_pc($C$DW$L$_main$24$B)
	.dwattr $C$DW$238, DW_AT_high_pc($C$DW$L$_main$24$E)
	.dwendtag $C$DW$237


$C$DW$239	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$239, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L28:1:1463466944")
	.dwattr $C$DW$239, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$239, DW_AT_TI_begin_line(0xf9)
	.dwattr $C$DW$239, DW_AT_TI_end_line(0xfd)
$C$DW$240	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$240, DW_AT_low_pc($C$DW$L$_main$22$B)
	.dwattr $C$DW$240, DW_AT_high_pc($C$DW$L$_main$22$E)
	.dwendtag $C$DW$239


$C$DW$241	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$241, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L26:1:1463466944")
	.dwattr $C$DW$241, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$241, DW_AT_TI_begin_line(0xf0)
	.dwattr $C$DW$241, DW_AT_TI_end_line(0xf4)
$C$DW$242	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$242, DW_AT_low_pc($C$DW$L$_main$20$B)
	.dwattr $C$DW$242, DW_AT_high_pc($C$DW$L$_main$20$E)
	.dwendtag $C$DW$241


$C$DW$243	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$243, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L24:1:1463466944")
	.dwattr $C$DW$243, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$243, DW_AT_TI_begin_line(0xe3)
	.dwattr $C$DW$243, DW_AT_TI_end_line(0xeb)
$C$DW$244	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$244, DW_AT_low_pc($C$DW$L$_main$18$B)
	.dwattr $C$DW$244, DW_AT_high_pc($C$DW$L$_main$18$E)
	.dwendtag $C$DW$243


$C$DW$245	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$245, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L22:1:1463466944")
	.dwattr $C$DW$245, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$245, DW_AT_TI_begin_line(0xce)
	.dwattr $C$DW$245, DW_AT_TI_end_line(0xd4)
$C$DW$246	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$246, DW_AT_low_pc($C$DW$L$_main$16$B)
	.dwattr $C$DW$246, DW_AT_high_pc($C$DW$L$_main$16$E)
	.dwendtag $C$DW$245


$C$DW$247	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$247, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L17:1:1463466944")
	.dwattr $C$DW$247, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$247, DW_AT_TI_begin_line(0xbf)
	.dwattr $C$DW$247, DW_AT_TI_end_line(0xc5)
$C$DW$248	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$248, DW_AT_low_pc($C$DW$L$_main$10$B)
	.dwattr $C$DW$248, DW_AT_high_pc($C$DW$L$_main$10$E)
$C$DW$249	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$249, DW_AT_low_pc($C$DW$L$_main$11$B)
	.dwattr $C$DW$249, DW_AT_high_pc($C$DW$L$_main$11$E)
$C$DW$250	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$250, DW_AT_low_pc($C$DW$L$_main$12$B)
	.dwattr $C$DW$250, DW_AT_high_pc($C$DW$L$_main$12$E)
$C$DW$251	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$251, DW_AT_low_pc($C$DW$L$_main$13$B)
	.dwattr $C$DW$251, DW_AT_high_pc($C$DW$L$_main$13$E)
$C$DW$252	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$252, DW_AT_low_pc($C$DW$L$_main$14$B)
	.dwattr $C$DW$252, DW_AT_high_pc($C$DW$L$_main$14$E)
	.dwendtag $C$DW$247


$C$DW$253	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$253, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L15:1:1463466944")
	.dwattr $C$DW$253, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$253, DW_AT_TI_begin_line(0xa8)
	.dwattr $C$DW$253, DW_AT_TI_end_line(0xaa)
$C$DW$254	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$254, DW_AT_low_pc($C$DW$L$_main$8$B)
	.dwattr $C$DW$254, DW_AT_high_pc($C$DW$L$_main$8$E)
	.dwendtag $C$DW$253


$C$DW$255	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$255, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L13:1:1463466944")
	.dwattr $C$DW$255, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$255, DW_AT_TI_begin_line(0xa5)
	.dwattr $C$DW$255, DW_AT_TI_end_line(0xa7)
$C$DW$256	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$256, DW_AT_low_pc($C$DW$L$_main$6$B)
	.dwattr $C$DW$256, DW_AT_high_pc($C$DW$L$_main$6$E)
	.dwendtag $C$DW$255


$C$DW$257	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$257, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L11:1:1463466944")
	.dwattr $C$DW$257, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$257, DW_AT_TI_begin_line(0xa2)
	.dwattr $C$DW$257, DW_AT_TI_end_line(0xa4)
$C$DW$258	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$258, DW_AT_low_pc($C$DW$L$_main$4$B)
	.dwattr $C$DW$258, DW_AT_high_pc($C$DW$L$_main$4$E)
	.dwendtag $C$DW$257


$C$DW$259	.dwtag  DW_TAG_TI_loop
	.dwattr $C$DW$259, DW_AT_name("C:\nightlybuilds\mathlib\ti\mathlib\src\asinhdp\c66\asinhdp_66_BE_COFF\Debug\asinhdp_d.asm:$C$L9:1:1463466944")
	.dwattr $C$DW$259, DW_AT_TI_begin_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$259, DW_AT_TI_begin_line(0x9f)
	.dwattr $C$DW$259, DW_AT_TI_end_line(0xa1)
$C$DW$260	.dwtag  DW_TAG_TI_loop_range
	.dwattr $C$DW$260, DW_AT_low_pc($C$DW$L$_main$2$B)
	.dwattr $C$DW$260, DW_AT_high_pc($C$DW$L$_main$2$E)
	.dwendtag $C$DW$259

	.dwattr $C$DW$180, DW_AT_TI_end_file("C:/nightlybuilds/mathlib/ti/mathlib/src/asinhdp/asinhdp_d.c")
	.dwattr $C$DW$180, DW_AT_TI_end_line(0x11d)
	.dwattr $C$DW$180, DW_AT_TI_end_column(0x01)
	.dwendentry
	.dwendtag $C$DW$180

;******************************************************************************
;* STRINGS                                                                    *
;******************************************************************************
	.sect	".const:.string"
$C$SL1:	.string	"asinhDP",0
$C$SL2:	.string	"----------------------------------------",0
$C$SL3:	.string	"----------------------------------------",10,0
;*****************************************************************************
;* UNDEFINED EXTERNAL REFERENCES                                             *
;*****************************************************************************
	.global	_printf
	.global	_asinh
	.global	_driver_init
	.global	_print_profile_results
	.global	_print_memory_results
	.global	_print_test_results
	.global	_gimme_random
	.global	_isequal
	.global	_asinhdp_c
	.global	_asinhdp
	.global	_asinhdp_v
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
	.global	__divf
	.global	__fltllif
	.global	__fixdlli

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

$C$DW$T$43	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$43, DW_AT_type(*$C$DW$T$10)
	.dwattr $C$DW$T$43, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$43, DW_AT_byte_size(0x14)
$C$DW$261	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$261, DW_AT_upper_bound(0x04)
	.dwendtag $C$DW$T$43

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

$C$DW$T$49	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$49, DW_AT_type(*$C$DW$T$14)
	.dwattr $C$DW$T$49, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$49, DW_AT_byte_size(0x28)
$C$DW$262	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$262, DW_AT_upper_bound(0x04)
	.dwendtag $C$DW$T$49

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
$C$DW$T$31	.dwtag  DW_TAG_const_type
	.dwattr $C$DW$T$31, DW_AT_type(*$C$DW$T$17)
$C$DW$T$32	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$32, DW_AT_type(*$C$DW$T$31)
	.dwattr $C$DW$T$32, DW_AT_address_class(0x20)
$C$DW$T$33	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$33, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$T$33, DW_AT_address_class(0x20)
$C$DW$T$57	.dwtag  DW_TAG_TI_restrict_type
	.dwattr $C$DW$T$57, DW_AT_type(*$C$DW$T$33)

$C$DW$T$58	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$58, DW_AT_type(*$C$DW$T$57)
	.dwattr $C$DW$T$58, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$58, DW_AT_byte_size(0x14)
$C$DW$263	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$263, DW_AT_upper_bound(0x04)
	.dwendtag $C$DW$T$58


$C$DW$T$59	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$59, DW_AT_type(*$C$DW$T$57)
	.dwattr $C$DW$T$59, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$59, DW_AT_byte_size(0x08)
$C$DW$264	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$264, DW_AT_upper_bound(0x01)
	.dwendtag $C$DW$T$59


$C$DW$T$61	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$61, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$T$61, DW_AT_language(DW_LANG_C)
$C$DW$265	.dwtag  DW_TAG_subrange_type
	.dwendtag $C$DW$T$61


$C$DW$T$63	.dwtag  DW_TAG_array_type
	.dwattr $C$DW$T$63, DW_AT_type(*$C$DW$T$17)
	.dwattr $C$DW$T$63, DW_AT_language(DW_LANG_C)
	.dwattr $C$DW$T$63, DW_AT_byte_size(0x648)
$C$DW$266	.dwtag  DW_TAG_subrange_type
	.dwattr $C$DW$266, DW_AT_upper_bound(0xc8)
	.dwendtag $C$DW$T$63

$C$DW$T$18	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$18, DW_AT_encoding(DW_ATE_float)
	.dwattr $C$DW$T$18, DW_AT_name("long double")
	.dwattr $C$DW$T$18, DW_AT_byte_size(0x08)
$C$DW$T$25	.dwtag  DW_TAG_base_type
	.dwattr $C$DW$T$25, DW_AT_encoding(DW_ATE_signed_char)
	.dwattr $C$DW$T$25, DW_AT_name("signed char")
	.dwattr $C$DW$T$25, DW_AT_byte_size(0x01)
$C$DW$T$26	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$26, DW_AT_type(*$C$DW$T$25)
	.dwattr $C$DW$T$26, DW_AT_address_class(0x20)
$C$DW$T$37	.dwtag  DW_TAG_const_type
	.dwattr $C$DW$T$37, DW_AT_type(*$C$DW$T$25)
$C$DW$T$38	.dwtag  DW_TAG_pointer_type
	.dwattr $C$DW$T$38, DW_AT_type(*$C$DW$T$37)
	.dwattr $C$DW$T$38, DW_AT_address_class(0x20)
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

$C$DW$267	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A0")
	.dwattr $C$DW$267, DW_AT_location[DW_OP_reg0]
$C$DW$268	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A1")
	.dwattr $C$DW$268, DW_AT_location[DW_OP_reg1]
$C$DW$269	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A2")
	.dwattr $C$DW$269, DW_AT_location[DW_OP_reg2]
$C$DW$270	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A3")
	.dwattr $C$DW$270, DW_AT_location[DW_OP_reg3]
$C$DW$271	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A4")
	.dwattr $C$DW$271, DW_AT_location[DW_OP_reg4]
$C$DW$272	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A5")
	.dwattr $C$DW$272, DW_AT_location[DW_OP_reg5]
$C$DW$273	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A6")
	.dwattr $C$DW$273, DW_AT_location[DW_OP_reg6]
$C$DW$274	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A7")
	.dwattr $C$DW$274, DW_AT_location[DW_OP_reg7]
$C$DW$275	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A8")
	.dwattr $C$DW$275, DW_AT_location[DW_OP_reg8]
$C$DW$276	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A9")
	.dwattr $C$DW$276, DW_AT_location[DW_OP_reg9]
$C$DW$277	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A10")
	.dwattr $C$DW$277, DW_AT_location[DW_OP_reg10]
$C$DW$278	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A11")
	.dwattr $C$DW$278, DW_AT_location[DW_OP_reg11]
$C$DW$279	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A12")
	.dwattr $C$DW$279, DW_AT_location[DW_OP_reg12]
$C$DW$280	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A13")
	.dwattr $C$DW$280, DW_AT_location[DW_OP_reg13]
$C$DW$281	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A14")
	.dwattr $C$DW$281, DW_AT_location[DW_OP_reg14]
$C$DW$282	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A15")
	.dwattr $C$DW$282, DW_AT_location[DW_OP_reg15]
$C$DW$283	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B0")
	.dwattr $C$DW$283, DW_AT_location[DW_OP_reg16]
$C$DW$284	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B1")
	.dwattr $C$DW$284, DW_AT_location[DW_OP_reg17]
$C$DW$285	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B2")
	.dwattr $C$DW$285, DW_AT_location[DW_OP_reg18]
$C$DW$286	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B3")
	.dwattr $C$DW$286, DW_AT_location[DW_OP_reg19]
$C$DW$287	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B4")
	.dwattr $C$DW$287, DW_AT_location[DW_OP_reg20]
$C$DW$288	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B5")
	.dwattr $C$DW$288, DW_AT_location[DW_OP_reg21]
$C$DW$289	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B6")
	.dwattr $C$DW$289, DW_AT_location[DW_OP_reg22]
$C$DW$290	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B7")
	.dwattr $C$DW$290, DW_AT_location[DW_OP_reg23]
$C$DW$291	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B8")
	.dwattr $C$DW$291, DW_AT_location[DW_OP_reg24]
$C$DW$292	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B9")
	.dwattr $C$DW$292, DW_AT_location[DW_OP_reg25]
$C$DW$293	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B10")
	.dwattr $C$DW$293, DW_AT_location[DW_OP_reg26]
$C$DW$294	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B11")
	.dwattr $C$DW$294, DW_AT_location[DW_OP_reg27]
$C$DW$295	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B12")
	.dwattr $C$DW$295, DW_AT_location[DW_OP_reg28]
$C$DW$296	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B13")
	.dwattr $C$DW$296, DW_AT_location[DW_OP_reg29]
$C$DW$297	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DP")
	.dwattr $C$DW$297, DW_AT_location[DW_OP_reg30]
$C$DW$298	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SP")
	.dwattr $C$DW$298, DW_AT_location[DW_OP_reg31]
$C$DW$299	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FP")
	.dwattr $C$DW$299, DW_AT_location[DW_OP_regx 0x20]
$C$DW$300	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("PC")
	.dwattr $C$DW$300, DW_AT_location[DW_OP_regx 0x21]
$C$DW$301	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IRP")
	.dwattr $C$DW$301, DW_AT_location[DW_OP_regx 0x22]
$C$DW$302	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IFR")
	.dwattr $C$DW$302, DW_AT_location[DW_OP_regx 0x23]
$C$DW$303	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("NRP")
	.dwattr $C$DW$303, DW_AT_location[DW_OP_regx 0x24]
$C$DW$304	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A16")
	.dwattr $C$DW$304, DW_AT_location[DW_OP_regx 0x25]
$C$DW$305	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A17")
	.dwattr $C$DW$305, DW_AT_location[DW_OP_regx 0x26]
$C$DW$306	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A18")
	.dwattr $C$DW$306, DW_AT_location[DW_OP_regx 0x27]
$C$DW$307	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A19")
	.dwattr $C$DW$307, DW_AT_location[DW_OP_regx 0x28]
$C$DW$308	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A20")
	.dwattr $C$DW$308, DW_AT_location[DW_OP_regx 0x29]
$C$DW$309	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A21")
	.dwattr $C$DW$309, DW_AT_location[DW_OP_regx 0x2a]
$C$DW$310	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A22")
	.dwattr $C$DW$310, DW_AT_location[DW_OP_regx 0x2b]
$C$DW$311	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A23")
	.dwattr $C$DW$311, DW_AT_location[DW_OP_regx 0x2c]
$C$DW$312	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A24")
	.dwattr $C$DW$312, DW_AT_location[DW_OP_regx 0x2d]
$C$DW$313	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A25")
	.dwattr $C$DW$313, DW_AT_location[DW_OP_regx 0x2e]
$C$DW$314	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A26")
	.dwattr $C$DW$314, DW_AT_location[DW_OP_regx 0x2f]
$C$DW$315	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A27")
	.dwattr $C$DW$315, DW_AT_location[DW_OP_regx 0x30]
$C$DW$316	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A28")
	.dwattr $C$DW$316, DW_AT_location[DW_OP_regx 0x31]
$C$DW$317	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A29")
	.dwattr $C$DW$317, DW_AT_location[DW_OP_regx 0x32]
$C$DW$318	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A30")
	.dwattr $C$DW$318, DW_AT_location[DW_OP_regx 0x33]
$C$DW$319	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("A31")
	.dwattr $C$DW$319, DW_AT_location[DW_OP_regx 0x34]
$C$DW$320	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B16")
	.dwattr $C$DW$320, DW_AT_location[DW_OP_regx 0x35]
$C$DW$321	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B17")
	.dwattr $C$DW$321, DW_AT_location[DW_OP_regx 0x36]
$C$DW$322	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B18")
	.dwattr $C$DW$322, DW_AT_location[DW_OP_regx 0x37]
$C$DW$323	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B19")
	.dwattr $C$DW$323, DW_AT_location[DW_OP_regx 0x38]
$C$DW$324	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B20")
	.dwattr $C$DW$324, DW_AT_location[DW_OP_regx 0x39]
$C$DW$325	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B21")
	.dwattr $C$DW$325, DW_AT_location[DW_OP_regx 0x3a]
$C$DW$326	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B22")
	.dwattr $C$DW$326, DW_AT_location[DW_OP_regx 0x3b]
$C$DW$327	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B23")
	.dwattr $C$DW$327, DW_AT_location[DW_OP_regx 0x3c]
$C$DW$328	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B24")
	.dwattr $C$DW$328, DW_AT_location[DW_OP_regx 0x3d]
$C$DW$329	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B25")
	.dwattr $C$DW$329, DW_AT_location[DW_OP_regx 0x3e]
$C$DW$330	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B26")
	.dwattr $C$DW$330, DW_AT_location[DW_OP_regx 0x3f]
$C$DW$331	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B27")
	.dwattr $C$DW$331, DW_AT_location[DW_OP_regx 0x40]
$C$DW$332	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B28")
	.dwattr $C$DW$332, DW_AT_location[DW_OP_regx 0x41]
$C$DW$333	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B29")
	.dwattr $C$DW$333, DW_AT_location[DW_OP_regx 0x42]
$C$DW$334	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B30")
	.dwattr $C$DW$334, DW_AT_location[DW_OP_regx 0x43]
$C$DW$335	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("B31")
	.dwattr $C$DW$335, DW_AT_location[DW_OP_regx 0x44]
$C$DW$336	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("AMR")
	.dwattr $C$DW$336, DW_AT_location[DW_OP_regx 0x45]
$C$DW$337	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CSR")
	.dwattr $C$DW$337, DW_AT_location[DW_OP_regx 0x46]
$C$DW$338	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ISR")
	.dwattr $C$DW$338, DW_AT_location[DW_OP_regx 0x47]
$C$DW$339	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ICR")
	.dwattr $C$DW$339, DW_AT_location[DW_OP_regx 0x48]
$C$DW$340	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IER")
	.dwattr $C$DW$340, DW_AT_location[DW_OP_regx 0x49]
$C$DW$341	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ISTP")
	.dwattr $C$DW$341, DW_AT_location[DW_OP_regx 0x4a]
$C$DW$342	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IN")
	.dwattr $C$DW$342, DW_AT_location[DW_OP_regx 0x4b]
$C$DW$343	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("OUT")
	.dwattr $C$DW$343, DW_AT_location[DW_OP_regx 0x4c]
$C$DW$344	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ACR")
	.dwattr $C$DW$344, DW_AT_location[DW_OP_regx 0x4d]
$C$DW$345	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ADR")
	.dwattr $C$DW$345, DW_AT_location[DW_OP_regx 0x4e]
$C$DW$346	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FADCR")
	.dwattr $C$DW$346, DW_AT_location[DW_OP_regx 0x4f]
$C$DW$347	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FAUCR")
	.dwattr $C$DW$347, DW_AT_location[DW_OP_regx 0x50]
$C$DW$348	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("FMCR")
	.dwattr $C$DW$348, DW_AT_location[DW_OP_regx 0x51]
$C$DW$349	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GFPGFR")
	.dwattr $C$DW$349, DW_AT_location[DW_OP_regx 0x52]
$C$DW$350	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DIER")
	.dwattr $C$DW$350, DW_AT_location[DW_OP_regx 0x53]
$C$DW$351	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("REP")
	.dwattr $C$DW$351, DW_AT_location[DW_OP_regx 0x54]
$C$DW$352	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSCL")
	.dwattr $C$DW$352, DW_AT_location[DW_OP_regx 0x55]
$C$DW$353	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSCH")
	.dwattr $C$DW$353, DW_AT_location[DW_OP_regx 0x56]
$C$DW$354	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ARP")
	.dwattr $C$DW$354, DW_AT_location[DW_OP_regx 0x57]
$C$DW$355	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ILC")
	.dwattr $C$DW$355, DW_AT_location[DW_OP_regx 0x58]
$C$DW$356	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RILC")
	.dwattr $C$DW$356, DW_AT_location[DW_OP_regx 0x59]
$C$DW$357	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DNUM")
	.dwattr $C$DW$357, DW_AT_location[DW_OP_regx 0x5a]
$C$DW$358	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("SSR")
	.dwattr $C$DW$358, DW_AT_location[DW_OP_regx 0x5b]
$C$DW$359	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GPLYA")
	.dwattr $C$DW$359, DW_AT_location[DW_OP_regx 0x5c]
$C$DW$360	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("GPLYB")
	.dwattr $C$DW$360, DW_AT_location[DW_OP_regx 0x5d]
$C$DW$361	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TSR")
	.dwattr $C$DW$361, DW_AT_location[DW_OP_regx 0x5e]
$C$DW$362	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ITSR")
	.dwattr $C$DW$362, DW_AT_location[DW_OP_regx 0x5f]
$C$DW$363	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("NTSR")
	.dwattr $C$DW$363, DW_AT_location[DW_OP_regx 0x60]
$C$DW$364	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("EFR")
	.dwattr $C$DW$364, DW_AT_location[DW_OP_regx 0x61]
$C$DW$365	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ECR")
	.dwattr $C$DW$365, DW_AT_location[DW_OP_regx 0x62]
$C$DW$366	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("IERR")
	.dwattr $C$DW$366, DW_AT_location[DW_OP_regx 0x63]
$C$DW$367	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DMSG")
	.dwattr $C$DW$367, DW_AT_location[DW_OP_regx 0x64]
$C$DW$368	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CMSG")
	.dwattr $C$DW$368, DW_AT_location[DW_OP_regx 0x65]
$C$DW$369	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_ADDR")
	.dwattr $C$DW$369, DW_AT_location[DW_OP_regx 0x66]
$C$DW$370	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_DATA")
	.dwattr $C$DW$370, DW_AT_location[DW_OP_regx 0x67]
$C$DW$371	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DT_DMA_CNTL")
	.dwattr $C$DW$371, DW_AT_location[DW_OP_regx 0x68]
$C$DW$372	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TCU_CNTL")
	.dwattr $C$DW$372, DW_AT_location[DW_OP_regx 0x69]
$C$DW$373	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_REC_CNTL")
	.dwattr $C$DW$373, DW_AT_location[DW_OP_regx 0x6a]
$C$DW$374	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_XMT_CNTL")
	.dwattr $C$DW$374, DW_AT_location[DW_OP_regx 0x6b]
$C$DW$375	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_CFG")
	.dwattr $C$DW$375, DW_AT_location[DW_OP_regx 0x6c]
$C$DW$376	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_RDATA")
	.dwattr $C$DW$376, DW_AT_location[DW_OP_regx 0x6d]
$C$DW$377	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_WDATA")
	.dwattr $C$DW$377, DW_AT_location[DW_OP_regx 0x6e]
$C$DW$378	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_RADDR")
	.dwattr $C$DW$378, DW_AT_location[DW_OP_regx 0x6f]
$C$DW$379	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("RTDX_WADDR")
	.dwattr $C$DW$379, DW_AT_location[DW_OP_regx 0x70]
$C$DW$380	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("MFREG0")
	.dwattr $C$DW$380, DW_AT_location[DW_OP_regx 0x71]
$C$DW$381	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DBG_STAT")
	.dwattr $C$DW$381, DW_AT_location[DW_OP_regx 0x72]
$C$DW$382	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("BRK_EN")
	.dwattr $C$DW$382, DW_AT_location[DW_OP_regx 0x73]
$C$DW$383	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP0_CNT")
	.dwattr $C$DW$383, DW_AT_location[DW_OP_regx 0x74]
$C$DW$384	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP0")
	.dwattr $C$DW$384, DW_AT_location[DW_OP_regx 0x75]
$C$DW$385	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP1")
	.dwattr $C$DW$385, DW_AT_location[DW_OP_regx 0x76]
$C$DW$386	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP2")
	.dwattr $C$DW$386, DW_AT_location[DW_OP_regx 0x77]
$C$DW$387	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("HWBP3")
	.dwattr $C$DW$387, DW_AT_location[DW_OP_regx 0x78]
$C$DW$388	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("OVERLAY")
	.dwattr $C$DW$388, DW_AT_location[DW_OP_regx 0x79]
$C$DW$389	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("PC_PROF")
	.dwattr $C$DW$389, DW_AT_location[DW_OP_regx 0x7a]
$C$DW$390	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("ATSR")
	.dwattr $C$DW$390, DW_AT_location[DW_OP_regx 0x7b]
$C$DW$391	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TRR")
	.dwattr $C$DW$391, DW_AT_location[DW_OP_regx 0x7c]
$C$DW$392	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("TCRR")
	.dwattr $C$DW$392, DW_AT_location[DW_OP_regx 0x7d]
$C$DW$393	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DESR")
	.dwattr $C$DW$393, DW_AT_location[DW_OP_regx 0x7e]
$C$DW$394	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("DETR")
	.dwattr $C$DW$394, DW_AT_location[DW_OP_regx 0x7f]
$C$DW$395	.dwtag  DW_TAG_TI_assign_register, DW_AT_name("CIE_RETA")
	.dwattr $C$DW$395, DW_AT_location[DW_OP_regx 0xe4]
	.dwendtag $C$DW$CU

