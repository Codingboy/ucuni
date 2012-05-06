#include "pinoperations.h"
#include <util/delay.h>
#include "typedefs.h"
#include "pin.h"

/*int main()
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
	Pin* warnLed = allocPin(2, 1);
	Pin* alarmLed = allocPin(2, 2);
	u32 scanTime = 0;

	setAsOutputPin(led1);//led als ausgang setzen
	setAsOutputPin(warnLed);//led als ausgang setzen
	setAsOutputPin(alarmLed);//led als ausgang setzen
	setAsInputPin(but1);//button als eingang setzen
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

	u32 counter = 0;//anzahl der schleifendurchläufe
	u8 breakStatus = 0;//status ob abschnitt verlassen werden soll
	u32 startTime = 0;//counter an der stelle n; zur zeitmessung
	u8 scanMode = 0;//modus für srf05 messung; bezieht sich nicht auf den betriebsmodus sondern auf den codeabschnitt

	start:
	while (1)//eingabe
	{
		if (counter%1000000 == 0)
		{
			clearOutputPin(led1);//led an
		}
		if (counter%1000000 == 99999)
		{
			setOutputPin(led1);//led aus
		}

		if (getInputADCPin(but1) == 1)
		{
			_delay_ms(20);
			while (getInputADCPin(but1) == 1)
			{
				_delay_ms(1);
			}
			_delay_ms(20);
			breakStatus = 1;
		}

		_delay_us(10);
		counter += 10;
		if (counter >= 1000000)
		{
			counter = 0;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			counter = 0;
			break;
		}
	}
	while (1)//wartezeit
	{
		if (counter%1000000 == 0)
		{
			clearOutputPin(led1);//led an
		}
		if (counter%1000000 == 499999)
		{
			setOutputPin(led1);//led aus
		}

		_delay_us(10);
		counter += 10;
		if (counter >= 5000000)//abbruch nach 5 sek
		{
			breakStatus = 1;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			counter = 0;
			break;
		}
	}
	while (1)//scannen
	{
		if (counter%1000000 == 0)
		{
			clearOutputPin(led1);//led an
		}
		if (counter%1000000 == 89999)
		{
			setOutputPin(led1);//led aus
		}

	//100 us highpegel als impuls
		if (scanMode == 0)
		{
			setAsOutputPin(srf05);
			setOutputPin(srf05);
			startTime = counter;
			scanMode = 1;
		}
		else if (scanMode == 1)
		{
			if (counter >= startTime + 100)
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
				startTime = counter;
				scanMode = 3;
			}
		}
		else if (scanMode == 3)
		{
			if (!getInputPin(srf05))
			{
				scanTime = counter - startTime;
				breakStatus = 1;
			}
			if (counter >= startTime + 30000)
			{
				scanTime = 30000;
				breakStatus = 1;
			}
		}

		_delay_us(10);
		counter += 10;
		if (scanTime != 0)//abbruch sobald scan fertig
		{
			breakStatus = 1;
		}
		if (breakStatus)
		{
			breakStatus = 0;
			counter = 0;
			startTime = 0;
			scanMode = 0;
			//break;
		}
	}
	clearOutputPin(led1);//led an
	while (1)//betrieb
	{
	//100 us highpegel als impuls
		if (scanMode == 0)
		{
			setAsOutputPin(srf05);
			setOutputPin(srf05);
			startTime = counter;
			scanMode = 1;
		}
		else if (scanMode == 1)
		{
			if (counter >= startTime + 100)
			{
				clearOutputPin(srf05);
				setAsInputPin(srf05);
				startTime = counter;
				scanMode = 2;
			}
		}
	//srf05 signallänge messen
		else if (scanMode == 2)
		{
			if (getInputPin(srf05))
			{
				startTime = counter;
				scanMode = 3;
			}
		}
		else if (scanMode == 3)
		{
			u32 localScanTime = 0;
			if (!getInputPin(srf05))
			{
				localScanTime = counter - startTime;
				if (localScanTime >= scanTime*1.05 || localScanTime <= scanTime*0.95)
				{
					breakStatus = 1;
				}
			}
			if (counter >= 30000 + startTime)
			{
				localScanTime = 30000;
				if (localScanTime >= scanTime*1.05 || localScanTime <= scanTime*0.95)
				{
					breakStatus = 1;
				}
				else
				{
					counter = 0;
					startTime = 0;
					scanMode = 0;
				}
			}
		}

		_delay_us(10);
		counter++;
		if (breakStatus)
		{
			breakStatus = 0;
			counter = 0;
			startTime = 0;
			scanMode = 0;
			scanTime = 0;
			break;
		}
	}
	setOutputPin(led1);//led aus
	while (1)//alarm
	{
		clearOutputPin(warnLed);//led an
		if (getInputADCPin(but1) == 1)//alarm deaktivieren
		{
			_delay_ms(20);
			while (getInputADCPin(but1) == 1)
			{
				_delay_ms(1);
			}
			_delay_ms(20);
			setOutputPin(warnLed);//led aus
			goto start;
		}
		_delay_ms(1);
		counter++;
		if (counter == 60000)//60 sek bis alarm endgültig
		{
			clearOutputPin(alarmLed);//led an
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
	freePin(&srf05);
	freePin(&warnLed);
	freePin(&alarmLed);
	return 0;
}*/
