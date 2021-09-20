#include "encoder.h"
#include "digital_out.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include "timer1_ms.h"

int ENCODER_COUNT = 0;
int ENCODER_SPEED = 0;
Digital_in dirPin(0, 'B');
Digital_in pin1(2, 'D');

Encoder::Encoder()
{
    EICRA |= (1 << ISC00); //Sense interrupt on any logic change (to account for both pulses)
    EIMSK |= (1 << INT0);  //Enable interrupt on PCINT0
    sei();                 //Enable interrupts
}

int Encoder::count()
{
    return ENCODER_COUNT;
}

int Encoder::pps()
{
    return ENCODER_SPEED;
}

ISR(INT0_vect)
{
    if (pin1.is_hi())
    {
        if (dirPin.is_hi())
        {
            ENCODER_COUNT++;
        }
        else
        {
            ENCODER_COUNT--;
        }
    }
    else
    {
        if (dirPin.is_lo())
        {
            ENCODER_COUNT++;
        }
        else
        {
            ENCODER_COUNT--;
        }
    }
}

ISR(TIMER0_COMPA_vect)
{
    ENCODER_SPEED = ENCODER_COUNT * 100;
    Serial.println(ENCODER_SPEED);
    ENCODER_COUNT = 0;
}
