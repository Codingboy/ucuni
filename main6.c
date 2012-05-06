#include "pin.h"
#include "time.h"
#include <avr/delay.h>

/*Pin* led1;
u8 toggle = 0;

ISR(TIMER0_OVF_vect)
{
	_time += 100;
	TCNT0 = 230;
}

int main()
{
	led1 = allocPin(2, 0);
	setAsOutputPin(led1);
	setOutputPin(led1);
	setTime(0);
	enableTime();
	while(1)
	{
		u32 time = getTime();
		if (time >= 100000)
		{
			toggleOutputPin(led1);
			setTime(0);
		}
	}
	freePin(&led1);
}*/

/*int main()
{
	Pin* led1 = allocPin(2, 0);
	Pin* led2 = allocPin(2, 1);
	led3 = allocPin(2, 3);
	setAsOutputPin(led1);
	setAsOutputPin(led2);
	setAsOutputPin(led3);
	setOutputPin(led1);
	setOutputPin(led2);
	setOutputPin(led3);
	setTime(0);
	enableTime();
	while (1)
	{
		u64 time = getTime();
		if (time != 0 && time%1000000 == 0)
		{
			toggleOutputPin(led1);
		}
		_delay_ms(50);
		toggleOutputPin(led2);
	}
	freePin(&led1);
	freePin(&led2);
	freePin(&led3);
}*/
