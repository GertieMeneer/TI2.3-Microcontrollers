#define F_CPU 8e6
#define PRESCALER 1024   // Define prescaler for Timer1

#include "spi.h"
#include "temp.h"
#include "lcd.h"

#include <avr/io.h>
#include <stdio.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

volatile uint8_t timer_overflow = 0;

int showLowTemp = 0;

static void init(void) {
	PORTB = ADCL;
	PORTA = ADCH;
	DDRB = 0x01;
	DDRF = 0x00;    // portf as input (adc)
	DDRA = 0xFF;    // porta as output
	DDRB = 0xFF;    // portb as output
}

void timer_init() {
	// Set Timer1 control registers: Mode 4 (CTC mode), prescaler = 1024
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

int main(void) {
	_delay_ms(2000);
	timer_init();
	adcInit();
	init();
	spi_masterInit();
	displayDriverInit();
	init_4bits_mode();

	//clear display (set 0's)
	clearDisplay();
	sei();
	_delay_ms(1000);
	
	int temperature = (ADCH * 5000UL) / 256;
	float highestTemp = temperature / 10.0;
	float lowestTemp = temperature / 10.0;
	
	while (1) {
		// Check if 5 seconds have passed
		if (timer_overflow >= 1) {
			int temperature = (ADCH * 5000UL) / 256;

			if(!showLowTemp) {
				if(temperature < lowestTemp) {
					lowestTemp = temperature / 10.0;
				}
				lcd_write_command(0x01);
				_delay_ms(10);
				char lowBuffer[20];
				char lowest[] = "Lowest";
				lcd_write_string(lowest);
				_delay_ms(10);
				lcd_write_command(192);
				_delay_ms(10);
				sprintf(lowBuffer, "Temp: %.1f", lowestTemp);
				lcd_write_string(lowBuffer);
				_delay_ms(10);
				showLowTemp = 1;
				
			} else {
				if(temperature > highestTemp) {
					highestTemp = temperature / 10.0;
				}
				lcd_write_command(0x01);
				_delay_ms(10);
				char highBuffer[20];
				char highest[] = "Highest";
				lcd_write_string(highest);
				_delay_ms(10);
				lcd_write_command(192);
				_delay_ms(10);
				sprintf(highBuffer, "Temp: %.1f", highestTemp);
				lcd_write_string(highBuffer);
				_delay_ms(10);
				showLowTemp = 0;
			}
			
		}
		timer_overflow = 0;
		

	}
	return 0;

}