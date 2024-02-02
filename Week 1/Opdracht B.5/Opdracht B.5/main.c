#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

typedef struct {
	unsigned char data;
	unsigned int delay;
} PatternStruct;

PatternStruct pattern[] = {
	{0x01, 100}, {0x02, 100}, {0x04, 100}, {0x08, 100}, {0x10, 100},
	{0x20, 100}, {0x40, 100}, {0x80, 100}, {0xFE, 100}, {0xFD, 100},
	{0xFB, 100}, {0xF7, 100}, {0xEF, 100}, {0xDF, 100}, {0xBF, 100},
	{0x7F, 100}, {0xFF, 1000}, {0x00, 0}
};

void wait(int ms) {
	for (int i = 0; i < ms; i++) {
		_delay_ms(1); // Library function (max 30 ms at 8 MHz)
	}
}

int main(void) {
	DDRD = 0xFF; // Set all pins on PORTD as outputs

	while (1) {
		int index = 0;

		while (pattern[index].delay != 0) {
			PORTD = pattern[index].data;
			wait(pattern[index].delay);
			index++;
		}
	}

	return 1;
}