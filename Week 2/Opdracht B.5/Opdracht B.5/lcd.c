#define F_CPU 10e6

#include <avr/io.h>                                                              
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

#define LCD_E 	6  // RA6 UNI-6
#define LCD_RS	4  // RA4 UNI-6

void lcd_strobe_lcd_e(void) {
	PORTA |= (1<<LCD_E);
	_delay_ms(1);			
	PORTA &= ~(1<<LCD_E);  
	_delay_ms(1);			
}

void lcd_write_string(char *str) {
	for(;*str; str++){
		lcd_write_data(*str);
	}
}

void lcd_write_data(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTA |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTA |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_write_command(unsigned char byte) {
	// First nibble.
	PORTC = byte;
	PORTA &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTA &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void display_text(char *str){
	lcd_write_string(str);
}

void set_cursor(int position){
	for (int i = 0; i < position; i++) {
		lcd_write_command (0x14);
	}
}

int main( void ) {
	init();
	
	set_cursor(3);
	
	char myString[] = "YAYYYYY!";

	// Write sample string
	lcd_write_string(myString);

	while (1) {
		PORTC ^= (1<<0);	// Toggle PORTD.7
		_delay_ms(250);
	}

	return 1;
}

void init(){
	// Init I/O
	DDRC = 0xFF;
	PORTC = 0xFF;

	// Init LCD
	// PORTC output mode and all low (also E and RS pin)
	DDRD = 0xFF;
	DDRA = 0xFF;
	PORTC = 0x00;
	PORTA = 0x00;
	//PORTA = 0xFF;

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
	
	_delay_ms(10);
	
	lcd_clear();
}

void lcd_clear() {
	lcd_write_command (0x01);						//Leeg display
	_delay_ms(2);
	lcd_write_command (0x80);						//Cursor terug naar start
}