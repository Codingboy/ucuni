#include "demo.h"
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
extern EZ3* ez31;
extern EZ3* ez32;

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

static const u8 ez3Toleranz = 2;
static const u8 initScans = 10;

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

	enableServos();
	setTime(0);
	enableTime();
	u64 time;
	u8 servoStatesNumber = 36;
	u8 servoStates[servoStatesNumber] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175};
	u16 ez3Values[32];
	u8 mode = 0;
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

		switch (mode)
		{
			case 0:
				u16 ez3InitValues[initScans][servoStatesNumber*2];
				for (int i=0; i<initScans; i++)
				{
					for (int j=0; j<servoStatesNumber*2; j++)
					{
						ez3InitValues[i][j] = i*j;
					}
				}
				u8 majorCounter = 0;
				while (true)
				{
					bool noMovings = true;
					for (int k=0; k<servoStatesNumber; k++)
					{
						setServo(servo, servoStates[k]);
						noMovings = true;
						for (int i=0; i<servoStatesNumber*2; i++)
						{
							u16 reference = ez3InitValues[0][i];
							for (int j=1; j<initScans; j++)
							{
								u16 diff = 0;
								if (reference > ez3InitValues[j][i])
								{
									diff = reference - ez3InitValues[j][i];
								}
								else
								{
									diff = ez3InitValues[j][i] - reference;
								}
								if (diff > ez3Toleranz)///\todo edit value
								{
									noMovings = false;
									break;
								}
							}
							if (!noMovings)
							{
								break;
							}
						}
						if (noMovings)
						{
							break;
						}
						while (!isReadyServo(servo))
						{
							//wait until correct position
						}
						measure2EZ3(ez31, ez32, &(ez3InitValues[majorCounter%initScans][k]), &(ez3InitValues[majorCounter%initScans][k+16]));
					}
					if (noMovings)
					{
						break;
					}
					majorCounter++;
					for (int k=servoStatesNumber-1; k>=0; k--)
					{
						setServo(servo, servoStates[k]);
						noMovings = true;
						for (int i=0; i<servoStatesNumber; i++)
						{
							u16 reference = ez3InitValues[0][i];
							for (int j=1; j<initScans; j++)
							{
								u16 diff = 0;
								if (reference > ez3InitValues[j][i])
								{
									diff = reference - ez3InitValues[j][i];
								}
								else
								{
									diff = ez3InitValues[j][i] - reference;
								}
								if (diff > ez3Toleranz)///\todo edit value
								{
									noMovings = false;
									break;
								}
							}
							if (!noMovings)
							{
								break;
							}
						}
						if (noMovings)
						{
							break;
						}
						while (!isReadyServo(servo))
						{
							//wait until correct position
						}
						measure2EZ3(ez31, ez32, &(ez3InitValues[majorCounter%initScans][k]), &(ez3InitValues[majorCounter%initScans][k+servoStatesNumber]));
					}
					if (noMovings)
					{
						break;
					}
					majorCounter++;
				}
				for (int i=0; i<servoStatesNumber*2; i++)
				{
					ez3Values[i] = ez3InitValues[0][i];
				}
				mode++;
				break;
			case 1:
				u8 actualServoState = 0;
				u8 servoClockwise = false;
				while (true)
				{
					setServo(servo, servoStates[actualServoState]);
					while (!isReadyServo(servo))
					{
						//wait until correct position
					}
					u16 value1, value2;
					measure2EZ3(ez31, ez32, &value1, &value2);
					u16 diff;
					if (value1 > ez3Values[actualServoState])
					{
						diff = value1 - ez3Values[actualServoState];
					}
					else
					{
						diff = ez3Values[actualServoState] - value1;
					}
					if (diff > ez3Toleranz)
					{
						break;
					}
					if (value2 > ez3Values[actualServoState+servoStatesNumber])
					{
						diff = value2 - ez3Values[actualServoState+servoStatesNumber];
					}
					else
					{
						diff = ez3Values[actualServoState+servoStatesNumber] - value2;
					}
					if (diff > ez3Toleranz)
					{
						break;
					}

					if (actualServoState == 0 && servoClockwise)
					{
						servoClockwise = false;
					}
					else
					{
						if (actualServoState ==  && !servoClockwise)
						{
							servoClockwise = true;
						}
					}

					if (servoClockwise)
					{
						actualServoState--;
					}
					else
					{
						actualServoState++;
					}
				}
				mode++;
				break;
			case 2:
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
	freeEZ3(&ez31);
	freeEZ3(&ez32);
	freeServo(&servo);
	return 0;
}
