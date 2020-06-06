#ifndef FOURIERDECOMPOSITION_H
#define FOURIERDECOMPOSITION_H
#include "fft/ifourier.h"
#include <math.h>

class FourierDecomposition : public IFourier
{
public:
    double* GetFrame(double x, int count) override;
private:
    double getY(double x, bool isRightChannel);
    double integral(double a, double freq, bool isRightChannel);
};

#endif // FOURIERDECOMPOSITION_H
