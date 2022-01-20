#include "sensors.h"
#include "utils.h"
#include "stdlib.h"

uint8_t sn_gyro;
uint8_t sn_sonar;
uint8_t sn_color;
uint8_t sn_touch;

void init_sensors()
{
    ev3_sensor_init();
    ev3_search_sensor(LEGO_EV3_GYRO, &sn_gyro, 0);
    ev3_search_sensor(LEGO_EV3_US, &sn_sonar, 0);
    ev3_search_sensor(LEGO_EV3_COLOR, &sn_color, 0);
    ev3_search_sensor(LEGO_EV3_TOUCH, &sn_touch, 0);
}

// Value in (0-2550) cm
void get_gyro_value(int *value_buf)
{
    get_sensor_value(0, sn_gyro, value_buf);
}

// Value in (0-2550) cm
void get_sonar_value(int *value_buf)
{
    get_sensor_value(0, sn_sonar, value_buf);
}

// Value in (0-100)
void get_color_value(int *value_buf)
{
    get_sensor_value(0, sn_color, value_buf);
}

bool check_pressed()
{
    int val;
    return (get_sensor_value(0, sn_touch, &val) && (val != 0));
}

bool detect_accident(int previous_angle, int current_angle)
{
    if (abs(previous_angle - current_angle) > ACCIDENT_THRESHOLD)
    {
        //last_gyro_val = curr_gyro_val; Use last_gyro_val to re-calibrate
        return true;
    }
    return false;
}

int get_stable_sonar_value(int *value_buf){
    int previous_sonar_value;
    get_sonar_value(&previous_sonar_value);
    const int ERROR_THRESHOLD = 20; // 2cm
    int sonar_value;
    int nb_constant_measures = 0;
    while (nb_constant_measures < 2){
        SLEEP(500);
        get_sonar_value(&sonar_value);
        printf("Getting stable sonar value ...\n");
        if (abs(previous_sonar_value - sonar_value) < ERROR_THRESHOLD) nb_constant_measures++;
        else nb_constant_measures = 0;
        previous_sonar_value = sonar_value;
    }
    *value_buf = sonar_value;
}