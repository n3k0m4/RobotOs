#ifndef STRATS_H
#define STRATS_H

#include "movement.h"
#include "sensors.h"
#include "utils.h"

bool recover_accident(int previous_angle, int current_angle);
void against_time();
void detect_obstacle();

#endif
