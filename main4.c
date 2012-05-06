#include "pinoperations.h"
#include <util/delay.h>
#include "typedefs.h"
#include "pin.h"

/*int main()
{
	setAsOutput(1 ,0);
	clearOutput(1, 0);//led an
	_delay_ms(100);
	setOutput(1, 0);//led aus
	_delay_ms(900);
	clearOutput(1, 0);//led an
	_delay_ms(1000);
	setOutput(1, 0);//led aus
	_delay_ms(1000);

	Pin* led1 = allocPin(1, 0);
	Pin* but1 = allocPin(1, 4);

	setAsOutputPin(led1);//led als ausgang setzen
	setAsInputPin(but1);//button als eingang setzen

	clearOutputPin(led1);//led an
	_delay_ms(100);
	setOutputPin(led1);//led aus
	_delay_ms(900);
	clearOutputPin(led1);//led an
	_delay_ms(1000);
	setOutputPin(led1);//led aus
	_delay_ms(1000);

	u32 ledCounter = 0;
	u8 breakStatus = 0;

	start:
	while (1)//eingabe
	{
		if (ledCounter%1000 == 0)
		{
			clearOutputPin(led1);//led an
		}
		if (ledCounter%1000 == 99)
		{
			setOutputPin(led1);//led aus
		}

		if (!getInputPin(but1))
		{
			_delay_ms(20);
			while (!getInputPin(but1))
			{
				_delay_ms(1);
			}
			_delay_ms(20);
			breakStatus = 1;
		}

		_delay_ms(1);
		ledCounter++;
		if (ledCounter == 1000)
		{
			ledCounter = 0;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			ledCounter = 0;
			break;
		}
	}
	while (1)//wartezeit
	{
		if (ledCounter%1000 == 0)
		{
			clearOutputPin(led1);//led an
		}
		if (ledCounter%1000 == 499)
		{
			setOutputPin(led1);//led aus
		}

		_delay_ms(1);
		ledCounter++;
		if (ledCounter == 5000)//abbruch nach 5 sek
		{
			breakStatus = 1;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			ledCounter = 0;
			break;
		}
	}
	while (1)//scannen
	{
		if (ledCounter%1000 == 0)
		{
			clearOutputPin(led1);//led an
		}
		if (ledCounter%1000 == 899)
		{
			setOutputPin(led1);//led aus
		}

		_delay_ms(1);
		ledCounter++;
		if (ledCounter == 5000)//abbruch nach 5 sek
		{
			breakStatus = 1;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			ledCounter = 0;
			break;
		}
	}
	clearOutputPin(led1);//led an
	while (1)//betrieb
	{
		_delay_ms(5000);
		break;
	}
	setOutputPin(led1);//led aus
	while (1)//alarm
	{
		if (!getInputPin(but1))//alarm deaktivieren
		{
			_delay_ms(20);
			while (!getInputPin(but1))
			{
				_delay_ms(1);
			}
			_delay_ms(20);
			goto start;
		}
		_delay_ms(1);
		ledCounter++;
		if (ledCounter == 60000)//60 sek bis alarm endgültig
		{
			while (1)
			{
				clearOutputPin(led1);
				_delay_ms(100);
				setAsOutputPin(led1);
				_delay_ms(10000);
			}
		}
	}
	freePin(&led1);
	freePin(&but1);
	return 0;
}*/
