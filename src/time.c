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
	TCCR0A &= ~(1<<WGM2);
	TCCR0A &= ~(1<<WGM1);
	TCCR0A &= ~(1<<WGM0);

	//normal port operation for OC0A
	TCCR0A &= ~(1<<COM0A1);
	TCCR0A &= ~(1<<COM0A0);

	//normal port operation for OC0B
	TCCR0A &= ~(1<<COM0B1);
	TCCR0A &= ~(1<<COM0B0);

	//set prescaler to 64
	TCCR0 &= ~(1<<CS02);
	TCCR0 |= 1<<CS01;
	TCCR0 |= 1<<CS00;

	//preload
	TCNT0 = 230;//for 100µs

	//enable overflow interrupts
	TIMSK0 |= 1<<TOIE0;

	//enable interrupts
	sei();
}

void disableTime()
{
	u8 tmpSREG = SREG;
	cli();

	//disable overflow interrupt
	TIMSK0 &= ~(1<<TOIE0);

	SREG = tmpSREG;
}
