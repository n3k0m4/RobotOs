#include "sensors.h"

uint8_t sn_gyro;
uint8_t sn_touch;

void init_sensors()
{
    ev3_sensor_init();
    ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0);
    ev3_search_sensor(LEGO_EV3_TOUCH, &sn_touch, 0);
}

void get_gyro_value(int *value_buf){
    get_sensor_value(0, sn_gyro, value_buf);
}

static bool _check_pressed()
{
	int val;
	return (get_sensor_value(0,sn_touch,&val)&&(val!=0));
}


