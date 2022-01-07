#ifndef SENSORS_H
#define SENSORS_H

#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_sensor.h"

void init_sensors();
void get_gyro_value(int *value_buf);
#endif