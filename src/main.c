#include "main.h"
#include "globals.h"
#include "led.h"
#include "button.h"
#include <util/delay.h>
#include <stdlib.h>
#include "time.h"
#include <USB.h>

extern Led* led1;
extern Led* led2;
extern Button* but1;

ISR(TIMER0_OVF_vect)//each 100µs
{
	_time += 100;
	TCNT0 = 230;
}

int main(int argc, char* argv[])
{
	led1 = allocLed(1, 0);//B0
	led2 = allocLed(1, 1);//B1
	but1 = allocButton(1, 2);//B2
	setTime(0);
	enableTime();
	u64 time;
	while (1)
	{
		time = getTime();
		if (time > 1000000)
		{
			setTime(0);
		}
		if (time%1000000 < 100000)
		{
			onLed(led1);
		}
		else
		{
			offLed(led1);
		}
		u8 butState = stateButton(but1);
		if (butState)
		{
			onLed(led2);
		}
		else
		{
			offLed(led2);
		}
		USB_USBTask();
	}
	freeLed(&led1);
	freeLed(&led2);
	freeButton(&but1);
	return 0;
}
