#define F_CPU 10e6
#include <avr/io.h>
#include <util/delay.h>

const unsigned char numbers [10] = {
	0b00000001, // 0
	0b00000011, // 1
	0b00000111, // 2
	0b00001110, // 3
	0b00011100, // 4
	0b00111000, // 5
	0b00110001, // 6
	0b00100011, // 7
	0b00000011, // 8
};

int main( void ) {
	DDRD = 0b11111111; // PORTD all output
	int index = 0;
	
	while (1) {
		
		// Write data to PORTD
		PORTD = numbers[index];
		
		_delay_ms(150);
		
		index++;
		
		if (index == 8){
			index = 2;
		}
		
	}

	return 1;
}