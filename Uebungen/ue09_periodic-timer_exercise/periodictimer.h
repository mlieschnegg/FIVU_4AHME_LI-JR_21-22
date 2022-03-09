#ifndef PERIODICTIMER_H_
#define PERIODICTIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*PERIODICTIMER_func_t)(void);

typedef struct 
{
    uint16_t ms;
    uint16_t cntms;
    PERIODICTIMER_func_t func;
} PERIODICTIMER_conf_t;


void PERIODICTIMER_startTimer(PERIODICTIMER_func_t func, uint16_t ms);
void PERIODICTIMER_stopTimer(PERIODICTIMER_func_t func);

#endif //PERIODICTIMER_H_