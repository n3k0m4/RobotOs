#include <stdio.h>
#include "utils.h"
#include "movement.h"


int main(void)
{
    int i;
    uint8_t sn;
    FLAGS_T state;
    char s[256];
    printf("*** ( EV3 ) Hello! ***\n");
    init_movement();
    move(0);
    sleep(10);
    stop(TACHO_COAST);
    ev3_uninit();
    printf("*** ( EV3 ) Bye! ***\n");

    return (0);
}
