#include "pin.h"
#include <util/delay.h>

Pin but1* = 0;
Pin but2* = 0;
Pin but3* = 0;
Pin but4* = 0;

int main(int argc, char* argv[])
{
	but1 = allocPin(0, 0);//A0
	setAsOutputPin(Pin* pin);
	
	while (1)
	{
		setOutputPin(Pin* pin);
		_delay_ms(100);
		clearOutputPin(Pin* pin);
		_delay_ms(900);
	}
	freePin(&but1);
	return 0;
}
