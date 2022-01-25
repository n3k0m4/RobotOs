#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

#define PORT_LEFT OUTPUT_B
#define PORT_RIGHT OUTPUT_C
#define PORT_OBSTACLE OUTPUT_A
#define ANGLE_THRESHOLD 10
#define FIRST_TURN_THRESHOLD 300
#define LONG_PART_THRESHOLD 800
#define SHORT_PART_THRESHOLD 600

void init_movement();
void move(int speed);
void move_keeping_angle(int angle, int speed);
void stop(uint8_t command);
void run_right_motor_only(int speed);
void run_left_motor_only(int speed);
void release_obstacle();
void turn_to_angle(int destination_angle, int thres);
void print_motor_speeds();

void get_left_motor_position(int *position);
void get_right_motor_position(int *position);
// void _test();

#endif
