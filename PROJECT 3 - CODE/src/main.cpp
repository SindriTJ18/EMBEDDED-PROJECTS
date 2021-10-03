#include "Arduino.h"
#include <avr/delay.h>

void setup()
{
  Serial.begin(9600);
}

char command;
class Context;

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

// ENTRY
void Initialize::Entry()
{
  Serial.println("Entry: Initialize");
  for (int i = 0; i < 4; i++)
  {
    Serial.println("Initializing..");
  }
  _delay_ms(5);
  this->context_->TransitionTo(new PreOperational);
}

void PreOperational::Entry()
{
  Serial.println("Entry: Pre-Operational");
  Serial.println("Waiting for user input to start!");
  while (!Serial.available())
  {
  }
  this->context_->TransitionTo(new Operational);
}

void Operational::Entry()
{
  Serial.println("Entry: Operational");
  _delay_ms(5);
}

// INITIALIZE THE STATE AS RED
Context *context;

void loop()
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