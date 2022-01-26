#ifndef STRATS_H
#define STRATS_H

#include "movement.h"
#include "sensors.h"
#include "utils.h"

void against_time();
void detect_obstacle();
bool _is_obstacle(int last_turn_position, int threshold);
void against_cars();
#endif
