#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void lcd_init(void);
void lcd_cmd(uint8_t cmd);
void lcd_data(char data);
void lcd_clear(void);
void lcd_goto(uint8_t row, uint8_t col);
void lcd_print(const char* str);
void lcd_putc(char c); 

#endif
