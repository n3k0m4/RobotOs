#include <movement.h>
#include <utils.h>

static void move(uint8_t speed, float distance){
    uint8_t leftMotor = getLeftMotor();
    uint8_t rightMotor = getRightMotor();
    set_tacho_speed_sp(leftMotor, 1500* 2 / 3);
    set_tacho_speed_sp(rightMotor, 1500* 2 / 3);
    Sleep(10);    
    exit(0);
}

static void turnLeft(float angle){

}

static void turnRight(float angle){
    
}