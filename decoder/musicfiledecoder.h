#ifndef MUSICFILEDECODER_H
#define MUSICFILEDECODER_H


#pragma once
#include <iostream>
#include "global_types/musicfile.h"


struct MusicFile;

namespace Decoder {
        int DecodeFile(MusicFile& file, errno_t& err, const char * fileName);
};

#endif // MUSICFILEDECODER_H
