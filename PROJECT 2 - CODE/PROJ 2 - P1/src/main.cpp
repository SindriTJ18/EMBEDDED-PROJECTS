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
  // CONFIGURE OBJECTS
  Encoder encoder;
  Digital_out outputPin(4);
  Timer1_ms timer(10);
  // INITS
  outputPin.init();
  outputPin.set_hi();
  timer.init();
  // PERMALOOP
  while (1)
  {
  }
  return 0;
}
