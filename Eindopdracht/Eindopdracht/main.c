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
	lcd_write_command(0x01);		//clear display
	lcd_write_string(text);			//write the text
	lcd_write_command(192);			//set cursor to next line		
	int integerPart = temp / 10;
	int fractionalPart = temp % 10;
	char tempString[20];			//string to write to lcd
	sprintf(tempString, "Temp: %d.%d", integerPart, fractionalPart);	//add temp value to string
	lcd_write_string(tempString);	//write string to lcd
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
	
	//first measurement
	int temperature = (ADCH * 5000UL) / 256;
	int highestTemp = temperature;
	int lowestTemp = temperature;
	
	while (1) {
		// check if timers has passed
		if (timer_overflow >= 1) {
			temperature = (ADCH * 5000UL) / 256;	//measure temp
			writeLedDisplay(temperature);			//write temp to spi display
			
			//check and set high/low temp
			if (temperature < lowestTemp) {
				lowestTemp = temperature;
			}
			if (temperature > highestTemp) {
				highestTemp = temperature;
			}
			
			//write low temp
			if (!showLowTemp) {
				writeLcdData(lowTempText, lowestTemp);
			//write high temp	
			} else {
				writeLcdData(highTempText, highestTemp);
			}
			showLowTemp = !showLowTemp;		//switch between low/high temp
		}
		timer_overflow = 0;
	}
	return 0;
}