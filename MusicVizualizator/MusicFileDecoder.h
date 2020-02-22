#pragma once
#include <iostream>

struct MusicFile;

namespace Decoder {
	int DecodeFile(MusicFile& file, errno_t& err, const char * fileName);
};