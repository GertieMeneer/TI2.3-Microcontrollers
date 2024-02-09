#define F_CPU 10000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect) {		//interrupt code 0
	//set led rd 4 on
	PORTD = 0x10;
}

ISR(INT1_vect) {		//interrupt code 1
	//set led rd 4 off
	PORTD = 0x00;
}

int main(void) {
	DDRD = 0xF0; //set ins and outs

	EICRA |= (1 << ISC10) | (1 << ISC11) | (1 << ISC00) | (1 << ISC01); //set bits in external interrupt control register to run trigger code
	EIMSK |= (1 << INT1) | (1 << INT0); //set bits in external interrupt mask register to turn on interrupt requests

	sei(); //enable global interrupt system

	while (1) {
		//empty :)
	}

	return 1;
}