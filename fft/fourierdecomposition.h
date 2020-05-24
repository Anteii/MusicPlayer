#ifndef FOURIERDECOMPOSITION_H
#define FOURIERDECOMPOSITION_H
#include "controllers/playercontroller.h"
#include <math.h>

class FourierDecomposition
{
protected:
    PlayerController *player;
    const double dpi = 2*acos(-1);
public:
    FourierDecomposition(PlayerController *player);
    virtual double* GetFrame(double x, int count);
private:
    double getY(double x, bool isRightChannel);
    double integral(double a, double freq, bool isRightChannel);
};

#endif // FOURIERDECOMPOSITION_H
