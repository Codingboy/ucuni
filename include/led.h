/*!
\file led.h
\brief Operations to control leds
*/

#ifndef LED_H
#define LED_H

#include "pin.h"
#include "typedefs.h"

typedef struct
{
	Pin* pin;///pin on which led is connected
} Led;

/*!
\brief Allocates an led.

\param[in] port port
\param[in] pin pin

\post led is off

\return allocated led
*/
Led* allocLed(u8 port, u8 pin);

/*!
\brief Frees an led.

\param[in] led led to be freed

\pre led is alloced
*/
void freeLed(Led** led);

/*!
\brief Switches an led on.

\param[in] led led

\post led is on
*/
void onLed(Led* led);

/*!
\brief Switches an led off.

\param[in] led

\post led is off
*/
void offLed(Led* led);

/*!
\brief Gets the state of the led.

\param[in] led led

\return 1 if led is on
\reeturn 0 if led is off
*/
u8 stateLed(Led* led);

///\todo
void toggleLed(Led* led);
#endif
