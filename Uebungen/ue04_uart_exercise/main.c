#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

int main ()
{
    //TODO: configue the onboard LED PB5 as output and clear de LED
    DDRB = 0;
    PORTB = 0;

    //Initialize the UART with the settings: 57600/8N1

    //TODO: Setup up the baudrate with 57600 bps -> configure the registers UBRR0H, UBRR0L and UCSR0A -> page 199-200
	//Note: Register UBRR0 is a 16-bit register. 
    //so the register UBRR0 ist splittet in two 8-Bit register UBRRH (high byte) and UBRRL (low byte)
    UBRR0H = 0;
    UBRR0L = 0;
	UCSR0A = 0;

    //TODO: configre the the asynchronous communcation with the data frame format 8N1 -> page 202, 201
	UCSR0C = 0;
	//TODO enable the receiver and transmitter peripheral 
    UCSR0B = 0;

    char text[100] = "Guten Morgen\n";
    for (uint8_t i=0; i < sizeof(text); i++)
    {
        //Wait until the transmit buffer is ready to receive new data -> see register UCSR0A on page 200
        //
        while ( 0 /*TODO: insert your condition*/ ) 
            ;
        //TODO: transmit the character
        UDR0 = 0;
    }

    while (1)
    {
        // TODO:    add a condition to check if a character was received by the UART peripheral.
        //          E.g. the user pressed a key on the keyboard. 
        //          -> see corresponding bit in register UCSR0A -> page 200
        if ( 0 /*add your condition here*/) 
        {
            char receivedKey = UDR0;
            switch (receivedKey)
            {
                case '1':   //turn on the onboard led on port PB5
                {
                    //The following text will be sent to the computer in a later step. 
                    strcpy(text, "LED switched on!\n");
                    //TODO: turn on the obnard led on port PB5
                    PORTB = 0;
                    break;
                }
                
                case '0':   //turn off the onboard led on port PB5
                {
                    //The following text will be sent to the computer in a later step. 
                    strcpy(text, "led switched off!\n");
                    //TODO: turn ff the obnard led on port PB5
                    PORTB = 0;
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
                while ( 0 /*TODO: insert your condition*/ ) 
                    ;
                //TODO: transmit the character
                UDR0 = 0;
            }
        }
    }
    return 0;
}
