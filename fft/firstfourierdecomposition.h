#ifndef FIRSTFOURIERDECOMPOSITION_H
#define FIRSTFOURIERDECOMPOSITION_H
#include "fft/ifourier.h"
#include <complex>

class FirstFourierDecomposition : public IFourier {
    int length;
    int size;
    unsigned int* adress;
    std::complex<double>* W2n;
    std::complex<double> *tempArr;

    enum Channels {
        Singl, Left, Right
    };

public:
    FirstFourierDecomposition();
    ~FirstFourierDecomposition();
    double* GetFrame(double x, int count) override;
private:
    void setAdress();
    void CreateW2n();
    void fft(std::complex<double> *x, bool complement);
    void getCompl(short int* arr, int i, Channels channels);
};

#endif // FIRSTFOURIERDECOMPOSITION_H
