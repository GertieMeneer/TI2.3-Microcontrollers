#define F_CPU 8e6

#define LCD_Dir  DDRB			/* Define LCD data port direction */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "lcd.h"

struct menu menus;

struct menu{
	char lowestTempMenu[25];
	char highestTempMenu[25];
};

void lcd_strobe_lcd_e(void) {
	PORTC |= (1<<LCD_E);	// E high
	_delay_ms(1);
	PORTC &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);
}

void init_4bits_mode(void) {
	// PORTC output mode and all low (also E and RS pin)
	DDRC = 0xFF;
	PORTC = 0x00;

	// Step 2 (table 12)
	PORTC = 0x20;	// function set
	lcd_strobe_lcd_e();

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	lcd_strobe_lcd_e();
	PORTC = 0x80;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	lcd_strobe_lcd_e();
	PORTC = 0xF0;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Entry mode set
	lcd_strobe_lcd_e();
	PORTC = 0x60;
	lcd_strobe_lcd_e();

}

void lcd_write_string(char *str, int line) {
	for (int i = 0; str[i] != '\0'; i++) {
		lcd_write_data(str[i]);
	}
	
}

void lcd_write_data(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_write_command(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void set_cursor(int position) {
	// Set cursor to home and wait (see documentation)
	lcd_write_command(0x02);
	_delay_ms(100);

	while(position--) {
		lcd_write_command(0x14);
	}
}
//
//void set_cursor(int position){
	////for (int i = 0; i < position; i++) {
		//lcd_write_command (0x80 + position);
	////}
//}

void init(void) {
	strcpy(menus.highestTempMenu, "Highest temperature: ");
	strcpy(menus.lowestTempMenu, "Lowest temperature: ");
}

void print_highest_temp(void)
{
	char highest[] = "Highest";
	char temperature[] = "temperature";
	lcd_write_string(highest, 1);
	lcd_write_command(0x28);
	_delay_ms(100);
	lcd_write_string(temperature, 2);
}

void LCD_Init (void)  /* LCD Initialize function */
{
	LCD_Dir = 0xFF;		/* Make LCD port direction as o/p */
	_delay_ms(20);		/* LCD Power ON delay always >15ms */
	
	lcd_write_command(0x33);
	lcd_write_command(0x32);	/* Send for 4 bit initialization of LCD  */
	lcd_write_command(0x28);	/* 2 line, 5*7 matrix in 4-bit mode */
	lcd_write_command(0x0c);	/* Display on cursor off */
	lcd_write_command(0x06);	/* Increment cursor (shift cursor to right) */
	lcd_write_command(0x01);	/* Clear display screen */
}

void clear_lcd(void)
{
	lcd_write_command(0x01);
}

void adcInit(void) {
	ADMUX = 0b01100000;	// aref=vcc, result left adjusted, channel 0 at pin PF0
	ADCSRA = 0b11100110;	// adc-enable, no interrupt, start, free running
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

int main(void) {
	DDRD = 0xFF;
	DDRF = 0x00;    // portf as input (adc)
	DDRA = 0xFF;    // porta as output
	DDRB = 0xFF;    // portb as output
	
	
	init();
	adcInit();
	init_4bits_mode();
	
	_delay_ms(1000);
	
	char buffer[10];
	
	while (1)
	{
		PORTB = ADCL;
		PORTA = ADCH;
		
		int temperature = ((ADCH * 5000UL) / 256) / 10; // omzetten naar graden Celsius
		int_to_string_temperature(temperature, buffer); // temp to string
		
		char highest[20] = "Highest: ";
		char lowest[20] = "Lowest: ";
		
		sprintf(highest, "Highest: %d", temperature);
		
		set_cursor(0);
		lcd_write_string(highest, 0);

		_delay_ms(3000);
		clear_lcd();
		set_cursor(0);
		sprintf(lowest, "Lowest: %d", temperature);
		
		lcd_write_string(lowest, 0);
		_delay_ms(3000);
	}

	return 0;
}
