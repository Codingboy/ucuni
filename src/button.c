#include "button.h"
#include "typedefs.h"
#include <stdlib.h>
#include "pin.h"

Button* allocButton(u8 port, u8 pin)
{
	Button* but = (Button*) malloc(sizeof(Button));
	if (but == NULL)
	{
		return NULL;
	}
	but->pin = allocPin(port, pin);
	if (but->pin == NULL)
	{
		free(but);
		but = NULL;
		return NULL;
	}
	setAsInputPin(but->pin);
	return but;
}

void freeButton(Button** but)
{
	freePin(&((*but)->pin));
	free(*but);
	*but = NULL;
}

u8 stateButton(Button* but)
{
	return !getInputPin(but->pin);
}
