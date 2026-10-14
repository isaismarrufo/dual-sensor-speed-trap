#include <avr/io.h>
#include <stdint.h>
#include "drivers.h"
#include "project.h"

/* Set the speed of the servo based on the raw speed */
void set_servo(uint32_t raw_spd)
{
    // Map speed (0-1000) to servo vals (11-35)
    uint32_t val = 35 - (int)((24 * raw_spd) / 1000);
    if (val > 35) val = 35;
    if (val < 11) val = 11;
    OCR2A = val;
}
 
/* Play a note on the buzzer */
void play_note(uint16_t freq)
{
    snd_done = 0;
    tone_dur = 0;
    tone_target = freq / 2;
    OCR0A = 16000000 / (512 * freq) - 1;
    TCNT0 = 0;
    TCCR0B |= (1 << CS02); 
}