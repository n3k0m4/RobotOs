#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "movement.h"
#include "sensors.h"

// Recover from a stuck situation. Robot is against a fence, 
// detect that situation, and align with the fence.
void recover(){
    //TODO: Detect that situation

    // Back off
    move(-500);
    SLEEP(500);
    stop(TACHO_COAST);

    // Align with the fence
    turn_90d_left(0);
}

int modulo (int a, int b){
    return ((a % b) + b) % b;
}

void always_left(int start_angle){
    int sonar_value;
    int speed = 0;
    int sonar_threshold = 20 * 10;
    printf("Start angle = %d\n", start_angle);
    int dest_angle = start_angle;
    int current_angle;
    move(speed);
    while(true){
        get_sonar_value(&sonar_value);
        if (sonar_value < sonar_threshold){
            printf("Sonar value before stop= %d\n", sonar_value);
            stop(TACHO_COAST);
            get_sonar_value(&sonar_value);
            printf("Sonar value after stop= %d\n", sonar_value);

            // dest_angle = (dest_angle - 90) % 360;
            dest_angle -= 90;
            current_angle = turn_to_angle(dest_angle, 10);
            // printf("angle = %d\n", current_angle % 360);
            printf("diff = %d\n", modulo(current_angle - start_angle, 90));

            // recover();
            SLEEP(750); // not needed
            move(speed);
        }
    }
}

void always_left_touch(){
    int speed = 0;
    move(speed);
    while(true){
        if (check_pressed()){
            stop(TACHO_COAST);
            printf("Turning left");
            // turn_90d_left(speed);
            recover();
            SLEEP(750);
            move(speed);
        }
    }
}

int avoid_obstacle(int start_angle, int choose_dir){
    int sonar_value, l_sonar_v, r_sonar_v;
    int speed = 500;
    int sonar_threshold = 15 * 10;
    bool dir_is_left = false;
    move(speed);
    while(true){
        get_sonar_value(&sonar_value);
        // found obstacle
        if (sonar_value < sonar_threshold){
            printf("Found something %d \n", choose_dir);
            if (choose_dir == 1){
                printf("Has to turn right \n");
                turn_to_angle(start_angle, 10);
                sleep(1);
                return -2;
            }
            else if (choose_dir == -1){
                printf("Has to turn left \n");
                turn_to_angle(start_angle, 10);
                sleep(1);
                return -2;
            }
            //choose dir
            turn_to_angle(start_angle - 90, 10);
            sleep(1);
            get_sonar_value(&l_sonar_v);
            turn_to_angle(start_angle + 90, 10);
            sleep(1);
            get_sonar_value(&r_sonar_v);
            if (l_sonar_v > r_sonar_v){
                printf("left better \n");
                turn_to_angle(start_angle - 90, 10);
                dir_is_left = true;
                return 1;
            }
            printf("right better \n");
            return -1;
        }

    }
    
}

int main(void)
{
    printf("*** ( EV3 ) Hello! ***\n");
    init_movement();
    init_sensors();
    printf("*** Initialisation done ***\n");
    // SLEEP(5000);
    int start_angle;
    get_gyro_value(&start_angle);
    setvbuf(stdout, NULL, _IONBF, 0);
    // always_left(start_angle);

    move(0);
    SLEEP(500);
    move(500);
    SLEEP(500);
    move(200);
    SLEEP(500);
    move(100);
    SLEEP(500);
    move(50);
    SLEEP(500);
    move(25);
    stop(TACHO_COAST);

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

