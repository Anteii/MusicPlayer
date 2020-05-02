#include "logger.h"

Logger::Logger() : _isReady(false), totalMessages(0), totalWarnings(0), totalErrors(0)
{
  // Construct filename
  auto curTime = std::chrono::system_clock::now();
  std::time_t ttime = std::chrono::system_clock::to_time_t(curTime);
  auto tt = std::string(std::ctime(&ttime));
  std::replace(tt.begin(), tt.end(), ':', '.');
  std::replace(tt.begin(), tt.end(), ' ', '_');
  tt.erase(std::remove(tt.begin(), tt.end(), '\n'), tt.end());
  tt.erase(std::remove(tt.begin(), tt.end(), '\r'), tt.end());
  auto finalPath = logPath + tt + ".txt";

  out.open(FileAssistant::getRootPath().toStdString() + "/" + finalPath, std::ios::out | std::ios::binary);
  _isReady = out.is_open();
  if(_isReady){
      log(Message, "Log file was successfully created", __FILE__, __LINE__);
    }
  else{
      log(Error, "Log file creating failed", __FILE__, __LINE__);
    }
}

Logger::~Logger()
{
  if (!_isReady) return;
  out << "total messages: " << totalMessages << "\t" <<
         "total warnings: " << totalWarnings << "\t" <<
         "total errors: " << totalErrors;
  out.flush();
  out.close();
}

void Logger::log(Logger::MsgType type, const std::string& msg, const char *file, int line)
{
  if (!_isReady) return;
  guard.lock();
  std::string prefix;
  std::string suffix;
  if (type == Message){
      prefix = "Message";
      ++totalMessages;
    }
  else if (type == Warning){
      prefix = "Warning";
      ++totalWarnings;
    }
  else if (type == Error){
      prefix = "Error";
      ++totalErrors;
    }
  auto as_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct tm tm;
  char buf[60];
  gmtime_r(&as_time_t, &tm);
  std::strftime(buf, sizeof(char) * 60, "%H:%M:%S", &tm);
  auto timeStr = std::string{buf};
  prefix += "(" + timeStr + ")>";
  suffix = "(Source file: " + std::string(file) + ", line: " + std::to_string(line) + ")";
  out << prefix << " " << msg << " " << suffix << std::endl;
  guard.unlock();
}
