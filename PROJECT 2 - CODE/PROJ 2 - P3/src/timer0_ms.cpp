#include "timer0_ms.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "arduino.h"

Timer0_ms::Timer0_ms(int us)
{
    // INIT COMPARE REGISTER TO MILLISECONDS
    // compare = (16 / 1) * us - 1;
    compare = 255;
}

void Timer0_ms::init()
{
    // INITIALIZE REGISTERS
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    // SET UP A 50% DUTY CYCLE IF DUTY FUNCTION IS UNUSED
    OCR0A = compare;
    OCR0B = 0;
    // TIMER RESET UPON REACHING TIMER COMPARE VEC A
    // TCCR0B |= (1 << WGM02);
    TCCR0A |= (1 << WGM01);
    // ENABLE TIMER INTERRUPTS
    TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B);
    // PRESCALER TO 1
    TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);
    // ENABLE INTERRUPTS
    sei();
}
