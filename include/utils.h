#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include <string.h>
#define leftMotor "outB"
#define rightMotor "outA"
static uint8_t i;

static uint8_t getLeftMotor(){
    if (ev3_tacho_init() < 1)
        printf("TACHO NOT INITIATED !");

    for (i = 0; i < DESC_LIMIT; i++)
    {
        if (ev3_tacho[i].type_inx != TACHO_TYPE__NONE_){
            if(strcmp(leftMotor, ev3_tacho_type(ev3_tacho[i].type_inx))==0)
        {
            printf("  index is = %d\n", i);
            return i;
        }
    }}
}
static uint8_t getRighttMotor(){
    
    if (ev3_tacho_init() < 1)
        printf("TACHO NOT INITIATED !");

    for (i = 0; i < DESC_LIMIT; i++)
    {
        if (ev3_tacho[i].type_inx != TACHO_TYPE__NONE_){
            if (strcmp(rightMotor, ev3_tacho_type(ev3_tacho[i].type_inx))==0)
        {
            printf("  index is = %d\n", i);
            return i;
        }
    }
}}
#endif