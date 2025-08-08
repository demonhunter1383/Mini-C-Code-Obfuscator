#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"
#include "keypad.h"
#include "game.h"
#include "seven_segment.h"

int main(void) {
	lcd_init();
	keypad_init();
	game_init();
	seven_segment_init();
	seven_segment_display(lives);

	while (1) {
		if (game_over) {
			lcd_clear();
			lcd_goto(1, 5);
			lcd_print("Game Over!");
			while (1); // Halt
		}

		if (game_won) {
			lcd_clear();
			lcd_goto(1, 5);
			lcd_print("You Win!");
			while (1); // Halt
		}

		char key = keypad_getkey();
		game_tick(key);       // Call game_tick EVERY loop iteration with current key (or 0 if none)
		seven_segment_display(lives);

		_delay_ms(100);       // Wait 100 ms per tick
	}
}
