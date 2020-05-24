#include "firstfourierdecomposition.h"

FirstFourierDecomposition::FirstFourierDecomposition(PlayerController *player) :
FourierDecomposition(player) {
    length = 10;
    size = 1 << length;
    tempArr = new std::complex<double>[size];
    setAdress();
    CreateW2n();
}
FirstFourierDecomposition::~FirstFourierDecomposition(){
    delete[] adress;
    delete[] W2n;
    delete[] tempArr;
}
template <class T> void FirstFourierDecomposition::getCompl(T arr, int i, int Channels) {
    if(Channels == 0) {
        for(int j = 0; j < size; ++j) {
            tempArr[j].real(arr[i+j] / 128.0);
            tempArr[j].imag(0);
        }
    }
    else if(Channels == 1) {
        for(int j = 0; j < size; ++j) {
            tempArr[j].real(arr[2*(i+j)] / 128.0);
            tempArr[j].imag(0);
        }
    }
    else {
        for(int j = 0; j < size; ++j) {
            tempArr[j].real(arr[2*(i+j) + 1] / 128.0);
            tempArr[j].imag(0);
        }
    }
}
double* FirstFourierDecomposition::GetFrame(double x, int count) {
    double* res = new double[2*count];
    auto getMod = [](std::complex<double> t){ return sqrt(t.real()*t.real() + t.imag()*t.imag()); };
    x *= player->getTrackFile()->getSampleRate();
    unsigned int i = (int)x;
    if(player->getTrackFile()->getBitsPerSample() == 8) {
        char* arr = player->getTrackFile()->getData();
        if(player->getTrackFile()->getNumChannels() == 1) getCompl(arr, i, 0);
        else getCompl(arr, i, 1);
        fft(tempArr, true);
        for(int j = 0; j < count; ++j)
            res[2*j] = getMod(tempArr[j * size / count]);
        if((player->getTrackFile()->getNumChannels() == 1))
            for(int j = 0; j < count; ++j) res[2*j + 1] = res[2*j];
        else {
            getCompl(arr, i, 2);
            fft(tempArr, true);
            for(int j = 0; j < count; ++j)
                res[2*j + 1] = getMod(tempArr[j * size / count]);
        }

    } else {
        short int* arr = (short int*)player->getTrackFile()->getData();
        if(player->getTrackFile()->getNumChannels() == 1) getCompl(arr, i, 0);
        else getCompl(arr, i, 1);
        fft(tempArr, false);
        for(int j = 0; j < count; ++j)
            res[2*j] = getMod(tempArr[j * size / count]);
        if((player->getTrackFile()->getNumChannels() == 1))
            for(int j = 0; j < count; ++j) res[2*j + 1] = res[2*j];
        else {
            getCompl(arr, i, 2);
            fft(tempArr, false);
            for(int j = 0; j < count; ++j)
                res[2*j + 1] = getMod(tempArr[j * size / count]);
        }
    }
    return res;
}
void FirstFourierDecomposition::setAdress() {
    adress = new unsigned int[size];
    for(int i = 0; i < size; ++i) {
        adress[i] = 0;
        int te = size >> 1;
        int tb = 1;
        do {
            if((i & tb) != 0)
                adress[i] |= (te & (unsigned int)(-1));
            te >>= 1;
            tb <<= 1;
        } while(te != 0);
    }
}
void FirstFourierDecomposition::CreateW2n() {
    W2n = new std::complex<double>[32];
    for(int i = 0; i < 32; ++i){
        W2n[i].real( cos(dpi/(unsigned long long)(2 << i)));
        W2n[i].imag( sin(dpi/(unsigned long long)(2 << i)));
    }
}
void FirstFourierDecomposition::fft(std::complex<double> *x, bool complement){
    unsigned int N, Nd2, k, mpNd2, Skew;
    std::complex<double> *Wstore, *Warray;
    std::complex<double> WN, W, Temp, *pWN;

    for(unsigned int i = 0; i < size; i++) {
        unsigned int j = adress[i];
        if(j > i) {
            std::complex<double> S = x[i];
            x[i] = x[j];
            x[j] = S;
        }
    }

    Wstore = new std::complex<double>[size / 2];
    Wstore[0].real(1.0);
    Wstore[0].imag(0.0);

    for(N = 2, Nd2 = 1, pWN = W2n, Skew = size >> 1; N <= size; Nd2 = N, N <<= 1, pWN++, Skew >>= 1)
    {
        //WN = W(1, N) = exp(-2*pi*j/N)
        WN= *pWN;
        if (complement)
            WN.imag(-WN.imag());
        for(Warray = Wstore, k = 0; k < Nd2; k++, Warray += Skew)
        {
            if (k & 1)
            {
                W *= WN;
                *Warray = W;
            }
            else
                W = *Warray;

            for(unsigned int m = k; m < size; m += N)
            {
                mpNd2 = m + Nd2;
                Temp = W;
                Temp *= x[mpNd2];
                x[mpNd2] = x[m];
                x[mpNd2] -= Temp;
                x[m] += Temp;
            }
        }
    }
    delete [] Wstore;

    if (complement)
    {
        for(int i = 0; i < size; i++)
            x[i] /= size;
    }
}
