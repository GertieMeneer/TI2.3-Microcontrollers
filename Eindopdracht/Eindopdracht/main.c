#define F_CPU 8e6

#include "spi.h"
#include "temp.h"
#include "lcd.h"

#include <avr/io.h>
#include <stdio.h>
#include <avr/delay.h>

static void init(void) {
	PORTB = ADCL;
	PORTA = ADCH;
	DDRB = 0x01;
	DDRF = 0x00;    // portf as input (adc)
	DDRA = 0xFF;    // porta as output
	DDRB = 0xFF;    // portb as output
}

int main(void) {
	_delay_ms(2000);
	adcInit();
	init();
	spi_masterInit();
	displayDriverInit();
	init_4bits_mode();

	//clear display (set 0's)
	clearDisplay();
	_delay_ms(1000);
	
	int highest = 0;
	int lowest = 100;
	
	int temperature = (ADCH * 5000UL) / 256;
	highest = temperature;
	lowest = temperature;
	
	while(1) {
		int temperature = (ADCH * 5000UL) / 256;
		
		if(temperature > highest) {
			highest = temperature;
		}
		if(temperature < lowest) {
			lowest = temperature;
		}
		char highBuffer[20];
		char lowBuffer[20];
		
		sprintf(highBuffer, "High: %d", highest);
		lcd_write_string(highBuffer);
		_delay_ms(10);
		lcd_write_command(192);
		_delay_ms(10);
		sprintf(lowBuffer, "Low: %d", lowest);
		lcd_write_string(lowBuffer);
		_delay_ms(10);
		
		writeLedDisplay(temperature);

		_delay_ms(1000);
		lcd_write_command(0x01);
		_delay_ms(5);
		
	}

	return 0;
}
