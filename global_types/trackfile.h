#ifndef TRACKFILE_H
#define TRACKFILE_H

class TrackFile
{
public:
  TrackFile(const char* _name, const char* _ext,
            unsigned short _numChannels, unsigned long _sampleRate,
            int _bitRate, char * _data, unsigned int _size);
  unsigned short getNumChannels();
  unsigned long getSampleRate();
  unsigned long getBitRate();
  unsigned int getSize();
  short getBitsPerSample();
  char * getData();
  ~TrackFile();
private:
  const char * name;
  const char * ext;
  unsigned short numChannels;
  unsigned long sampleRate;
  int bitRate;
  unsigned long byteRate;
  short bitsPerSample;
  char * data;
  unsigned int size;
};

#endif // TRACKFILE_H
