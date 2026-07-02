//*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

-stack 0x1000
-heap 0x1000
/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    systemHeap : {} > DSS_L2
    .l2data : {} > DSS_L2
    .l3data : {} > DSS_L3
    .MCPILogBuffer : {} > DSS_L3
}
