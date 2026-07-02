/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    systemHeap : {} >> M4_RAM
    .l3ram: {} >> DSS_L3
#if MSS_AOA_ENABLED
    .l2Heap (NOLOAD) : {} > MSS_L2
    .dpc_l2Heap (NOLOAD) : { } >> MSS_L2
#else
    .l2Heap (NOLOAD) : {} > DSS_L2
    .dpc_l2Heap (NOLOAD) : { } >> DSS_L2
#endif
    .preProcBuf: {} >> M4_RAM
    .customCode: {} palign(8) > DSS_L3_REUSABLE  /* This is where one time config code goes */
    .text.pow: {} >> DSS_L3_REUSABLE   
    .text.sin: {} >> DSS_L3_REUSABLE   
    .text.cos: {} >> DSS_L3_REUSABLE   
    .text.log2f: {} >> DSS_L3_REUSABLE   
    .text.HWA_open: {} >> DSS_L3_REUSABLE
    printf: { nortos*.lib<printf.obj>(.text*) } >> DSS_L3_REUSABLE 
    .text.DPU_DopplerProcHWA_config: {} >> DSS_L3_REUSABLE
    .text.rangeProcHWA_ConfigHWA: {} >> DSS_L3_REUSABLE
    .text.HWA_configParamSet: {} >> DSS_L3_REUSABLE
    .text.DPU_RangeProcHWA_config: {} >> DSS_L3_REUSABLE
    .text.HWA_validateParamSetConfig: {} >> DSS_L3_REUSABLE
    .text.DPU_DopplerProcHWA_process: {} >> DSS_L3_REUSABLE
    .text.DPU_RangeCFARProcHWA_config: {} >> DSS_L3_REUSABLE
}
/*----------------------------------------------------------------------------*/
MEMORY
{
#if MSS_AOA_ENABLED
    /* This section is overlapping with SBL_RESERVED_L2_RAM, hence it should not be initiliased */
    MSS_L2  : ORIGIN = 0xC0200000 , LENGTH = 0x00020000
#else
    DSS_L2  : ORIGIN = 0x80840000 , LENGTH = 0x00020000
#endif
}
