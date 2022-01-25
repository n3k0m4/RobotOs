#ifndef STRATS_H
#define STRATS_H

#include "movement.h"
#include "sensors.h"
#include "utils.h"
bool recover_accident(int previous_angle, int current_angle);
void against_time();
void keep_inline(int angle, int speed);
void move_inline_smooth(int angle, int speed);
void count_distance();
void detect_obstacle();
bool _is_obstacle(int last_turn_position, int threshold);
void against_cars();

#endif
