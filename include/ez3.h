/*!
\file ez3.h
\brief Operations to control the ultrasonic sensor EZ3
*/

#ifndef EZ3_H
#define EZ3_H

#include <pin.h>
#include "typedefs.h"

typedef struct
{
	//Pin* tx;
	Pin* rx;///signal
	Pin* an;///analog
	Pin* pw;//pwm
	//Pin* bw;
} EZ3;

/*!
\brief Allocates the memory the ez3 needs.

\pre anPort = 5
\pre anPin = 0

\return ez3 instance
*/
EZ3* allocEZ3(u8 rxPort, u8 rxPin, u8 anPort, u8 anPin, u8 pwPort, u8 pwPin);

/*!
\brief Measures the distance to an object.

\param[in] ez3 sensor to use

\pre shall first be called 250ms after boot
\pre ez3 is alloced
\pre while first call no target shall be in the first 36cm or at least 18cm else targets at this range will not be detected
\pre time is enabled
\pre time is not resettet or set to a smaller value while measuring

\return distance in cm
\return If no target is detected 1024 is returned.
*/
u16 measureEZ3(EZ3* ez3);

/*!
\brief Frees the allocated memory.

\param[in] ez3 sensor to be freed

\pre ez3 is alloced

\post ez3 is freed
*/
void freeEZ3(EZ3** ez3);

/*!
\brief Initializes the sensor.

\pre pins must be the correct ones -> see implementation
*/
void initEZ3();

/*!
\brief Measures the distance from 2 EZ3 to objects.

\param[in] ez31 first EZ3
\param[in] ez32 second EZ3
\param[out result1 result for ez31
\param[out result2 result for ez32

\pre shall first be called 250ms after boot
\pre ez31 is alloced
\pre ez32 is alloced
\pre while first call no target shall be in the first 36cm or at least 18cm else targets at this range will not be detected
\pre time is enabled
\pre time is not resettet or set to a smaller value while measuring

\post result1 contains result of ez31
\post result2 contains result of ez32
*/
void measure2EZ3(EZ3* ez31, EZ3* ez32, u16* result1, u16* result2);

#endif
