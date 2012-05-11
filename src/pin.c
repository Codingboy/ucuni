#include "pin.h"
#include "typedefs.h"
#include "pinoperations.h"
#include <stdlib.h>
#include <avr/io.h>

Pin* allocPin(u8 port, u8 pin)
{
	Pin* p = (Pin*) malloc(sizeof(Pin));
	if (p == 0)
	{
		return 0;
	}
	p->port = port;
	p->pin = pin;
	return p;
}

void freePin(Pin** pin)
{
	free(*pin);
	*pin = 0;
}

void setAsInputPin(Pin* pin)
{
	setAsInput(pin->port, pin->pin);
}

void setAsOutputPin(Pin* pin)
{
	setAsOutput(pin->port, pin->pin);
}

void setOutputPin(Pin* pin)
{
	setOutput(pin->port, pin->pin);
}

void clearOutputPin(Pin* pin)
{
	clearOutput(pin->port, pin->pin);
}

void toggleOutputPin(Pin* pin)
{
	toggleOutput(pin->port, pin->pin);
}

u8 getInputPin(Pin* pin)
{
	return getInput(pin->port, pin->pin);
}

u8 getInputADCPin(Pin* pin)
{
	return getInputADC(pin->port, pin->pin);
}

u8 getOutputPin(Pin* pin)
{
	return getOutput(pin->port, pin->pin);
}
