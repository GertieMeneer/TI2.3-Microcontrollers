#ifndef LCD_H
#define LCD_H
#pragma once

#define LCD_E     3
#define LCD_RS    2

void lcd_strobe_lcd_e(void);
void init_4bits_mode(void);
void lcd_write_string(char *str, int line);
void lcd_write_data(unsigned char byte);
void lcd_write_cmd(unsigned char byte);
void set_cursor(int position);
void print_highest_temp(void);

#endif /* LCD_H */