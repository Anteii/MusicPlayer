#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <mutex>
#include "static_classes/fileassistant.h"

#define LOG(type, msg) {\
  if(logger_name != NULL)\
  logger_name->log(type, msg, __FILE__, __LINE__);\
  }


/*!
 * \brief The Logger class
 *
 * This class provide opporunities to write messages in log
 */
class Logger
{
public:
  //! Type of message
  enum MsgType{
    Message, Warning, Error
  };
  //! Constructor
  Logger();
  //! Destructor
  ~Logger();
  /*!
   * \brief Write message in log
   * \param type type of message
   * \param msg message
   * \param file name of file
   * \param line line in file
   *
   * @sa MsgType
   */
  void log(MsgType type, const std::string& msg, const char * file, int line);
private:
  std::mutex guard;
  std::string logPath = "logs/";
  std::ofstream out;
  bool _isReady;
  long totalMessages;
  long totalWarnings;
  long totalErrors;
};

#endif // LOGGER_H
