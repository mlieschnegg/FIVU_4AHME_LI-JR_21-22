#include "global.h"
#include <avr/io.h>
#include <util/delay.h>

void runLEDsEasy();
void runLEDsCoolDigga();
void runLEDsCoolerNoFrontDigga();

int main ()
 {
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);

	while (1)
	{
		runLEDsEasy();
		runLEDsCoolDigga();
		runLEDsCoolerNoFrontDigga();
	}
	
	return 0;
}

void runLEDsEasy()
{
	PORTB &= ~(1 << PB3);
	PORTB |= (1 << PB1);
	_delay_ms(250);
	PORTB &= ~(1 << PB1);
	PORTB |= (1 << PB2);
	_delay_ms(250);
	PORTB &= ~(1 << PB2);
	PORTB |= (1 << PB3);
	_delay_ms(250);
}

void runLEDsCoolDigga()
{
	PORTB = (PORTB & ~(1 << PB3)) | (1 << PB1);
	_delay_ms(250);
	PORTB = (PORTB & ~(1 << PB1)) | (1 << PB2);
	_delay_ms(250);
	PORTB = (PORTB & ~(1 << PB2)) | (1 << PB3);
	_delay_ms(250);
}

void runLEDsCoolerNoFrontDigga()
{
	char counter = 0;
	for(counter = (1 << PB1); counter <= (1 << PB3); counter <<= 1)
	{
		PORTB = (PORTB & ~((1 << PB1) | (1 << PB2) | (1 << PB3))) | counter;
		_delay_ms(250);
	}
}