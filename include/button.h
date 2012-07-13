/*!
\file button.h
\brief Operations to control buttons
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "pin.h"
#include "typedefs.h"
#include <stdbool.h>

typedef struct
{
	Pin* pin;
} Button;

/*!
\brief Allocates a button.

\param[in] port port
\param[in] pin pin

\return allocated button
*/
Button* allocButton(u8 port, u8 pin);

/*!
\brief Frees a button.

\param[in] but button to be freed

\pre but is alloced
*/
void freeButton(Button** but);

/*!
\brief Gets the state of the button if it is pressed.

\return true if pressed
\return false if not pressed
*/
bool stateButton(Button* but);

#endif
