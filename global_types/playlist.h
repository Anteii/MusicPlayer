#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QDebug>

#include <experimental/filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "static_classes/fileassistant.h"
#include "global_types/trackinfo.h"
/*
Playlist file format:
Just lines including full path to file
*/
namespace fs = std::experimental::filesystem;

class PlayList
{
public:
  // Construct playlist wit hgiven path
  // If path - directory, so it grabs all tracksfrom there
  // Else path - file, so it is expected to be playlist file
  PlayList(const std::string& path);

  ~PlayList();
  std::string getName();
  void setName(const std::string & _name);
  TrackInfo getNextSong();
  TrackInfo getPrevSong();
  TrackInfo getCurrentSong();
  int getSongCount();
  bool contains(const TrackInfo& info);
  std::vector<TrackInfo>* getSongList();
  static std::string getPlaylistsDirectory(){ return playlistsDirectory; }
  void setSong(const TrackInfo &);
  int size();
  void toFirstSong();
  void toLastSong();
  PlayList* clone();
  void setPosition(int index);
  int getPosition(){ return currentSong;}

  static void createPlaylistFile(const std::string& name, PlayList * pl){
    // check if exist
    // .....
    std::ofstream out(playlistsDirectory + "//" + name + ".txt");
    auto vec = pl->getSongList();
    for (int i = 0; i < vec->size(); ++i){
        out << vec->at(i).getName() + "." +vec->at(i).getExt() << (i == vec->size() - 1 ? "" : "\n");
      }
    out.flush();
    out.close();
  }
  static void createBasePlaylistFile(){
    std::string name = "All tracks";
    createPlaylistFile(name, new PlayList(defaultTrackDirectory));

  }
  static PlayList * getBaseTrackPlaylist(){
    return new PlayList(defaultTrackDirectory);
  }
  static void updatePlaylistFile(PlayList * pl){
    createPlaylistFile(pl->playlistName, pl);
  }
  static void deletePlaylistFile(const std::string & name){
    std::remove( ( playlistsDirectory + "//" + name + ".txt").c_str() );
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
  std::vector<TrackInfo> list;
  std::string supportedFormats[2] = {"wav", "mp3"};
  int currentSong;
};

#endif // PLAYLIST_H
