#include "keypad.h"

#define KEYPAD_DDR  DDRA
#define KEYPAD_PORT PORTA
#define KEYPAD_PIN  PINA

const char keymap[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

void keypad_init(void) {
	KEYPAD_DDR = 0x0F;         // Lower 4 bits (rows) as output, upper 4 bits (cols) as input
	KEYPAD_PORT = 0xFF;        // Enable pull-ups on input pins
}

char keypad_getkey(void) {
	for (uint8_t row = 0; row < 4; row++) {
		KEYPAD_PORT = ~(1 << row); // Set one row low at a time, others high
		_delay_ms(1);
		for (uint8_t col = 0; col < 4; col++) {
			if (!(KEYPAD_PIN & (1 << (col + 4)))) { // Check corresponding column
				while (!(KEYPAD_PIN & (1 << (col + 4)))); // Wait for key release
				_delay_ms(20); // Debounce
				return keymap[row][col];
			}
		}
	}
	return 0;
}
