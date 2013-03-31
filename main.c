////////////////////////////////////////////////////////////////////////
//	I2C	Master
////////////////////////////////////////////////////////////////////////





/*

I2C Master MCU implementaiton:

(1) Master Transmitting to Slave:

	A- Specify the Slave Address (Target_Slave)

	use the function I2C_Writer(SlaveAddress, Location, Data)
	- SlaveAddress: The slave you want to write to, e.g. "Target_Slave"
	- Location: The location on the slave receving buffer you want to write to, range is 0-254
	- Data: The data you wish to transmit
	


(2) Master Reading from slave

	A- Specify the Slave Address (Target_Slave)
	
	use the function I2C_Reader(SlaveAddress, Location)
	
	which will return the data retreived from the slave
	
	


*/











#ifndef F_CPU
#define F_CPU 16000000UL
#endif



#include <avr/io.h>
#include <util/delay.h>


#include "I2C_master.h"


#define Target_Slave	0x02



int main(void)
{
	uint8_t x=0;
	uint8_t LEDs=0;
	I2C_init();
	DDRB=0xFF;
	
	
	for(;;)
	{
		
		I2C_Writer(Target_Slave,0x00,x);
		I2C_Writer(Target_Slave,0x01,255-x);
		
		x+=1;
		
		
		PORTB = I2C_Reader(Target_Slave, 0x01);
		
		
		_delay_ms(100);
		
		
		
	}
	
	return 0;
}