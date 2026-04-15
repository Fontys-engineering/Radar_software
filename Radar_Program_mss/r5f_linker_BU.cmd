/*----------------------------------------------------------------------------*/
/* r5f_linker.cmd                                                             */
/*                                                                            */
/* (c) Texas Instruments 2020, All rights reserved.                           */
/*----------------------------------------------------------------------------*/

--retain="*(.intc_text)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x4000                              /* HEAP AREA SIZE                */

-e_vectors  /* Entry point */

/* Stack Sizes for various modes */
__IRQ_STACK_SIZE = 256;
__FIQ_STACK_SIZE = 256;
__ABORT_STACK_SIZE = 256;
__UNDEFINED_STACK_SIZE = 256;
__SVC_STACK_SIZE = 4096;


/*----------------------------------------------------------------------------*/
/* Memory Map                                                                 */
/*----------------------------------------------------------------------------*/
MEMORY{
PAGE 0:
    RESET_VECTORS  (X)  : origin=0x00000000 length=0x00000040
    TCMA_RAM (RX) : origin=0x00000040 length=0x0000FFC0
    TCMB_RAM (RW) : origin=0x00080000 length=0x00010000

    CPPI_DESC (RW) : origin=0x102EC000 length=0x00004000

    SBL_RESERVED_L2_RAM (RW)   : origin=0x10200000 length=0x00020000
    MSS_L2 (RW)               : origin=0x10220000 length=0x000CC000

    DSS_L3 (RW)   : origin=0x88000000 length=0x00100000
    HWA_RAM (RW)  : origin=0x82000000 length=0x00020000

    RTOS_NORTOS_IPC_SHM_MEM : ORIGIN = 0xC5000200, LENGTH = 0x1D40
}


/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
/*----------------------------------------------------------------------------*/
SECTIONS{

    /* Vector table */
    .vectors:{} palign(8) > RESET_VECTORS




    /* Boot / critical code */
    GROUP {
        .text.hwi: palign(8)
        .text.cache: palign(8)
        .text.mpu: palign(8)
        .text.boot: palign(8)
        .text:abort: palign(8)
    } > MSS_L2

    /* Main code */
    GROUP {
        .text:   {} palign(8)
        .rodata: {} palign(8)
    } > MSS_L2

    /* Initialized data */
    GROUP {
        .data:   {} palign(8)
    } > MSS_L2

    GROUP {
        .stack:  {} palign(8)
        .sysmem: {} palign(8)

    } >SBL_RESERVED_L2_RAM

    /* lwIP pools */
    .lwip.pools (NOLOAD) :
    {
        *(.bss.memp*)
        "C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mcu_plus_sdk_awr294x_10_02_00_04/source/networking/enet/lib/lwipif-cpsw-freertos.awr294x.r5f.ti-arm-clang.debug.lib"(.bss*)

        "C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mcu_plus_sdk_awr294x_10_02_00_04/source/networking/lwip/lib/lwip-freertos.awr294x.r5f.ti-arm-clang.debug.lib"(.bss*)
    } > DSS_L3


    

    /* Uninitialized data */
    GROUP {
        .bss:    {} palign(8)
        RUN_START(__BSS_START)
        RUN_END(__BSS_END)
    } > MSS_L2

    /* Mode stacks */
    GROUP {
        .irqstack: {. = . + __IRQ_STACK_SIZE;} align(8)
        RUN_START(__IRQ_STACK_START)
        RUN_END(__IRQ_STACK_END)

        .fiqstack: {. = . + __FIQ_STACK_SIZE;} align(8)
        RUN_START(__FIQ_STACK_START)
        RUN_END(__FIQ_STACK_END)

        .svcstack: {. = . + __SVC_STACK_SIZE;} align(8)
        RUN_START(__SVC_STACK_START)
        RUN_END(__SVC_STACK_END)

        .abortstack: {. = . + __ABORT_STACK_SIZE;} align(8)
        RUN_START(__ABORT_STACK_START)
        RUN_END(__ABORT_STACK_END)

        .undefinedstack: {. = . + __UNDEFINED_STACK_SIZE;} align(8)
        RUN_START(__UNDEFINED_STACK_START)
        RUN_END(__UNDEFINED_STACK_END)
    } > MSS_L2

    /* Ethernet buffers */
    .bss:ENET_CPPI_DESC        (NOLOAD) {} ALIGN (128) > CPPI_DESC
    .bss:ENET_DMA_PKT_MEMPOOL  (NOLOAD) {} ALIGN (128) > DSS_L3

    /* Optional L3 buffers */
    .bss.dss_l3 {} > DSS_L3

    /* IPC shared memory */
    .bss.ipc_vring_mem   (NOLOAD) : {} > RTOS_NORTOS_IPC_SHM_MEM
}