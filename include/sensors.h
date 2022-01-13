#ifndef SENSORS_H
#define SENSORS_H

#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_sensor.h"

#define ACCIDENT_THRESHOLD 15

void init_sensors();
void get_gyro_value(int *value_buf);
void get_sonar_value(int *value_buf);
void get_color_value(int *value_buf);
void get_color_value2(int *value_buf1, int *value_buf2, int *value_buf3);
bool check_pressed();
bool detect_accident();
#endif