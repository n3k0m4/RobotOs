#include "movement.h"
uint8_t m_left;
uint8_t m_right;
int max_speed;
int max_speed_left;
int max_speed_right;

// Init the motors and set max_speed
void init() {
    while (ev3_tacho_init() < 1)
        sleep(1000);
    ev3_search_tacho_plugged_in(port_left, 0, &m_left, 0);
    ev3_search_tacho_plugged_in(port_right, 0, &m_right, 0);
    get_tacho_max_speed(m_left, &max_speed_left);
    get_tacho_max_speed(m_right, &max_speed_right);
    max_speed = max_speed_right > max_speed_left ? max_speed_left : max_speed_right;
}
// Work done, Stop motors
void finished() {
    set_tacho_stop_action_inx(m_left, TACHO_COAST);
    set_tacho_stop_action_inx(m_right, TACHO_COAST);
    set_tacho_command_inx(m_left, TACHO_STOP);
    set_tacho_command_inx(m_right, TACHO_STOP);
}

// Zoom forward with a speed
int moveForward(int speed) {
    // speed 0 just to able to launch with max_speed without knowing it
    if (speed == 0 || speed > max_speed)
        speed = max_speed;
    set_tacho_speed_sp(m_left, speed);
    set_tacho_speed_sp(m_right, speed);
    set_tacho_command_inx(m_left, TACHO_RUN_FOREVER);
    set_tacho_command_inx(m_right, TACHO_RUN_FOREVER);
    return 0;
}

// Move only the left motor
int turnLeftMotor(int speed) {
    if (speed == 0 || speed > max_speed)
        speed = max_speed;
    set_tacho_speed_sp(m_left, speed);
    set_tacho_stop_action_inx(m_right, TACHO_COAST);
    set_tacho_command_inx(m_right, TACHO_STOP);
    set_tacho_command_inx(m_left, TACHO_RUN_FOREVER);
    return 0;
}

// Move only the right motor
int turnRightMotor(int speed) {
    if (speed == 0 || speed > max_speed)
        speed = max_speed;
    set_tacho_speed_sp(m_left, speed);
    set_tacho_stop_action_inx(m_left, TACHO_COAST);
    set_tacho_command_inx(m_left, TACHO_STOP);
    set_tacho_command_inx(m_right, TACHO_RUN_FOREVER);
    return 0;
}

// Move backward
int moveBackward(int speed) {
    if (speed == 0 || speed > max_speed)
        speed = max_speed;
    speed = -speed;
    set_tacho_speed_sp(m_left, speed);
    set_tacho_speed_sp(m_right, speed);
    set_tacho_command_inx(m_left, TACHO_RUN_FOREVER);
    set_tacho_command_inx(m_right, TACHO_RUN_FOREVER);
    return 0;
}

// Stop with hard brakes, effective to halt and recover 
int stop() {
    set_tacho_stop_action_inx(m_left, TACHO_HOLD);
    set_tacho_stop_action_inx(m_right, TACHO_HOLD);
    set_tacho_command_inx(m_left, TACHO_STOP);
    set_tacho_command_inx(m_right, TACHO_STOP);
    return 0;
}

// Stop with soft brakes
int softStop(){
    set_tacho_stop_action_inx(m_left, TACHO_BRAKE);
    set_tacho_stop_action_inx(m_right, TACHO_BRAKE);
    set_tacho_command_inx(m_left, TACHO_STOP);
    set_tacho_command_inx(m_right, TACHO_STOP);
    return 0;
}