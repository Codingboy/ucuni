#include "pinoperations.h"
#include "typedefs.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

inline void setAsInput(u8 port, u8 pin)
{
	switch (port)
	{
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
		case 4:
			PORTE = PORTE & ~(1<<pin);
			DDRE = DDRE & ~(1<<pin);
			break;
		case 5:
			PORTF = PORTF & ~(1<<pin);
			DDRF = DDRF & ~(1<<pin);
			break;
	}
}

inline void setAsOutput(u8 port, u8 pin)
{
	switch (port)
	{
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
		case 4:
			PORTE = PORTE | 1<<pin;
			DDRE = DDRE | 1<<pin;
			break;
		case 5:
			PORTF = PORTF | 1<<pin;
			DDRF = DDRF | 1<<pin;
			break;
	}
}

inline void setOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 1:
			PORTB = PORTB | 1<<pin;
			break;
		case 2:
			PORTC = PORTC | 1<<pin;
			break;
		case 3:
			PORTD = PORTD | 1<<pin;
			break;
		case 4:
			PORTE = PORTE | 1<<pin;
			break;
		case 5:
			PORTF = PORTF | 1<<pin;
			break;
	}
}

inline void clearOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 1:
			PORTB = PORTB & ~(1<<pin);
			break;
		case 2:
			PORTC = PORTC & ~(1<<pin);
			break;
		case 3:
			PORTD = PORTD & ~(1<<pin);
			break;
		case 4:
			PORTE = PORTE & ~(1<<pin);
			break;
		case 5:
			PORTF = PORTF & ~(1<<pin);
			break;
	}
}

inline void toggleOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 1:
			PORTB = PORTB ^ 1<<pin;
			break;
		case 2:
			PORTC = PORTC ^ 1<<pin;
			break;
		case 3:
			PORTD = PORTD ^ 1<<pin;
			break;
		case 4:
			PORTE = PORTE ^ 1<<pin;
			break;
		case 5:
			PORTF = PORTF ^ 1<<pin;
			break;
	}
}

inline bool getInput(u8 port, u8 pin)
{
	switch (port)
	{
		case 1:
			return PINB & 1<<pin;
		case 2:
			return PINC & 1<<pin;
		case 3:
			return PIND & 1<<pin;
		case 4:
			return PINE & 1<<pin;
		case 5:
			return PINF & 1<<pin;
	}
	return 0;
}

inline bool getOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 1:
			return PORTB & 1<<pin;
		case 2:
			return PORTC & 1<<pin;
		case 3:
			return PORTD & 1<<pin;
		case 4:
			return PORTE & 1<<pin;
		case 5:
			return PORTF & 1<<pin;
	}
	return 0;
}
