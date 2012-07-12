#include "servo.h"
#include "typedefs.h"
#include "pin.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>

const u16 _rightAngleTime = 150;///magic
const u16 _leftAngleTime = 542;///magic

Servo* allocServo(u8 port, u8 pin)
{
	Servo* servo = (Servo*) malloc(sizeof(Servo));
	if (servo == NULL)
	{
		return NULL;
	}
	servo->pin = allocPin(port, pin);
	if (servo->pin == NULL)
	{
		free(servo);
		servo = NULL;
		return NULL;
	}
	setAsOutputPin(servo->pin);
	servo->targetAngleTime = (_leftAngleTime-_rightAngleTime)/2 + _rightAngleTime;///time to wait to set the target angle
	servo->actualAngleTime = servo->targetAngleTime;///time to wait to set the actual angle
	OCR1A = servo->targetAngleTime;
	return servo;
}

void enableServo()
{
	//normal mode
	TCCR1A &= ~(1<<COM1A1);
	TCCR1A &= ~(1<<COM1A0);
	TCCR1A &= ~(1<<COM1B1);
	TCCR1A &= ~(1<<COM1B0);
	TCCR1A &= ~(1<<COM1C1);
	TCCR1A &= ~(1<<COM1C0);

	//ctc
	TCCR1A &= ~(1<<WGM11);
	TCCR1A &= ~(1<<WGM10);
	TCCR1B |= 1<<WGM12;
	TCCR1B &= ~(1<<WGM13);

	//set prescaler to 64
	TCCR1B &= ~(1<<CS12);
	TCCR1B |= 1<<CS11;
	TCCR1B |= 1<<CS10;

	//TIMSK1 |= 1<<TOIE1;//enable overflow interrupt
	TIMSK1 |= 1<<OCIE1A;//enable ctc interrupt
	sei();//enable interrupts
}

void freeServo(Servo** servo)
{
	freePin(&((*servo)->pin));
	free(*servo);
	*servo = NULL;
}

void setStateServo(Servo* servo, u8 degree)
{
	u16 diff = _leftAngleTime - _rightAngleTime;
	float f = (float)((float)(degree)*(float)(diff))/(float)(180);
	servo->targetAngleTime = _rightAngleTime + f;
}
