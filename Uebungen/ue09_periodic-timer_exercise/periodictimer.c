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

                //ToDo:
                //call the corresponding timer function which was set by the user
                
            }
        }
    }
}

void PERIODICTIMER_initializeTimer()
{

    // ToDo: Comment the following line of code 
    memset(periodic_timers, 0, sizeof(periodic_timers));

    // TODO: Initialize Timer 0 of the uC with folloing settings
    // Mode: CTC Mode
    // Enable OCRA Interrupt: Yes
    // Use the defines TIMER0_PRESCALER and TIMER0_FREQUENCY to set timer period

    //ToDO: enable the global interrupt
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
            return;
        }
    }

}
