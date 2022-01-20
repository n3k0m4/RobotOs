#ifndef SENSORS_H
#define SENSORS_H

#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_sensor.h"

#define ACCIDENT_THRESHOLD 15
#define INLINE_THRESHOLD 10
#define GYRO_CAL_MODE "GYRO-CAL"
#define GYRO_ANG_MODE "GYRO-ANG"

void init_sensors();
void get_gyro_value(int *value_buf);
void get_sonar_value(int *value_buf);
int get_stable_sonar_value(int *value_buf);
void get_color_value(int *value_buf);
void get_color_value2(int *value_buf1, int *value_buf2, int *value_buf3);
bool check_pressed();
bool detect_accident(int previous_angle, int current_angle);
void calibrate_gyro();
#endif