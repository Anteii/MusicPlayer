#include "musicfiledecoder.h"
#include <QDebug>
#include "fmt123.h"

std::string MusicFileDecoder::getFileName(const std::string &path)
{
  int namePos;
  int nameEnd = path.size();
  if ((namePos = path.find_last_of("/")) != std::string::npos);
  else if ((namePos = path.find_last_of("\\")) != std::string::npos);
  else namePos = 0;
  if (path.find_last_of(".") != std::string::npos && path.find_last_of(".") > namePos)
    nameEnd = path.find_last_of(".");
  return path.substr(namePos + 1, nameEnd - namePos - 1);
}

std::string MusicFileDecoder::getFileExt(const std::string &path)
{
  int ext_pos = path.find_last_of('.');
  std::string ext = path.substr(ext_pos + 1, path.size() - 1).c_str();
  int t1 = path.find_last_of("/");
  int t2 = path.find_last_of("\\");
  int name_pos = (t1 > t2 ? t1 : t2);
  std::string name = path.substr(name_pos + 1, ext_pos - 1);
}

TrackFile *MusicFileDecoder::decodeWAV(std::string path)
{
  errno_t err;
  MusicFile * mf = new MusicFile;

  FILE* file;
  err = fopen_s(&file, path.c_str(), "rb");
  if (err)
  {
          printf_s("Failed open file, error %d", err);
          qDebug() << QString("Failed open file, error %d");
          return 0;
  }

  fread(mf, sizeof (*mf), 1, file);
  // SKIP USELESS CHUNKS
  while (true) {
          unsigned long size = mf->header.subchunk2Size;
          //0x64617461
          //0x61746164
          if (*(unsigned int*)& (mf->header.subchunk2Id) == 0x61746164 ||
              *(unsigned int*)& (mf->header.subchunk2Id) == 0x64617461)
                  break;
          fseek(file, size, SEEK_CUR);
          fread(
                  &(mf->header.subchunk2Id),
                  sizeof(mf->header.subchunk2Id),
                  1,
                  file);
          fread(
                  &(mf->header.subchunk2Size),
                  sizeof(mf->header.subchunk2Size),
                  1,
                  file);
  }
  size_t size = mf->header.subchunk2Size;
  short int* arr = new short int[size];
  fread(
          arr,
          sizeof(short int),
          size,
          file);

  fclose(file);

  int ext_pos = path.find_last_of('.');
  std::string ext = path.substr(ext_pos + 1, path.size() - 1).c_str();
  int t1 = path.find_last_of("/");
  int t2 = path.find_last_of("\\");
  int name_pos = (t1 > t2 ? t1 : t2);
  std::string name = path.substr(name_pos + 1, ext_pos - 1);
  //qDebug() << "bps= " << mf->header.bitsPerSample;
  //qDebug() << "sampleRate=" << mf->header.sampleRate;
  //qDebug() << "bitrate=" << mf->header.byteRate;
  return new TrackFile(name.c_str(), ext.c_str(), mf->header.numChannels, mf->header.sampleRate, mf->header.byteRate, (char*)arr, size);
}

TrackFile *MusicFileDecoder::decodeMP3(std::string path)
{
  auto tttt = getFileName(path);

  mpg123_open(mh, path.c_str());
  int channels;
  int encoding;
  long rate;
  //mpg123_format_none(mh);

  mpg123_getformat(
          mh, &rate, &channels, &encoding
  );
  mpg123_frameinfo info;
  mpg123_info(mh, &info);
  qDebug() << info.rate << " " << info.bitrate;
  std::vector<char> *temp = new std::vector<char>;
  std::size_t done;
  int totalBytes = 0;
  qDebug() << "asas";
  for (; mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK; ) {
          //short* tst = reinterpret_cast<short*>(buffer);
          for (int i = 0; i < done; i++) {
                  temp->push_back(buffer[i]);
          }
          totalBytes += done;
          mpg123_getformat(
                  mh, &rate, &channels, &encoding
          );
          if (encoding == MPG123_ENC_SIGNED_16){
              //qDebug() << 123;
            }
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

  TrackFile * t = new TrackFile(name.c_str(), ext.c_str(), channels, rate, info.bitrate * 1024, data, totalBytes);

  return t;
}

MusicFileDecoder::MusicFileDecoder()
{
  supportedFormats[0] = "wav";
  supportedFormats[1] = "mp3";
  mpg123_init();
  qDebug() << *mpg123_supported_decoders();
  int err;
  mh = mpg123_new(NULL, &err);
  //mpg123_feature(MPG123_FEATURE_OUTPUT_16BIT);
  mpg123_param(mh, MPG123_DOWN_SAMPLE, 0, 0);
  //mpg123_param(mh, MPG123_FORCE_RATE, 00044100, 0);
  MPG123_SAMPLESIZE(MPG123_ENC_SIGNED_16);
  buffer_size = mpg123_outblock(mh);
  buffer = (unsigned char*)malloc(buffer_size * sizeof(unsigned char));
  /*
  if (mpg123_format_none(mh) == MPG123_ERR){
      qDebug() << mpg123_strerror(mh);
    }
  if ( mpg123_format(mh, 44100, MPG123_STEREO, MPG123_ENC_16) == MPG123_ERR){
      qDebug() << "lololo";
      qDebug() << mpg123_strerror(mh);
    }
    */
}

MusicFileDecoder::~MusicFileDecoder()
{
  free(buffer);
  mpg123_close(mh);
  mpg123_delete(mh);
  mpg123_exit();
}

std::string *MusicFileDecoder::getSupportedFormats()
{
  return supportedFormats;
}

TrackFile* MusicFileDecoder::decodeFile(const std::string& path)
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
