#ifndef PROJECT_H
#define PROJECT_H

#include <avr/io.h>
#include <stdint.h>

extern volatile uint8_t timing;
extern volatile uint8_t timing_ready;
extern volatile uint8_t timing_error;
extern volatile uint32_t t_counts;
extern volatile uint16_t bound_l;
extern volatile uint16_t bound_h;
extern volatile uint8_t sel_mode;
extern volatile uint8_t enc_flag;
extern volatile uint8_t last_enc_val;
extern volatile uint16_t tone_dur;
extern volatile uint16_t tone_target;
extern volatile uint8_t snd_done;

extern uint8_t err_seq_h;
extern uint8_t err_seq_l;

#endif