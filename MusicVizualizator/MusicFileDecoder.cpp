#include "MusicFileDecoder.h"
#include "MusicFile.h"

int Decoder::DecodeFile(MusicFile& musicFile, errno_t& err, const char* fileName)
{
	FILE* file;
	err = fopen_s(&file, fileName, "rb");

	if (err)
	{
		printf_s("Failed open file, error %d", err);
		return 0;
	}

	fread_s(
		&(musicFile.header),
		sizeof(musicFile.header),
		sizeof(musicFile.header),
		1,
		file
	);

	MusicFile::SamplesBuffer* buffer = &(musicFile.samplesBuffer);
	int sample_size = musicFile.header.bitsPerSample / 8;
	buffer->length = musicFile.header.subchunk2Size / sample_size;
	buffer->size = musicFile.header.subchunk2Size;
	buffer->arr = new short int[buffer->size];

	fread_s(
		buffer->arr,
		buffer->size * sizeof(short int),
		sizeof(short int),
		buffer->size,
		file
	);

	fclose(file);
}
