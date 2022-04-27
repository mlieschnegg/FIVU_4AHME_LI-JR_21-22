//***********************************************************************
// AIIT Template Level 1 for Arduino Nano (Atmega328P 16MHz)
// ----------------------------------------------------------------------
// Description: PLF_20_APR_2022_Metronom Jerman/Lieschnegg
// ----------------------------------------------------------------------
// Created: 19. April 2022
//***********************************************************************

#include "global.h"
#include <avr/interrupt.h>

#define TIMER0_PRESCALER                ?
#define TIMER0_FREQUENCY                ?   //Hz

#define PWM_PERIOD_MS                   ?   //SERVO-PWM-period in milli seconds
#define TIMER1_PRESCALER                ?  
#define TIMER1_CLOCKS_PER_S             ?   //prescaled timer clocks per second
#define TIMER1_CLOCKS_PER_MS            ?
#define TIMER1_CLOCKS_PER_PWM_PERIOD    ?

#define POS_LEFT                        1.0*TIMER1_CLOCKS_PER_MS-1
#define POS_RIGHT                       2.0*TIMER1_CLOCKS_PER_MS-1

unsigned int timer0_counter = 0;

int main ()
{
    //CTC-MODE FOR METRONOM-TIMING USING TIMER0
    OCR0A  = ?;
    TCCR0A = ?;                         //CTC-Mode TOP-Value = OCR0A
    TCCR0B = ?;                         //Prescaler = 64
    TIMSK0 = ?;
    TIFR0  = ?;

    //FAST PWM FOR SERVO-PWM USING TIMER1
    DDRB |= ?;                          //set PB1 for OC1A to output mode
    TCCR1A = ?;                         //Clear OC1A on compare match, set OC1A at BOTTOM value of TCNT1 (non-inverting mode)
    TCCR1B = ?;                         //Fast PWM in Mode 14, prescaler = 8
    OCR1A = ?;                          //Compare value for pulse width equal to servo middle position
    ICR1 = ?;                           //Top value to set pwm-period

    //ENABLING GLOBAL INTERRUPTS
    ?;

    while (1);
}

ISR(TIMER0_COMPA_vect)
{
  ?;
}
