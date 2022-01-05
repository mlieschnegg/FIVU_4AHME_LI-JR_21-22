#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

#define LED_PORT    PORTB
#define LED_DDR     DDRB
#define LED_PIN     PB5

typedef struct 
{
    char *cmd;
    void (*func)(char *);
} TerminalCMD;


//declare the function prototypes for the received commands
void execLEDOn(char *s);
void execLEDOff(char *s);
void execInfo(char *s);

//a list off all supported commands
TerminalCMD mycommands[] = {
    {
        "INFO?",
        execInfo
    },
    {
        "LED=ON",
        execLEDOn
    },
    {
        "LED=OFF",
        execLEDOff
    }
};


void execLEDOn(char *s)
{
    LED_PORT |= (1 << LED_PIN);
    UART_printText("LED was set to ON\n");
}

void execLEDOff(char *s)
{
    LED_PORT &= ~(1 << LED_PIN);
    UART_printText("LED was set to OFF\n");
}

void execInfo(char *s)
{
   UART_printText("This is the UART-Demo program\n"); 
}

uint8_t executeCommand(char *s)
{
    for(uint8_t i = 0, n=0; i < sizeof(mycommands)/sizeof(mycommands[0]); ++i)
    {
        n = strlen(mycommands[i].cmd);
        if (strncmp(s, mycommands[i].cmd, n) == 0)
        {
            if (mycommands[i].func != NULL)
                mycommands[i].func(s);
            return 0;
        }
    }
    return 1;
}
//TODO: 
//test the programm and use the serial port of your computer 
//to execute the some commands. (e.q. turn on or turn off the onbaord led)
int main()
{
    char s[UART_BUFFER_SIZE] = "";

    LED_DDR |= (1 << LED_PIN);
    LED_PORT &= ~(1 << LED_PIN);

    UART_initializeInterface(115200, PARITY_NO);

    UART_printText("Exercise 5 - UART/IRs and a simple terminal\n");
    while(1)
    {
        if (UART_getText(s) == UART_OK)
        {
            if (executeCommand(s))
                UART_printText("Invalid Command!\n");
        }
    }
    return 0;
}
