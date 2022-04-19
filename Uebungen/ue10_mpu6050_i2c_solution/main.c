#include "global.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mpu6050.h"
#include "uart.h"
#include "periodictimer.h"


typedef struct 
{
    char *cmd;
    void (*func)(char *);
} TerminalCMD_t;

volatile uint8_t exec_new_calculation_ = 0;

//callbacks for the periodic timer module
void calcTiltAngle(void)
{
    exec_new_calculation_ = 1;
}

//callbacks for received commands
void execTerminalStartMeasurement(char *s)
{
    PERIODICTIMER_startTimer(&calcTiltAngle, 1000);
}

void execTerminalStopMeasurement(char *s)
{
    PERIODICTIMER_stopTimer(&calcTiltAngle);
}

//a list off all supported commmands
TerminalCMD_t mycommands[] = {
    {
        "start",
        execTerminalStartMeasurement
    },
    {
        "stop",
        execTerminalStopMeasurement
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
            {
                
                mycommands[i].func(&s[n]);
            }
            return 0;
        }
    }
    return 1;
}

int main()
{
    char s[UART_BUFFER_SIZE] = "";

    PERIODICTIMER_initializeTimer();
    MPU6050_initializeSensor();
    UART_initializeInterface(115200, PARITY_NO);

    UART_printText("Exercise 10 - calculation of tilt angle\n");
    
    while(1)
    {
        if (UART_getText(s) == UART_OK)
        {
            
            if (executeCommand(s))
                UART_printText("Invalid Command!\n");
        }
        if (exec_new_calculation_)
        {
            exec_new_calculation_ = 0;
            
            MPU6050_readSensorData();
            
            sprintf(s, "%2.2lf,%2.2lf,%2.2lf,%3.3lf,%3.3lf,%3.3lf,%2.1lf %2.1lf\n", 
                        MPU6050_getACCX(), MPU6050_getACCY(), MPU6050_getACCZ(),
                        MPU6050_getGYRX(), MPU6050_getGYRY(), MPU6050_getGYRZ(),
                        MPU6050_getTemp(), 90.0-acos(MPU6050_getACCY())*(180/M_PI));
            
            UART_printText(s);
        }
    }
    return 0;
}
