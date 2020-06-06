#ifndef IFOURIER_H
#define IFOURIER_H
#include "controllers/playercontroller.h"
#include <math.h>

class IFourier
{
protected:
    PlayerController *player;
    const double dpi = 2*acos(-1);
public:
    virtual double* GetFrame(double x, int count) = 0;
    void SetPlayer(PlayerController *player);
};

#endif // IFOURIER_H
