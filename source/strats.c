#include "strats.h"
#include <stdlib.h>

static void _re_calibrate(int gyro_angle)
{
    stop(TACHO_COAST);
    turn_to_angle(gyro_angle, ANGLE_THRESHOLD);
}

bool recover_accident(int *previous_angle, int *current_angle)
{

    if (detect_accident(*previous_angle, *current_angle))
    {
        _re_calibrate(*previous_angle);
        return true;
    }
    return false;
}

void recover()
{
    // TODO: Detect that situation

    // Back off
    move(-500);
    SLEEP(300);
    stop(TACHO_COAST);
    SLEEP(200);

    // Align with the fence
    int angle;
    get_gyro_value(&angle);
    turn_to_angle(angle - 90, 5);
}

void against_time()
{
    int c = 0;
    int sonar_value;
    const int SPEED = 800; // DO NOT SET TO 0
    const int SONAR_THRESHOLD = 20 * 10;
    int angle;
    get_gyro_value(&angle);
    printf("Start angle = %d\n", angle);
    move(SPEED);
    while (true)
    {
        // print_motor_speeds();
        get_sonar_value(&sonar_value);
        if (c % 2 != 0)
        {
            if (sonar_value < SONAR_THRESHOLD)
            {
                // printf("Sonar value before stop= %d\n", sonar_value);
                stop(TACHO_COAST);
                get_sonar_value(&sonar_value);
                // printf("Sonar value after stop= %d\n", sonar_value);

                angle -= 90;
                turn_to_angle(angle, 5);
                c += 1;

                // SLEEP(200);
                // get_gyro_value(&angle);
                // // printf("Difference in angle= %d\n", abs(angle - dest_angle) % 360 );

                move(SPEED);
            }   
        }
        else
        {
            recalibrate();
            SLEEP(200);
            int old_angle = angle;
            get_gyro_value(&angle);
            printf("Corrected with: %d\n", abs(angle - old_angle + 90));
            c += 1;
            move(SPEED);
        }
    }
}

void always_left_touch()
{
    int speed = 0;
    move(speed);
    while (true)
    {
        if (check_pressed())
        {
            stop(TACHO_COAST);
            printf("Turning left");
            // turn_90d_left(speed);
            recover();
            SLEEP(750);
            move(speed);
        }
    }
}

int avoid_obstacle(int start_angle, int choose_dir)
{
    int sonar_value, l_sonar_v, r_sonar_v;
    int speed = 500;
    int sonar_threshold = 15 * 10;
    bool dir_is_left = false;
    move(speed);
    while (true)
    {
        get_sonar_value(&sonar_value);
        // found obstacle
        if (sonar_value < sonar_threshold)
        {
            printf("Found something %d \n", choose_dir);
            if (choose_dir == 1)
            {
                printf("Has to turn right \n");
                turn_to_angle(start_angle, 10);
                sleep(1);
                return -2;
            }
            else if (choose_dir == -1)
            {
                printf("Has to turn left \n");
                turn_to_angle(start_angle, 10);
                sleep(1);
                return -2;
            }
            // choose dir
            turn_to_angle(start_angle - 90, 10);
            sleep(1);
            get_sonar_value(&l_sonar_v);
            turn_to_angle(start_angle + 90, 10);
            sleep(1);
            get_sonar_value(&r_sonar_v);
            if (l_sonar_v > r_sonar_v)
            {
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

void recalibrate()
{
    while (!check_pressed())
    {
    }
    recover();
}
