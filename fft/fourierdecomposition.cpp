#include "fourierdecomposition.h"

FourierDecomposition::FourierDecomposition(PlayerController *player) {
    this->player = player;
}
double FourierDecomposition::getY(double x, bool isRightChannel) {
    x *= player->getTrackFile()->getSampleRate();
    if (player->getTrackFile()->getBitsPerSample() == 8)
    {
        char* arr = player->getTrackFile()->getData();
        if (player->getTrackFile()->getNumChannels() == 1) return arr[(int)x];
        else if (isRightChannel) return arr[(int)x * 2 + 1];
        else return arr[(int)x * 2];
    }
    else
    {
        short int* arr = (short int*)player->getTrackFile()->getData();
        if (player->getTrackFile()->getNumChannels() == 1) return arr[(int)x];
        else if (isRightChannel) return arr[(int)x * 2 + 1];
        else return arr[(int)x * 2];
    }
}
double FourierDecomposition::integral(double a, double freq, bool isRightChannel) {
    double x = 0, y = 0;
    double b = freq == 0 ? dpi : dpi / freq;
    if(freq > 1000) b *= 4;
    else if(freq > 100) b *= 2;
    double step = b / 2000;
    b += a;
    for (double i = a; i <= b; i += step) {
        double t = (i - a) * freq;
        double Y = getY(i, isRightChannel);
        x += Y * cos(t);
        y += Y * sin(t);
    }
    return step * sqrt(x*x + y*y);
}
double* FourierDecomposition::GetFrame(double x, int count) {
    double C = 100;                        // 100 - минимальная частота
    double alpha = log(16000/C) / count;    // 16000 - максимальная частота
    double* arrFreq = new double[count * 2];
    for(int i = 0; i < count; i++) {
        arrFreq[i*2] = integral(x, C*exp(alpha*i), true);
        if(player->getTrackFile()->getNumChannels() == 1) arrFreq[i*2 + 1] = arrFreq[i*2];
        else arrFreq[i*2 + 1] = integral(x, C*exp(alpha*i), false);
    }
    return arrFreq;
}



FirstFourierDecomposition::FirstFourierDecomposition(PlayerController *player) :
FourierDecomposition(player) {
    length = 7;
    tempArr = new std::complex<double>[1 << length];
    setAdress();
    CreateW2n();
}
FirstFourierDecomposition::~FirstFourierDecomposition(){
    delete[] adress;
    delete[] W2n;
    delete[] tempArr;
}
template <class T> double* FirstFourierDecomposition::getCompl(T arr, int i, int Channels) {
    if(Channels == 0) {
        for(int j = 0; j < 1 << length; ++j) {
            tempArr[j].real(arr[i+j]);
            tempArr[j].imag(0);
        }
    }
    else if(Channels == 1) {
        for(int j = 0; j < 1 << length; ++j) {
            tempArr[j].real(arr[2*(i+j)]);
            tempArr[j].imag(0);
        }
    }
    else {
        for(int j = 0; j < 1 << length; ++j) {
            tempArr[j].real(arr[2*(i+j) + 1]);
            tempArr[j].imag(0);
        }
    }
}
double* FirstFourierDecomposition::GetFrame(double x, int count) {
    double* res = new double[2*count];
    auto getMod = [](std::complex<double> t){ return sqrt(t.real()*t.real() + t.imag()*t.imag()); };
    x *= player->getTrackFile()->getSampleRate();
    int i = (int)x;
    if(player->getTrackFile()->getBitsPerSample() == 8) {
        char* arr = player->getTrackFile()->getData();
        if(player->getTrackFile()->getNumChannels() == 1) getCompl(arr, i, 0);
        else getCompl(arr, i, 1);
        fft(tempArr, false);
        for(int j = 0; j < count; ++j)
            res[2*j] = getMod(tempArr[j * (1<<length) / count]);
        if((player->getTrackFile()->getNumChannels() == 1))
            for(int j = 0; j < count; ++j) res[2*j + 1] = res[2*j];
        else {
            getCompl(arr, i, 2);
            fft(tempArr, false);
            for(int j = 0; j < count; ++j)
                res[2*j + 1] = getMod(tempArr[j * (1<<length) / count]);
        }

    } else {
        short int* arr = (short int*)player->getTrackFile()->getData();
        if(player->getTrackFile()->getNumChannels() == 1) getCompl(arr, i, 0);
        else getCompl(arr, i, 1);
        fft(tempArr, false);
        for(int j = 0; j < count; ++j)
            res[2*j] = getMod(tempArr[j * (1<<length) / count]);
        if((player->getTrackFile()->getNumChannels() == 1))
            for(int j = 0; j < count; ++j) res[2*j + 1] = res[2*j];
        else {
            getCompl(arr, i, 2);
            fft(tempArr, false);
            for(int j = 0; j < count; ++j)
                res[2*j + 1] = getMod(tempArr[j * (1<<length) / count]);
        }
    }
    return res;
}
void FirstFourierDecomposition::setAdress() {
    adress = new unsigned int[1 << length];
    for(int i = 0; i < 1 << length; ++i) {
        adress[i] = 0;
        int te = 1 << length;
        int tb = 1;
        do {
            if(i & tb != 0)
                adress[i] |= (te & -1);
            te >>= 1;
            tb <<= 1;
        } while(te != 0);
    }
}
void FirstFourierDecomposition::CreateW2n() {
    W2n = new std::complex<double>[32];
    for(int i = 0; i < 32; ++i){
        W2n[i].real(cos(dpi/(2 << i)));
        W2n[i].imag(-sin(dpi/(2 << i)));
    }
}
void FirstFourierDecomposition::fft(std::complex<double> *x, bool complement){
    unsigned int I, J, Nmax, N, Nd2, k, m, mpNd2, Skew;
    unsigned char *Ic = (unsigned char*) &I;
    unsigned char *Jc = (unsigned char*) &J;
    std::complex<double> S;
    std::complex<double> *Wstore, *Warray;
    std::complex<double> WN, W, Temp, *pWN;

    Nmax = 1 << length;

    //first interchanging
    for(I = 1; I < Nmax - 1; I++)
    {
        Jc[0] = adress[Ic[3]];
        Jc[1] = adress[Ic[2]];
        Jc[2] = adress[Ic[1]];
        Jc[3] = adress[Ic[0]];
        J >>= (32 - length);
        if (I < J)
        {
            S = x[I];
            x[I] = x[J];
            x[J] = S;
        }
    }

    //rotation multiplier array allocation
    Wstore = new std::complex<double>[Nmax / 2];
    Wstore[0].real(1.0);
    Wstore[0].imag(0.0);

    //main loop
    for(N = 2, Nd2 = 1, pWN = W2n, Skew = Nmax >> 1; N <= Nmax; Nd2 = N, N << 1, pWN++, Skew >>= 1)
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

            for(m = k; m < Nmax; m += N)
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
        for( I = 0; I < Nmax; I++ )
            x[I] /= Nmax;
    }
}
