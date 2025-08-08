#include <avr/io.h>
#include <util/delay.h>
#include "seven_segment.h"

const uint8_t digit_encoding[10] = {
	0b11000000, // 0 (a,b,c,d,e,f ON, g OFF)
	0b11111001, // 1 (b,c ON)
	0b10100100, // 2
	0b10110000, // 3
	0b10011001, // 4
	0b10010010, // 5
	0b10000010, // 6
	0b11111000, // 7
	0b10000000, // 8
	0b10010000  // 9
};

void seven_segment_init(void) {
	DDRD = 0xFF;    // Set PORTD all output (PD0-PD7)
	PORTD = 0xFF;   // Turn off all segments (all HIGH)
}

void seven_segment_display(uint8_t num) {
	if (num < 10) {
		PORTD = digit_encoding[num];
		} else {
		PORTD = 0xFF; // turn off all segments if invalid number
	}
}

void seven_segment_set_dp(uint8_t on) {
	if (on) {
		PORTD &= ~(1 << 7);  // DP ON (PD7 LOW)
		} else {
		PORTD |= (1 << 7);   // DP OFF (PD7 HIGH)
	}
}

void seven_segment_test(void) {
	PORTD = 0x00;    // All segments ON (PD0-PD6 LOW, DP LOW)
	_delay_ms(1000);
	PORTD = 0xFF;    // All segments OFF
}
