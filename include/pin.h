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

/*!
\brief Allocates memory for a Pin.

\param[in] port PORT to select (0 1 2 3) -> (A B C D)
\param[in] pin PIN of a PORT to select (0 up to 7)

\return SUCCESS: allocated pin
\return Error: 0
*/
Pin* allocPin(u8 port, u8 pin);

/*!
\brief Frees allocated memory of a Pin.

\param pin pin to be freed; pointer to a pointer pin
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

\return state of the PIN
*/
u8 getInputPin(Pin* pin);

/*!
///\todo
*/
u8 getInputADCPin(Pin* pin);

/*!
\brief Gets the state of a PIN.

\pre pin shall be specified as output

\return state of the PIN
*/
u8 getOutputPin(Pin* pin);

#endif
