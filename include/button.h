#ifndef BUTTON_H
#define BUTTON_H

#include "pin.h"
#include "typedefs.h"

typedef struct
{
	Pin* pin;
} Button;

Button* allocButton(u8 port, u8 pin);
void freeButton(Button** but);
u8 stateButton(Button* but);

#endif
