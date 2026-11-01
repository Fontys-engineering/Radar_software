#ifndef ENET_STREAM_MODE_H
#define ENET_STREAM_MODE_H

#include <stdint.h>

#define ENET_STREAM_MODE_CARTESIAN (0U)
#define ENET_STREAM_MODE_TEST      (1U)

/* The hook for the enet stream testing, the TCP sender reads it once per radar result. */
void AppTcp_setStreamMode(uint8_t mode);
uint8_t AppTcp_getStreamMode(void);

#endif
