#ifndef FILEASSISTANT_H
#define FILEASSISTANT_H

#include <QString>
#include <QDir>
#include <QFile>

class FileAssistant
{
public:
  FileAssistant();

  inline QString getRootPath() { return QDir::current().path(); }
  inline QString getPlaylistPath(QString name){
    return getPlaylistsPath().append("\\").append(name).append(playlistSuffix);
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
  static QString getStyle(QString type, QString fileName){
    QFile file(":/" + type + "/resources/styles/" + type + "/" + fileName + ".css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    return styleSheet;
  }
private:
  QString playlistSuffix = ".txt";
};

#endif // FILEASSISTANT_H
