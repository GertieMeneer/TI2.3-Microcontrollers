#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define NUM_LEDS 8

volatile uint8_t activeLedIndex = 0;

ISR(INT0_vect) {		//interrupt code 0
	if (activeLedIndex == (NUM_LEDS - 1)) {
		//reached the end, move to beginning
		activeLedIndex = 0;
		} else {
		//move to next led
		activeLedIndex++;
	}

	//turn on led
	PORTC = (1 << activeLedIndex << 1) & 0xFE;
}

ISR(INT1_vect) {		//interrupt code 1
	if (activeLedIndex == 0) {
		//reached beginning, move to end
		activeLedIndex = NUM_LEDS - 1;
		} else {
		//move to previous led
		activeLedIndex--;
	}

	//turn on led
	PORTC = (1 << (activeLedIndex + 1)) & 0xFE;
}

int main(void) {
	DDRD = 0xF0;
	DDRC = 0xFF;

	EICRA |= (1 << ISC10) | (1 << ISC11) | (1 << ISC00) | (1 << ISC01); //set bits in external interrupt control register to run trigger code
	EIMSK |= (1 << INT1) | (1 << INT0); //set bits in external interrupt mask register to turn on interrupt requests

	sei(); //enable global interrupt system

	while (1) {
		// empty :)
	}

	return 1;
}