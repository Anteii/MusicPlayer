#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QList>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <iostream>
#include <QRegExp>
#include "static_classes/fileassistant.h"
/*
Playlist file format:
Just lines including full path to file
*/

class PlayList
{
public:
  PlayList(QDir & path);
  PlayList(QFile & path);

  ~PlayList();

  QString getNextSong();
  QString getPrevSong();
  QString getSong();
  QList<QString>* getSongList();

  void toFirstSong();
  void toLastSong();
  void setPosition(int index);
  void setSong(QString);
  static void createPlaylist(QString name, PlayList * pl){
    QFile file(FileAssistant::getPlaylistsPath() + "/" + name + ".txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream st(&file);
    st.setCodec("UTF-8");
    QString line;

    for(int i = 0; i < pl->list->length(); ++i){
        st << pl->list->at(i) << ( i == pl->list->length() - 1 ? "" : "\n");
      }
    st.flush();
    file.close();
  }
  static void createBasePlaylist(){
    QString name = "All tracks";
    QDir tracks(FileAssistant::getTracksPath());
    createPlaylist(name, new PlayList(tracks));

  }
  int getSongCount();
  static PlayList * getAllTrackPlaylist(){
    QDir traks(FileAssistant::getTracksPath());
    return new PlayList(traks);
  }
  PlayList* clone();

  bool operator==(const PlayList &) const;
  bool operator!=(const PlayList &) const;

private:
  PlayList();
  QList<QString> * list;
  int currentSong;
};

#endif // PLAYLIST_H
