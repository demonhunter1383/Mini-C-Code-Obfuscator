#ifndef KEYPAD_H
#define KEYPAD_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void keypad_init(void);
char keypad_getkey(void);

#endif
