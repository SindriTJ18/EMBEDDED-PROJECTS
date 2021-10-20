#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <avr/io.h>
#include "digital_out.h"

extern Digital_out drivePin;
extern Digital_out nonDrivePin;

int limit(int val);
int absolute(int val);
// PORTB |= (1 << 2);

class Controller
{
public:
    // P_controller(double constant);
    virtual double update(double ref, double actual) = 0;
    virtual int control(int u) = 0;
    virtual ~Controller() {}

    // private:
    //     double k_p;
    //     double k_i;
};

class P_controller : public Controller
{
public:
    P_controller(double constant) { k_p = constant; }
    double update(double ref, double actual) { return k_p * (ref - actual); }
    int control(int u)
    {
        int V_m_1 = limit(u);
        OCR0B = V_m_1;
        return V_m_1;
    }

private:
    double k_p;
};
class PI_controller : public Controller
{
public:
    PI_controller(double constant_p, double constant_i)
    {
        sum = 0;
        k_p = constant_p;
        k_i = constant_i;
    }
    double update(double ref, double actual)
    {
        int error = ref - actual;
        if (k_i * sum < 255)
        {
            sum += error * t;
        }
        return k_p * error + k_i * error;
    }
    int control(int u)
    {
        if (u > 0)
        {
            // nonDrivePin.switchPin(2);
            drivePin.switchPin(1);
        }
        else
        {
            // nonDrivePin.switchPin(1);
            drivePin.switchPin(2);
        }
        u = absolute(limit(u));
        OCR0B = u;
        return u;
    }

private:
    double k_p;
    double k_i;
    int sum;
    double t = 0.004;
};

int limit(int val)
{
    if (val > 255)
    {
        return 255;
    }
    if (val < -255)
    {
        return -255;
    }
    return val;
}

int absolute(int val)
{
    if (val < 0)
    {
        return -1 * val;
    }
    else
    {
        return val;
    }
}

#endif