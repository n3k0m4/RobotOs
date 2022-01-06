#include "stdlib.h"
#include "movement.h"

uint8_t motor_left;
uint8_t motor_right;
int max_speed;

static void _run_motor_forever(uint8_t motor_sn, int speed)
{
    set_tacho_speed_sp(motor_sn, speed);
    set_tacho_command_inx(motor_sn, TACHO_RUN_FOREVER);
}

static void _stop_motor(uint8_t motor_sn, uint8_t command)
{
    set_tacho_stop_action_inx(motor_sn, command);
    set_tacho_command_inx(motor_sn, TACHO_STOP);
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

// Init the motors and set max_speed
void init_movement()
{
    int max_speed_left;
    int max_speed_right;

    while (ev3_tacho_init() < 1)
        sleep(1);

    ev3_search_tacho_plugged_in(PORT_LEFT, 0, &motor_left, 0);
    ev3_search_tacho_plugged_in(PORT_RIGHT, 0, &motor_right, 0);
    get_tacho_max_speed(motor_left, &max_speed_left);
    get_tacho_max_speed(motor_right, &max_speed_right);
    max_speed = max_speed_right > max_speed_left ? max_speed_left : max_speed_right;
}

// Can be called with speed = 0 to use max_speed
void move(int speed)
{
    speed = _validate_speed(speed);
    if (speed == 0)
        return;
    _run_motor_forever(motor_right, speed);
    _run_motor_forever(motor_left, speed);
}

void stop(uint8_t command)
{
    _stop_motor(motor_right, command);
    _stop_motor(motor_left, command);
}

// Move only the left motor
// Can be called with speed = 0 to use max_speed
void turn_left_motor(int speed)
{
    speed = _validate_speed(speed);
    if (speed == 0)
        return;
    _stop_motor(motor_right, TACHO_COAST);
    _run_motor_forever(motor_left, speed);
}

// Move only the right motor
// Can be called with speed = 0 to use max_speed
void turn_right_motor(int speed)
{
    speed = _validate_speed(speed);
    if (speed == 0)
        return;
    _stop_motor(motor_left, TACHO_COAST);
    _run_motor_forever(motor_right, speed);
}
