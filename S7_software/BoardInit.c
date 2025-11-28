#include "BoardInit.h"

void initUart()
{
    /* Open drivers to open the UART driver for console */
    Drivers_open();
    Board_driversOpen();

    DebugP_log("UART succesfully initialized\r\n");
}

void deInitUart()
{
    DebugP_log("UART is now de-initializing\r\n");

    /* Close the UART drivers*/
    Board_driversClose();
    Drivers_close();
}
