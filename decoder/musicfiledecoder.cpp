#include "musicfiledecoder.h"
#include <QDebug>


int Decoder::DecodeFile(MusicFile& musicFile, errno_t& err, const char* fileName)
{
	FILE* file;
	err = fopen_s(&file, fileName, "rb");
	if (err)
	{
		printf_s("Failed open file, error %d", err);
		qDebug() << QString("Failed open file, error %d");
		return 0;
	}

	fread(&(musicFile), sizeof (musicFile), 1, file);
	// SKIP USELESS CHUNKS
	while (true) {
		unsigned long size = musicFile.header.subchunk2Size;
		//0x64617461
		//0x61746164
		if (*(unsigned int*)& (musicFile.header.subchunk2Id) == 0x61746164)
			break;
		fseek(file, size, SEEK_CUR);
		fread(
			&(musicFile.header.subchunk2Id),
			sizeof(musicFile.header.subchunk2Id),
			1,
			file);
		fread(
			&(musicFile.header.subchunk2Size),
			sizeof(musicFile.header.subchunk2Size),
			1,
			file);
	}
	MusicFile::SamplesBuffer* buffer = &(musicFile.samplesBuffer);
	int sample_size = musicFile.header.bitsPerSample / 8;
	buffer->length = musicFile.header.subchunk2Size / sample_size;
	buffer->size = musicFile.header.subchunk2Size;
	buffer->arr = new short int[buffer->size];
	fread(
		buffer->arr,
		sizeof(short int),
		buffer->size,
		file);

	fclose(file);
	return 0;
}

TrackFile *MusicFileDecoder::decodeWAV(std::string path)
{
  errno_t err;
  MusicFile * mf = new MusicFile;
  Decoder::DecodeFile(*mf, err, path.c_str());
  int ext_pos = path.find_last_of('.');
  std::string ext = path.substr(ext_pos + 1, path.size() - 1).c_str();
  int t1 = path.find_last_of("/");
  int t2 = path.find_last_of("\\");
  int name_pos = (t1 > t2 ? t1 : t2);
  std::string name = path.substr(name_pos + 1, ext_pos - 1);
  return new TrackFile(name.c_str(), ext.c_str(), mf->header.numChannels, mf->header.sampleRate, mf->header.byteRate * 8, (char*)mf->samplesBuffer.arr, mf->samplesBuffer.size);
}

TrackFile *MusicFileDecoder::decodeMP3(std::string path)
{
  mpg123_open(mh, path.c_str());
  int channels;
  int encoding;
  long rate;

  mpg123_getformat(
          mh, &rate, &channels, &encoding
  );
  mpg123_frameinfo info;
  mpg123_info(mh, &info);
  qDebug() << info.rate << " " << info.bitrate;
  std::vector<char> *temp = new std::vector<char>;
  std::size_t done;
  int totalBytes = 0;

  for (; mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK; ) {
          //short* tst = reinterpret_cast<short*>(buffer);
          for (int i = 0; i < done; i++) {
                  temp->push_back(buffer[i]);
          }
          totalBytes += done;
  }

  int ext_pos = path.find_last_of('.');
  std::string ext = path.substr(ext_pos + 1, path.size() - 1).c_str();
  int t1 = path.find_last_of("/");
  int t2 = path.find_last_of("\\");
  int name_pos = (t1 > t2 ? t1 : t2);
  std::string name = path.substr(name_pos + 1, ext_pos);
  //char * data = temp->data();

  char * data = new char[totalBytes];
  auto x = temp->at(temp->size()-1);
  qDebug() << temp->size() << " " << totalBytes << " " << x;
  for(int i = 0; i < totalBytes; ++i){
      data[i] = temp->at(i);
    }
  delete temp;

  TrackFile * t = new TrackFile(name.c_str(), ext.c_str(), channels, rate, info.bitrate, data, totalBytes);

  return t;
}

MusicFileDecoder::MusicFileDecoder()
{
  supportedFormats[0] = "wav";
  supportedFormats[1] = "mp3";
  mpg123_init();
  int err;
  mh = mpg123_new(NULL, &err);
  buffer_size = mpg123_outblock(mh);
  buffer = (unsigned char*)malloc(buffer_size * sizeof(unsigned char));
}

MusicFileDecoder::~MusicFileDecoder()
{


  free(buffer);
  mpg123_close(mh);
  mpg123_delete(mh);
  mpg123_exit();
}

TrackFile* MusicFileDecoder::decodeFile(std::string path)
{
  int ext_pos = path.find_last_of('.');
  std::string ext = path.substr(ext_pos + 1, path.size() - 1).c_str();
  bool flag = false;

  for(std::string sf : supportedFormats){
      if (ext == sf){
          flag = true;
          break;
        }
    }
  int t1 = path.find_last_of("/");
  int t2 = path.find_last_of("\\");
  int name_pos = (t1 > t2 ? t1 : t2);
  std::string name = path.substr(name_pos + 1, ext_pos);
  if (!flag){
      // Some shit to prevent unsupp format
      return NULL;
    }
  if (ext == "wav"){
      return decodeWAV(path);
    }
  else if (ext == "mp3"){
      return decodeMP3(path);
    }
  return NULL;
}
