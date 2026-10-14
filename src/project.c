#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "project.h"
#include "ports.h"
#include "drivers.h"
#include "lcd.h"

volatile uint8_t timing = 0;
volatile uint8_t timing_ready = 0;
volatile uint8_t timing_error = 0;
volatile uint32_t t_counts = 0;
volatile uint16_t bound_l = 1;
volatile uint16_t bound_h = 99;
volatile uint8_t sel_mode = 0; 
volatile uint8_t enc_flag = 0;
volatile uint8_t last_enc_val = 0;
volatile uint16_t tone_dur = 0;
volatile uint16_t tone_target = 0;
volatile uint8_t snd_done = 1;

uint8_t err_seq_h = 0;
uint8_t err_seq_l = 0;

void ports_init(void);
void set_servo(uint32_t raw_spd);
void play_note(uint16_t hz);

int main(void)
{
    // buffer to write text for LCD
    char txt_buf[17];

    // Variables to calculate speed
    uint32_t calc_ms;
    uint32_t final_spd;

    // Init ports
    ports_init();

    // Write splash screen
    lcd_moveto(0,0);
    lcd_stringout("Isais Marrufo");
    _delay_ms(1000);
    lcd_writecommand(1);

    // Start interrupts
    sei();

    // Initial screen
    lcd_moveto(1,0);
    sprintf(txt_buf, ">Low=%2d  High=%2d", bound_l, bound_h);
    lcd_stringout(txt_buf);

    // Main loop - runs forever
    while(1)
    {
        // If the encoder has a new value
        if(enc_flag)
        {
            // Reset encoder flag
            enc_flag = 0;

            // Update LCD with new selection
            lcd_moveto(1,0);
            if(sel_mode == 0)
                sprintf(txt_buf, ">Low=%2d  High=%2d", bound_l, bound_h);
            else
                sprintf(txt_buf, " Low=%2d >High=%2d", bound_l, bound_h);

            lcd_stringout(txt_buf);
        }

        // If timing is in progess
        if(timing)
        {
            lcd_moveto(0,0);
            lcd_stringout("Timing...       ");
        }

        // If timing has an error
        if(timing_error)
        {
            timing_error = 0;
            lcd_moveto(0,0);
            lcd_stringout("Time out occured");
        }

        // Frequenciees for the buzzer to play
        uint16_t freq[] = { 350, 400, 450, 500 };

        // If 3 consecutive timings are out of bounds
        if(err_seq_h >= 3 || err_seq_l >= 3)
        { 
            err_seq_h = 0;
            err_seq_l = 0;

            // Play buzzer frequenciees
            for(uint8_t k=0; k<4; k++)
            {
                play_note(freq[k]);
                // Play next note, only when the previous note is finished
                while(!snd_done);
            }
        }

        // If the timer is finished 
        if(timing_ready)
        {
            timing_ready = 0;

            // Prescalar of 64 * count = number of microseconds / 1000 to get ms
            calc_ms = (t_counts * 64) / 1000; 
        
            // Dont divide by zer0
            if(calc_ms == 0)
            {
                calc_ms = 1;
            }

            // Distance is 4cm multiply by 1000 to get ms in seconds and then multiply by another 10 to get avoid floats
            final_spd = 4*1000*10 / calc_ms; 

            // Set servo position based on speed
            set_servo(final_spd);
        
            // Mask for RGB led
            uint8_t led_mask = (1 << PC4) | (1 << PC5); 

            // If the final speed was above the bound
            if(final_spd > bound_h * 10)
            {
                // Turn on red LED (by setting PC4 low)
                led_mask &= ~(1 << PC4); 
                err_seq_h++;
                err_seq_l = 0;
            }
            // If the final speed was below the bound
            else if(final_spd < bound_l * 10)
            {
                // Turn on blue LED (set PC5 low)
                led_mask &= ~(1 << PC5); 
                err_seq_l++;
                err_seq_h = 0;
            }
            // If the final speed was between the bound
            else
            {
                // Turn on green LED (by setting both PC4 and PC5 high (default value)
                err_seq_h = 0;
                err_seq_l = 0;
            }

            // Update the RGB LED state
            PORTC = (PORTC & ~((1 << PC4)|(1 << PC5))) | (led_mask & ((1 << PC4)|(1 << PC5)));

            // Update LCD with speed info
            // To get the tens place, divide by 10
            // To get the decimal place, get the remainder of division by 10
            lcd_moveto(0,0);
            sprintf(txt_buf, "%5d ms = %3d.%1d", calc_ms, final_spd/10, final_spd%10);
            lcd_stringout(txt_buf);
        }
    }
}
