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

void init_movement();
void move(int speed);
void stop(uint8_t command);
void run_right_motor(int speed);
void run_left_motor(int speed);
void turn_90d_right(int speed);
void turn_90d_left(int speed);
void release_obstacle();
int turn_to_angle(int destination_angle, int thres);
// void _test();

#endif
