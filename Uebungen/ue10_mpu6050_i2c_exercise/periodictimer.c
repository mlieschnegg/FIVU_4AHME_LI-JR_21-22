#include "periodictimer.h"

#define TIMER0_PRESCALER        64
#define TIMER0_FREQUENCY        1000 // Hz

#define MAX_PERIODIC_TIMER_FUNCTION    3


volatile PERIODICTIMER_conf_t periodic_timers[MAX_PERIODIC_TIMER_FUNCTION];


// Timer 0 Output/Compare Interrupt
ISR (TIMER0_COMPA_vect)
{
    for (uint8_t i = 0; i < sizeof(periodic_timers)/sizeof(PERIODICTIMER_conf_t); ++i)
    {
        if ( (periodic_timers[i].func != NULL) && (periodic_timers[i].ms > 0))
        {
            periodic_timers[i].cntms = periodic_timers[i].cntms + 1;
            if ( periodic_timers[i].cntms == periodic_timers[i].ms )
            {
                periodic_timers[i].cntms = 0;
                periodic_timers[i].func();
            }
        }
    }
}

void PERIODICTIMER_initializeTimer()
{

    memset(periodic_timers, 0, sizeof(periodic_timers));

    OCR0A  = (F_CPU)/TIMER0_PRESCALER/TIMER0_FREQUENCY/2+0.5-1;
    
    TCCR0A = (1 << WGM01); //CTC-Mode TOP-Value = OCR0A
    TCCR0B = (1 << CS01) | (1 << CS00); //Prescaler = 64
    
    TIMSK0 = (1 << OCIE0A);
    TIFR0  = (1 << OCF0A);

    sei();
}


void PERIODICTIMER_startTimer(PERIODICTIMER_func_t func, uint16_t ms)
{
    //existing timer
    for (uint8_t i = 0; i < sizeof(periodic_timers)/sizeof(PERIODICTIMER_conf_t); ++i)
    {
        if (periodic_timers[i].func == func)
        {
            periodic_timers[i].ms = ms;
            periodic_timers[i].cntms = 0;
            return;
        }
    }
    //new timer
    for (uint8_t i = 0; i < sizeof(periodic_timers)/sizeof(PERIODICTIMER_conf_t); ++i)
    {
        if (periodic_timers[i].func == NULL)
        {
            periodic_timers[i].ms = ms;
            periodic_timers[i].cntms = 0;
            periodic_timers[i].func = func;
            return;
        }
    }
}
void PERIODICTIMER_stopTimer(PERIODICTIMER_func_t func)
{
    //delete existing timer
    for (uint8_t i = 0; i < sizeof(periodic_timers)/sizeof(PERIODICTIMER_conf_t); ++i)
    {
        if (periodic_timers[i].func == func)
        {
            periodic_timers[i].func = NULL;
            periodic_timers[i].ms = 0;
            return;
        }
    }

}
