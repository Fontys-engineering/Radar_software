#include "main.h"

int main(void)
{
    System_init();
    Board_init();

    initUart();

    deInitUart();

    Board_deinit();
    System_deinit();

    return 0;
}
