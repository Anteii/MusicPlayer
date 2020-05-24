#ifndef MUSICFILEDECODER_H
#define MUSICFILEDECODER_H

#include <string>
#include <vector>

#include "global_types/musicfile.h"
#include "global_types/trackfile.h"
#include "dep/mpg123/mpg123.h"


namespace Decoder {
        int DecodeFile(MusicFile& file, errno_t& err, const char * fileName);
};

class MusicFileDecoder{

private:
  std::string getFileName(const std::string& path);
  std::string getFileExt(const std::string& path);
  std::string supportedFormats[2];
  mpg123_handle* mh;
  unsigned char* buffer;
  size_t buffer_size;

  TrackFile *decodeWAV(std::string path);
  TrackFile *decodeMP3(std::string path);
public:
  MusicFileDecoder();
  ~MusicFileDecoder();

  std::string* getSupportedFormats();
  TrackFile* decodeFile(const std::string& path);
};
#endif // MUSICFILEDECODER_H
