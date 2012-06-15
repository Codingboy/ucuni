#ifndef TIME_H
#define TIME_H

#include "typedefs.h"
#include <avr/interrupt.h>

volatile u64 _time;

void setTime(u64 time);
u64 getTime();
void enableTime();
void disableTime();

#endif
