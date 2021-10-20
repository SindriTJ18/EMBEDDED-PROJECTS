#include "encoder.h"
#include "p_controller.h"
#include "digital_out.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include "timer1_ms.h"
#include "analog_in.h"

Analog_in::Analog_in(int pin)
{
    ADMUX = pin;            // use ADC0
    ADMUX |= (1 << REFS0);  // use AVcc as the reference
    ADMUX |= (1 << ADLAR);  // Right adjust for 8 bit resolution
    ADCSRA |= (1 << ADATE); // Set ADC Auto Trigger Enable
    ADCSRB = 0;             // 0 for free running mode
    ADCSRA |= (1 << ADEN);  // Enable the ADC
    ADCSRA |= (1 << ADSC);  // Start the ADC conversion
}

int Analog_in::sample()
{
    return (ADCH - 127) * 2 - 1;
}
