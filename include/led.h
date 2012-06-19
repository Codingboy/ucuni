#ifndef LED_H
#define LED_H

#include "pin.h"
#include "typedefs.h"

typedef struct
{
	Pin* pin;
} Led;

Led* allocLed(u8 port, u8 pin);
void freeLed(Led** led);
void onLed(Led* led);
void offLed(Led* led);
u8 stateLed(Led* led);

#endif
