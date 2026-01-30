#include "main.h"
#include "RfLink.h"

int main(void)
{
    System_init();
    Board_init();

    initUart();

    if (RfLink_init() == 0)
    {
        /* Process any pending spawn requests from power-on sequence */
        RfLink_processSpawn();

        rlVersion_t ver;
        if (RfLink_getVersion(&ver) == 0)
        {
            DebugP_log("mmWaveLink: rlDeviceGetVersion OK\r\n");
        }
        else
        {
            DebugP_log("mmWaveLink: rlDeviceGetVersion FAILED\r\n");
        }
    }
    else
    {
        DebugP_log("mmWaveLink: RfLink_init FAILED\r\n");
    }

    deInitUart();

    Board_deinit();
    System_deinit();

    return 0;
}
