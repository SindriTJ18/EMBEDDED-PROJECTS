#ifndef TIMER1_MS_H
#define TIMER1_MS_H

class Timer1_ms
{
public:
    Timer1_ms(int ms);
    void init();
    void duty(int percent);

private:
    int compare;
};

#endif