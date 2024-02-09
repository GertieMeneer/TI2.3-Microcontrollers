#define F_CPU 10e6
#include <avr/io.h>
#include <util/delay.h>

const unsigned char numbers [10] = {		//all "frames" of the snake crawling
	0b00000001,
	0b00000011,
	0b00000111,
	0b00001110,
	0b00011100,
	0b00111000,
	0b00110001,
	0b00100011,
	0b00000011,
};

int main( void ) {
	DDRD = 0b11111111; //PORTD all output
	int index = 0;
	
	//iterate through array
	while (1) {
		
		//write "frame" to 7 segment
		PORTD = numbers[index];
		
		_delay_ms(150);
		
		index++;
		
		if (index == 8){
			index = 2;
		}
		
	}

	return 1;
}