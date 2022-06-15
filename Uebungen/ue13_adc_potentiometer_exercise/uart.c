#include "uart.h"

char gl_uart_rxbuffer[UART_BUFFER_SIZE + 1] = "";
char gl_uart_txbuffer[UART_BUFFER_SIZE + 1] = "";

volatile uint8_t gl_uart_rxbuffer_pos = 0;
volatile uint8_t gl_uart_txbuffer_pos = 0;
volatile uint8_t gl_uart_str_complete = 0;

ISR (USART_RX_vect)
{
    uint8_t c = UDR0;

    //received text was not proecessed by the application -> reset the buffer
    if (gl_uart_str_complete) 
    {
        gl_uart_rxbuffer_pos = 0;
        gl_uart_str_complete = 0;
    }
    //copy received character to the internal receive buffer
    if ( gl_uart_rxbuffer_pos < UART_BUFFER_SIZE)
        gl_uart_rxbuffer[gl_uart_rxbuffer_pos++] = c;
    
    //termination for the string was received
    if (c == UART_DELIMITER)
    {
        gl_uart_rxbuffer[gl_uart_rxbuffer_pos-1] = '\0';
        gl_uart_rxbuffer_pos = 0;
        gl_uart_str_complete = 1;
    }
}


//TODO:
//Implement the Interrupt Service Routine of the USART Data Register Empty Interrupt
//chose the correct interrupt service vector and send the characters which are stored 
//in the transmit buffer (gl_uart_txbuffer_pos)
//Deactivate the interrupt source as soon as all characters have been sent. 
ISR (USART_UDRE_vect)
{
    char c = gl_uart_txbuffer[gl_uart_txbuffer_pos];
        
    if (c != '\0')
    {
        //write the charcter to the uart peripheral
        UDR0 = c;
        gl_uart_txbuffer_pos++;
    }
    //deactivate the data register empty interrupt, if the complete text has been sent
    else
        UCSR0B &= ~(1 << UDRIE0);
}

UARTResult UART_initializeInterface(uint32_t bps, UARTParity parity)
{
    //disable gloabl interrupt
    cli(); //is equivalent to  SREG |= SREG_I;
    
    //set baud rate
    //TODO: Set the baud rate and calcuate the value for the baud rate register
    //use the function argument bps for the calculation.
    //The formula for the calculation can be found on page 182
    uint16_t psc_value = UART_calc2XPrescaler(bps);
    UBRR0H = (psc_value >> 8) & 0xFF;
    UBRR0L = psc_value & 0xFF;

    //double baudrate
    UCSR0A |= (1 << U2X0);

    //asynchronous mode
    UCSR0C &= ~((1 << UMSEL01) | (1 << UMSEL00));
    
    //no paritiy
    UCSR0C &= ~((1 << UPM01) | (1 << UPM00));
    UCSR0C |= parity;

    //1 Stopp-Bit
    UCSR0C &= ~(1 << USBS0);

    //8 Bit-Daten
    UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00));
    UCSR0B &= ~(1 << UCSZ02);

    //enable transmitter and receiver
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

    //RX Complete Interrupt Enable
    UCSR0B |= (1 << RXCIE0);

    //USART Data Register Empty Interrupt Enable
    //UCSR0B |= (1 << UDRIE0);

    //enable global interrupt
    sei(); // is equivalent to  SREG |= SREG_I;

    return UART_OK;
    
}

UARTResult UART_getChar(char *c)
{
    //check if a character was received
    if ( UCSR0A & (1 << RXC0) )
        *c = UDR0;
    else
        return UART_ERR;
    return UART_OK;
}

UARTResult UART_printChar(char c)
{
    while( !(UCSR0A & (1 << UDRE0)) )
        ;
    UDR0 = c;
    return UART_OK;
}


UARTResult UART_getCharBuffer(char *c, uint8_t clearbuffer)
{
    if (gl_uart_rxbuffer_pos == 0)
        return UART_ERR;
    
    *c = gl_uart_rxbuffer[gl_uart_rxbuffer_pos];
    if (clearbuffer)
        gl_uart_rxbuffer_pos = 0;

    return UART_OK;
}

UARTResult UART_getText(char *s)
{
    if (gl_uart_str_complete)
    {
        strcpy(s, gl_uart_rxbuffer);
        gl_uart_str_complete = 0;
        gl_uart_rxbuffer_pos = 0;
        return UART_OK;
    }
    s[0] = '\0';
    return UART_ERR;

}

UARTResult UART_printText( char *s )
{
    uint8_t i = 0;

    //TODO:
    //Wait until the Data Register Empty Interrupt is deactivated by the isr
    //-> the transceiver is now ready to send a new text
    while(UCSR0B & (1 << UDRIE0))
        ;

    while(!(UCSR0A & (1 << UDRE0)))
        ;
    
    //it can be cleared by writing a one to its bit location
    UCSR0A |= (1 << TXC0);

    //TODO: copy the string s to the internal transmit buffer gl_uart_txbuffer
    for(i = 0; s[i] != '\0'; ++i)
        gl_uart_txbuffer[i] = s[i];
    gl_uart_txbuffer[i] = '\0';
    gl_uart_txbuffer_pos = 0;

    //TODO:
    //enable the Data Register Empty Interrupt to start the transmission of the text
    UCSR0B |= (1 << UDRIE0);

    return UART_OK;
}
