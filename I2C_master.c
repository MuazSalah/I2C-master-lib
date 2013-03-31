#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>

#include "I2C_master.h"

#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)

void I2C_init(void)
{
	TWBR = TWBR_val;
}

uint8_t I2C_start(uint8_t address)
{
	// reset TWI control register
	TWCR = 0;
	// transmit START condition 
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the start condition was successfully transmitted
	if((TWSR & 0xF8) != TW_START)
	{
		return 1;
	}
	
	// load slave address into data register
	TWDR = address;
	// start transmission of address
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	// check if the device has acknowledged the READ / WRITE mode
	uint8_t twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) )
		return 1;
	
	return 0;
}

uint8_t I2C_write(uint8_t data)
{
	// load data into data register
	TWDR = data;
	// start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK )
	{
		return 1;
	}
	
	return 0;
}

uint8_t I2C_read_ack(void)
{
	
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA); 
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

uint8_t I2C_read_nack(void)
{
	
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Function Name:
		I2C_Writer
	
	Input Parameters:
		<unsigned 8-bit integer>		<Address>			<Address of the slave I2C member>
		<unsigned 8-bit integer>		<Location>				<Location in the slave receiver buffer>
		<unsigned 8-bit integer>		<Data>					<Data to be transmitted>
		
	Outputs:
		N/A


*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void I2C_Writer(uint8_t Address, uint8_t Location, uint8_t Data)
{

	I2C_start(TargetSlave);			//Start an I2C transfer to the requested Target Slave Address
	I2C_write(LocationToWriteTo);	//Write data to the specified location in the Slave's receving buffer
	I2C_write(DataToWrite);			//The data to be written
	I2C_stop();						//End the I2C transmition

}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Function Name:
		I2C_Read
	
	Input Parameters:
		<unsigned 8-bit integer>		<Address>		<Address of the slave I2C member>
		<unsigned 8-bit integer>		<Location>		<Location in the slave transmitting buffer>
		
	Outputs:
		<unsigned 8-bit integer>		<temp>			<Data the have been read from slave>


*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////


uint8_t I2C_Reader(uint8_t Address, uint8_t Location)
{

	uint8_t temp;

	I2C_start(Address);
	I2C_write(Location);
	I2C_stop();

	I2C_start(Address | 0x01);
	I2C_read_ack();
	temp=I2C_read_nack();
	I2C_stop();
	
	return temp;


}




void I2C_stop(void){
	// transmit STOP condition
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}
