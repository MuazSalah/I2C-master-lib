#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>

#include "I2C_master.h"

#define F_SCL 100000UL // SCL frequency
#define Prescaler 4
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)

void I2C_init(void){
	TWSR = (1<<TWPS0);
	TWBR = TWBR_val;
}

void I2C_start(uint8_t address){
	// transmit START condition 
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the start condition was successfully transmitted
	//if((TWSR & 0xF8) != TW_START){ uart_puts("Start condition could not be sent!\r\n"); }
	
	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the device has acknowledged the READ / WRITE mode
	//if( (TWSR & 0xF8) != TW_MT_SLA_ACK ){ uart_puts("Slave has not acknowledged master transmit mode!\r\n"); }
	//else if( (TWSR & 0xF8) != TW_MR_SLA_ACK ){ uart_puts("Slave has not acknowledged master receive mode!\r\n"); }
}

void I2C_write(uint8_t data){
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	//if( (TWSR & 0xF8) != TW_MT_DATA_ACK ){ uart_puts("Slave has not acknowledged received data!\r\n"); }
}

uint8_t I2C_read_ack(void){
	
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

uint8_t I2C_read_nack(void){
	
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

void I2C_stop(void){
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}