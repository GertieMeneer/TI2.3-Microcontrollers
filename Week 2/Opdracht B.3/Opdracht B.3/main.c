#define F_CPU 10000000UL
#include <avr/io.h>
#include <util/delay.h>

const unsigned char numbers[16] = {
	// 7-segment display codes for digits 0-F
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9
	0b01110111, // A
	0b01111100, // B
	0b00111001, // C
	0b01011110, // D
	0b01111001, // E
	0b01110001  // F
};

int main(void) {
	DDRD = 0xFF; // portd as output
	PORTB |= (1 << PB1) | (1 << PB2); // enable pull-up resistors
	unsigned char index = 0;
	unsigned char display_value = 0;

	while (1) {
		// check buttons
		if (!(PINB & (1 << PB1))) { // if pb1 pressed
			display_value++;
			if (display_value > 15) display_value = 0; // wrap around
			_delay_ms(200);
			} else if (!(PINB & (1 << PB2))) { //if pb2 down
			display_value--;
			if (display_value > 15) display_value = 15; // dont go negative
			_delay_ms(200);
			} else if (!((PINB & (1 << PB1)) && (PINB & (1 << PB2)))) { // if both pressed
			display_value = 0;
			_delay_ms(200);
		}

		// display value
		PORTD = numbers[display_value];
	}

	return 0;
}
