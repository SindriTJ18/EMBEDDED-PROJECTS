#include <avr/io.h>
#include <avr/delay.h>
#include "encoder.h"
#include "arduino.h"
#include "digital_out.h"
#include "timer1_ms.h"
#include "timer0_ms.h"
#include "p_controller.h"
#include "analog_in.h"

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
}

Digital_out outputPin(4);
int main()
{
  // SET OBJ. AND INIT
  outputPin.init();
  int updatedValue = 0;
  int motorInput = 0;
  Encoder encoder;
  P_controller control(5);
  Timer0_ms timer0(1);
  Timer1_ms timer1(10);
  timer0.init();
  timer1.init();

  Analog_in analog(0);

  while (1)
  {
    // SEEK VALUE AND APPLY
    updatedValue = int(control.update(analog.sample(), encoder.pps() * 10));
    motorInput = control.control(motorInput, updatedValue);
    // UPDATE RATE (250Hz)
    _delay_ms(4);
  }
  return 0;
}

// TIMER 0 INTERRUPT
ISR(TIMER0_COMPA_vect)
{
  outputPin.set_hi();
}

ISR(TIMER0_COMPB_vect)
{
  outputPin.set_lo();
}
