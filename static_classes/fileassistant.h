#ifndef FILEASSISTANT_H
#define FILEASSISTANT_H

#include <QString>
#include <QDir>
#include <QFile>


class FileAssistant
{
public:
  static inline QString getRootPath() { return QDir::current().path(); }
  static inline QString getPlaylistPath(QString const & name){
    return getPlaylistsPath().append("\\").append(name).append(".txt");}
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
    return QFile::exists(getTracksPath() + "\\" + name);
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
  static inline QString getStyle(QString const & type, QString const & fileName){
    return getFileContent(":/" + type + "/resources/styles/" + type + "/" + fileName + ".css");
  }
private:
  FileAssistant();
};

#endif // FILEASSISTANT_H
