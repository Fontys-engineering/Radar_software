#ifndef PLASMA_ENET_TEST_H
#define PLASMA_ENET_TEST_H

#include "lwip/api.h"

/* sized like Plasma_Record. */
err_t PlasmaEnet_sendTestRecord(struct netconn *conn);

#endif
