#include "timer1_ms.h"
#include <avr/io.h>
#include <avr/interrupt.h>

Timer1_ms::Timer1_ms(int ms)
{
    // INIT COMPARE REGISTER TO MILLISECONDS
    compare = (16000 / 1024) * ms - 1;
}

void Timer1_ms::init()
{
    // INITIALIZE REGISTERS
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    // SET UP A 50% DUTY CYCLE IF DUTY FUNCTION IS UNUSED
    OCR1A = compare;
    OCR1B = compare;
    // TIMER RESET UPON REACHING TIMER COMPARE VEC A
    TCCR1B |= (1 << WGM12);
    // ENABLE TIMER INTERRUPTS
    TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B);
    // PRESCALER TO 1024
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
    // ENABLE INTERRUPTS
    sei();
}
