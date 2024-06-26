/*
 * main.c
 *
 * Created: 3/30/2024 4:08:07 PM
 *  Author: stefk
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "serial.h"
#include "DHT11.h"

int main(void)
{
	uart_init();
	
	while (1)
	{
		char cTempInt, cTempDec, cHumInt, cHumDec;

		DHT11_Init();

		DHT11_GetData(&cHumInt, &cHumDec, &cTempInt, &cTempDec);

		// uncomment to get 21.9 C like 219 for SPI display
		//int temperature = cTempInt * 10 + cTempDec;

		char buffer[30];
		sprintf(buffer, "Temperature: %d.%d C", cTempInt, cTempDec);
		
		// uncomment to get 21.9 C like 219 for SPI display
		//sprintf(buffer, "Temperature: %d C", temperature); 
		uart_putstr(buffer);
		
		_delay_ms(2000);
	}
}