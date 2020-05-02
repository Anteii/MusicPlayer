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



class Logger
{
public:
  enum MsgType{
    Message, Warning, Error
  };
  Logger();
  ~Logger();
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
