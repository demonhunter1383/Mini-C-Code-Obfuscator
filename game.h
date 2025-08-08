#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdlib.h>

#define NUM_ROWS 4
#define NUM_COLS 20

#define OBSTACLE_MOVE_TICKS 6 // ~2 seconds 

#define DIR_UP    0
#define DIR_DOWN  1

typedef struct {
	uint8_t row;
	uint8_t col;
} Frog;

typedef struct {
	uint8_t pos;
	uint8_t dir;
} ObstacleColumn;

extern Frog frog;
extern uint8_t lives;
extern uint8_t game_over;
extern uint8_t game_won;

void game_init(void);
void game_tick(char key);
void draw_game(void);
uint8_t is_rest_column(uint8_t col); 

#endif
