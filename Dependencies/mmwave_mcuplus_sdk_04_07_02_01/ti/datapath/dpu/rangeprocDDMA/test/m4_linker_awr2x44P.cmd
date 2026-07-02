/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    systemHeap : {} > DSS_L2
    .dataBufL2 : {} > DSS_L2
    .dpc_l2Heap : {} > DSS_L2
    .l3data : {} > DSS_L3
    .l3ram : {} > DSS_L3
    .dataBuf : {} > DSS_L3
}
/*----------------------------------------------------------------------------*/
MEMORY
{
    DSS_L2  : ORIGIN = 0x80840000 , LENGTH = 0x00020000
}
