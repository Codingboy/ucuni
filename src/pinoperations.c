#include "pinoperations.h"
#include "typedefs.h"
#include <avr/io.h>
#include <util/delay.h>

u16 adcwert(u8 kanal);

inline void setAsInput(u8 port, u8 pin)
{
	switch (port)
	{
		case 0:
			PORTA = PORTA & ~(1<<pin);
			DDRA = DDRA & ~(1<<pin);
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
			PORTA = PORTA | 1<<pin;
			DDRA = DDRA | 1<<pin;
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
			PORTA = PORTA | 1<<pin;
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
			PORTA = PORTA & ~(1<<pin);
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
			PORTA = PORTA ^ 1<<pin;
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
			return PINA & 1<<pin;
		case 1:
			return PINB & 1<<pin;
		case 2:
			return PINC & 1<<pin;
		case 3:
			return PIND & 1<<pin;
	}
	return 0;
}

//übernommen von http://www.rn-wissen.de/index.php/RN-Control_Demoprogramm_in_C
u8 getInputADC(u8 port, u8 pin)
{
	uint8_t taste = 0; 	//Variable für Nummer des Tasters
	uint16_t analog7 = adcwert(7);	//Wert des Ports

	PORTA |= (1<<7);		//Ohne das hier "flackern" die Werte aus irgend einem Grund -> es werden mitunter Tasten erkannt, die gar nicht gedrückt wurden oder das Programm bleibt für einige Sekunden "hängen"
	_delay_ms(1);
	PORTA &= ~(1<<7);

	//Abfrage des gedrückten Tasters - um Störungen zu vermeiden wurden
        //die Bereiche sehr eng gefasst, sollten bei Bedarf an jedes Board extra angepasst werden.
	if((analog7>=337) && (analog7<=343)) {taste = 1;}
	else if((analog7>=268) && (analog7<=274)) {taste = 2;}
	else if((analog7>=200) && (analog7<=206)) {taste = 3;}
	else if((analog7>=132) && (analog7<=138)) {taste = 4;}
	else if((analog7>=64) && (analog7<=70)) {taste = 5;}
	else {}

	return taste;
}

inline u8 getOutput(u8 port, u8 pin)
{
	switch (port)
	{
		case 0:
			return PORTA & 1<<pin;
		case 1:
			return PORTB & 1<<pin;
		case 2:
			return PORTC & 1<<pin;
		case 3:
			return PORTD & 1<<pin;
	}
	return 0;
}

//übernommen von http://www.rn-wissen.de/index.php/RN-Control_Demoprogramm_in_C
u16 adcwert(u8 kanal)
{
	u16 wert = 0; //Variable für Ergebnis deklarieren

	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	//ADEN aktiviert überhaupt erst den internen ADC-Wandler, ADPS2 bis ADPS0 stellen den verwendeten Prescaler ein, denn die Wandlerfrequenz muss immer zwischen 50 und 200 kHz liegen! Der Prescaler muss bei 16MHz also zwischen 80 und 320 eingestellt werden, als einzige Möglichkeit bleibt hier 128 (=alle auf 1).

	ADMUX = kanal;
	//ADMUX = (1<<REFS1)|(1<<REFS0); //Einstellen der Referenzspannung auf "extern", also REFS1 und REFS0 auf "0" - daher auskommentierte Zeile

	ADCSRA |= (1<<ADSC);	//nach Aktivierung des ADC wird ein "Dummy-Readout" empfohlen, man liest also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen"
    while (ADCSRA & (1<<ADSC)) {} //auf Abschluss der Konvertierung warten
	wert = ADCW;	//ADCW muss einmal gelesen werden, sonst wird Ergebnis der nächsten Wandlung nicht übernommen.

	/* Eigentliche Messung - Mittelwert aus 4 aufeinanderfolgenden Wandlungen */
	wert = 0;
	for (u8 i=0; i<4; i++)
	{
		ADCSRA |= (1<<ADSC); 	//eine Wandlung "single conversion" starten
		while(ADCSRA & (1<<ADSC)) {} 	//auf Abschluss der Konvertierung warten
		wert = wert + ADCW;	 //Wandlungsergebnisse aufaddieren
    }

	ADCSRA &= ~(1<<ADEN);	//ADC deaktivieren

	wert = wert/4;		//Durchschnittswert bilden

	return wert;
}
