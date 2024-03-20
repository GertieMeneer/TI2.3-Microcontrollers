#include "delay.h"
#include "spi.h"

#include <avr/io.h>

int main() {
	DDRB = 0x01;
	spi_masterInit();
	displayDriverInit();

	//clear display (set 0's)
	for (char i = 1; i <= 4; i++) {
		spi_writeWord(i, 0);
	}
	wait(1000);

	//showing positive and negative number
	while(1)
	{
		for (int i = 9999; i > -1000; i--)
		{
			writeLedDisplay(i);
			wait(10);
		}
		wait(1000);
	}

	return 1;
}
