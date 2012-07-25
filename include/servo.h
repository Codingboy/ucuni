/*!
\file servo.h
\brief Operations to control servos
\warning ISR have be implement
*/

#ifndef SERVO_H
#define SERVO_H

#include "pin.h"
#include "typedefs.h"
#include <stdbool.h>

typedef struct
{
	Pin* pin;
	u16 targetAngleTime;///time to wait to set the target angle
	u16 actualAngleTime;///time to wait to set the actual angle
	u8 speed;///speed of the servo
} Servo;

/*!
\brief Allocates a servo.

\param[in] port port
\param[in] pin pin

\return allocated servo
*/
Servo* allocServo(u8 port, u8 pin, u8 speed);

/*!
\brief Checks if the servo is in the specified position.

\warning This checks only the softwareside not the real hardwarestate. To keep the drift between software and hardware small use small values for speed.

\pre servo is alloced

\return true if servo is in correct position
\return false if servo is not on correct position
*/
bool checkReadyServo(Servo* servo);

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
\brief Enables servos.
*/
void enableServos();

/*!
\brief Disables servos.
*/
void disableServos();

#endif
