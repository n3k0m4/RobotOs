#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"

/** @file */ 

#define PORT_LEFT OUTPUT_B ///< Left motor port
#define PORT_RIGHT OUTPUT_C ///< Right motor port
#define PORT_OBSTACLE OUTPUT_A  ///< Front "obstacle" motor port
#define ANGLE_THRESHOLD 5 ///< Gyro angle precision

/**
 * Detects the motors and sets their serial numbers. 
 * 
 * Computes the max speed attainable by both left and right motors. 
 */
void init_movement();

/**
 * Run both left and right motors at given \p speed.
 * 
 * @param speed Speed to use for motors
 */
void move(int speed);

/**
 *  Move in the direction of \p angle_to_keep at given \p speed
 * 
 * @param angle_to_keep Angle that robot should keep
 * @param speed Speed to use for motors
 */
void move_keeping_angle(int angle_to_keep, int speed);

/**
 * Stop left and right motors with the given \p command
 * 
 * @param command Can be any of the enum values : TACHO_COAST, TACHO_BRAKE, TACHO_HOLD
 *  found in ev3_tacho.h 
 */
void stop(uint8_t command);

/**
 * Release obstacle using front motor
 */
void release_obstacle();

/**
 * Turn to \p destination_angle with an error less than treshold \p thres
 * 
 * @param destination_angle Destination angle
 * @param thres Error threshold
 */
void turn_to_angle(int destination_angle, int thres);

/**
 * Get left motor position in degrees.
 * 
 * @param[out] position Buffer for result
 */
void get_left_motor_position(int *position);

#endif
