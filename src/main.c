#include "main.h"
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
static u8 servospeed = 10;

ISR(TIMER0_OVF_vect)//each 100µs
{
	_time += 100;
	TCNT0 = 230;
}

ISR(TIMER1_COMPA_vect)
{
	if (!getOutputPin(servo->pin))//off
	{
		if (servo->targetAngleTime < servo->actualAngleTime)
		{
			servo->actualAngleTime -= servospeed;
		}
		else
		{
			if (servo->targetAngleTime > servo->actualAngleTime)
			{
				servo->actualAngleTime += servospeed;
			}
		}
		setOutputPin(servo->pin);
		OCR1A = servo->actualAngleTime;//max value
	}
	else//on
	{
		clearOutputPin(servo->pin);
		OCR1A = 5000;//20ms
	}
}

int main(int argc, char* argv[])
{
	led1 = allocLed(1, 0);//B0
	led2 = allocLed(1, 1);//B1
	but1 = allocButton(1, 2);//B2
	servo = allocServo(1, 4);//B4
	bool blink = false;
	onLed(led1);
	MCUSR &= ~(1<<WDRF);
	wdt_disable();
	clock_prescale_set(clock_div_1);
	USB_Init();
	sei();

	EZ3* ez3 = allocEZ3(1,3, 5,0, 3,0);
//B3
//F0
//D0
	_delay_ms(250);
	measureEZ3(ez3);//calibration

	u8 butMutex = 1;

	enableServo();
	setTime(0);
	enableTime();
	u64 time;
u8 magic = 0;
	while (1)
	{
		time = getTime();

		if (blink)
		{
			if (time%(u64)1000000 < (u64)100000)
			{
				onLed(led1);
			}
			else
			{
				offLed(led1);
			}
		}

		u8 butState = stateButton(but1);
		u16 dist = 0;
		if (butState)
		{
magic++;
			if (butMutex)
			{
				butMutex = 0;
				dist = measureEZ3(ez3);
				if (dist < 30)
				{
					onLed(led2);
				}
				_delay_ms(100);//prevent prelling
			}
		}
		else
		{
			butMutex = 1;
			offLed(led2);
		}

		//setStateServo(90);
		/*if (time%(u64)2000000 < (u64)1000000)
		{
			setStateServo(180);
		}
		else
		{
			setStateServo(0);
		}*/
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

		if (USB_DeviceState != DEVICE_STATE_Configured)
        {
			continue;
		}
		blink = true;

Endpoint_SelectEndpoint(IN_EPNUM);
if (Endpoint_IsConfigured() && Endpoint_IsINReady() && Endpoint_IsReadWriteAllowed())
{
//do_something(state, data, &len);
//err = Endpoint_Write_Stream_LE((void *)data, len);
// FIXME handle err
Endpoint_ClearIN();
}

Endpoint_SelectEndpoint(OUT_EPNUM);
if (Endpoint_IsConfigured() && Endpoint_IsOUTReceived() && Endpoint_IsReadWriteAllowed())
{
//err = Endpoint_Read_Stream_LE(data, len);
//do_other_thing(data, len);
Endpoint_ClearOUT();
}

		USB_USBTask();
	}
	freeLed(&led1);
	freeLed(&led2);
	freeButton(&but1);
	freeEZ3(&ez3);
	freeServo(&servo);
	return 0;
}
