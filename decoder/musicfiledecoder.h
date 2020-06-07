#ifndef MUSICFILEDECODER_H
#define MUSICFILEDECODER_H

#include <string>
#include <vector>

#include "global_types/musicfile.h"
#include "global_types/trackfile.h"
#include "dep/mpg123/mpg123.h"

/*!
 * \brief The music decoder class
 *
 * Provide wav and mp3 decoding
 *
 */
class MusicFileDecoder{
public:
  //! Constructor
  MusicFileDecoder();
  //! Destructor
  ~MusicFileDecoder();
  /*!
   * \brief Return supported formats array
   * \return array of string
   */
  std::string* getSupportedFormats();
  /*!
   * \brief Deocde music file by path
   *
   * \param path path to file
   * \return TrackFile
   *
   * @sa TrackFile decodeWAV() decodeMP3()
   */
  TrackFile* decodeFile(const std::string& path);

private:
  //! Extract file|directory name from path
  std::string getFileName(const std::string& path);
  /*!
   * \brief Extract file extension from path
   * \param path path to file
   * \return file extension or empty string otherwise (probably)
   */
  std::string getFileExt(const std::string& path);
  //! Array of supported formats
  std::string supportedFormats[2];
  //! mpg123 handler
  mpg123_handle* mh;
  //! mpg123 buffer
  unsigned char* buffer;
  //! mpg123 buffer size
  size_t buffer_size;

  TrackFile *decodeWAV(std::string path);
  TrackFile *decodeMP3(std::string path);
};
#endif // MUSICFILEDECODER_H
