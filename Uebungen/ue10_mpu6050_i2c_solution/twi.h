#ifndef _TWI_H_
#define _TWI_H_


#include "global.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_TWI                   400000                          //Hz
#define TWI_PRESCALER           1
#define TWI_BUFFER_SIZE         20


//Page 227 - Status Codes
#define TWI_MTM_SCODE_START             0x08                            //A START condition has been transmitted
#define TWI_MTM_SCODE_RSTART            0x10                            //A repeated START condition has been transmitted
#define TWI_MTM_SCODE_SLAW_ACK          0x18                            //SLA+W has been transmitted. ACK has been received
#define TWI_MTM_SCODE_SLAW_NACK         0x20                            //SLA+W has been transmitted. NOT ACK has been received
#define TWI_MTM_SCODE_DATA_ACK          0x28                            //Data byte has been transmitted. ACK has been received
#define TWI_MTM_SCODE_DATA_NACK         0x30                            //Data byte has been transmitted. NACK has been received
#define TWI_MTM_SCODE_ARBITRATION_LOST  0x38                            //Arbitration lost in SLA+W or data bytes

#define TWI_MRM_SCODE_SLAR_ACK          0x40                            //TWI_SLA+R has been transmitted. ACK has been received
#define TWI_MRM_SCODE_SLAR_NACK         0x48                            //TWI_SLA+R has been transmitted. NACK has been received
#define TWI_MRM_SCODE_DATA_ACK          0x50                            //Data byte has been received. ACK has been returned
#define TWI_MRM_SCODE_DATA_NACK         0x58                            //Data byte has been received. NACK has been returned

#define TWI_sendSTART()         TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) | (1 << TWIE); gl_uc_twi_busy = 1
#define TWI_sendSTOP()          TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN) | (1 << TWIE); gl_uc_twi_busy = 0
#define TWI_clearINT()          TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWIE)
#define TWI_sendACK()           TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN) | (1 << TWIE)
#define TWI_sendNACK()          TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWIE)


uint8_t gl_uc_twi_data[TWI_BUFFER_SIZE];
//extern uint8_t gl_uc_twi_busy;

struct TWI_DataPacket
{
    
    uint8_t uc_address_rw;
    uint8_t uc_tx_len;
    uint8_t uc_rx_len;
    uint8_t uc_txrx_counter;
    uint8_t *uc_data;

}; 


void TWI_initializeTWI(void);
void TWI_txBytes( uint8_t address, uint8_t *data, uint8_t tx_len);
void TWI_txrxBytes( uint8_t address, uint8_t *data, uint8_t tx_len, uint8_t rx_len);
void TWI_rxBytes( uint8_t address, uint8_t *data, uint8_t rx_len);
void TWI_waitUntilIdle(void);

#endif /* _TWI_H_ */

