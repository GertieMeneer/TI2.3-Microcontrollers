#define F_CPU 8e6
#define PRESCALER 1024

#include "spi.h"
#include "temp.h"
#include "lcd.h"

#include <avr/io.h>
#include <stdio.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

volatile uint8_t timer_overflow = 0;

int showLowTemp = 0;
char lowTempText[] = "Lowest";
char highTempText[] = "Highest";

static void init(void) {
	PORTB = ADCL;
	PORTA = ADCH;
	DDRB = 0x01;
	DDRF = 0x00;    // portf as input (adc)
	DDRA = 0xFF;    // porta as output
	DDRB = 0xFF;    // portb as output
}

static void timer_init() {
	// set timer1 registers in ctc mode
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

	// Set the output compare register to the value for 5-second delay
	OCR1A = F_CPU / PRESCALER * 5 - 1;

	// Enable Timer1 compare match interrupt
	TIMSK |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
	// Increment overflow count
	timer_overflow++;
}

static void writeLcdData(char text[], int temp) {
	lcd_write_command(0x01);
	lcd_write_string(text);
	lcd_write_command(192);
	int integerPart = temp / 10;
	int fractionalPart = temp % 10;
	char tempString[20];
	sprintf(tempString, "Temp: %d.%d", integerPart, fractionalPart);
	lcd_write_string(tempString);
	_delay_ms(10);
}

int main(void) {
	_delay_ms(2000);		//delay for usb-link issue
	timer_init();			//init timer
	adcInit();				//init adc
	init();					//init ports
	spi_masterInit();		//init spi
	displayDriverInit();	//init spi display
	init_4bits_mode();		//init lcd in 4bits mode

	//clear display (set 0's)
	clearDisplay();
	
	//enable global interrupts
	sei();
	_delay_ms(1000);
	
	int temperature = (ADCH * 5000UL) / 256;
	int highestTemp = temperature;
	int lowestTemp = temperature;
	
	while (1) {
		// check if timers has passed
		if (timer_overflow >= 1) {
			temperature = (ADCH * 5000UL) / 256;
			writeLedDisplay(temperature);
			
			if (temperature < lowestTemp) {
				lowestTemp = temperature;
			}
			if (temperature > highestTemp) {
				highestTemp = temperature;
			}

			if (!showLowTemp) {
				writeLcdData(lowTempText, lowestTemp);
				
			} else {
				writeLcdData(highTempText, highestTemp);
			}
			showLowTemp = !showLowTemp;
		}
		timer_overflow = 0;
	}
	return 0;
}