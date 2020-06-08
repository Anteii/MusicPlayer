#include "fourierdecomposition.h"

double FourierDecomposition::getY(double x, bool isRightChannel) {
    x += 0.05;
    x *= player->getTrackFile()->getSampleRate();
    unsigned int t = x;
//    if (player->getTrackFile()->getBitsPerSample() == 8)
//    {
//        char* arr = player->getTrackFile()->getData();
//        if (player->getTrackFile()->getNumChannels() == 1)
//            if(t < player->getTrackFile()->getSize())return arr[t];
//            else return 0;
//        t <<= 1;
//        if(t >= player->getTrackFile()->getSize()) return 0;
//        if (isRightChannel) return arr[t - 1];
//        else return arr[t];
//    }
//    else
    {
        short int* arr = (short int*)player->getTrackFile()->getData();
        if (player->getTrackFile()->getNumChannels() == 1)
            if(t < player->getTrackFile()->getSize())return arr[t];
            else return 0;
        t <<= 1;
        if(t >= player->getTrackFile()->getSize() >> 1) return 0;
        if (isRightChannel) return arr[t - 1];
        else return arr[t];
    }
}
double FourierDecomposition::integral(double a, double freq, bool isRightChannel) {
    double x = 0, y = 0;
    double b = freq == 0 ? dpi : dpi / freq;
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
