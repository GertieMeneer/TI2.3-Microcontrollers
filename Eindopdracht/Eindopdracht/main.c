#include "delay.h"
#include "spi.h"
#include "temp.h"

#include <avr/io.h>

static void init(void) {
	PORTB = ADCL;
	PORTA = ADCH;
	DDRB = 0x01;
	DDRF = 0x00;    // portf as input (adc)
	DDRA = 0xFF;    // porta as output
	DDRB = 0xFF;    // portb as output
}

int main(void) {
	adcInit();
	init();
	spi_masterInit();
	displayDriverInit();

	//clear display (set 0's)
	clearDisplay();
	wait(1000);
	
	//showing positive and negative number
	while(1) {
		int temperature = (ADCH * 5000UL) / 256;

		writeLedDisplay(temperature);

		wait(250);
	}

	return 0;
}
