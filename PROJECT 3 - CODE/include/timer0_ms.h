#ifndef TIMER0_MS_H
#define TIMER0_MS_H

class Timer0_ms
{
public:
    Timer0_ms(int ms);
    void init();
    void duty(int percent);

private:
    int compare;
};

#endif