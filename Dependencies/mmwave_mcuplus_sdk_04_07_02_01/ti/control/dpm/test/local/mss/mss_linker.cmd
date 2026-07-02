/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    /* System Heap: One heap for all dynamic memory allocations */
    systemHeap      : { } > MSS_L2

    /* The LL Profile uses this memory section. */
    llProfileData   : {} > DSS_L3

    /* The MCPI Logs are stored here */
    .MCPILogBuffer  : { } > MSS_L2
}
/*----------------------------------------------------------------------------*/

