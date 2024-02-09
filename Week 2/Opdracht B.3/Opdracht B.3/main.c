#define F_CPU 10e6
#include <avr/io.h>
#include <util/delay.h>

const unsigned char numbers [10] = {
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
};

int main( void ) {
	DDRD = 0b11111111; // PORTD all output 
	int index = 0;
	
	while (1) {
		
		// Write data to PORTD	
		PORTD = numbers[index];
		
		_delay_ms(1000);
		
		index++;
		
		if (index == 10){
			index = 0;
		}
		
	}

	return 1;
}