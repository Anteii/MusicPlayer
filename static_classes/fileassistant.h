#ifndef FILEASSISTANT_H
#define FILEASSISTANT_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

/*!
 * @brief The file assistance class
 * This class provides Qt - based static functions to work with files etc.
 */
class FileAssistant
{
public:
  static inline QString getRootPath() { return QDir::current().path(); }
  static inline QString getPlaylistPath(QString const & name){
    return getPlaylistsPath().append("/").append(name).append(".txt");}
  static inline void initPlaylistsDir(){
    QDir temp = QDir::current();
    if (!temp.cd("playlists")){
        temp.mkdir("playlists");
      }
  }
  static inline void initMusicDir(){
    QDir temp = QDir::current();
    if (!temp.cd("music")){
        temp.mkdir("music");
      }
  }
  static inline bool checkIfFileExist(QString const & name){
    return QFile::exists(getRootPath() + "//" + name);
  }
  static QString getPlaylistsPath() {
    QDir temp = QDir::current();
    temp.cd("playlists");
    return temp.path();
  }
  static QString getTracksPath() {
    QDir temp = QDir::current();
    temp.cd("music");
    return temp.path();
  }
  static QString getFileContent(QString const & filePath){
    QFile file(filePath);
    file.open(QFile::ReadOnly);
    QString result = QLatin1String(file.readAll());
    file.close();
    return result;
  }
  static int WriteFile(const QString& path, QList<QString>* list){
    QFile file(path);
    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream stream(&file);
        for(int i = 0; i < list->size(); ++i){
            stream << list->at(i) << '\n';
          }
        if ( !file.flush() ){
            qDebug() << "Error occured when writing in file";
          }
        else{
            qDebug() << "Success";
          }
        file.close();
        return 0;
    }
    return -1;
  }
  static inline QString getStyle(QString const & type, QString const & fileName){
    return getFileContent(":/" + type + "/resources/styles/" + type + "/" + fileName + ".css");
  }
  static void removeFile(const QString& path){
    QFile file (path);
    if (!file.remove())
      qDebug() << "Fuck";
    qDebug() << file.errorString();
    qDebug() << file.permissions();
  }
  static bool fileExists(const QString& path) {
      QFileInfo check_file(path);
      if (check_file.exists() && check_file.isFile()) {
          return true;
      } else {
          return false;
      }
  }
private:
  FileAssistant();
};

#endif // FILEASSISTANT_H
