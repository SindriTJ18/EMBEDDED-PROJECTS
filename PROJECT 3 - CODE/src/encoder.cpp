#include "encoder.h"
#include "p_controller.h"
#include "digital_out.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>
#include "timer1_ms.h"

int ENCODER_COUNT = 0;
double ENCODER_SPEED = 4;
double speed_ratio;
Digital_in dirPin(0, 'B');
Digital_in pin1(2, 'D');

Encoder::Encoder()
{
    EICRA |= (1 << ISC00); //Sense interrupt on pin0 change rising edge (NOT SAME AS IN DATASHEET)
    EIMSK |= (1 << INT0);
    // outputPin.init();
    // outputPin.set_hi(); //Enable interrupt on PCINT0
    // sei(); //Enable interrupts
}

int Encoder::count()
{
    return ENCODER_COUNT;
}

double Encoder::pps()
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

// // TIMER 1 INTERRUPT
ISR(TIMER1_COMPA_vect)
{
    // outputPin.set_hi();
    // ENCODER_SPEED = ENCODER_COUNT * 100.0;
    ENCODER_SPEED = ENCODER_COUNT * 10;
    // Serial.println("TIMER 1");
    // Serial.println(ENCODER_SPEED);
    speed_ratio = ENCODER_SPEED / 2400.0;
    // OCR1B = (ADCH / 255.0) * ((16000 / 1024.0) * 10.0 - 1);
    ENCODER_COUNT = 0;
    // Serial.println("DOGS1");
}

ISR(TIMER1_COMPB_vect)
{
    // Serial.println("FOKOF");
}
