// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void draw_frog(uint8_t row, uint8_t col);
void clear_frog(uint8_t row, uint8_t col);
void draw_obstacles(void);
void update_lives_display(uint8_t lives);
void display_game_screen(void);

#endif
