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
extern Button* but2;
extern Button* but3;
extern Servo* servo;
extern EZ3* ez3;
static u8 servospeed = 10;

static bool blink = false;

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
			if (servo->actualAngleTime-servo->targetAngleTime < servo->speed)
			{
				servo->actualAngleTime = servo->targetAngleTime;
			}
			else
			{
				servo->actualAngleTime -= servo->speed;
			}
		}
		else
		{
			if (servo->targetAngleTime > servo->actualAngleTime)
			{
				if (servo->targetAngleTime-servo->actualAngleTime < servo->speed)
				{
					servo->actualAngleTime = servo->targetAngleTime;
				}
				else
				{
					servo->actualAngleTime += servo->speed;
				}
			}
		}
		setOutputPin(servo->pin);
		OCR1A = servo->actualAngleTime;//max value
	}
	else//on
	{
		clearOutputPin(servo->pin);
		OCR1A = 5000;//20ms

		//USB stuff included to minimize overhead and deny use of a new timer/counter
		//shall be called each 30 ms
		//is called each 2X ms
		USB_USBTask();
		if (USB_DeviceState == DEVICE_STATE_Configured)
        {
			blink = true;

///\todo do i need those endpoints?
	Endpoint_SelectEndpoint(OUT_EPNUM);

	/* Check to see if a packet has been sent from the host */
	if (Endpoint_IsOUTReceived())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Create a temporary buffer to hold the read in report from the host */
			//uint8_t GenericData[GENERIC_REPORT_SIZE];

			/* Read Generic Report Data */
			Endpoint_Read_8();

			/* Process Generic Report Data */
			//ProcessGenericHIDReport(GenericData);
		}

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearOUT();
	}

	Endpoint_SelectEndpoint(IN_EPNUM);

	/* Check to see if the host is ready to accept another packet */
	if (Endpoint_IsINReady())
	{
		/* Create a temporary buffer to hold the report to send to the host */
		//uint8_t GenericData[GENERIC_REPORT_SIZE];

		/* Create Generic Report Data */
		//CreateGenericHIDReport(GenericData);

		/* Write Generic Report Data */
		Endpoint_Write_8(42);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
	}

		}
	}
}

int main(int argc, char* argv[])
{
	led1 = allocLed(1, 0);//B0
	led2 = allocLed(1, 1);//B1
	but1 = allocButton(1, 2);//B2
	but2 = allocButton(3, 0);//D0
	but3 = allocButton(3, 1);//D1
	onLed(led1);
	MCUSR &= ~(1<<WDRF);
	wdt_disable();
	clock_prescale_set(clock_div_1);
	USB_Init();
	sei();
	///\warning also usb task!!!
	servo = allocServo(1, 4, 10);//B4

	ez3 = allocEZ3(1,3, 5,0, 3,0);
//B3
//F0
//D0
	_delay_ms(250);
	measureEZ3(ez3);//calibration

	u8 butMutex = 1;

	enableServos();
	setTime(0);
	enableTime();
	u64 time;
	u8 magic = 0;
	bool left = true;
	while (true)
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
			if (left)
			{
				magic++;
			}
			else
			{
				magic--;
			}
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

		//setStateServo(servo, 0);
		switch (magic)
		{
			case 0:
				setStateServo(servo, 0);
				if (!left)
				{
					left = true;
				}
				break;
			case 1:
				setStateServo(servo, 22);
				break;
			case 2:
				setStateServo(servo, 45);
				break;
			case 3:
				setStateServo(servo, 68);
				break;
			case 4:
				setStateServo(servo, 90);
				break;
			case 5:
				setStateServo(servo, 112);
				break;
			case 6:
				setStateServo(servo, 135);
				break;
			case 7:
				setStateServo(servo, 158);
				break;
			case 8:
				setStateServo(servo, 180);
				if (left)
				{
					left = false;
				}
				break;
		}

		if (USB_DeviceState != DEVICE_STATE_Configured)
        {
			continue;
		}
		blink = true;
	}
	freeLed(&led1);
	freeLed(&led2);
	freeButton(&but1);
	freeButton(&but2);
	freeButton(&but3);
	freeEZ3(&ez3);
	freeServo(&servo);
	return 0;
}
