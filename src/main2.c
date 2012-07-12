#include "main2.h"
#include "globals.h"
#include "led.h"
#include "button.h"
#include <util/delay.h>
#include <stdlib.h>
#include "time.h"
#include <USB.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include "usb.h"
#include <stdbool.h>
#include "ez3.h"
#include "servo.h"

extern Led* led1;
extern Led* led2;
extern Button* but1;
extern Servo* servo;

ISR(TIMER1_COMPA_vect)
{
	if (!getOutputPin(servo->pin))//off
	{
		if (servo->targetAngleTime < servo->actualAngleTime)
		{
			servo->actualAngleTime -= 3;
		}
		else
		{
			if (servo->targetAngleTime > servo->actualAngleTime)
			{
				servo->actualAngleTime += 3;
			}
		}
onLed(led1);
offLed(led2);
		setOutputPin(servo->pin);
		OCR1AH = (servo->actualAngleTime>>8) & 0xff;
		OCR1AL = servo->actualAngleTime & 0xff;
	}
	else//on
	{
offLed(led1);
onLed(led2);
		clearOutputPin(servo->pin);

		//20ms
		OCR1AH = (5000>>8) & 0xff;
		OCR1AL = 5000 & 0xff;
	}
}

int main(int argc, char* argv[])
{
	led1 = allocLed(1, 0);//B0
	led2 = allocLed(1, 1);//B1
	but1 = allocButton(1, 2);//B2
	servo = allocServo(1, 4);//B4
	
	u8 butMutex = 1;

	enableServo();
u8 magic = 0;
	while (1)
	{
		u8 butState = stateButton(but1);
		if (butState)
		{
magic++;
			if (butMutex)
			{
				butMutex = 0;
				_delay_ms(100);//prevent prelling
			}
		}
		else
		{
			butMutex = 1;
		}

		switch (magic%5)
		{
			case 0:
				setStateServo(servo, 0);
				break;
			case 1:
				setStateServo(servo, 45);
				break;
			case 2:
				setStateServo(servo, 90);
				break;
			case 3:
				setStateServo(servo, 135);
				break;
			case 4:
				setStateServo(servo, 180);
				break;
		}
	}
	freeLed(&led1);
	freeLed(&led2);
	freeButton(&but1);
	freeServo(&servo);
	return 0;
}
