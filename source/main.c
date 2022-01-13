#include <stdio.h>
#include "utils.h"
#include "movement.h"
#include "sensors.h"

int main(void)
{
    int i;
    uint8_t sn;
    FLAGS_T state;
    char s[256];
    printf("*** ( EV3 ) Hello! ***\n");
    init_movement();
    init_sensors();
    printf("*** Initialisation done ***\n");

    turn_90d_left(0);
    while (true)
    {
    	if _check_pressed()
    	{
		turn_90_d_left(0);
	}
	sleep(1);
    }

    ev3_uninit();
    printf("*** ( EV3 ) Bye! ***\n");

    return (0);
}
