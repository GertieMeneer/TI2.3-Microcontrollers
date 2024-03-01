#define F_CPU 8e6
#define LCD_E 	3
#define LCD_RS	2

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void lcd_strobe_lcd_e();

void wait( int ms ) {
	for (int tms = 0; tms < ms; tms++) {
		_delay_ms(1);			// library function (max 30 ms at 8MHz)
	}
}

void adcInit(void) {
	ADMUX = 0b01100000;	// AREF=VCC, result left adjusted, channel 0 at pin PF0
	ADCSRA = 0b11100110;	// ADC-enable, no interrupt, start, free running, division by 64
}

void init_4bits_mode(void) {
	DDRC = 0xFF;
	PORTC = 0x00;

	PORTC = 0x20;   // function set
	lcd_strobe_lcd_e();

	PORTC = 0x20;   // function set
	lcd_strobe_lcd_e();
	PORTC = 0x80;
	lcd_strobe_lcd_e();

	PORTC = 0x00;   // Display on/off control
	lcd_strobe_lcd_e();
	PORTC = 0xF0;
	lcd_strobe_lcd_e();

	PORTC = 0x00;   // Entry mode set
	lcd_strobe_lcd_e();
	PORTC = 0x60;
	lcd_strobe_lcd_e();
}

void lcd_strobe_lcd_e(void) {
	PORTC |= (1 << LCD_E);	// E high
	_delay_ms(1);
	PORTC &= ~(1 << LCD_E);	// E low
	_delay_ms(1);
}

void lcd_write_data(unsigned char byte) {
	PORTC = byte;
	PORTC |= (1 << LCD_RS);
	lcd_strobe_lcd_e();

	PORTC = (byte << 4);
	PORTC |= (1 << LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_write_string(char *str) {
	for (; *str; str++) {
		lcd_write_data(*str);
	}
}

void lcd_write_command(unsigned char byte)
{
	// First nibble.
	PORTC = byte;
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void int_to_string_temperature(int temperature, char *buffer) {
	int digit, i = 0;
	if (temperature == 0) {
		buffer[0] = '0';
		buffer[1] = '\0';
		return;
	}

	if (temperature < 0) {
		buffer[i++] = '-';
		temperature = -temperature;
	}

	// Extracting the digits of the temperature
	while (temperature != 0) {
		digit = temperature % 10;
		buffer[i++] = digit + '0';
		temperature /= 10;
	}

	// Reversing the digits in the buffer
	int length = i;
	for (int j = 0; j < length / 2; j++) {
		char temp = buffer[j];
		buffer[j] = buffer[length - j - 1];
		buffer[length - j - 1] = temp;
	}
	// Null-terminate the string
	buffer[length] = '\0';
}


int main(void) {
	DDRF = 0x00;    // set PORTF for input (ADC)
	DDRA = 0xFF;    // set PORTA for output
	DDRB = 0xFF;    // set PORTB for output
	init_4bits_mode();
	adcInit();

	char buffer[10];

	while (1) {
		PORTB = ADCL;
		PORTA = ADCH;
		
		lcd_write_command(0x01);  // Clear display command
		
		lcd_write_command(0x80);    // Move cursor to the beginning of the first line
		int temperature = (ADCH * 5000UL) / 256; // Convert ADC value to temperature in millidegrees Celsius
		int_to_string_temperature(temperature, buffer); // Convert temperature to string
		lcd_write_string(buffer);   // Write temperature to LCD

		wait(1000); // every 100 ms (busy waiting)
	}

	return 0;
}
