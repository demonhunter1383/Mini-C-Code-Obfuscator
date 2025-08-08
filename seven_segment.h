#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include <avr/io.h>
#include <stdint.h>

void seven_segment_init(void);
void seven_segment_display(uint8_t num);

#endif
