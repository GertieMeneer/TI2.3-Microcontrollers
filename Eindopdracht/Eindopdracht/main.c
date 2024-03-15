/*
 * main.c
 *
 * Created: 3/15/2024 9:12:40 AM
 *  Author: GertieMeneer
 */ 

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
		_delay_ms(1);
	}
}

void init_temp_sensor(void)
{
	DDRF = 0x00;	//portf as input
	ADMUX = 0b01100000;	// aref=vcc, result left adjusted, channel 0 at pin PF0
	ADCSRA = 0b11100110;	// adc-enable, no interrupt, start, free running
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

	PORTC = 0x00;   // display on/off control
	lcd_strobe_lcd_e();
	PORTC = 0xF0;
	lcd_strobe_lcd_e();

	PORTC = 0x00;   // entry mode set
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
	// first nibble
	PORTC = byte;
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// second nibble
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

	// extracting digits of temperature
	while (temperature != 0) {
		digit = temperature % 10;
		buffer[i++] = digit + '0';
		temperature /= 10;
	}

	// reversing digits in buffer
	int length = i;
	for (int j = 0; j < length / 2; j++) {
		char temp = buffer[j];
		buffer[j] = buffer[length - j - 1];
		buffer[length - j - 1] = temp;
	}
	// null-terminate the string
	buffer[length] = '\0';
}

int main(void)
{
	DDRA = 0xFF;    // porta as output
	DDRB = 0xFF;    // portb as output
	init_temp_sensor();
	
	char buffer[10];
	
    while(1)
    {
		PORTB = ADCL;
		PORTA = ADCH;
		
        lcd_write_command(0x01);
		lcd_write_command(0x80);
		int temperature = (ADCH * 5000UL) / 256;
		int_to_string_temperature(temperature, buffer); // temp to string
		lcd_write_string(buffer);
		
		wait(1000);

    }
	return 0;
}