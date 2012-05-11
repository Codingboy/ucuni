#ifndef PIN_H
#define PIN_H

#include "typedefs.h"

/*!
\brief Stores a port and a pin.
*/
typedef struct
{
	u8 port;
	u8 pin;
} Pin;

Pin* allocPin(u8 port, u8 pin);
void freePin(Pin** pin);

void setAsInputPin(Pin* pin);
void setAsOutputPin(Pin* pin);
void setOutputPin(Pin* pin);
void clearOutputPin(Pin* pin);
void toggleOutputPin(Pin* pin);
u8 getInputPin(Pin* pin);
u8 getInputADCPin(Pin* pin);
u8 getOutputPin(Pin* pin);

#endif
