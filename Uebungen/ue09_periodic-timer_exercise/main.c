#include "global.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include "periodictimer.h"


#define LED_PORT    PORTB
#define LED_DDR     DDRB
#define LED_PIN     PB5


typedef struct 
{
    char *cmd;
    void (*func)(char *);
} TerminalCMD_t;


//callbacks for the periodic timer module
void blinkYellowLED(void)
{
    LED_PORT ^= (1 << LED_PIN);
}

//callbacks for received commands
void execLEDFrequency(char *s)
{


    uint16_t ms = 0;

    //ToDo: Parse the milliseconds from the serial command s which was send by the UART 
    // and convert the ascii string to a number!
    ms = 0;

    PERIODICTIMER_startTimer(&blinkYellowLED, ms);
}

//a list off all supported commmands
TerminalCMD_t mycommands[] = {
    {
        "LEDFREQ=",
        execLEDFrequency
    }
};

uint8_t executeCommand(char *s)
{
    for(uint8_t i = 0, n=0; i < sizeof(mycommands)/sizeof(mycommands[0]); ++i)
    {
        n = strlen(mycommands[i].cmd);
        if (strncmp(s, mycommands[i].cmd, n) == 0)
        {
            if (mycommands[i].func != NULL)
                mycommands[i].func(&s[n]);
            return 0;
        }
    }
    return 1;
}

int main()
{
    char s[UART_BUFFER_SIZE] = "";

    LED_DDR |= (1 << LED_PIN);
    LED_PORT &= ~(1 << LED_PIN);

    PERIODICTIMER_initializeTimer();
    UART_initializeInterface(115200, PARITY_NO);

    UART_printText("Exercise 8 - Periodic Timer\n");
    
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
