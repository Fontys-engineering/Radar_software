/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    /* System Heap: One heap for all dynamic memory allocations */
    systemHeap      : {} > MSS_L2
    llProfileData   : {} > DSS_L3
    .MCPILogBuffer  : {} > MSS_L2
}
/*----------------------------------------------------------------------------*/

