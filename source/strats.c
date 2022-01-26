#include "strats.h"
#include <stdlib.h>
#include <time.h>

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
    int current_angle; // TODO: Remove useless var
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

bool _is_obstacle(int last_turn_motor_position, int threshold)
{
    int curr_position;
    const double WHEEL_RADIUS = 5.6 / 2;
    get_left_motor_position(&curr_position);
    return (curr_position - last_turn_motor_position) * 2 * PI * WHEEL_RADIUS / 360 < threshold;
}

void _avoid_obstacle(int angle_to_keep, int sonar_threshold, int speed)
{
    const int TIME_THRESHOLD = 1; // TODO: Rename ?
    int left_sonar_value, right_sonar_value, sonar_value;
    turn_to_angle(angle_to_keep + 90, 5);
    get_stable_sonar_value(&right_sonar_value);
    turn_to_angle(angle_to_keep - 90, 5);
    get_stable_sonar_value(&left_sonar_value);
    int orthogonal_angle = angle_to_keep - 90;
    if (right_sonar_value > left_sonar_value)
        orthogonal_angle = angle_to_keep + 90;

    time_t mov_start_time = time(NULL);
    turn_to_angle(orthogonal_angle, 5);
    bool obstacle_avoided = false;
    time_t obstacle_avoided_time;
    while (1)
    {
        move_keeping_angle(orthogonal_angle, speed);
        if (!obstacle_avoided && difftime(time(NULL), mov_start_time) >= TIME_THRESHOLD)
        {
            turn_to_angle(angle_to_keep, 5);
            get_stable_sonar_value(&sonar_value);
            if (sonar_value >= sonar_threshold)
            {
                // Obstacle avoided. Wait a little before continuing because
                // Robot's width might still touch obstacle.
                obstacle_avoided = true;
                obstacle_avoided_time = time(NULL);
            }
            turn_to_angle(orthogonal_angle, 5);
            mov_start_time = time(NULL);
        }
        if (obstacle_avoided && difftime(time(NULL), obstacle_avoided_time) >= 2 * TIME_THRESHOLD){
            turn_to_angle(angle_to_keep, 5);
            return;
        }
        get_sonar_value(&sonar_value);
        if (sonar_value < sonar_threshold)
        {
            stop(TACHO_COAST);
            get_stable_sonar_value(&sonar_value);
            if (sonar_value < sonar_threshold)
            { // No more room for checking on this direction
                turn_to_angle(angle_to_keep, 5);
                get_stable_sonar_value(&sonar_value);
                if (sonar_value >= sonar_threshold)
                    return; // Obstacle avoided
                else
                {
                    // Couldn't find a way to avoid obstacle on this direction
                    // Check on the other direction
                    _avoid_obstacle(angle_to_keep, sonar_threshold, speed);
                    return;
                }
            }
        }
    }
}

bool _is_obstacle_in_turn(int nb_turns, int last_turn_motor_position)
{
    const int FIRST_TURN_THRESHOLD = 300;
    const int LONG_PART_THRESHOLD = 800;
    const int SHORT_PART_THRESHOLD = 600;
    if (nb_turns == 0)
    {
        return _is_obstacle(last_turn_motor_position, FIRST_TURN_THRESHOLD);
    }
    else if (MODULO(nb_turns, 2) == 0)
    {
        return _is_obstacle(last_turn_motor_position, LONG_PART_THRESHOLD);
    }
    else
    {
        return _is_obstacle(last_turn_motor_position, SHORT_PART_THRESHOLD);
    }
}

void against_cars()
{
    const int SPEED = 800; // DO NOT SET TO 0
    const int SONAR_THRESHOLD = 20 * 10;
    int nb_turns = 0;
    int sonar_value;
    int angle_to_keep;
    int left_motor_pos;
    get_left_motor_position(&left_motor_pos);
    get_gyro_value(&angle_to_keep);
    while (1)
    {
        move_keeping_angle(angle_to_keep, SPEED);
        // move(SPEED);
        get_sonar_value(&sonar_value);
        if (sonar_value < SONAR_THRESHOLD)
        {
            printf("Found something. \n");
            stop(TACHO_COAST);
            get_stable_sonar_value(&sonar_value);
            if (sonar_value >= SONAR_THRESHOLD)
            {
                printf("False alarm. \n");
                continue;
            }
            if (_is_obstacle_in_turn(nb_turns, left_motor_pos))
            {
                printf("It's an obstacle. \n");
                _avoid_obstacle(angle_to_keep, 10 * 10, 400);
            }
            else if (nb_turns % 2 == 0)
            {
                angle_to_keep -= 90;
                turn_to_angle(angle_to_keep, 5);
                nb_turns++;
                get_left_motor_position(&left_motor_pos);
            }
        }
        else
        {
            if (!check_pressed())
                continue;
            SLEEP(200);
            recover();
            SLEEP(200);

            calibrate_gyro();
            get_gyro_value(&angle_to_keep);
            get_left_motor_position(&left_motor_pos);
            nb_turns++;
        }
    }
}
