#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "strats.h"
#include <signal.h>

void stop_handler()
{
    stop(TACHO_COAST);
    exit(0);
}

int main(void)
{
    // always_left(start_angle);
    setvbuf(stdout, NULL, _IONBF, 0);
    struct sigaction sa;
    sa.sa_handler = stop_handler;
    sigaction(SIGINT, &sa, NULL);
    int gyro_value;
    int starting_motor_position;
    printf("*** ( EV3 ) Hello! ***\n");
    init_movement();
    init_sensors();
    get_gyro_value(&gyro_value);
    printf("*** Initialisation done ***\n");
    get_left_motor_position(&starting_motor_position);
    //against_time();
    while (1)
    {
        move(500);
        if (_is_obstacle(starting_motor_position, 120))
        {
            stop(TACHO_COAST);
        }
    }

    ev3_uninit();
    printf("*** ( EV3 ) Bye! ***\n");

    return (0);
}
