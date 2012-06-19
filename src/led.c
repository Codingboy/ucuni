#include "led.h"
#include "pin.h"
#include "typedefs.h"
#include <stdlib.h>

Led* allocLed(u8 port, u8 pin)
{
	Led* led = (Led*) malloc(sizeof(Led));
	if (led == NULL)
	{
		return NULL;
	}
	led->pin = allocPin(port, pin);
	if (led->pin == NULL)
	{
		free(led);
		led = NULL;
		return NULL;
	}
	setAsOutputPin(led->pin);
	return led;
}

void freeLed(Led** led)
{
	freePin(&((*led)->pin));
	free(*led);
	*led = NULL;
}

void onLed(Led* led)
{
	setOutputPin(led->pin);
}

void offLed(Led* led)
{
	clearOutputPin(led->pin);
}

u8 stateLed(Led* led)
{
	return getOutputPin(led->pin);
}
