#include <avr/io.h>
#include <avr/delay.h>
#include "encoder.h"
#include "arduino.h"
#include "digital_out.h"
#include "timer1_ms.h"

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for Native USB only
  }
}

int main()
{
  // drive the motor high!
  Encoder encoder;
  Digital_out outputPin(4);
  Timer1_ms timer1(10);
  outputPin.init();
  outputPin.set_hi();
  timer1.init();

  // INIT OUTPUT PIN AND POSITION INTEGER

  while (1)
  {
  }
  return 0;
}
