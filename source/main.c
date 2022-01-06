/*
     ____ __     ____   ___    ____ __         (((((()
    | |_  \ \  /   ) ) | |  ) | |_  \ \  /  \(@)- /
    |_|__  \_\/  __)_) |_|_/  |_|__  \_\/   /(@)- \
                                               ((())))
 */
/**
 *  \file  tacho.c
 *  \brief  ev3dev-c Tacho Motors example.
 *  \author  Vitaly Kravtsov (in4lio@gmail.com)
 *  \copyright  See the LICENSE file.
 */

#include <stdio.h>
#include "utils.h"
#include "movement.h"


int main(void)
{
    int i;
    uint8_t sn;
    FLAGS_T state;
    char s[256];
    printf("*** ( EV3 ) Hello! ***\n");
    init();
    moveForward(0);
    sleep(10);
    stop();
    ev3_uninit();
    printf("*** ( EV3 ) Bye! ***\n");

    return (0);
}
