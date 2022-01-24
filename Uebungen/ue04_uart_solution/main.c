#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

int main ()
{
    //TODO
    DDRB |= _BV(PB5);
    PORTB &= ~_BV(PB5);

    //Initialize the UART with the settings: 57600/8N1

    //TODO: Setup up the baudrate with 57600 bps -> configure the registers UBRR0H, UBRR0L and UCSR0A -> page 199-200
	//Note: Register UBRR0 is a 16-bit register. 
    //so the register UBRR0 ist splittet in two 8-Bit register UBRRH (high byte) and UBRRL (low byte)
    UBRR0H = 0;
    UBRR0L = 34;
	UCSR0A = 2;

    //TODO: configre the the asynchronous communcation with the data frame format 8N1 -> page 202, 201
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	//TODO enable the receiver and transmitter peripheral 
    UCSR0B = _BV(TXEN0) | _BV(RXEN0); //0x18; //0b00011000

    char text[100] = "Guten Morgen\n";
    for (uint8_t i=0; i < sizeof(text) && text[i] != 0; i++)
    {
        //Wait until the transmit buffer is ready to receive new data -> see register UCSR0A on page 200
        //
        while ( !( UCSR0A & (1 << UDRE0)) /*TODO: insert your condition*/ ) 
            ;
        //TODO: transmit the character
        UDR0 = text[i];
    }

    while (1)
    {
        // TODO:    add a condition to check if a character was received by the UART peripheral.
        //          E.g. the user pressed a key on the keyboard. 
        //          -> see corresponding bit in register UCSR0A -> page 200
        if ( UCSR0A & (1 << RXC0) /*add your condition here*/) 
        {
            char receivedKey = UDR0;
            switch (receivedKey)
            {
                case '1':   //turn on the onboard led on port PB5
                {
                    //The following text will be sent to the computer in a later step. 
                    strcpy(text, "LED switched on!\n");
                    //TODO: turn on the obnard led on port PB5
                    PORTB |= _BV(PB5);
                    break;
                }
                
                case '0':   //turn off the onboard led on port PB5
                {
                    //The following text will be sent to the computer in a later step. 
                    strcpy(text, "led switched off!\n");
                    //TODO: turn ff the obnard led on port PB5
                    PORTB &= ~_BV(PB5);
                    break;
                }

                default:
                {
                    //The following text will be sent to the computer in a later step. 
                    strcpy(text, "unknown command!\n");
                }

            }

            //A response is sent to the computer. 
            for (uint8_t i = 0; text[i] != 0; i++)
            {
                //Wait until the transmit buffer is ready to receive new data -> see register UCSR0A on page 200
                while ( !( UCSR0A & (1 << UDRE0)) /*TODO: insert your condition*/ ) 
                    ;
                UDR0 = text[i];
            }
        }
    }
    return 0;
}
