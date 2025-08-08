// display.c
#include "display.h"
#include "lcd.h"
#include "game.h"

extern Frog frog;
extern ObstacleColumn obstacles[NUM_COLS];
extern uint8_t lives;
extern uint8_t is_rest_column(uint8_t col);

void draw_frog(uint8_t row, uint8_t col) {
	lcd_goto(row, col);
	lcd_putc('*');
}

void clear_frog(uint8_t row, uint8_t col) {
	lcd_goto(row, col);
	lcd_putc(' ');
}

void draw_obstacles(void) {
	for (uint8_t c = 1; c < NUM_COLS - 1; c++) {
		if (is_rest_column(c)) continue;

		lcd_goto(obstacles[c].pos, c);
		lcd_putc(obstacles[c].dir == DIR_UP ? '^' : 'v');
	}
}

void update_lives_display(uint8_t lives) {
	lcd_goto(NUM_ROWS - 1, 0); // Assuming last row
	lcd_print("Lives: ");
	lcd_putc('0' + lives); // Display number as char
}

void display_game_screen(void) {
	lcd_clear();
	draw_obstacles();
	draw_frog(frog.row, frog.col);
	update_lives_display(lives);
}
