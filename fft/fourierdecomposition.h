#ifndef FOURIERDECOMPOSITION_H
#define FOURIERDECOMPOSITION_H
#include "controllers/playercontroller.h"
#include <math.h>
#include <complex>

class FourierDecomposition
{
protected:
    PlayerController *player;
    const double dpi = 2*acos(-1);
    std::complex<double> *tempArr;
public:
    FourierDecomposition(PlayerController *player);
    virtual double* GetFrame(double x, int count);
private:
    double getY(double x, bool isRightChannel);
    double integral(double a, double freq, bool isRightChannel);
};

class FirstFourierDecomposition : public FourierDecomposition {
    int length;
    unsigned int* adress;
    std::complex<double>* W2n;
public:
    FirstFourierDecomposition(PlayerController *player);
    ~FirstFourierDecomposition();
    double* GetFrame(double x, int count) override;
private:
    void setAdress();
    void CreateW2n();
    void fft(std::complex<double> *x, bool complement);
    template <class T> double*
    getCompl(T arr, int i, int Channels);
};

#endif // FOURIERDECOMPOSITION_H
