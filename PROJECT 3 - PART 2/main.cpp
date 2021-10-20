#include "Arduino.h"
#include <util/delay.h>
#include <avr/io.h>
#include <digital_out.h>
#include <digital_in.h>
#include <encoder.h>
#include <timer0_ms.h>
#include <timer1_ms.h>
#include <p_controller.h>
#include <analog_in.h>
/*
void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
}
*/

// GLOBAL INITS
char command;
class Context;

Digital_out LED(5);
Digital_out drivePin(1);
Analog_in analogPin(3);
Encoder encoder;
Timer1_ms timer1(100);
Digital_in fltPin(3, 'B');

class State
{
protected:
  Context *context_;

public:
  virtual ~State()
  {
  }

  void set_context(Context *context)
  {
    this->context_ = context;
  }

  virtual void Reset() = 0;
  virtual void Entry() = 0;
};

class Context
{
private:
  State *state_;

public:
  Context(State *state) : state_(nullptr)
  {
    this->TransitionTo(state);
  }
  ~Context()
  {
    delete state_;
  }
  void TransitionTo(State *state)
  {
    Serial.print("Context: Transition to ");
    if (this->state_ != nullptr)
      delete this->state_;
    this->state_ = state;
    this->state_->set_context(this);
    this->state_->Entry();
  }
  void ResetRequest()
  {
    this->state_->Reset();
  }
  void EntryRequest()
  {
    this->state_->Entry();
  }
};

class Initialize : public State
{
public:
  void Reset() override;

  void Entry() override;
};

class PreOperational : public State
{
public:
  void Reset() override;

  void Entry() override;
};

class Operational : public State
{
public:
  void Reset() override;

  void Entry() override;
};

class Stopped : public State
{
public:
  void Reset() override;

  void Entry() override;
};
// THE FUNCTIONS FOR CONTROLLING

// RESET
void Initialize::Reset()
{
  Serial.println("Reset: Reset the machine");
  this->context_->TransitionTo(new Initialize);
}

void PreOperational::Reset()
{
  Serial.println("Reset: Reset the machine");
  this->context_->TransitionTo(new Initialize);
}

void Operational::Reset()
{
  Serial.println("Reset: Reset the machine");
  this->context_->TransitionTo(new Initialize);
}

void Stopped::Reset()
{
  Serial.println("Reset: Reset the machine");
  this->context_->TransitionTo(new Initialize);
}

// ENTRY
void Initialize::Entry()
{
  //cli();
  // OPERATIONS TO HAPPEN IN STATE GO HERE!
  LED.init();
  drivePin.init();
  LED.set_lo();
  drivePin.set_lo();
  fltPin.init();
  fltPin.enablePullup();

  Serial.println("Entry: Initialized!");
  _delay_ms(5);
  this->context_->TransitionTo(new PreOperational);
}

void PreOperational::Entry()
{
  // OPERATIONS TO HAPPEN IN STATE GO HERE!
  Serial.println("Entry: Pre-Operational");
  Serial.println("Waiting for user input to start!");
  while (!Serial.available())
  {
    LED.toggle();
    _delay_ms(500);
  }
  this->context_->TransitionTo(new Operational);
}

void Operational::Entry()
{

  _delay_ms(100);
  LED.set_hi();
  drivePin.set_hi();
  Serial.println("Entry: Operational");
  _delay_ms(100);
  // LED.set_hi();
  // LED.set_hi();
  timer1.init();
  // OPERATIONS TO HAPPEN IN STATE GO HERE!
  sei();
  while (fltPin.is_hi())
  {
    Serial.print("Analog value: ");
    Serial.println(analogPin.sample());
  }
  this->context_->TransitionTo(new Stopped);
}

void Stopped::Entry()
{
  drivePin.set_lo();
  while (fltPin.is_lo())
  {
    LED.toggle();
    _delay_ms(250);
  }
  this->context_->TransitionTo(new PreOperational);
}
// INITIALIZE THE STATE AS RED
Context *context;

int main()
{
  init();
  Serial.begin(9600);
  while (1)
  {
    // INPUT REQUESTS INTO SERIAL
    if (Serial.available() > 0)
    {
      command = Serial.read();
      switch (command)
      {
      case 's':
      {
        Serial.println("Start command.");
        Context *context = new Context(new Initialize);
        break;
      }
      // GERIR EKKERT EINS OG ER
      case 'r':
      {
        Serial.println("Reset command.");
        _delay_ms(100);
        // NÆ EKKI AÐ CALLA REQUEST THROWAR ERROR, POTTÞETT POINTER BULL I CONTEXT
        context->ResetRequest();
        break;
      }
      }
    }
  }

  return 550;
}