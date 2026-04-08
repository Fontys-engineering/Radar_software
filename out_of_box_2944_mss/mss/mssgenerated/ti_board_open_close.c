/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Auto generated file 
 */

#include "ti_board_open_close.h"

int32_t Board_driversOpen(void)
{
    int32_t status = SystemP_SUCCESS;
    if(status==SystemP_SUCCESS)
    {
        status = Board_flashOpen();
    }
	SOC_rcmConfigEthMacIf();
	
    return status;
}

void Board_driversClose(void)
{
    Board_flashClose();
}

/*
 * FLASH
 */
/* FLASH Object - initialized during Flash_open() */
Flash_NorQspiObject gFlashObject_GD25B64C;

/* FLASH Driver handles - opened during Board_flashOpen() */
Flash_Handle gFlashHandle[CONFIG_FLASH_NUM_INSTANCES];

/* FLASH Attrs */
/* FLASH Attrs - populated from SysConfig options */
Flash_Attrs gFlashAttrs_GD25B64C =
{
    .flashName = "GD25B64C",
    .deviceId = 0x4017,
    .manufacturerId = 0xC8,
    .flashSize = 8388608,
    .blockCount = 128,
    .blockSize = 65536,
    .pageCount = 256,
    .pageSize = 256,
    .sectorCount = 2048,
    .sectorSize = 4096,
};

/* FLASH DevConfig */
/* FLASH DevConfig - populated from SysConfig options */
Flash_DevConfig gFlashDevCfg_GD25B64C =
{
    .enable4BAddr = false,
    .addrByteSupport = 0x00,
    .fourByteAddrEnSeq = 0x00,
    .cmdWren = 0x06,
    .cmdRdsr = 0x05,
    .srWip = (1 << 0),
    .srWel = (1 << 1),
    .resetType = 0x10,
    .eraseCfg = {
        .blockSize = 65536,
        .sectorSize = 4096,
        .cmdBlockErase3B = 0xD8,
        .cmdBlockErase4B = 0x00,
        .cmdSectorErase3B = 0x20,
        .cmdSectorErase4B = 0x00,
        .cmdChipErase = 0xC7,
    },
    .idCfg = {
        .cmd = 0x9F, /* Constant */
        .numBytes = 3,
        .dummy4 = 0,
        .dummy8 = 0,
    },
    .protocolCfg = {
        .protocol = FLASH_CFG_PROTO_1S_1S_4S,
        .isDtr = FALSE, /* DTR not supported in this version of flash driver with QSPI */
        .cmdRd = 0x6B,
        .cmdWr = 0x02,
        .modeClksCmd = 0,
        .modeClksRd = 0,
        .dummyClksCmd = 0,
        .dummyClksRd = 8,
        .enableType = 1,
    },
    .flashWriteTimeout = 4000,
    .flashBusyTimeout = 6000000,
};

/* FLASH Config */
Flash_Config gFlashConfig[CONFIG_FLASH_NUM_INSTANCES] =
{
    {
        .attrs = &gFlashAttrs_GD25B64C,
        .fxns = &gFlashNorQspiFxns,
        .devConfig = &gFlashDevCfg_GD25B64C,
        .object = (void *)&gFlashObject_GD25B64C,
    },
};

/* Externs of quirk functions and custom protocol functions */

/* FLASH Open Params - populated from SysConfig options */
Flash_Params gFlashParams[CONFIG_FLASH_NUM_INSTANCES] =
{
    {
        .quirksFxn = NULL,
        .custProtoFxn = NULL,
    },
};

uint32_t gFlashConfigNum = CONFIG_FLASH_NUM_INSTANCES;

int32_t Board_flashOpen()
{
    uint32_t instCnt;
    int32_t  status = SystemP_SUCCESS;

    /* Set the peripheral instance to use in the under lying flash implementation */
    gFlashConfig[CONFIG_FLASH0].attrs->driverInstance = CONFIG_QSPI0;

    for(instCnt = 0U; instCnt < CONFIG_FLASH_NUM_INSTANCES; instCnt++)
    {
        gFlashHandle[instCnt] = NULL;   /* Init to NULL so that we can exit gracefully */
    }

    /* Set the underlying driver instance to the FLASH config */
    gFlashAttrs_GD25B64C.driverInstance = CONFIG_QSPI0;

    /* Open all instances */
    for(instCnt = 0U; instCnt < CONFIG_FLASH_NUM_INSTANCES; instCnt++)
    {
        gFlashHandle[instCnt] = Flash_open(instCnt, &gFlashParams[instCnt]);
        if(NULL == gFlashHandle[instCnt])
        {
            DebugP_logError("FLASH open failed for instance %d !!!\r\n", instCnt);
            status = SystemP_FAILURE;
            break;
        }
    }

    if(SystemP_FAILURE == status)
    {
        Board_flashClose();   /* Exit gracefully */
    }
    return status;
}

void Board_flashClose(void)
{
    uint32_t instCnt;

    /* Close all instances that are open */
    for(instCnt = 0U; instCnt < CONFIG_FLASH_NUM_INSTANCES; instCnt++)
    {
        if(gFlashHandle[instCnt] != NULL)
        {
            Flash_close(gFlashHandle[instCnt]);
            gFlashHandle[instCnt] = NULL;
        }
    }
    return;
}

