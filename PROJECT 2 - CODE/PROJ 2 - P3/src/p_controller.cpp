#include "p_controller.h"
#include <avr/io.h>

int limit(int val);
P_controller::P_controller(double constant)
{
    k_p = constant;
}

double P_controller::update(double ref, double actual)
{
    return k_p * (ref - actual);
}

int P_controller::control(int V_m, int u)
{
    int V_m_1 = limit(V_m + u);
    OCR0B = V_m_1;
    return V_m_1;
}

int limit(int val)
{
    if (val > 244)
    {
        return 244;
    }
    if (val < 1)
    {
        return 1;
    }
    return val;
}