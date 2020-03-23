#ifndef MUSICFILEDECODER_H
#define MUSICFILEDECODER_H


#pragma once
#include <iostream>

struct MusicFile;

namespace Decoder {
        int DecodeFile(MusicFile& file, errno_t& err, const char * fileName);
};

#endif // MUSICFILEDECODER_H
