#ifndef TRACKFILE_H
#define TRACKFILE_H

class TrackFile
{
public:
  TrackFile();

private:
  const char * name;
  const char * ext;
  unsigned short numChannels;
  unsigned long sampleRate;
  unsigned long byteRate;
  short bitsPerSample;
  short * data;
  long dataSize;
};

#endif // TRACKFILE_H
