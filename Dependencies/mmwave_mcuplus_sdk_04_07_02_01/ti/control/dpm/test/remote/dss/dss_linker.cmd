/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

-stack 0x1000
/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    systemHeap      : { } > DSS_L2

    /* The LL Profile uses this memory section. */
    llProfileData           : {} > DSS_L3

    /* The MCPI Logs are placed here */
    .MCPILogBuffer  : { } > DSS_L2

    /* Data Injection Control: Shared between the MSS & DSS */
    .dataInjectionControl   : {} > DSS_L3
}
/*----------------------------------------------------------------------------*/

