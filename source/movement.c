#include "stdlib.h"
#include "movement.h"
#include "sensors.h"
#include "utils.h"

uint8_t sn_motor_left;
uint8_t sn_motor_right;
int max_speed;
uint8_t sn_obstacle;

// Init the motors and set max_speed
void init_movement()
{
    int max_speed_left;
    int max_speed_right;

    while (ev3_tacho_init() < 1)
        sleep(1);

    ev3_search_tacho_plugged_in(PORT_LEFT, 0, &sn_motor_left, 0);
    ev3_search_tacho_plugged_in(PORT_RIGHT, 0, &sn_motor_right, 0);
    get_tacho_max_speed(sn_motor_left, &max_speed_left);
    get_tacho_max_speed(sn_motor_right, &max_speed_right);
    max_speed = max_speed_right > max_speed_left ? max_speed_left : max_speed_right;
    ev3_search_tacho_plugged_in(PORT_OBSTACLE, 0, &sn_obstacle, 0);
}

// Can call with speed = 0 to use max_speed without knowing its value
// If speed is not valid, 0 is returned
static int _validate_speed(int speed)
{
    if (speed == 0)
        speed = max_speed;
    if (abs(speed) > max_speed)
    {
        printf("Speed %d cannot be bigger than max_speed %d", speed, max_speed);
        return 0;
    }
    return speed;
}

//TODO: Same arguments order everywhere
static void _run_motor_forever(uint8_t sn_motor, int speed)
{
    set_tacho_speed_sp(sn_motor, speed);
    set_tacho_command_inx(sn_motor, TACHO_RUN_FOREVER);
}

// Can be called with speed = 0 to use max_speed
void move(int speed)
{
    speed = _validate_speed(speed);
    if (speed == 0)
        return;
    _run_motor_forever(sn_motor_right, speed);
    _run_motor_forever(sn_motor_left, speed);
}

void enforce_move_angle_smooth(int angle, int speed)
{
    int current_angle;
    get_gyro_value(&current_angle);
    // printf("Angle deviation: %d \n", abs(current_angle - angle));
    int deviation = abs(current_angle - angle);
    printf("deviation = %d, current_angle = %d, angle_to_keep = %d\n", deviation,current_angle,angle);
    int reduced_speed = speed * (1 - (float) deviation / 90);
    reduced_speed = _validate_speed(reduced_speed);
    // printf("Reduced speed = %d for a %d deviation", reduced_speed, deviation);
    if (current_angle > angle){
        _run_motor_forever(sn_motor_right, speed);
        _run_motor_forever(sn_motor_left, reduced_speed);
    }
    else{
        _run_motor_forever(sn_motor_right, reduced_speed);
        _run_motor_forever(sn_motor_left, speed);
    }
}

static void _stop_motor(uint8_t sn_motor, uint8_t command)
{
    set_tacho_stop_action_inx(sn_motor, command);
    set_tacho_command_inx(sn_motor, TACHO_STOP);
}

void stop(uint8_t command)
{
    _stop_motor(sn_motor_right, command);
    _stop_motor(sn_motor_left, command);
}

static void _run_motor_only(int speed, uint8_t sn_motor)
{
    speed = _validate_speed(speed);
    if (speed == 0)
        return;
    uint8_t sn_other_motor = sn_motor == sn_motor_right ? sn_motor_left : sn_motor_right;
    _stop_motor(sn_other_motor, TACHO_COAST);
    _run_motor_forever(sn_motor, speed);
}

// Can be called with speed = 0 to use max_speed
void run_left_motor_only(int speed)
{
    _run_motor_only(speed, sn_motor_left);
}

// Can be called with speed = 0 to use max_speed
void run_right_motor_only(int speed)
{
    _run_motor_only(speed, sn_motor_right);
}

static void _turn_90d(int speed, uint8_t sn_motor)
{
    int start_angle;
    get_gyro_value(&start_angle);
    int current_angle = start_angle;

    _run_motor_only(speed, sn_motor);
    while (abs(abs(start_angle - current_angle) % 360 - 90) > ANGLE_THRESHOLD)
    {
        get_gyro_value(&start_angle);
        // printf("gyro angle: %d \n", abs(start_angle - current_angle) % 360);
    }
    _stop_motor(sn_motor, TACHO_HOLD);
}

void turn_90d_left(int speed)
{
    _turn_90d(speed, sn_motor_right);
}

void turn_90d_right(int speed)
{
    _turn_90d(speed, sn_motor_left);
}

void release_obstacle()
{
    _run_motor_forever(sn_obstacle, 450);
    sleep(1);
    _stop_motor(sn_obstacle, TACHO_COAST);
    _run_motor_forever(sn_obstacle, -450);
    sleep(1);
    _stop_motor(sn_obstacle, TACHO_COAST);
    printf("*** Obstacle released ***\n");
}

void turn_to_angle(int destination_angle, int thres)
{
    stop(TACHO_COAST);
    int current_angle;
    get_gyro_value(&current_angle);
    int angle_to_turn = (destination_angle - current_angle) % 360;
    uint8_t sn_motor = angle_to_turn >= 0 ? sn_motor_left : sn_motor_right;
    uint8_t sn_other_motor = sn_motor == sn_motor_right ? sn_motor_left : sn_motor_right;
    int speed = max_speed;
    _run_motor_forever(sn_motor, speed);
    _run_motor_forever(sn_other_motor, -speed);
    while (abs(angle_to_turn) > thres)
    {
        get_gyro_value(&current_angle);
        angle_to_turn = (destination_angle - current_angle) % 360;
        speed = (int)(max_speed * ((float)abs(angle_to_turn) / 180));
        _run_motor_forever(sn_motor, speed);
        _run_motor_forever(sn_other_motor, -speed);
    }
    stop(TACHO_HOLD);
}

void print_motor_speeds()
{
    int left;
    int right;
    get_tacho_speed(sn_motor_right, &left);
    get_tacho_speed(sn_motor_left, &right);
    printf("%d %d\n", left, right);
}
/*
uint8_t get_left_motor()
{
    return sn_motor_left;
}
uint8_t get_right_motor()
{
    return sn_motor_right;
}
*/
void get_left_motor_position(int *position)
{
    get_tacho_position(sn_motor_left, position);
}
void get_right_motor_position(int *position)
{
    get_tacho_position(sn_motor_right, position);
}
// void _test(){
//     int a; int b;
//     get_tacho_max_speed(sn_motor_right, &a);
//     get_tacho_max_speed(sn_motor_left, &b);
//     printf("%d, %d\n", a, b);
//     while(true){
//         get_tacho_speed(sn_motor_right, &a);
//         get_tacho_speed(sn_motor_left, &b);
//         printf("%d, %d\n", a, b);
//     }
// }
