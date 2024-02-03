#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main(void) {
	DDRD = 0xff;
	PORTD = 0b00000001;
	
	while (1) {
		wait(100);
		PORTD = PORTD << 1;
		if(PORTD & 0x80)
		{
			wait(500);
			PORTD = 0b00000001;
		}
	}
	return 0;
}