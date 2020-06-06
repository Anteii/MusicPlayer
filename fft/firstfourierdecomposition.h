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
public:
    FirstFourierDecomposition();
    ~FirstFourierDecomposition();
    double* GetFrame(double x, int count) override;
private:
    void setAdress();
    void CreateW2n();
    void fft(std::complex<double> *x, bool complement);
    template <class T> void getCompl(T arr, int i, int Channels);
};

#endif // FIRSTFOURIERDECOMPOSITION_H
