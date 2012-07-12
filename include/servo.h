/*!
\file servo.h
\brief Operations to control servos
\warning ISR have be implement
*/

#ifndef SERVO_H
#define SERVO_H

#include "pin.h"
#include "typedefs.h"

typedef struct
{
	Pin* pin;
	u16 targetAngleTime;///time to wait to set the target angle
	u16 actualAngleTime;///time to wait to set the actual angle
} Servo;

/*!
\brief Allocates a servo.

\param[in] port port
\param[in] pin pin

\return allocated servo
*/
Servo* allocServo(u8 port, u8 pin);

/*!
\brief Frees a servo.

\param[in] servo servo to be freed

\pre servo is alloced
*/
void freeServo(Servo** servo);

/*!
\brief Gets the state of the servo.

\param[in] servo servo

\pre servo is alloced

\return servo state in degrees (0-180), 0 means right
*/
u8 getStateServo(Servo* servo);

/*!
\brief Sets the state of the servo it will move to.

\param[in] servo servo
\param[in] degree state to set the servo to, 0 means right, 180 left, 90 front

\pre servo is alloced
\pre degree >= 0
\pre degree <= 180
\pre servo is enabled
*/
void setStateServo(Servo* servo, u8 degree);

/*!
\brief Enables the servo.
*/
void enableServo();

#endif
