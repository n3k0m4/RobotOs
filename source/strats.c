#include "strats.h"
#include <stdlib.h>

static void _re_calibrate(int gyro_angle)
{
    stop(TACHO_COAST);
    turn_to_angle(gyro_angle, ANGLE_THRESHOLD);
}

bool recover_accident(int previous_angle, int current_angle)
{

    if (detect_accident(previous_angle, current_angle))
    {
        _re_calibrate(previous_angle);
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

bool _is_a_non_moving_object_ahead(int previous_sonar_value, int threshold){
    int sonar_value;
    int nb_constant_measures = 0;
    while (nb_constant_measures < 3){
        SLEEP(500);
        get_sonar_value(&sonar_value);
        if (previous_sonar_value == sonar_value) nb_constant_measures++;
        else nb_constant_measures = 0;
        previous_sonar_value = sonar_value;
    }
    return sonar_value <= threshold;
}

void against_time()
{
    int nb_turns = 0;
    int sonar_value;
    const int SPEED = 800; // DO NOT SET TO 0
    const int SONAR_THRESHOLD = 20 * 10;
    int current_angle;
    int goal_angle;
    get_gyro_value(&current_angle);
    goal_angle = current_angle;
    printf("Start angle = %d\n", current_angle);
    while (true)
    {
        get_sonar_value(&sonar_value);
        get_gyro_value(&current_angle);
        move_inline_smooth(goal_angle, SPEED);
        if (nb_turns % 2 == 0)
        {
            if (sonar_value < SONAR_THRESHOLD)
            {
                // Check again in 500 ms (In case it's a moving target (e.g. adversary robot))
                stop(TACHO_HOLD);
                SLEEP(1000);
                if (sonar_value < SONAR_THRESHOLD)
                {
                    goal_angle -= 90;
                    turn_to_angle(goal_angle, 5);
                    nb_turns++;
                }
            }
        }
        else
        {
            if (!check_pressed())
                continue;
            SLEEP(200);
            recover();
            SLEEP(200);
            int old_angle = current_angle;
            get_gyro_value(&current_angle);
            goal_angle = current_angle;
            printf("Corrected %d with wall.\n", abs(current_angle - old_angle + 90));
            nb_turns++;
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

void keep_inline(int angle, int speed)
{
    int current_angle;
    get_gyro_value(&current_angle);
    // printf("Angle deviation: %d \n", abs(current_angle - angle));
    int deviation = abs(current_angle - angle);
    if (deviation > INLINE_THRESHOLD)
    {
        stop(TACHO_COAST);
        SLEEP(1000);
        turn_to_angle(angle, 3);
        SLEEP(1000);
        get_gyro_value(&current_angle);
        printf("Corrected %d with keep_inline.\n", abs(deviation - abs(current_angle - angle)));
        move(speed);
    }
}

void move_inline_smooth(int angle, int speed)
{
    int current_angle;
    get_gyro_value(&current_angle);
    // printf("Angle deviation: %d \n", abs(current_angle - angle));
    int deviation = abs(current_angle - angle);
    int reduced_speed = speed * (1 - (float) deviation / 90);
    // printf("Reduced speed = %d for a %d deviation", reduced_speed, deviation);
    if (current_angle > angle){
        move_separate(reduced_speed, speed);
    }
    else{
        move_separate(speed, reduced_speed);
    }
}
