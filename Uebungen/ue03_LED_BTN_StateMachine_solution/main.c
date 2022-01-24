#include "global.h"
#include <avr/io.h>
#include <util/delay.h>

#define BTN					!(PINB & (1 << PB0))
#define BTN_RELEASED		0
#define BTN_PRESSED			1
#define BTN_LOCKED			2
#define BTN_LOCKED_LONG		3
#define BTN_BOUNCE_DELAY	10000

#define LED_OFF				0
#define LED_ON				1
#define LED_PULSE_WIDTH		250000

int main ()
 {
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
	PORTB |= (1 << PB0);	//enabling internal PULL-UP

	char btn_state = BTN_RELEASED;
	unsigned long btn_counter = 0;

	char led_state = LED_OFF;
	unsigned long led_counter = 0;

	while (1)
	{
		//LED State Machine
		switch(led_state)
		{
			case LED_OFF:			if(led_counter++ == LED_PULSE_WIDTH)
									{
										PORTB |= (1 << PB3);
										led_counter = 0;
										led_state = LED_ON;
									}
									break;

			case LED_ON:			if(led_counter++ == LED_PULSE_WIDTH)
									{
										PORTB &= ~(1 << PB3);
										led_counter = 0;
										led_state = LED_OFF;
									}
		}

		//Button State Machine
		switch(btn_state)
		{
			case BTN_RELEASED:		if(BTN == BTN_PRESSED)
									{
										btn_counter = 0;
										btn_state = BTN_PRESSED;
									}
									break;

			case BTN_PRESSED:		if(BTN == BTN_RELEASED)	btn_state = BTN_RELEASED;
									else if(btn_counter++ == BTN_BOUNCE_DELAY)
									{
										if(PORTB & (1 << PB1))	PORTB &= ~(1 << PB1);
										else					PORTB |= (1 << PB1);
										btn_counter = 0;
										btn_state = BTN_LOCKED;
									}
							
			case BTN_LOCKED:		if(BTN == BTN_RELEASED)	btn_state = BTN_RELEASED;
									else if(btn_counter++ == LED_PULSE_WIDTH * 2)
									{
										if(PORTB & (1 << PB2))	PORTB &= ~(1 << PB2);
										else					PORTB |= (1 << PB2);
										btn_state = BTN_LOCKED_LONG;
									}
									break;

			case BTN_LOCKED_LONG:	if(BTN == BTN_RELEASED)	btn_state = BTN_RELEASED;
		}
	}
	return 0;
}