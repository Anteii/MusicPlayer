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
	while (true) {
		unsigned long size = musicFile.header.subchunk2Size;
		//0x64617461
		//0x61746164
		if (*(unsigned int*)& (musicFile.header.subchunk2Id) == 0x61746164)
			break;
		fseek(file, size, SEEK_CUR);
		fread_s(
			&(musicFile.header.subchunk2Id), 
			sizeof(musicFile.header.subchunk2Id), 
			sizeof(musicFile.header.subchunk2Id), 
			1,
			file);
		fread_s(
			&(musicFile.header.subchunk2Size),
			sizeof(musicFile.header.subchunk2Size),
			sizeof(musicFile.header.subchunk2Size),
			1,
			file);
	}
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
