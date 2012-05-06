#include "pinoperations.h"
#include <util/delay.h>
#include "typedefs.h"
#include "pin.h"
#include "time.h"

ISR(TIMER0_OVF_vect)//each 100µs
{
	_time += 100;
	TCNT0 = 230;
}

int main()
{
	setAsOutput(2 ,0);
	clearOutput(2, 0);//led an
	_delay_ms(100);
	setOutput(2, 0);//led aus
	_delay_ms(900);
	clearOutput(2, 0);//led an
	_delay_ms(1000);
	setOutput(2, 0);//led aus
	_delay_ms(1000);

	Pin* led1 = allocPin(2, 0);
	Pin* but1 = allocPin(0, 7);
	Pin* srf05 = allocPin(0, 0);
	Pin* srf05Control = allocPin(2, 3);
	Pin* warnLed = allocPin(2, 1);
	Pin* alarmLed = allocPin(2, 2);
	u32 scanTime = 0;

	setAsOutputPin(led1);//led als ausgang setzen
	setAsOutputPin(warnLed);//led als ausgang setzen
	setAsOutputPin(alarmLed);//led als ausgang setzen
	setAsInputPin(but1);//button als eingang setzen
	setAsOutputPin(srf05Control);
	clearOutputPin(srf05Control);
	setOutputPin(led1);//led aus
	setOutputPin(warnLed);//led aus
	setOutputPin(alarmLed);//led aus

	clearOutputPin(led1);//led an
	_delay_ms(100);
	setOutputPin(led1);//led aus
	_delay_ms(900);
	clearOutputPin(led1);//led an
	_delay_ms(1000);
	setOutputPin(led1);//led aus
	_delay_ms(1000);

	u64 time1 = 0;//variable to measure time
	u8 breakStatus = 0;//status if section shall be quited
	u8 scanMode = 0;//mode for srf05 calculation; intern used mode; does not apply to the datasheet's mode

	start:
	setTime(0);
	enableTime();
	while (1)//eingabe
	{
		time1 = getTime();
		if (time1%1000000 < 100000)//100ms
		{
			clearOutputPin(led1);//led on
			if (time1 > 3600000000)//1h deny overflow
			{
				setTime(0);
			}
		}
		else//900ms
		{
			setOutputPin(led1);//led off
		}

		if (getInputADCPin(but1) == 1)
		{
			_delay_ms(20);//wait until button completely pressed
			while (getInputADCPin(but1) == 1)//wait until button released
			{
				_delay_ms(1);
			}
			_delay_ms(20);//wait until button completely released
			breakStatus = 1;
		}

		if (breakStatus)
		{
			breakStatus = 0;
			break;
		}
	}
	setTime(0);
	while (1)//wartezeit
	{
		time1 = getTime();
		if (time1%1000000 < 500000)//500ms
		{
			clearOutputPin(led1);//led on
		}
		else//500ms
		{
			setOutputPin(led1);//led off
		}

		if (time1 >= 5000000)//break after 5s
		{
			breakStatus = 1;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			break;
		}
	}
	setOutputPin(led1);//led off
	while (1)//scannen
	{
	//10 us highpegel als impuls
		if (scanMode == 0)
		{
			setAsOutputPin(srf05);
			setOutputPin(srf05);
			setOutputPin(srf05Control);
			scanMode = 1;
			setTime(0);
		}
		else if (scanMode == 1)
		{
			time1 = getTime();
			if (time1 > 10)
			{
				clearOutputPin(srf05);
				clearOutputPin(srf05Control);
				setAsInputPin(srf05);
				scanMode = 2;
			}
		}
	//srf05 signallänge messen
		else if (scanMode == 2)
		{
			if (getInputPin(srf05))
			{
				setOutputPin(srf05Control);
				setTime(0);
				scanMode = 3;
			}
		}
		else if (scanMode == 3)
		{
			time1 = getTime();
			if (!getInputPin(srf05))
			{
				clearOutputPin(srf05Control);
				scanTime = time1;
				breakStatus = 1;
			}
			if (time1 > 30000)
			{
				clearOutputPin(srf05Control);
				scanTime = 30000;
				breakStatus = 1;
			}
		}

		if (breakStatus)
		{
			breakStatus = 0;
			scanMode = 0;
			//break;
		}
	}
	clearOutputPin(led1);//led on
	while (1)//betrieb
	{
	//10 us highpegel als impuls
		if (scanMode == 0)
		{
			setAsOutputPin(srf05);
			setOutputPin(srf05);
			setTime(0);
			scanMode = 1;
		}
		else if (scanMode == 1)
		{
			time1 = getTime();
			if (time1 > 10)
			{
				clearOutputPin(srf05);
				setAsInputPin(srf05);
				scanMode = 2;
			}
		}
	//srf05 signallänge messen
		else if (scanMode == 2)
		{
			if (getInputPin(srf05))
			{
				setTime(0);
				scanMode = 3;
			}
		}
		else if (scanMode == 3)
		{
			if (!getInputPin(srf05))
			{
				time1 = getTime();
				if (time1 >= scanTime*1.25 || time1 <= scanTime*0.75)
				{
					breakStatus = 1;
				}
			}
			if (time1 > 30000)
			{
				time1 = 30000;
				if (time1 >= scanTime*1.25 || time1 <= scanTime*0.75)
				{
					breakStatus = 1;
				}
				else
				{
					scanMode = 0;
				}
			}
		}

		if (breakStatus)
		{
			breakStatus = 0;
			scanMode = 0;
			scanTime = 0;
			break;
		}
	}
	setOutputPin(led1);//led off
	setTime(0);
	while (1)//alarm
	{
		time1 = getTime();
		clearOutputPin(warnLed);//led on
		if (getInputADCPin(but1) == 1)//deactivate alarm
		{
			_delay_ms(20);
			while (getInputADCPin(but1) == 1)
			{
				_delay_ms(1);
			}
			_delay_ms(20);
			setOutputPin(warnLed);//led off
			goto start;
		}
		if (time1 >= 60000000)//60s until real alarm
		{
			clearOutputPin(alarmLed);//led on
			while (1)
			{
				time1 = getTime();
				if (time1%10000000 < 100000)//100ms
				{
					clearOutputPin(led1);//led on
				}
				else//9.9s
				{
					setOutputPin(led1);//led off
				}
			}
		}
	}
	freePin(&led1);
	freePin(&but1);
	freePin(&srf05);
	freePin(&srf05Control);
	freePin(&warnLed);
	freePin(&alarmLed);
	return 0;
}
