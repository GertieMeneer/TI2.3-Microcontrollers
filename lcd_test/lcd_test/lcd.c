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

int main(void) {
	DDRD = 0xFF;
	
	init();
	init_4bits_mode();
	
	_delay_ms(1000);
	
	int degree = 35;
	int lowdegree = 6;
	
	while (1)
	{
		char highest[] = "Highest: ";
		char lowest[] = "Lowest: ";
		
		sprintf(highest, "Highest: %d", degree);
		
		set_cursor(0);
		lcd_write_string(highest, 0);

		_delay_ms(3000);
		clear_lcd();
		set_cursor(0);
		sprintf(lowest, "Lowest: %d", lowdegree);
		
		lcd_write_string(lowest, 0);
		_delay_ms(3000);
	}
	
	//LCD_Init();

	//lcd_write_string(menus.highestTempMenu);
	//print_highest_temp();
	//set_cursor(0);
	//_delay_ms(100);
	//set_cursor(2);
	
	_delay_ms(5000);
	lcd_write_string(menus.highestTempMenu, 0);
	_delay_ms(100);
	
	lcd_write_command(0xC0);
	char tekst[] = "wtf";
	lcd_write_string(tekst, 0);

	//while (1) {
		//for (int i = 0; i < 32; i++)
		//{
			//set_cursor(i);
			//_delay_ms(1000);
		//}
	//}

	return 0;
}
