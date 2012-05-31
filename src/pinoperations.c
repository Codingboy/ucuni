#include "pinoperations.h"
#include "typedefs.h"
#include <avr/io.h>
#include <util/delay.h>

inline void setAsInput(u8 port, u8 pin)
{
	switch (port)
	{
		case 0:
			//PORTA = PORTA & ~(1<<pin);
			//DDRA = DDRA & ~(1<<pin);
			break;
		case 1:
			PORTB = PORTB & ~(1<<pin);
			DDRB = DDRB & ~(1<<pin);
			break;
		case 2:
			PORTC = PORTC & ~(1<<pin);
			DDRC = DDRC & ~(1<<pin);
			break;
		case 3:
			PORTD = PORTD & ~(1<<pin);
			DDRD = DDRD & ~(1<<pin);
			break;
	}
}

inline void setAsOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 0:
			//PORTA = PORTA | 1<<pin;
			//DDRA = DDRA | 1<<pin;
			break;
		case 1:
			PORTB = PORTB | 1<<pin;
			DDRB = DDRB | 1<<pin;
			break;
		case 2:
			PORTC = PORTC | 1<<pin;
			DDRC = DDRC | 1<<pin;
			break;
		case 3:
			PORTD = PORTD | 1<<pin;
			DDRD = DDRD | 1<<pin;
			break;
	}
}

inline void setOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 0:
			//PORTA = PORTA | 1<<pin;
			break;
		case 1:
			PORTB = PORTB | 1<<pin;
			break;
		case 2:
			PORTC = PORTC | 1<<pin;
			break;
		case 3:
			PORTD = PORTD | 1<<pin;
			break;
	}
}

inline void clearOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 0:
			//PORTA = PORTA & ~(1<<pin);
			break;
		case 1:
			PORTB = PORTB & ~(1<<pin);
			break;
		case 2:
			PORTC = PORTC & ~(1<<pin);
			break;
		case 3:
			PORTD = PORTD & ~(1<<pin);
			break;
	}
}

inline void toggleOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 0:
			//PORTA = PORTA ^ 1<<pin;
			break;
		case 1:
			PORTB = PORTB ^ 1<<pin;
			break;
		case 2:
			PORTC = PORTC ^ 1<<pin;
			break;
		case 3:
			PORTD = PORTD ^ 1<<pin;
			break;
	}
}

inline u8 getInput(u8 port, u8 pin)
{
	switch (port)
	{
		case 0:
			//return PINA & 1<<pin;
		case 1:
			return PINB & 1<<pin;
		case 2:
			return PINC & 1<<pin;
		case 3:
			return PIND & 1<<pin;
	}
	return 0;
}

inline u8 getOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 0:
			//return PORTA & 1<<pin;
		case 1:
			return PORTB & 1<<pin;
		case 2:
			return PORTC & 1<<pin;
		case 3:
			return PORTD & 1<<pin;
	}
	return 0;
}
