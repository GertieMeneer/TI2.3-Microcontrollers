#define F_CPU 10e6

#include <avr/io.h>                                                              
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

#define LCD_E 	6
#define LCD_RS	4

//used to enable a signal (E)
void lcd_strobe_lcd_e(void) {
	PORTA |= (1<<LCD_E);
	_delay_ms(1);			
	PORTA &= ~(1<<LCD_E);  
	_delay_ms(1);			
}

//writes characters to lcd
void display_text(char *str) {
	for(;*str; str++){
		lcd_write_data(*str);
	}
}

//writing data to display
void lcd_write_data(unsigned char byte) {
	PORTC = byte;
	PORTA |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	PORTC = (byte<<4);
	PORTA |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}

//same as lcd_write_data, but without enabling register select, because command bytes, not data bytes
void lcd_write_command(unsigned char byte) {
	PORTC = byte;
	PORTA &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	PORTC = (byte<<4);
	PORTA &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void set_cursor(int position){
	for (int i = 0; i < position; i++) {
		lcd_write_command (0x14);
	}
}

int main( void ) {
	//init display
	init();
	
	//set cursor to position
	set_cursor(3);
	
	char myString[] = "YAYYYYY!";

	//write string
	display_text(myString);

	while (1) {
		PORTC ^= (1<<0);
		_delay_ms(250);
	}

	return 1;
}

void init(){
	//setup ins and outs
	DDRC = 0xFF;
	PORTC = 0xFF;

	DDRD = 0xFF;
	DDRA = 0xFF;
	PORTC = 0x00;
	PORTA = 0x00;

	PORTC = 0x20;	// function set
	lcd_strobe_lcd_e();

	PORTC = 0x20;   // function set
	lcd_strobe_lcd_e();
	PORTC = 0x80;
	lcd_strobe_lcd_e();

	PORTC = 0x00;   // display on/off
	lcd_strobe_lcd_e();
	PORTC = 0xF0;
	lcd_strobe_lcd_e();

	PORTC = 0x00;   // entry mode set 4 bit mode
	lcd_strobe_lcd_e();
	PORTC = 0x60;
	lcd_strobe_lcd_e();
	
	_delay_ms(10);
	
	lcd_clear();
}

void lcd_clear() {
	lcd_write_command (0x01);	//clear display
	_delay_ms(2);
	lcd_write_command (0x80);	//cursor back to start
}