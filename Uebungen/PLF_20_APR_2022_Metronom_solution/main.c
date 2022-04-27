//***********************************************************************
// AIIT Template Level 1 for Arduino Nano (Atmega328P 16MHz)
// ----------------------------------------------------------------------
// Description: PLF_20_APR_2022_Metronom Jerman/Lieschnegg
// ----------------------------------------------------------------------
// Created: 19. April 2022
//***********************************************************************

#include "global.h"
#include <avr/interrupt.h>

#define TIMER0_PRESCALER                64                                                                                                                              //1
#define TIMER0_FREQUENCY                1000 //Hz                                                                                                                       //1

#define PWM_PERIOD_MS                   20                                  //SERVO-PWM-period in milli seconds                                                         //1
#define TIMER1_PRESCALER                8                                   //prescaler value                                                                           //1
#define TIMER1_CLOCKS_PER_S             F_CPU/TIMER1_PRESCALER              //prescaled timer clocks per second                                                         //2
#define TIMER1_CLOCKS_PER_MS            TIMER1_CLOCKS_PER_S/1000                                                                                                        //2
#define TIMER1_CLOCKS_PER_PWM_PERIOD    PWM_PERIOD_MS*TIMER1_CLOCKS_PER_MS                                                                                              //2

#define POS_LEFT                        1.0*TIMER1_CLOCKS_PER_MS-1
#define POS_RIGHT                       2.0*TIMER1_CLOCKS_PER_MS-1

unsigned int timer0_counter = 0;

int main ()
{
    //CTC-MODE FOR METRONOM-TIMING USING TIMER0
    OCR0A  = (F_CPU)/TIMER0_PRESCALER/TIMER0_FREQUENCY+0.5-1;                                                                                                           //4
    TCCR0A = (1 << WGM01);                                                   //CTC-Mode TOP-Value = OCR0A                                                               //1
    TCCR0B = (1 << CS01) | (1 << CS00);                                      //Prescaler = 64                                                                           //2    
    TIMSK0 = (1 << OCIE0A);                                                                                                                                             //1
    TIFR0  = (1 << OCF0A);                                                                                                                                              //1

    //FAST PWM FOR SERVO-PWM USING TIMER1
    DDRB |= (1 << PB1);                                                     //set PB1 for OC1A to output mode                                                           //1
    TCCR1A = (1 << COM1A1) | (1 << WGM11);                                  //Clear OC1A on compare match, set OC1A at BOTTOM value of TCNT1 (non-inverting mode)       //2
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);                     //Fast PWM in Mode 14, prescaler = 8                                                        //3
    OCR1A = POS_LEFT;                                                       //Compare value for pulse width equal to servo middle position                              //1
    ICR1 = TIMER1_CLOCKS_PER_PWM_PERIOD;                                    //Top value to set pwm-period                                                               //1

    //ENABLING GLOBAL INTERRUPTS
    SREG |= 0x80;                                                                                                                                                       //1

    while (1);
}

ISR(TIMER0_COMPA_vect)
{
  if(++timer0_counter == TIMER0_FREQUENCY)                                                                                                                              //3
  {
    timer0_counter = 0;                                                                                                                                                 //1
    if(OCR1A == POS_LEFT)   OCR1A = POS_RIGHT;                                                                                                                          //5
    else                    OCR1A = POS_LEFT;                                                                                                                           //3
  }
}
