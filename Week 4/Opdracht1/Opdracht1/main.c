#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)	(1 << (x))

void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );
	}
}


void adcInit( void )
{
	ADMUX = 0b01100001;		//aref=vcc, channel1 at pin pf1
	ADCSRA = 0b11100110;	//adc-enable, no interrupt, start, free running
}


int main( void )
{
	DDRF = 0x00;	//portf = input
	DDRA = 0xFF;	//porta = output
	DDRB = 0xFF;	//portb = output
	adcInit();		//init adc

	while (1)
	{
		PORTB = ADCL;	//show msb/lsb of adc
		PORTA = ADCH;
		wait(100);
	}
}