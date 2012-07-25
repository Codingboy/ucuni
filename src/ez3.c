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
	ADCSRA &= ~(1<<ADIE);//disable adcinterrpt

	//set prescaler to 128
	ADCSRA |= 1<<ADPS2;
	ADCSRA |= 1<<ADPS1;
	ADCSRA |= 1<<ADPS0;

	ADCSRA |= 1<<ADEN;//enable adc
	return ez3;
}

u16 measureEZ3(EZ3* ez3)
{
	setOutputPin(ez3->rx);
	_delay_us(20);
	clearOutputPin(ez3->rx);
	u64 time1 = getTime();
	u64 time2 = time1;
	while (true)
	{
		if (getInputPin(ez3->pw))
		{
			break;
		}
	}

	//get the result b measuring the time
	while (true)///\todo replace by pwm
	{
		time2 = getTime();
		u64 diff = time2 - time1;
		if (diff >= (u64)(37500))
		{
			return 1024;
		}
		if (!getInputPin(ez3->pw))
		{
			u16 inch = diff/147;
			u16 cm = 2.54*inch;
			return cm;
		}
	}
	return 1024;

	//get result by using the adc
	/*ADCSRA |= 1<<ADSC;//start adc
	while (ADCSRA & (1<<ADSC))//wait
	{
	}
	u16 vin = ADCL;
	vin += ADCH<<8;
	float voltage = (float)(vin*1024)/(float)(Vref);
	if (voltage < 1000)
	{
		onLed(led1);
	}
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
