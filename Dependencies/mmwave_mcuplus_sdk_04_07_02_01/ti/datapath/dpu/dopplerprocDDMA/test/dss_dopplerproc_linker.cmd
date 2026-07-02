/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    systemHeap : {} >> DSS_L2
    .coreLocalRam : {} >> DSS_L2
    .MCPILogBuffer  : { } > DSS_L2
    .dpc_l2Heap > DSS_L2
    .l3ram : {} > DSS_L3
    .customCode : {} > DSS_L2
}
/*----------------------------------------------------------------------------*/
