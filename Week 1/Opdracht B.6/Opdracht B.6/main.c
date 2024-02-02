/*
 * main.c
 *
 * Created: 2/2/2024 12:58:12 PM
 *  Author: Gerti
 */ 

#include <avr/io.h>
#include <util/delay.h>

void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1); // library function (max 30 ms at 8 MHz)
	}
}

int main(void)
{
	DDRD = 0b11111111; // All pins PORTD are set to output
	DDRC = 0b11111110; // Set button RC0 to input
	int timer = 10000;
	int counter = 0;

	while (1)
	{
		PORTD = 0x80;
		wait(timer);
		PORTD = 0x00;
		wait(timer);
		
		if (PINC & 0x01){
			counter = !counter;
		}
		
		if(counter == 0){
			timer = 2500;
		}
		
		if(counter == 1){
			timer = 10000;
		}
	}
}