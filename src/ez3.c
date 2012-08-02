#include "pin.h"
#include "typedefs.h"
#include <stdlib.h>
#include <avr/delay.h>
#include "time.h"
#include "ez3.h"
#include <stdbool.h>
#include "globals.h"
#include "led.h"

extern u8 Vcc;
extern u8 Vref;
extern Led* led1;
extern Led* led2;

///an must be connected to f0
EZ3* allocEZ3(u8 rxPort, u8 rxPin, u8 anPort, u8 anPin, u8 pwPort, u8 pwPin)
{
	EZ3* ez3 = (EZ3*) malloc(sizeof(EZ3));
	if (ez3 == NULL)
	{
		return NULL;
	}
	ez3->rx = allocPin(rxPort, rxPin);
	if (ez3->rx == NULL)
	{
		free(ez3);
		ez3 = NULL;
		return NULL;
	}
	ez3->an = allocPin(anPort, anPin);
	if (ez3->an == NULL)
	{
		freePin(&(ez3->rx));
		free(ez3);
		ez3 = NULL;
		return NULL;
	}
	ez3->pw = allocPin(pwPort, pwPin);
	if (ez3->pw == NULL)
	{
		freePin(&(ez3->an));
		freePin(&(ez3->rx));
		free(ez3);
		ez3 = NULL;
		return NULL;
	}
	setAsOutputPin(ez3->rx);
	setAsInputPin(ez3->an);
	setAsInputPin(ez3->pw);
	clearOutputPin(ez3->rx);

	initEZ3();

	return ez3;
}

void initEZ3()
{
	//AVcc as reference with external capacitor on AREF pin
	ADMUX &= ~(1<<REFS1);
	ADMUX |= 1<<REFS0;

	//select adcpin (see page 308)
	ADCSRB &= ~(1<<MUX5);
	ADMUX &= ~(1<<MUX4);
	ADMUX &= ~(1<<MUX3);
	ADMUX &= ~(1<<MUX2);
	ADMUX &= ~(1<<MUX1);
	ADMUX &= ~(1<<MUX0);

	ADMUX &= ~(1<<ADLAR);//result right adjusted
	ADCSRB &= ~(1<<ADHSM);//low speed mode (less power)
	ADCSRA &= ~(1<<ADATE);//disable autotrigger
	ADCSRA &= ~(1<<ADIE);//disable adcinterrupt

	//set prescaler to 128
	ADCSRA |= 1<<ADPS2;
	ADCSRA |= 1<<ADPS1;
	ADCSRA |= 1<<ADPS0;
}

void measure2EZ3(EZ3* ez31, EZ3* ez32, u16* result1, u16* result2)
{
	setOutputPin(ez31->rx);
	setOutputPin(ez32->rx);
	_delay_us(20);
	clearOutputPin(ez31->rx);
	clearOutputPin(ez32->rx);
	while (true)
	{
		if (getInputPin(ez31->pw))
		{
			break;
		}
	}
	while (true)
	{
		if (getInputPin(ez32->pw))
		{
			break;
		}
	}
	u64 time1 = getTime();
	u64 time2 = time1;
	bool ready1 = false;
	bool ready2 = false;
	*result1 = 1024;//defaultresult
	*result2 = 1024;//defaultresult
	while (true)
	{
		time2 = getTime();
		u64 diff = time2 - time1;
		if (diff >= (u64)(37500))
		{
			ready1 = true;
			ready2 = true;
		}
		if (!ready1)
		{
			if (!getInputPin(ez31->pw))
			{
				u16 inch = diff/147;//convert measured time to a distance
				if (diff%147 >= 147/2)//round mathematical correct
				{
					inch++;
				}
				u16 cm = 2.54*inch;//convert to cm
				*result1 = cm;
				ready1 = true;
			}
		}
		if (!ready2)
		{
			if (!getInputPin(ez32->pw))
			{
				u16 inch = diff/147;//convert measured time to a distance
				if (diff%147 >= 147/2)//round mathematical correct
				{
					inch++;
				}
				u16 cm = 2.54*inch;//convert to cm
				*result2 = cm;
				ready2 = true;
			}
		}
		if (ready1 && ready2)//both ez3 finish
		{
			break;
		}
	}
}

u16 measureEZ3(EZ3* ez3)
{
	setOutputPin(ez3->rx);
	_delay_us(20);
	clearOutputPin(ez3->rx);
	while (true)
	{
		if (getInputPin(ez3->pw))
		{
			break;
		}
	}
	u64 time1 = getTime();
	u64 time2 = time1;

	//get the result b measuring the time
	while (true)///\todo replace by pwm
	{
		time2 = getTime();
		u64 diff = time2 - time1;
		//if (diff >= (u64)(37500))
		if (diff >= (u64)(5787))
		{
			return 1024;
		}
		if (!getInputPin(ez3->pw))
		{
			u16 cm = (diff*2.54)/147;//convert measured time to a distance
			return cm;
		}
	}
	return 1024;

	///\debug adc
	//get result by using the adc
	/*ADCSRA |= 1<<ADEN;//enable adc
	ADCSRA |= 1<<ADSC;//start adc
	while (ADCSRA & (1<<ADSC))//wait
	{
	}
	u16 vin = ADCL;
	u16 vin2 = ADCH;
	ADCSRA &= ~(1<<ADEN);//disable adc to save power
	vin += vin2<<8;
	float voltage = (float)(vin*Vref)/(float)(1024);
	u16 inch = Vcc/(512*voltage);
	u16 cm = inch*2.54;
	return cm;*/
}

void freeEZ3(EZ3** ez3)
{
	freePin(&((*ez3)->an));
	freePin(&((*ez3)->pw));
	freePin(&((*ez3)->rx));
	free(*ez3);
	*ez3 = NULL;
}
