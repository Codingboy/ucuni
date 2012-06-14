#include "led.h"
#include "button.h"
#include <util/delay.h>
#include <stdlib.h>

Led* led1 = NULL;
Led* led2 = NULL;
Button* but1 = NULL;

int main(int argc, char* argv[])
{
	led1 = allocLed(1, 0);//B0
	led2 = allocLed(1, 1);//B1
	but1 = allocButton(1, 2);//B2
	while (1)
	{
		onLed(led1);
		//_delay_ms(100);
		offLed(led1);
		//_delay_ms(900);
		u8 butState = stateButton(but1);
		if (butState)
		{
			onLed(led2);
		}
		else
		{
			offLed(led2);
		}
	}
	freeLed(&led1);
	freeLed(&led2);
	freeButton(&but1);
	return 0;
}
