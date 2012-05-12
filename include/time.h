#ifndef TIME_H
#define TIME_H

#include "typedefs.h"
#include <avr/interrupt.h>

///stores time in µs
volatile u64 _time;

/*!
\brief Sets the time to a value.

\param[in] time value to set time to in µs
*/
void setTime(u64 time);

/*!
\brief Gets the time in µs.

\return time in µs
*/
u64 getTime();

/*!
\brief Enables the timecounting.

\pre _time shall be set to a value.
*/
void enableTime();

/*!
\brief Disables the timecounting.
*/
void disableTime();

#endif
