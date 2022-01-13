#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

#define SLEEP(msec) usleep((msec)*1000)

#define MODULO(A, B) ((A % B) + B) % B

#endif