#include <complex>
#include "global_types/musicfile.h"
#include <math.h>

double dpi = acos(-1) * 2;

double getY(MusicFile& file, double x, bool isRightChannel) {
    x *= file.header.sampleRate;
    if (file.header.bitsPerSample == 8)
	{
        char* arr = (char*)file.samplesBuffer.arr;
        if (file.header.numChannels == 1) return arr[(int)x];
		else if (isRightChannel) return arr[(int)x * 2 + 1];
		else return arr[(int)x * 2];
	}
	else 
	{
        short int* arr = file.samplesBuffer.arr;
        if (file.header.numChannels == 1) return arr[(int)x];
		else if (isRightChannel) return arr[(int)x * 2 + 1];
		else return arr[(int)x * 2];
	}
	/*if (file.header.numChannels == 2)
		if (isRightChannel)	x *= 2;
		else x *= 2 + 1;
	return file.samplesBuffer.arr[(int)x];*/
}
std::complex<double> integral(double a, double b, double freq, MusicFile& file, bool isRightChannel) {
	std::complex<double> sum = 0;
	double step = (b - a) / 4096;
	for (double i = a; i <= b; i += step) {
		double t = dpi * i * freq;
        sum += getY(file, i, isRightChannel) * std::complex<double>(cos(t), sin(t));
	}
	return sum;
}
unsigned short int* GetFrame(double x, double length, int count, MusicFile& file) {
    double C = 20;                        // 20 - минимальная частота
    double alpha = log(20000) / count;    // 20000 - максимальная частота
    unsigned short int* arrFreq = new unsigned short int[count * 2];
    for(int i = 0; i < count; i++) {
        auto com = integral(x, x+length, C*exp(alpha*i), file, true);
        arrFreq[i*2] = sqrt(com.real() * com.real() + com.imag() * com.imag());
        if(file.header.numChannels == 1) arrFreq[i*2 + 1] = arrFreq[i*2];
        else {
            com = integral(x, x+length, C*exp(alpha*i), file, false);
            arrFreq[i*2 + 1] = sqrt(com.real() * com.real() + com.imag() * com.imag());
        }
    }
    return arrFreq;
}
void Draving() {
    while(true) {

    }
}
