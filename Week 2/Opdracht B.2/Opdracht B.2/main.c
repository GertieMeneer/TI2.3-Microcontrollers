#define F_CPU 10000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void wait(int ms) {
	for (int i = 0; i < ms; i++) {
		_delay_ms(1);
	}
}

ISR(INT0_vect) {
	//set led rd 4 on
	PORTD = 0x10;
}

ISR(INT1_vect) {
	//set led rd 4 off
	PORTD = 0x00;
}

int main(void) {
	DDRD = 0xF0; // Set PORTD(7:4) as outputs and PORTD(3:0) as inputs

	EICRA |= (1 << ISC10) | (1 << ISC11) | (1 << ISC00) | (1 << ISC01); // Trigger INT0 on rising edge and INT1 on falling edge
	EIMSK |= (1 << INT1) | (1 << INT0); // Enable both INT1 and INT0

	sei(); // Enable global interrupt system

	while (1) {
		PORTD ^= (1 << 7); // Toggle PORTD.7
		wait(500);
	}

	return 1;
}