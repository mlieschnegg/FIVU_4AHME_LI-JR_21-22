#include "global.h"
#include <avr/io.h>
#include <util/delay.h>

int main ()
 {
	char counter = 1;
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
	PORTB |= (1 << PB0);	//enabling internal PULL-UP

	while (1)
	{
		if(!(PINB & (1 << PB0)))
		{
			while(!(PINB & (1 << PB0)));
			counter <<= 1;
			PORTB = (PORTB & ~((1 << PB1) | (1 << PB2) | (1 << PB3))) | counter;
			if(counter == (1 << PB3))	counter = 1;
			_delay_ms(200);
		}
	}
	
	return 0;
}