#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

#include <unistd.h>
#define Sleep(msec) usleep((msec)*1000)


static void move(uint8_t speed, float distance);
static void turnLeft(float angle);
static void turnRight(float angle);



#endif