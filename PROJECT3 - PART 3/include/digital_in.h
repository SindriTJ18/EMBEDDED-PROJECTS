#ifndef DIGITAL_IN
#define DIGITAL_IN

#include <stdint.h>

class Digital_in
{
public:
    Digital_in(int pinNumber, char port);
    void init();
    void enablePullup();
    bool is_hi();
    bool is_lo();
    bool is_hi_d();
    bool is_lo_d();

private:
    uint8_t pinMask;
    char portChar;
};

#endif