#include "forms/mainwindow.h"
#include <QApplication>
#include "dep/mpg123/mpg123.h"
#include <fstream>
#include <vector>
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
void test(){
  mpg123_init();

     int err;
     mpg123_handle *mh = mpg123_new(NULL, &err);
     unsigned char *buffer;
     size_t buffer_size;
     size_t done;

     int channels, encoding;
     long rate;
     buffer_size = mpg123_outblock(mh);
     buffer = (unsigned char*)malloc(buffer_size * sizeof(unsigned char));

     mpg123_open(mh, "music/test.mp3");
     mpg123_getformat(mh, &rate, &channels, &encoding);

     std::ofstream out("res.txt");
     unsigned int counter = 0;
     std::vector<short*> temp();
     qDebug() << "s";
     for (int totalBtyes = 0; mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK; ) {
         short* tst = reinterpret_cast<short*>(buffer);
         for (auto i = 0; i < buffer_size / 2; i++) {
             out<< counter + i<<"\t"<< tst[i] << "\n";
         }
         counter += buffer_size/2;
         totalBtyes += done;
     }
     qDebug() << "e";
     out.close();
     free(buffer);
     mpg123_close(mh);
     mpg123_delete(mh);
     mpg123_exit();
}
int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;

  test();
  w.show();
  return a.exec();
}
