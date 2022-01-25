#include "strats.h"
#include <stdlib.h>

// Deprecated. Use move_keeping_angle
bool recover_accident(int previous_angle, int current_angle)
{

    if (detect_accident(previous_angle, current_angle))
    {
        stop(TACHO_COAST);
        turn_to_angle(previous_angle, ANGLE_THRESHOLD);
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

bool _is_a_non_moving_object_ahead(int previous_sonar_value, int threshold)
{
    int sonar_value;
    int nb_constant_measures = 0;
    while (nb_constant_measures < 3)
    {
        SLEEP(500);
        get_sonar_value(&sonar_value);
        if (previous_sonar_value == sonar_value)
            nb_constant_measures++;
        else
            nb_constant_measures = 0;
        previous_sonar_value = sonar_value;
    }
    return sonar_value <= threshold;
}

void against_time()
{
    const int SPEED = 800; // DO NOT SET TO 0
    const int SONAR_THRESHOLD = 20 * 10;
    int nb_turns = 0;
    int sonar_value;
    int current_angle;
    get_gyro_value(&current_angle);
    int angle_to_keep = current_angle;
    while (true)
    {
        get_sonar_value(&sonar_value);
        move_keeping_angle(angle_to_keep, SPEED);
        if (nb_turns % 2 == 0)
        {
            if (sonar_value < SONAR_THRESHOLD)
            {
                stop(TACHO_HOLD);
                get_stable_sonar_value(&sonar_value);
                if (sonar_value < SONAR_THRESHOLD)
                {
                    angle_to_keep -= 90;
                    turn_to_angle(angle_to_keep, 5);
                    nb_turns++;
                }
            }
        }
        else
        {
            // TODO: Have get_stable_sonar_value logic here too
            if (!check_pressed())
                continue;
            SLEEP(200);
            recover();
            SLEEP(200);

            calibrate_gyro();
            get_gyro_value(&current_angle);
            angle_to_keep = current_angle;
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

// Deprecated. Use move_keeping_angle
void _keep_inline(int angle, int speed)
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

bool _is_obstacle(int last_turn_position, int threshold)
{
    int curr_position;
    //int count_per_rot; no need for it as it is always 360 for motors
    get_left_motor_position(&curr_position);
    printf("the differnce is  %d", curr_position - last_turn_position);
    //printf("the difference is %d\n ", (double)(last_turn_position - curr_position) / 2 * PI * 360);
    if ((curr_position - last_turn_position) / 2 * PI * 360 > threshold)
        return true;
    return false;
}

void avoid_obstacle(int current_angle, int direction)
{
}
void against_cars()
{
    const int SPEED = 800; // DO NOT SET TO 0
    const int SONAR_THRESHOLD = 20 * 10;
    int nb_turns = 0;
    bool presence_obst = false;
    int sonar_value;
    int current_angle;
    int left_motor_ps;
    get_left_motor_position(&left_motor_ps);
    get_gyro_value(&current_angle);
    int angle_to_keep = current_angle;
    while (1)
    {
        get_sonar_value(&sonar_value);
        move_keeping_angle(angle_to_keep, SPEED);
        if (nb_turns == 0 && sonar_value < FIRST_TURN_THRESHOLD)
        {
            if (_is_obstacle(left_motor_ps, FIRST_TURN_THRESHOLD))
            {
                printf("ESCAPING OBTSACLE");
                avoid_obstacle(current_angle, -1);
            }
        }
        else if (MODULO(nb_turns, 2) == 0 && nb_turns != 0 && sonar_value < LONG_PART_THRESHOLD)
        {
            if (_is_obstacle(left_motor_ps, LONG_PART_THRESHOLD))
                avoid_obstacle(current_angle, -1);
        }
        else if (MODULO(nb_turns, 2) == 1 && sonar_value < SHORT_PART_THRESHOLD)
        {
            if (_is_obstacle(left_motor_ps, SHORT_PART_THRESHOLD))
                avoid_obstacle(current_angle, -1);
        }
        else
        {
        }

        if (nb_turns % 2 == 0)
        {
            if (sonar_value < SONAR_THRESHOLD)
            {
                stop(TACHO_HOLD);
                get_stable_sonar_value(&sonar_value);
                if (sonar_value < SONAR_THRESHOLD)
                {
                    angle_to_keep -= 90;
                    turn_to_angle(angle_to_keep, 5);
                    nb_turns++;
                }
            }
            get_left_motor_position(&left_motor_ps);
        }
        else
        {
            // TODO: Have get_stable_sonar_value logic here too
            if (!check_pressed())
                continue;
            SLEEP(200);
            recover();
            SLEEP(200);

            calibrate_gyro();
            get_gyro_value(&current_angle);
            angle_to_keep = current_angle;
            nb_turns++;
            get_left_motor_position(&left_motor_ps);
        }
    }
}
