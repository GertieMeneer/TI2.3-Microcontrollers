#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)    (1 << (x))

void wait(int ms)
{
	for (int tms = 0; tms < ms; tms++)
	{
		_delay_ms(1);
	}
}

void adcInit(void)
{
	ADMUX = 0b01100011;    // aref=vcc, channel 3 at pin pf3
	ADCSRA = 0b10000110;   // adc-enable, no interrupt, start conversion manually, prescaler = 64
}

void startADC(void)
{
	ADCSRA |= (1 << ADSC);  // start adc conversion
}

int main(void)
{
	DDRF &= ~(1 << PF0);    // set pin PF0 as input
	DDRA = 0xFF;    // porta = output
	DDRB = 0xFF;    // portb = output

	adcInit();      // init adc

	while (1)
	{
		// check if button is pressed
		if (PINF & BIT(PF0)) {
			startADC();     // start ADC conversion
			while (ADCSRA & (1 << ADSC)); // wait for ADC conversion
			PORTB = ADCL;   // show msb/lsb of adc
			PORTA = ADCH;
			wait(100);
		}
	}
}
