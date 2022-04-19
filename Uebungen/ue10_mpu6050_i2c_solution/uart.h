
//include guards
#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define UART_BUFFER_SIZE        100
#define UART_DELIMITER          '\n'

//#define BAUD_PRESCALE(bps)  (((F_CPU / (bps * 8UL))) - 1)
//#define BAUD_PRESCALE(bps) ((((F_CPU / 16) + (bps / 2)) / ( bps )) - 1)

#define UART_calc2XPrescaler(bps)  ((uint16_t) ((((F_CPU / (bps * 8UL))) - 1) + 0.5))

typedef enum 
{
    UART_OK,
    UART_ERR
} UARTResult;

typedef enum
{
    PARITY_NO = 0,
    PARITY_EVEN = 2,
    PARITY_ODD = 3
} UARTParity;

UARTResult UART_initializeInterface(uint32_t bps, UARTParity parity);

UARTResult UART_getCharBuffer(char *c, uint8_t clearbuffer);

UARTResult UART_getChar(char *c);
UARTResult UART_printChar(char c);

UARTResult UART_getText(char *s);
UARTResult UART_printText( char *s );
#endif //UART_H_