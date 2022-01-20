#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "strats.h"
#include <signal.h>

void stop_handler(){
    stop(TACHO_COAST);
    exit(0);
}

int main(void)
{
    // always_left(start_angle);
    setvbuf(stdout, NULL, _IONBF, 0);
    struct sigaction sa;
    sa.sa_handler = stop_handler;
    sigaction (SIGINT, &sa, NULL);
    int gyro_value;
    int last_gyro_value;
    printf("*** ( EV3 ) Hello! ***\n");
    init_movement();
    init_sensors();
    printf("*** Initialisation done ***\n");

    against_time();
    ev3_uninit();
    printf("*** ( EV3 ) Bye! ***\n");

    return (0);
}
