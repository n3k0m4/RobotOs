#include "sensors.h"

uint8_t sn_gyro;
uint8_t sn_sonar;
uint8_t sn_color;

void init_sensors()
{
    ev3_sensor_init();
    ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0);
    ev3_search_sensor(LEGO_EV3_US, &sn_sonar, 0);
    ev3_search_sensor(LEGO_EV3_COLOR, &sn_color, 0);
}

void get_gyro_value(int *value_buf){
    get_sensor_value(0, sn_gyro, value_buf);
}

// Value in (0-2550) cm
void get_sonar_value(int *value_buf){
    get_sensor_value(0, sn_sonar, value_buf);
}

// Value in (0-100)
void get_color_value(int *value_buf){
    get_sensor_value(0, sn_color, value_buf);
}