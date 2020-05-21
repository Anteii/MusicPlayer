#include "trackfile.h"

TrackFile::TrackFile(const char* _name, const char* _ext,
                     unsigned short _numChannels, unsigned long _sampleRate,
                     int _bitRate, char * _data, unsigned int _size) :
  name(_name), ext(_ext), numChannels(_numChannels), sampleRate(_sampleRate),
  bitRate(_bitRate), data(_data), size(_size)
{
  bitsPerSample = bitRate * 1024 * 8 / numChannels / sampleRate;
}

unsigned short TrackFile::getNumChannels()
{
  return numChannels;
}

unsigned long TrackFile::getSampleRate()
{
  return sampleRate;
}

unsigned long TrackFile::getBitRate()
{
  return bitRate;
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
