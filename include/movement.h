#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

#define port_left OUTPUT_B
#define port_right OUTPUT_C


void init();
void finished();
int moveForward(int speed);
int moveBackward(int speed);
int turnRightMotor(int speed);
int turnLeftMotor(int speed);
int stop();
int softStop();


#endif