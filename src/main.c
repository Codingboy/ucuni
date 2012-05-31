#include "pin.h"
#include <util/delay.h>

Pin* but1 = 0;

int main(int argc, char* argv[])
{
	but1 = allocPin(1, 0);//A0
	setAsOutputPin(but1);
	
	while (1)
	{
		setOutputPin(but1);
		_delay_ms(100);
		clearOutputPin(but1);
		_delay_ms(900);
	}
	freePin(&but1);
	return 0;
}
