/*!
\file time.h
\brief Operations to measure relative time.
\warning ISR have to be implement
*/

#ifndef TIME_H
#define TIME_H

#include "typedefs.h"
#include <avr/interrupt.h>

volatile u64 _time;///stores the actual timevalue in µs and is up to date until 200000000 days are over

/*!
\brief Sets the time.

\param[in] time time to set
*/
void setTime(u64 time);

/*!
\brief Gets the time.

\pre time was set

\return actual time
*/
u64 getTime();

/*!
\brief Enables timecounting in 100µs steps.

\pre time was set to a value (in general 0)
*/
void enableTime();

/*!
\brief Disables timecounting.
*/
void disableTime();

#endif
