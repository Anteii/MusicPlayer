#include <complex>
#include "global_types/musicfile.h"
#include <math.h>

double dpi = acos(-1) * 2;

double getY(MusicFile file, double x, bool isRightChannel) {
	x *= file.header.sampleRate;
	if (file.header.bitsPerSample == 8)
	{
		short int* arr = file.samplesBuffer.arr;
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

std::complex<double> integral(double a, double b, double freq, MusicFile file) {
	std::complex<double> sum = 0;
	double step = (b - a) / 4096;
	for (double i = a; i <= b; i += step) {
		double t = dpi * i * freq;
		sum += getY(file, i, true) * std::complex<double>(cos(t), sin(t));
	}
	return sum;
}
