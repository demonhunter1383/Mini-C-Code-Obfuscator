#include "lcd.h"

#define LCD_DDR     DDRC
#define LCD_PORT    PORTC
#define RS PC0
#define EN PC1

void lcd_enable_pulse(void) {
	LCD_PORT |= (1 << EN);
	_delay_us(1);
	LCD_PORT &= ~(1 << EN);
	_delay_us(100);
}

void lcd_send_nibble(uint8_t nibble) {
	LCD_PORT &= 0x0F; // Clear high nibble
	LCD_PORT |= (nibble & 0xF0); // Send upper nibble to PC4-PC7
	lcd_enable_pulse();
}

void lcd_cmd(uint8_t cmd) {
	LCD_PORT &= ~(1 << RS); // RS = 0 for command
	lcd_send_nibble(cmd & 0xF0); // Upper nibble
	lcd_send_nibble((cmd << 4) & 0xF0); // Lower nibble
	_delay_ms(2);
}

void lcd_data(char data) {
	LCD_PORT |= (1 << RS); // RS = 1 for data
	lcd_send_nibble(data & 0xF0);
	lcd_send_nibble((data << 4) & 0xF0);
	_delay_ms(2);
}

void lcd_init(void) {
	LCD_DDR |= 0xFF; // Set PORTC as output
	_delay_ms(20);

	// Init sequence
	lcd_cmd(0x33);
	lcd_cmd(0x32);
	lcd_cmd(0x28); // 4-bit, 2 lines, 5x8
	lcd_cmd(0x0C); // Display on, cursor off
	lcd_cmd(0x06); // Entry mode
	lcd_cmd(0x01); // Clear display
	_delay_ms(2);
}

void lcd_clear(void) {
	lcd_cmd(0x01);
	_delay_ms(2);
}

void lcd_goto(uint8_t row, uint8_t col) {
	const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	lcd_cmd(0x80 | (col + row_offsets[row]));
}

void lcd_print(const char* str) {
	while (*str) {
		lcd_data(*str++);
	}
}

void lcd_putc(char c) {
	lcd_data(c);
}