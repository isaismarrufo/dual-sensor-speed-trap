#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "project.h"

/* ISR for high/low buttons */
ISR(PCINT0_vect)
{
    if (!(PINB & (1 << PB4))) sel_mode = 1;
    if (!(PINB & (1 << PB5))) sel_mode = 0;
}
 
/* ISR for encoder values */
ISR(PCINT1_vect)
{
    uint8_t pins = PINC;
    uint8_t bit_a = (pins & (1 << PC1)) >> 1;
    uint8_t bit_b = (pins & (1 << PC2)) >> 2;
    uint8_t curr = (bit_b << 1) | bit_a;
 
    if (curr == last_enc_val) return;
 
    int8_t move = 0;
    uint8_t combo = (last_enc_val << 2) | curr;
 
    if (combo == 0b0001 || combo == 0b0111 || combo == 0b1110 || combo == 0b1000) move = 1;
    if (combo == 0b0010 || combo == 0b1011 || combo == 0b1101 || combo == 0b0100) move = -1;
 
    last_enc_val = curr;
 
    if (move != 0)
    {
        enc_flag = 1;
        if (move == 1)
        {
            if (sel_mode == 0)
            {
                if (bound_l < bound_h - 1) bound_l++;
            }
            else
            {
                if (bound_h < 99) bound_h++;
            }
        }
        else
        {
            if (sel_mode == 0)
            {
                if (bound_l > 1) bound_l--;
            }
            else
            {
                if (bound_h > bound_l + 1) bound_h--;
            }
        }
        eeprom_update_byte((void*)0, bound_l);
        eeprom_update_byte((void*)1, bound_h);
    }
}
 
/* ISR for sensor timing */
ISR(PCINT2_vect)
{
    uint8_t d_port = PIND;

    if (!(d_port & (1 << PD2)))
    {
        if (!timing)
        {
            timing = 1;
            timing_ready = 0;
            timing_error = 0;
            PORTD |= (1 << PD1);
 
            TCNT1 = 0;
            TCCR1B |= (1 << CS12) | (1 << CS10);
        }
    }
    else if (!(d_port & (1 << PD3)))
    {
        if (timing)
        {
            timing = 0;
            t_counts = TCNT1;
 
            TCCR1B &= ~((1 << CS12) | (1 << CS10)); 
 
            timing_ready = 1;
            PORTD &= ~(1 << PD1);
        }
    }
}
 
/* ISR for timing out the sensor */
ISR(TIMER1_COMPA_vect)
{
    if(timing)
    {
        TCCR1B &= ~((1 << CS12) | (1 << CS10)); 
        timing = 0;
        timing_error = 1;
        PORTD &= ~(1 << PD1);
    }
}
 
/* ISR for playing buzzer for 0.25seconds */
ISR(TIMER0_COMPA_vect)
{
    PORTC ^= (1 << PC3);
    tone_dur++;
    if(tone_dur >= tone_target)
    {
        TCCR0B &= ~(1 << CS02); 
        PORTC &= ~(1 << PC3);
        snd_done = 1;
    }
}
