#include <stdio.h>
#include "utils.h"
#include "movement.h"
#include "sensors.h"

// Recover from a stuck situation. Robot is against a fence, 
// detect that situation, and align with the fence.
void recover(){
    //TODO: Detect that situation

    // Back off
    move(-500);
    SLEEP(300);
    stop(TACHO_COAST);

    // Align with the fence
    turn_90d_left(0);
}

void always_left(){
    int sonar_value;
    int speed = 600;
    int sonar_threshold = 15 * 10;
    move(speed);
    while(true){
        get_sonar_value(&sonar_value);
        printf("%d -- ", sonar_value);
        if (sonar_value < sonar_threshold){
            stop(TACHO_COAST);
            printf("Turning left");
            // turn_90d_left(speed);
            recover();
            SLEEP(750);
            move(speed);
            // get_sonar_value(&sonar_value);
            // if (sonar_value < sonar_threshold){
            //     printf("Nothing left, getting back");
            //     turn_90d_right(speed);
            //     SLEEP(500);
            // }
        }
    }
}

int main(void)
{
    int i;// int j; int k;
    uint8_t sn;
    FLAGS_T state;
    char s[256];
    printf("*** ( EV3 ) Hello! ***\n");
    init_movement();
    init_sensors();
    printf("*** Initialisation done ***\n");
    // SLEEP(5000);
    always_left();

    ev3_uninit();
    printf("*** ( EV3 ) Bye! ***\n");

    return (0);
}

