#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)	(1 << (x))

void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}


// Initialize ADC: 10-bits
void adcInit( void )
{
	ADMUX = 0b01100001;			
	ADCSRA = 0b11100110;		
}


// Main program: ADC at PF1
int main( void )
{
	DDRF = 0x00;                // set PORTF for input (ADC)
	DDRA = 0xFF;                // set PORTA for output
	adcInit();                  // initialize ADC

	while (1)
	{
		uint16_t adc_value = ADCL | (ADCH << 8); // combine ADCL and ADCH to get 10-bit ADC value
		
		// Scale ADC value to match the LED output range
		uint8_t led_output = adc_value >> 6; // divide by 64 for 10-bit ADC
		
		// Set PORTA based on the LED output value
		PORTA = (1 << PA0) | (1 << PA4) | (1 << PA5) | (1 << PA6) | (1 << PA7); // Turn on PA0, PA4, PA5, PA6, and PA7
		
		// Turn off LEDs based on ADC value
		if (led_output < 16) {
			PORTA &= ~(1 << PA7); // Turn off PA7
		}
		if (led_output < 32) {
			PORTA &= ~(1 << PA6); // Turn off PA6
		}
		if (led_output < 64) {
			PORTA &= ~(1 << PA5); // Turn off PA5
		}
		if (led_output < 128) {
			PORTA &= ~(1 << PA4); // Turn off PA4
		}
		if (led_output < 256) {
			PORTA &= ~(1 << PA0); // Turn off PA0
		}
		
		wait(100); // every 100 ms (busy waiting)
	}
}
