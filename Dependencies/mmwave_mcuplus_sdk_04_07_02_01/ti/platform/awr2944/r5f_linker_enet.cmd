#include "ti_enet_config.h"

/* This is the stack that is used by code running within main()
 * In case of NORTOS,
 * - This means all the code outside of ISR uses this stack
 * In case of FreeRTOS
 * - This means all the code until vTaskStartScheduler() is called in main()
 *   uses this stack.
 * - After vTaskStartScheduler() each task created in FreeRTOS has its own stack
 */
--stack_size=0x2000

/* This is the heap size for malloc() API in NORTOS and FreeRTOS
 * This is also the heap used by pvPortMalloc in FreeRTOS
 */
--heap_size=0xC000

-e_vectors  /* This is the entry of the application, _vector MUST be plabed starting address 0x0 */

/* This is the size of stack when R5 is in IRQ mode
 * In NORTOS,
 * - Here interrupt nesting is disabled as of now
 * - This is the stack used by ISRs registered as type IRQ
 * In FreeRTOS,
 * - Here interrupt nesting is enabled
 * - This is stack that is used initally when a IRQ is received
 * - But then the mode is switched to SVC mode and SVC stack is used for all user ISR callbacks
 * - Hence in FreeRTOS, IRQ stack size is less and SVC stack size is more
 */
__IRQ_STACK_SIZE = 256;

/* This is the size of stack when R5 is in IRQ mode
 * - In both NORTOS and FreeRTOS nesting is disabled for FIQ
 */
__FIQ_STACK_SIZE = 256;
__ABORT_STACK_SIZE = 256;
__UNDEFINED_STACK_SIZE = 256;
__SVC_STACK_SIZE = 4096;

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
SECTIONS
{
    /* This has the R5F entry point and vector table, this MUST be at 0x0 */
    .vectors:{} palign(8) > RESET_VECTORS

    /* This has the R5F boot code until MPU is enabled,  this MUST be at a address < 0x80000000
     * i.e this cannot be placed in DDR
     */
    GROUP {
        .text.hwi: palign(8)
        .text.cache: palign(8)
        .text.mpu: palign(8)
        .text.boot: palign(8)
        .text:abort: palign(8) /* this helps in loading symbols when using XIP mode */
    } > MSS_L2

    /* This is rest of code. This can be placed in DDR if DDR is available and needed */
    GROUP {
        .text:   {} palign(8)   /* This is where code resides */
        .rodata: {} palign(8)   /* This is where const's go */
    } > MSS_L2

    /* This is rest of initialized data. This can be placed in DDR if DDR is available and needed */
    GROUP {
        .data:   {} palign(8)   /* This is where initialized globals and static go */
    } > MSS_L2

    GROUP {
        .lwip_pools:  (NOLOAD)
        {
            . = align(128);
            *(*memp_memory_POOL_1792_base*)
            . = align(128);
            *(*memp_memory_POOL_4096_base*)
            . = align(128);
            *(*memp_memory_POOL_128_base*)
            . = align(128);
            *(*memp_memory_PBUF_POOL_base*)
        } palign(128)
    } > DSS_L3

    /* This is rest of uninitialized data. This can be placed in DDR if DDR is available and needed */
    GROUP {
        .bss:    {} palign(8)   /* This is where uninitialized globals go */
        RUN_START(__BSS_START)
        RUN_END(__BSS_END)
    } > SBL_RESERVED_L2_RAM | MSS_L2

    .sysmem: {} palign(8)  > SBL_RESERVED_L2_RAM | MSS_L2 /* This is where the malloc heap goes */
    .stack:  {} palign(8)  > SBL_RESERVED_L2_RAM | MSS_L2 /* This is where the main() stack goes */

    /* This is where the stacks for different R5F modes go */
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

    /* any data buffer needed to be put in L3 can be assigned this section name */
    .bss.dss_l3 {} > DSS_L3

    /* For NDK packet memory*/
    .bss:ENET_CPPI_DESC        (NOLOAD) {} ALIGN (128) > CPPI_DESC

    .enet_dma_mem {
#if (ENET_SYSCFG_PKT_POOL_ENABLE == 1)
        *(*ENET_DMA_PKT_MEMPOOL)
#endif
    } (NOLOAD) {} ALIGN (128) > DSS_L3
    .bss:UDP_IPERF_SND_BUF  (NOLOAD) {} ALIGN (128) > DSS_L3

    /* this is used only when IPC RPMessage is enabled, else this is not used */
    .bss.ipc_vring_mem   (NOLOAD) : {} > RTOS_NORTOS_IPC_SHM_MEM
}


MEMORY
{
    RESET_VECTORS  (X)  : origin=0x00000000 length=0x00000040
    TCMA_RAM (RX) : origin=0x00000040 length=0x0000FFC0
    TCMB_RAM (RW) : origin=0x00080000 length=0x00010000

    /* when using multi-core application's i.e more than one R5F active, make sure
     * this memory does not overlap with other R5F's
     */
    SBL_RESERVED_L2_RAM (RW)   : origin=0x10200000 length=0x00020000

    MSS_L2 (RW)   : origin=0x10220000 length=0x000CC000

    /* CPPI descriptor memory */
    CPPI_DESC (RW): origin=0x102EC000 length=0x00004000

    DSS_L3 (RW)   : origin=0x88000000 length=0x00100000
    HWA_RAM (RW)  : origin=0x82000000 length=0x00020000

    /* 1st 512 B of DSS mailbox memory and MSS mailbox memory is used for IPC with R4 and should not be used by application */
    /* MSS mailbox memory is used as shared memory, we dont use bottom 32*6 bytes, since its used as SW queue by ipc_notify */
    RTOS_NORTOS_IPC_SHM_MEM : ORIGIN = 0xC5000200, LENGTH = 0x1D40
}

