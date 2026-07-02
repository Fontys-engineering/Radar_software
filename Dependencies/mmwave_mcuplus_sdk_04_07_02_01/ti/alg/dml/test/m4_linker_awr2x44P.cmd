/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    systemHeap : {} > DSS_L2
    .localRAM : {} > M4_RAM
    .MCPILogBuffer  : {} > DSS_L2
    .l3ram : {} > DSS_L3
    .customCode: {} > DSS_L3
}
/*----------------------------------------------------------------------------*/
MEMORY
{
    DSS_L2  : ORIGIN = 0x80840000 , LENGTH = 0x00020000
}
