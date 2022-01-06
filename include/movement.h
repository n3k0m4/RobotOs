#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

#define PORT_LEFT OUTPUT_B
#define PORT_RIGHT OUTPUT_C

void init_movement();
void move(int speed);
void stop(uint8_t command);
void turn_left_motor(int speed);
void turn_right_motor(int speed);

#endif