#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "strats.h"

int main(void)
{
    // always_left(start_angle);
    setvbuf(stdout, NULL, _IONBF, 0);
    int gyro_value;
    int last_gyro_value;
    printf("*** ( EV3 ) Hello! ***\n");
    init_movement();
    init_sensors();
    get_gyro_value(&last_gyro_value);
    while (1)
    {
        get_gyro_value(&gyro_value);
        move(500);
        bool accident = recover_accident(&last_gyro_value, &gyro_value);
        if (accident == false)
            last_gyro_value = gyro_value;
        SLEEP(1);
    }
    printf("*** Initialisation done ***\n");
    // SLEEP(5000);

    // int angle;
    // for (int i = 1; i <= 8; i++){
    //     int sign = (i % 2) * 2 - 1;
    //     move(1000 * sign);
    //     sleep(1);
    //     angle = turn_to_angle(start_angle - 90, 10);
    //     printf("diff = %d\n", abs(angle - start_angle));
    //     angle = turn_to_angle(start_angle, 10);
    //     printf("diff = %d\n", abs(angle - start_angle));
    //     sleep(1);
    // }
    // int ret = 0;
    // int dir = 0;
    // while(true){
    //     ret = avoid_obstacle(start_angle, dir);
    //     if (ret == -2) break;
    //     sleep(2);
    //     dir = ret;
    // }
    // move(400);

    // release_obstacle();

    // turn_to_angle(start_angle + 90, 10);
    // SLEEP(1500);
    // turn_to_angle(start_angle + 90, 10);
    // SLEEP(1500);
    // turn_to_angle(start_angle - 90, 10);
    // SLEEP(1500);
    // turn_to_angle(start_angle + 90, 10);
    // always_left_touch();
    // while (true)
    // move(0);
    // {
    // 	if _check_pressed()
    // 	{
    // 	turn_90_d_left(0);
    // }
    // sleep(1);
    // }

    ev3_uninit();
    printf("*** ( EV3 ) Bye! ***\n");

    return (0);
}
