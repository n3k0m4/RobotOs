#include "sensors.h"

uint8_t sn_gyro;

void init_sensors()
{
    ev3_sensor_init();
    ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0);
}

void get_gyro_value(int *value_buf){
    get_sensor_value(0, sn_gyro, value_buf);
}