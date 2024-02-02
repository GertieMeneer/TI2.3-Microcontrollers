#include <avr/io.h>
#include <util/delay.h>
void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1); // library function (max 30 ms at 8 MHz)
	}
}

int main(void)
{
	DDRD = 0b11111111; // All pins PORTD are set to output
	DDRC = 0b11111110;

	while (1)
	{
		if (PINC & 0x01)
		{
			while (1)
			{
				PORTD = 0x80;
				_delay_ms(500);
				PORTD = 0x00;
				_delay_ms(500);
				if (!(PINC & 0x01)) break;
			}
		}
		else
		{
			PORTD = 0x00;
			_delay_ms(1);
		}
	}
}