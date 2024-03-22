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
	
	while(1) {
		int temperature = (ADCH * 5000UL) / 256;
		char buffer[20];
		
		sprintf(buffer, "Temp: %d", temperature);
		
		lcd_write_string(buffer);

		writeLedDisplay(temperature);

		_delay_ms(1000);
		lcd_write_command(0x01);
		_delay_ms(1000);
		
	}

	return 0;
}
