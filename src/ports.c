#include <avr/io.h>
#include <avr/eeprom.h>
#include "ports.h"
#include "project.h"
#include "lcd.h"

/* Set the initial values for the ports */
void ports_init(void)
{
    // Read bounds from EEPROM
    bound_l = eeprom_read_byte((void*)0);
    bound_h = eeprom_read_byte((void*)1);
    
    // If they are outside of the valid range, set to defaults
    if(bound_l < 1 || bound_l > 99) bound_l = 10;
    if(bound_h < 1 || bound_h > 99) bound_h = 90;
    
    // Initialize ports for high/low selection buttons
    PORTB |= (1<<PB4)|(1<<PB5);
    PCMSK0 |= (1<<PCINT4)|(1<<PCINT5);
    PCICR |= (1<<PCIE0);
    
    // Intialize ports for thr rotary encoder
    PORTC |= (1<<PC1)|(1<<PC2);
    DDRC |= (1<<PC3) | (1<<PC4) | (1<<PC5); 
    PORTC |= (1<<PC4) | (1<<PC5); 
    PCMSK1 |= (1<<PCINT9)|(1<<PCINT10);
    PCICR |= (1<<PCIE1);
    
    // Intialize ports for sensor
    PORTD |= (1<<PD2)|(1<<PD3);
    DDRD |= (1<<PD1);
    PORTD &= ~(1<<PD1);
    PCMSK2 |= (1<<PCINT18)|(1<<PCINT19);
    PCICR |= (1<<PCIE2);
    
    // Set up buzzer as output
    DDRB |= (1<<PB3);

    // Set up timers
    TCCR0A = (1 << WGM01); 
    TIMSK0 = (1 << OCIE0A);
 
    // Timer1 for longer timing
    TCCR1B = (1 << WGM12); 
    OCR1A = 62500; 
    TIMSK1 = (1 << OCIE1A);
 
    // Timer2 for the buzzer control
    TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20); 
    TCCR2A = 0b00100011; 
    TCCR2B = 0b00000111; 
    OCR2A = 24;
    
    // Read the initial encoder state
    uint8_t p_init = PINC;
    uint8_t s1 = (p_init & (1<<PC1)) >> 1;
    uint8_t s2 = (p_init & (1<<PC2)) >> 2;
    last_enc_val = (s2 << 1) | s1;
    
    // Start the lcd
    lcd_init();
}
