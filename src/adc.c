#include <avr/io.h>

#include "adc.h"


void adc_init(void)
{
    // Initialize the ADC
    // REFS1=0, REFS0=1, avcc reference
    // ADLAR=1, left adjust, 8bit result in ADCH
    ADMUX = (1 << REFS0) | (1 << ADLAR);
    // Enable ADC "ADEN=1" and set prescaler to 128, ADPS2:0 = 111
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t adc_sample(uint8_t channel)
{
    // Set ADC input mux bits to 'channel' value
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    // Convert an analog input and return the 8-bit result
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADCH;
}
