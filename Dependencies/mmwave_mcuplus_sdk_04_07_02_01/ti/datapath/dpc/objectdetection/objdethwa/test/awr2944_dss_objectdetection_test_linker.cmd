/*----------------------------------------------------------------------------*/
/* Linker Settings                                                            */
--retain="*(.intvecs)"

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    systemHeap : {} > DSS_L2
    .l2Heap  : {} > DSS_L2
    .MCPILogBuffer  : {} > DSS_L3
    .l3ram : {} > DSS_L3

    .code:
    {
        mathlib.ae66(.text)
    } >> L1PSRAM | DSS_L2

    GROUP : > DSS_L3
    {
        .l3ram align=128 
        /* Due to limited space in L2SRAM, put some code in DSS_L3 */
        {
            main.oe66(.fardata:gFrameConfig)
            libtestlogger_awr2944.ae66<*.oe66>(.text)
            libdpm_awr2944.ae66<*.oe66>(.text)
            gen_frame_data.oe66(.text)
        }
    }
}
/*----------------------------------------------------------------------------*/

