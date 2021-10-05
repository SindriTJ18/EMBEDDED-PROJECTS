#ifndef ENCODER_H
#define ENCODER_H

#include "digital_in.h"

class Encoder
{
public:
    Encoder();
    int count();
    int pps();
    void duty(int percent);
    void init();

private:
};

#endif