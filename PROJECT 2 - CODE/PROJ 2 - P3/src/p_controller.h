#ifndef P_CONTEOLLER_H
#define P_CONTEOLLER_H

class P_controller
{
public:
    P_controller(double constant);
    double update(double ref, double actual);
    int control(int V_m, int u);

private:
    double k_p;
};

#endif