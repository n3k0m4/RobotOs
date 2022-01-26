#ifndef SENSORS_H
#define SENSORS_H

#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_sensor.h"

#define ACCIDENT_THRESHOLD 15    ///< Angular difference threshold to detect accident
#define INLINE_THRESHOLD 10      ///< Agular differece to keep the robot's movement inline
#define GYRO_CAL_MODE "GYRO-CAL" ///< Gyroscope's calibration mode
#define GYRO_ANG_MODE "GYRO-ANG" ///< Gyroscope's angular mode

/**
 * Detects the sensors and sets their serial numbers. 
 * 
 * The sensors initiated are Gyroscope, sonar, and Touch sensor 
 */
void init_sensors();

/**
 * Get gyroscope value in degrees.
 * 
 * @param[out] value_buf Buffer for result
 */
void get_gyro_value(int *value_buf);

/**
 * Get distance from sonar in cm
 * 
 * @param[out] value_buf Buffer for result
 */
void get_sonar_value(int *value_buf);

/**
 * Get distance stable from sonar. This method stops the robot and compare consecutive values with a threshold
 * 
 * @param[out] value_buf Buffer for result
 */
void get_stable_sonar_value(int *value_buf);

/**
 * Checks if the touch sensor is pushed
 * 
 * @return boolean valus depending on the state of the sensor 
 */
bool check_pressed();

/**
 * Checks if the touch sensor is pushed
 * @param[in] previous_angle The original angle of the robot before the accident
 * @param[in] current_angle The current angle of the robot before the accident
 * @return boolean valus depending on the state of the sensor 
 */
bool detect_accident(int previous_angle, int current_angle);

/**
 * Util function to calibrate the gyroscope by swiping to GYRO_CAL_MODE and back to GYRO_ANG_MODE 
 * 
 */
void calibrate_gyro();
#endif