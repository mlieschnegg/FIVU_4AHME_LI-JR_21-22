#include "global.h"
#include <avr/io.h>
#include <util/delay.h>

#define COUNTER_START_VALUE		1	//define to be adapted
#define COUNTER_END_VALUE		1	//define to be adapted
#define COUNTER_CHANGE_FACTOR	1	//define to be adapted

void runLEDsEasy();
void runLEDsCoolDigga();
void runLEDsCoolerNoFrontDigga();

int main ()
 {
	//Set PB1, PB2 and PB3 as OUTPUT


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
	//Set PB1 on PORTB high, without changing the state of any other pin on PORTB

	//wait 250 ms

	//Reset PB1 on PORTB, without changing the state of any other pin on PORTB

	//Set PB2 on PORTB high, without changing the state of any other pin on PORTB

	//wait 250 ms

	//Reset PB2 on PORTB, without changing the state of any other pin on PORTB

	//Set PB3 on PORTB high, without changing the state of any other pin on PORTB

	//wait 250 ms

	//Reset PB3 on PORTB, without changing the state of any other pin on PORTB
}

void runLEDsCoolDigga()
{
	//Reset PB3 and set PB1 on PORTB high using only one line of code, without changing the state of any other pin on PORTB

	//wait 250 ms

	//Reset PB1 and set PB2 on PORTB high using only one line of code, without changing the state of any other pin on PORTB

	//wait 250 ms

	//Reset PB2 and set PB3 on PORTB high using only one line of code, without changing the state of any other pin on PORTB

	//wait 250 ms
}

void runLEDsCoolerNoFrontDigga()
{
	char counter;
	for(counter = COUNTER_START_VALUE; counter < COUNTER_END_VALUE; counter *= COUNTER_CHANGE_FACTOR)
	{
		//Set the current counter value to PB1-PB3 on PORTB, without changing the state of any other pin on PORTB

		//wait 250 ms
	}

}