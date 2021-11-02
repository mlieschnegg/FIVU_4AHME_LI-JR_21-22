#include "global.h"
#include <avr/io.h>
#include <util/delay.h>

#define SWITCH_PRESSED_CONDITION	1	//define to be adapted

int main ()
 {
	char counter = 1;
	//Set PB1, PB2 and PB3 as OUTPUT

	//Enable internal pull-up on PB0 and make sure PB0 is set as input. Refer to chapter 14.4.1 of ATmega328P's datasheet for clarification.
	
	while (1)
	{
		if(SWITCH_PRESSED_CONDITION)
		{
			//Change counter in order to step through the LEDs on PB1, PB2 and PB3

			//Set the current counter value to PB1-PB3 on PORTB, without changing the state of any other pin on PORTB

			//Make sure counter is set back once PB3 was high

		}
	}
	
	return 0;
}