#include <xc.h>
#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
    while(1)
    {
        DDRD = 0b11111111;			// All pins PORTD are set to output

        while (1) {

	        PORTD = 0b10000000;		// Make LED 7 on PORTD HIGH others low

	        wait( 500 );

	        PORTD = 0b01000000;		// Make LED 6 on PORTD HIGH	others low

	        wait( 500 );

        }

        return 1;
    }
}