#ifndef MUSICFILEDECODER_H
#define MUSICFILEDECODER_H

#include <iostream>
#include "global_types/musicfile.h"
#include "global_types/trackfile.h"
#include "dep/mpg123/mpg123.h"
#include <vector>


namespace Decoder {
        int DecodeFile(MusicFile& file, errno_t& err, const char * fileName);
};

class MusicFileDecoder{

private:
  std::string supportedFormats[2];
  mpg123_handle* mh;
  unsigned char* buffer;
  size_t buffer_size;

  TrackFile *decodeWAV(std::string path);
  TrackFile *decodeMP3(std::string path);
public:
  MusicFileDecoder();
  ~MusicFileDecoder();
  TrackFile* decodeFile(std::string name);
};
#endif // MUSICFILEDECODER_H
