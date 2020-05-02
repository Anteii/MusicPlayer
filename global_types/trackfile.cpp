#include "trackfile.h"

TrackFile::TrackFile(const char* _name, const char* _ext,
                     unsigned short _numChannels, unsigned long _sampleRate,
                     char * _data, unsigned int _size) :
  name(_name), ext(_ext), numChannels(_numChannels), sampleRate(_sampleRate), data(_data), size(_size)
{

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
  delete[] name;
  delete[] ext;
  delete[] (char*) data;
}
