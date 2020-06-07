#include "trackfile.h"
#include <qDebug>
TrackFile::TrackFile(const char* _name, const char* _ext,
                     unsigned short _numChannels, unsigned long _sampleRate,
                     int _byteRate, char * _data, unsigned int _size) :
  name(_name), ext(_ext), numChannels(_numChannels), sampleRate(_sampleRate),
  byteRate(_byteRate), data(_data), size(_size)
{
  double t = 1.0 * byteRate / numChannels / sampleRate;
  bitsPerSample = (int)t;
  bitsPerSample <<= 3;
  qDebug() << bitsPerSample;
}

unsigned short TrackFile::getNumChannels()
{
  return numChannels;
}

unsigned long TrackFile::getSampleRate()
{
  return sampleRate;
}

unsigned long TrackFile::getByteRate()
{
  return byteRate;
}

unsigned int TrackFile::getSize()
{
  return size;
}

short TrackFile::getBitsPerSample()
{
  return bitsPerSample;
}

char *TrackFile::getData()
{
  return data;
}

TrackFile::~TrackFile()
{
  delete[] (char*) data;
}
