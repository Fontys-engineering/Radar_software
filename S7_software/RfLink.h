#pragma once

#include <stdint.h>

#include <ti/control/mmwavelink/mmwavelink.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t RfLink_init(void);
int32_t RfLink_getVersion(rlVersion_t *ver);
void RfLink_processSpawn(void);

#ifdef __cplusplus
}
#endif
