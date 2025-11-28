#include <stdio.h>
#include <kernel/dpl/DebugP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

/* Opens and initializes the UART*/
void initUart();

/* Closes the UART drivers so the board can be turned off?*/
void deInitUart();