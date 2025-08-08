#include "game.h"
#include "lcd.h"
#include "keypad.h"
#include "seven_segment.h"

Frog frog;
ObstacleColumn obstacles[NUM_COLS];

uint8_t rest_columns[] = {6};
uint8_t lives = 3;
uint8_t game_over = 0;
uint8_t game_won = 0;
uint16_t tick_counter = 0;

uint8_t is_rest_column(uint8_t col); 

void game_init(void) {
	lcd_clear();
	lcd_goto(0, 0);
	lcd_print("FROGGER - Parsa");
	lcd_goto(1, 0);
	lcd_print("Press any key...");
	while (!keypad_getkey()); // wait for any key press

	lcd_clear();
	frog.row = 0;
	frog.col = 0;
	seven_segment_init();
	seven_segment_display(lives);  // Display 3 at game start


	for (uint8_t c = 1; c < NUM_COLS - 1; c++) {
		obstacles[c].dir = (c % 2 == 0) ? DIR_UP : DIR_DOWN;
		obstacles[c].pos = rand() % NUM_ROWS;
	}

	draw_game();
	
}

void draw_game(void) {
	lcd_clear();
	// Draw obstacles
	for (uint8_t c = 1; c < NUM_COLS - 1; c++) {
		if (is_rest_column(c)) continue;
		lcd_goto(obstacles[c].pos, c);
		lcd_putc(obstacles[c].dir == DIR_UP ? '^' : 'v'); 
	}

	// Draw frog
	lcd_goto(frog.row, frog.col);
	lcd_putc('*');
}

uint8_t is_rest_column(uint8_t col) {
	for (uint8_t i = 0; i < sizeof(rest_columns); i++) {
		if (rest_columns[i] == col) return 1;
	}
	return 0;
}

void update_obstacles(void) {
	for (uint8_t c = 1; c < NUM_COLS - 1; c++) {
		if (is_rest_column(c)) continue;
		if (obstacles[c].dir == DIR_UP) {
			if (obstacles[c].pos == 0)
			obstacles[c].pos = NUM_ROWS - 1;
			else
			obstacles[c].pos--;
			} else {
			obstacles[c].pos++;
			if (obstacles[c].pos >= NUM_ROWS)
			obstacles[c].pos = 0;
		}
	}
}

void check_collision(void) {
	if (frog.col == 0 || frog.col == NUM_COLS - 1) return;
	if (is_rest_column(frog.col)) return;

	if (obstacles[frog.col].pos == frog.row) {
		lives--;
		seven_segment_display(lives); 
		if (lives == 0) {
			game_over = 1;
			} else {
			frog.row = 0;
			frog.col = 0;
		}
	}
}

void check_win(void) {
	if (frog.col == NUM_COLS - 1) {
		game_won = 1;
	}
}

void game_tick(char key) {
	if (game_over || game_won) return;

	switch (key) {
		case '1': if (frog.row > 0) frog.row--; break;
		case '2': if (frog.row < NUM_ROWS - 1) frog.row++; break;
		case '3': if (frog.col < NUM_COLS - 1) frog.col++; break;
		case '4': if (frog.col > 0) frog.col--; break;
	}

	check_collision();
	check_win();

	tick_counter++;
	if (tick_counter >= OBSTACLE_MOVE_TICKS) {
		update_obstacles();
		tick_counter = 0;
	}

	draw_game();
}
