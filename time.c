#include "time.h"
#include <avr/interrupt.h>
#include <avr/io.h>

void setTime(u64 time)
{
	u8 tmpSREG = SREG;
	cli();
	_time = time;
	SREG = tmpSREG;
}

u64 getTime()
{
	u8 tmpSREG = SREG;
	cli();
	u64 ret = _time;
	SREG = tmpSREG;
	return ret;
}

void enableTime()
{
	//disable interrupts
	cli();

	//normal mode
	TCCR0 &= ~(1<<WGM01);
	TCCR0 &= ~(1<<WGM00);

	//normal port operation for OC0
	TCCR0 &= ~(1<<COM01);
	TCCR0 &= ~(1<<COM00);

	//set prescaler to 64
	TCCR0 &= ~(1<<CS02);
	TCCR0 |= 1<<CS01;
	TCCR0 |= 1<<CS00;

	//preload
	TCNT0 = 230;

	//enable overflow interrupts
	TIMSK |= 1<<TOIE0;

	//enable interrupts
	sei();
}

void disableTime()
{
	u8 tmpSREG = SREG;
	cli();

	//disable overflow interrupt
	TIMSK &= ~(1<<TOIE1);

	//used no clock
	TCCR0 &= ~(1<<CS12);
	TCCR0 &= ~(1<<CS11);
	TCCR0 &= ~(1<<CS10);

	SREG = tmpSREG;
}
