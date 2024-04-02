#include "DHT11.h"

void DHT11_Init()
{
	// Set DHT11_PINX as an output pin
	DHT11_DDR |= (1<<DHT11_PINX);
	 // Set DHT11_PINX high (initialization step)
	DHT11_PORT |= (1<<DHT11_PINX);
	// Set the timer prescaler for DHT11
	DHT11_SETTPRESCALE;
	
	// Delay for initialization
	for (char c=0; c<8;c++)
	{
		_delay_ms(250);	
	}
}

void DHT11_GetData(char* cHumInt, char* cHumDec, char* cTempInt, char* cTempDec)
{
	unsigned char acResult[5];
	
	// Initialize result array to 0x00
	for (char i=0;i<5;i++)
	{
		acResult[i] = 0x00;
	}
	
	// Set the pin low for communication
	DHT11_PORT &= ~(1<<DHT11_PINX);
	_delay_ms(20);
	
	// Set DHT11_PINX as input for receiving data
	DHT11_DDR &= ~(1<<DHT11_PINX);
	DHT11_PORT &= ~(1<<DHT11_PINX);

	//After setting PINX to HIGH and wait.
	//DHT11 must set line to low within 20-40 us, check timer overrun for 60 us

	DHT11_TCNT = 0;	
	
	//Set Timer value to after 60 us
	while (((DHT11_PIN & (1<<DHT11_PINX))>0) && (DHT11_TCNT < 60 / DHT11_T0_INC));
	
	//DHT must set Line to low for 80 us, check if timer overrun 100 us
	DHT11_TCNT = 0;
	while (((DHT11_PIN & (1<<DHT11_PINX))==0) && (DHT11_TCNT < 100/DHT11_T0_INC));
	
	//DHT must set Line to high for 80 us, check if timer overrun 100 us
	DHT11_TCNT = 0;
	while (((DHT11_PIN & (1<<DHT11_PINX))>0) && (DHT11_TCNT < 100/DHT11_T0_INC));
	
	//get 5 bytes of 8 bits
	for (char count=0; count<5; count++)
	{
		for (char bitcount=0;bitcount<8; bitcount++)
		{
			acResult[count] = acResult[count]<<1;
			acResult[count] += DHT11_GetBit();
		}
	}
	
	*cHumInt = acResult[0];
	*cHumDec = acResult[1];
	*cTempInt = acResult[2];
	*cTempDec = acResult[3];
	
	_delay_ms(5);	
	
	//Put line to output
	DHT11_DDR |= (1<<DHT11_PINX);
	DHT11_PORT |= (1<<DHT11_PINX);
}

static char DHT11_GetBit()
{
	char cResult = 0xFF;
	
	//DHT set Line to low for 50 us
	DHT11_TCNT = 0;
	while (((DHT11_PIN & (1<<DHT11_PINX))==0) && (DHT11_TCNT < 64/DHT11_T0_INC));
		
	//DHT set Line to high for 28 us for 0 or 70 us for 1
	DHT11_TCNT = 0;
	while (((DHT11_PIN & (1<<DHT11_PINX))>0) && (DHT11_TCNT < 100/DHT11_T0_INC));
		
	//IF timer less then
	if (DHT11_TCNT < 52 / DHT11_T0_INC)
	{
		return 0;
	}
	else if (DHT11_TCNT <= 100 / DHT11_T0_INC - 1)
	{
		return 1;
	}
				
	return cResult;
}