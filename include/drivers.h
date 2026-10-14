#ifndef DRIVERS_H
#define DRIVERS_H

#include <stdint.h>

void set_servo(uint32_t raw_spd);
void play_note(uint16_t hz);

#endif