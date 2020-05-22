#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QList>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QRegExp>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "static_classes/fileassistant.h"
/*
Playlist file format:
Just lines including full path to file
*/

class PlayList
{
public:
  // Construct playlist wit hgiven path
  // If path - directory, so it grabs all tracksfrom there
  // Else path - file, so it is expected to be playlist file
  PlayList(const std::string& path);

  ~PlayList();
  std::string getName();
  std::string getNextSong();
  std::string getPrevSong();
  std::string getCurrentSong();
  int getSongCount();
  std::vector<std::string>* getSongList();

  void setSong(const std::string &);
  void toFirstSong();
  void toLastSong();
  PlayList* clone();
  void setPosition(int index);
  int getPosition(){ return currentSong;}

  static void createPlaylist(std::string& name, PlayList * pl){
    // check if exist
    // .....
    std::ofstream out(playlistsDirectory + "//" + name + ".txt");
    auto vec = pl->getSongList();
    for (int i = 0; i < vec->size(); ++i){
        out << vec->at(i) << (i == vec->size() - 1 ? "" : "\n");
      }
    out.flush();
    out.close();
  }
  static void createBasePlaylist(){
    std::string name = "All tracks";
    createPlaylist(name, new PlayList(defaultTrackDirectory));

  }
  static PlayList * getBaseTrackPlaylist(){
    return new PlayList(defaultTrackDirectory);
  }
  static void updatePlaylistFile(PlayList * pl){
    createPlaylist(pl->playlistName, pl);
  }

  bool operator==(const PlayList &) const;
  bool operator!=(const PlayList &) const;

private:
  PlayList();

  bool isSupportedFile(const std::string & fileName);

  static std::string playlistsDirectory;
  static std::string defaultTrackDirectory;
  std::string trackDirectory;
  std::string playlistName;
  std::vector<std::string> list;
  std::string supportedFormats[2] = {"wav", "mp3"};
  int currentSong;
};

#endif // PLAYLIST_H
