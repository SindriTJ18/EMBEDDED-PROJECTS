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
    TCCR0A = 0;
    // TCCR1B = 0;
    TCNT0 = 0;
    // SET UP A 50% DUTY CYCLE IF DUTY FUNCTION IS UNUSED
    OCR0A = compare;
    // OCR1B = compare;
    // TIMER RESET UPON REACHING TIMER COMPARE VEC A
    TCCR0A |= (1 << WGM01);
    // ENABLE TIMER INTERRUPTS
    TIMSK0 |= (1 << OCIE1A); // | (1 << OCIE1B);
    // PRESCALER TO 64
    TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
    // ENABLE INTERRUPTS
    sei();
}
/*
void Timer1_ms::duty(int percent)
{
    int inverse = 100 - percent;
    // CONFIGURE LED "ON" TIMER FLAG
    OCR1B = int(compare * (inverse / 100.0));
}
*/
