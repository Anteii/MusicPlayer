#include "forms/mainwindow.h"
#include <QApplication>
#include "dep/mpg123/mpg123.h"
#include <fstream>
#include <vector>
#include <decoder/musicfiledecoder.h>
#include "logger/logger.h"

Logger logger;
/*!
* @brief Redirect qDebug() stream to log
*/
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   std::string localMsg = msg.toStdString();
   switch (type) {
   case QtDebugMsg:
       logger.log(Logger::Message, localMsg, context.file, context.line);
       break;
   case QtInfoMsg:
       logger.log(Logger::Message, localMsg, context.file, context.line);
       break;
   case QtWarningMsg:
       logger.log(Logger::Warning, localMsg, context.file, context.line);
       break;
   case QtCriticalMsg:
       logger.log(Logger::Error, localMsg, context.file, context.line);
       break;
   case QtFatalMsg:
       logger.log(Logger::Error, localMsg, context.file, context.line);
       abort();
   }
}
/*!
* @brief Entry point
*
* Execution of program
* starts here
*
* @param argc Number of arguments
* @param argv List of arguments
*
* @return Program exit status
*/
int main(int argc, char *argv[])
{
  //qInstallMessageHandler(myMessageOutput); // Install the handler
  QApplication a(argc, argv);
  MainWindow w(&logger);
  w.show();
  return a.exec();
}
