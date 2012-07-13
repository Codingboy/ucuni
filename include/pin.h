/*!
\file pin.h
\brief Abstraction from the hardware ports and pins.
*/

#ifndef PIN_H
#define PIN_H

#include "typedefs.h"
#include <stdbool.h>

/*!
\brief Stores a port and a pin.
*/
typedef struct
{
	u8 port;///port
	u8 pin;///pin
} Pin;

/*!
\brief Allocates memory for a Pin.

\param[in] port PORT to select
\param[in] pin PIN of a PORT to select

\pre port >= 1
\pre port < 6
\pre pin >= 0
\pre pin < 8
\warning Not all pins are supported for each port! For details watch the atmega32u4 manual. If a non existing pin is selected the behaviour of this function is not defined.

\return SUCCESS: allocated Pin
\return Error: 0
*/
Pin* allocPin(u8 port, u8 pin);

/*!
\brief Frees allocated memory of a Pin.

\param[in] pin pin to be freed; pointer to a pointer pin

\pre pin is alloced

\post pin is freed

\mod pin
*/
void freePin(Pin** pin);

/*!
\brief Sets a PIN as input.

\param[in] pin PIN to set as input
*/
void setAsInputPin(Pin* pin);

/*!
\brief Sets a PIN as output.

\param[in] pin PIN to set as output
*/
void setAsOutputPin(Pin* pin);

/*!
\brief Sets a PIN.

\param[in] pin PIN to be set
*/
void setOutputPin(Pin* pin);

/*!
\brief Clears a PIN.

\param[in] pin PIN to be cleared
*/
void clearOutputPin(Pin* pin);

/*!
\brief Toggles a PIN.

\param[in] pin PIN to be toggled
*/
void toggleOutputPin(Pin* pin);

/*
\brief Gets the state of a PIN.

\param[in] pin PIN to get state from

\pre pin shall be specified as input

\return true if high
\return false if low
*/
bool getInputPin(Pin* pin);

/*!
\brief Gets the state of a PIN.

\pre pin shall be specified as output

\return true if high
\return false if low
*/
bool getOutputPin(Pin* pin);


#endif
