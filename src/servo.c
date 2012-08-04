#include "servo.h"
#include "typedefs.h"
#include "pin.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>

const u16 _rightAngleTime = 150;///magic
const u16 _leftAngleTime = 542;///magic

Servo* allocServo(u8 port, u8 pin, u8 speed)
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
	servo->speed = speed;
	return servo;
}

void enableServos()
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

void disableServos()
{
	TIMSK1 &= ~(1<<OCIE1A);
}

void freeServo(Servo** servo)
{
	freePin(&((*servo)->pin));
	free(*servo);
	*servo = NULL;
}

u8 getStateServo(Servo* servo)
{
	u8 tmpSREG = SREG;
	cli();
	u16 diff = _leftAngleTime - _rightAngleTime;
	float degree = (float)(diff)/(float)(180);
	float f = (float)((servo->actualAngleTime - _rightAngleTime))/degree;
	SREG = tmpSREG;
	return (u8)f;
}

void setStateServo(Servo* servo, u8 degree)
{
//	u8 tmpSREG = SREG;
//	cli();
	u16 diff = _leftAngleTime - _rightAngleTime;
	float f = (float)((float)(degree)*(float)(diff))/(float)(180);
	servo->targetAngleTime = _rightAngleTime + f;
//	SREG = tmpSREG;
}

bool checkReadyServo(Servo* servo)
{
	return servo->targetAngleTime == servo->actualAngleTime;
}
