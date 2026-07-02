/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    systemHeap : {} > DSS_L2
    .dpc_l2Heap  : {} > DSS_L2
    .dataBufL3 : {} > DSS_L3
    .MCPILogBuffer  : {} > DSS_L3
    .l3ram : {} > DSS_L3
    .preProcBuf : {} > DSS_L2
}
/*----------------------------------------------------------------------------*/
