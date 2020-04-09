#include "forms/mainwindow.h"
#include <QApplication>
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
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
