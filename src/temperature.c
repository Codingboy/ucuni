#include "temperature.h"
#include "typedefs.h"
#include <avr/io.h>

void initTemperature()
{
	ADCSRB |= 1<<MUX5;
	ADMUX &= ~(1<<MUX4);
	ADMUX &= ~(1<<MUX3);
	ADMUX |= 1<<MUX2;
	ADMUX |= 1<<MUX1;
	ADMUX |= 1<<MUX0;

	ADMUX |= 1<<REFS1;
	ADMUX |= 1<<REFS0;

	ADCSRA |= 1<<ADEN;
}

u8 measureTemperature()
{
	///\debug adc
	ADCSRA |= 1<<ADSC;
	while (ADCSRA & (1<<ADIF))
	{
		
	}
	ADCSRA |= 1<<ADIF;
	u16 vin = ADCL;
	u16 vin2 = ADCH;
	vin += vin2<<8;
	return vin - 273;
}
