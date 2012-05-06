#include "pinoperations.h"
#include <util/delay.h>
#include "typedefs.h"

/*int main()
{
	setAsOutput(1, 0);//led als ausgang setzen
	setAsInput(1, 4);//button als eingang setzen

	clearOutput(1, 0);//led an
	_delay_ms(100);
	setOutput(1, 0);//led aus
	_delay_ms(900);
	clearOutput(1, 0);//led an
	_delay_ms(100);
	setOutput(1, 0);//led aus
	_delay_ms(900);

	u16 ledCounter = 0;
	u8 roundCounter = 0;
	u8 breakStatus = 0;

	start:
	while (1)
	{
		if (ledCounter%1000 == 0)
		{
			clearOutput(1, 0);//led an
		}
		if (ledCounter%1000 == 99)
		{
			setOutput(1, 0);//led aus
		}

		int buttonStatus = getInput(1, 4);
		if (!buttonStatus)
		{
			_delay_ms(20);
			if (!buttonStatus)
			{
				while (!getInput(1, 4))
				{
					_delay_ms(1);
				}
				breakStatus = 1;
			}
		}

		_delay_ms(1);
		ledCounter++;
		if (ledCounter == 1000)
		{
			ledCounter = 0;
			roundCounter++;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			ledCounter = 0;
			roundCounter = 0;
			break;
		}
	}
	while (1)
	{
		if (ledCounter%1000 == 0)
		{
			clearOutput(1, 0);//led an
		}
		if (ledCounter%1000 == 499)
		{
			setOutput(1, 0);//led aus
		}

		_delay_ms(1);
		ledCounter++;
		if (ledCounter == 1000)
		{
			ledCounter = 0;
			roundCounter++;
		}
		if (roundCounter == 5)
		{
			breakStatus = 1;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			ledCounter = 0;
			roundCounter = 0;
			break;
		}
	}
	while (1)
	{
		if (ledCounter%1000 == 0)
		{
			clearOutput(1, 0);//led an
		}
		if (ledCounter%1000 == 899)
		{
			setOutput(1, 0);//led aus
		}

		_delay_ms(1);
		ledCounter++;
		if (ledCounter == 1000)
		{
			ledCounter = 0;
			roundCounter++;
		}
		if (roundCounter == 5)
		{
			breakStatus = 1;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			ledCounter = 0;
			roundCounter = 0;
			break;
		}
	}
	clearOutput(1, 0);//led an
	while (1)
	{
		_delay_ms(5000);
		break;
	}
	setOutput(1, 0);//led aus
	while (1)
	{
		_delay_ms(5000);
		break;
	}
	goto start;
	return 0;
}*/
