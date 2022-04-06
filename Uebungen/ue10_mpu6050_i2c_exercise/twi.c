#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "twi.h"

volatile struct TWI_DataPacket gl_twi_data_packet = { .uc_address_rw = 0, .uc_tx_len = 0, .uc_rx_len = 0, .uc_txrx_counter = 0, .uc_data = NULL};

volatile uint8_t gl_uc_twi_busy = 0;

ISR (_VECTOR(0) /*TODO: replace _VECTOR(0) with the correct Interrupt vector*/)
{
    switch(TWSR & 0xF8)
    {
        case TWI_MTM_SCODE_START:               //A START condition has been transmitted
        case TWI_MTM_SCODE_RSTART:              //A repeated START condition has been transmitted
            //reset transmission counter (new transmission).
            gl_twi_data_packet.uc_txrx_counter = 0;
            //Load SLA_W into TWDR Register.
            TWDR = gl_twi_data_packet.uc_address_rw;
            //Clear TWINT bit in TWCR to start transmission of address
            //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
        case TWI_MTM_SCODE_SLAW_ACK:            //SLA+W has been transmitted. ACK has been received
            TWDR = gl_twi_data_packet.uc_data[gl_twi_data_packet.uc_txrx_counter];
            gl_twi_data_packet.uc_txrx_counter++;
            //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
        case TWI_MTM_SCODE_SLAW_NACK:           //SLA+W has been transmitted. NOT ACK has been received
            //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
        case TWI_MTM_SCODE_DATA_ACK:            //Data byte has been transmitted. ACK has been received
            if (gl_twi_data_packet.uc_txrx_counter < gl_twi_data_packet.uc_tx_len)
            {
                TWDR = gl_twi_data_packet.uc_data[gl_twi_data_packet.uc_txrx_counter];
                gl_twi_data_packet.uc_txrx_counter++;
                //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            }
            else if (gl_twi_data_packet.uc_rx_len > 0)
            {
                gl_twi_data_packet.uc_address_rw |= 0x01; //set up receive adress
                //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            }
            else
                ;//TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
        case TWI_MTM_SCODE_DATA_NACK:           //Data byte has been transmitted. NACK has been received
            //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
        case TWI_MTM_SCODE_ARBITRATION_LOST:    //Arbitration lost in SLA+W or data bytes
            break;

        case TWI_MRM_SCODE_SLAR_ACK:            //TWI_SLA+R has been transmitted. ACK has been received
            if (gl_twi_data_packet.uc_rx_len > 1)
                //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            else
                //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
        case TWI_MRM_SCODE_SLAR_NACK:           //TWI_SLA+R has been transmitted. NACK has been received
            //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
        case TWI_MRM_SCODE_DATA_ACK:            //Data byte has been received. ACK has been returned
            gl_twi_data_packet.uc_data[gl_twi_data_packet.uc_txrx_counter] = TWDR;
            gl_twi_data_packet.uc_txrx_counter++;
            if (gl_twi_data_packet.uc_txrx_counter < (gl_twi_data_packet.uc_rx_len - 1))
                ;//TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            else
                ;//TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
        case TWI_MRM_SCODE_DATA_NACK:           //Data byte has been received. NACK has been returned
            gl_twi_data_packet.uc_data[gl_twi_data_packet.uc_txrx_counter] = TWDR;
            gl_twi_data_packet.uc_txrx_counter++;
            //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
        default: 
            //TODO: add the correct TWI command: TWI_sendSTART(), TWI_sendSTOP(), TWI_clearINT(), TWI_sendACK(), TWI_sendNACK()
            break;
    }
    
}

void TWI_initializeTWI(void)
{
    cli();

    //ToDo: Initialize TWI with 400 kHz and enable the TWI Interrupt 
    
    sei();

}

void TWI_txBytes( uint8_t address, uint8_t *data, uint8_t tx_len)
{

    gl_twi_data_packet.uc_address_rw = address << 1;
    gl_twi_data_packet.uc_data = data;
    gl_twi_data_packet.uc_tx_len = tx_len;
    gl_twi_data_packet.uc_rx_len = 0;

    TWI_sendSTART();
}

void TWI_txrxBytes( uint8_t address, uint8_t *data, uint8_t tx_len, uint8_t rx_len)
{

    gl_twi_data_packet.uc_address_rw = address << 1;
    gl_twi_data_packet.uc_data = data;
    gl_twi_data_packet.uc_tx_len = tx_len;
    gl_twi_data_packet.uc_rx_len = rx_len;

    TWI_sendSTART();
}

void TWI_rxBytes( uint8_t address, uint8_t *data, uint8_t rx_len)
{

    gl_twi_data_packet.uc_address_rw = (address << 1) | 0x01;
    gl_twi_data_packet.uc_data = data;
    gl_twi_data_packet.uc_tx_len = 0;
    gl_twi_data_packet.uc_rx_len = rx_len;

    TWI_sendSTART();
}

void TWI_waitUntilIdle(void)
{
    
    //Fix Me: add a timeout!!!!
    //while (TWCR  & (1<<TWEN)) 
    while(gl_uc_twi_busy > 0)
        ;
    _delay_ms(2);
    
}