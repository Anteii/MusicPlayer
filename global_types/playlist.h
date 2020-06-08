#ifndef PLAYLIST_H
#define PLAYLIST_H

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
// Namespace alias
namespace fs = std::experimental::filesystem;

/*!
 * \brief The playlist class
 *
 * This class describe set of tracks
 * and provide interface to interact with them
 */
class PlayList
{
public:
  /*!
   * \brief Construct playlist with given path
   *
   * If path - directory, so it grabs all tracksfrom there
   * Else path - file, so it is expected to be playlist file
   * \param path path
   */
  PlayList(const std::string& path);
  //! Destructor
  ~PlayList();
  //! Return playlist name
  std::string getName();
  //! Set playlist name
  void setName(const std::string & _name);
  /*!
   * \brief try to return next track
   *
   * Throw std::out_of_range if current track is last
   * Throw std::logic_error if it's impossible to open track file (e.g. track file was deleted)
   *
   * \return track info
   * @sa TrackInfo
   */
  TrackInfo getNextSong();
  /*!
   * \brief try to return previous track
   *
   * Throw std::out_of_range if current track is first
   * Throw std::logic_error if it's impossible to open current track (e.g. track file was deleted)
   *
   * \return track info
   * @sa TrackInfo
   */
  TrackInfo getPrevSong();
  /*!
   * \brief return next track
   *
   * Throw std::logic_error if it's impossible to open current track (e.g. track file was deleted)
   *
   * \return track info
   * @sa TrackInfo
   */
  TrackInfo getCurrentSong();
  //! Return track number in playlist
  bool contains(const TrackInfo& info);
  //! Return vector of tracks
  std::vector<TrackInfo>* getSongList();
  //! Return directory contains playlists
  static std::string getPlaylistsDirectory(){ return playlistsDirectory; }
  //! Set position to the given track
  void setSong(const TrackInfo &);
  //! Return amount of tracks
  int size();
  //! Set playlist position to the first track
  void toFirstSong();
  //! Set playlist position to the last track
  void toLastSong();
  //! Return deep copy of playlist
  PlayList* clone();
  //! Set position in playlist
  void setPosition(int index);
  //! Return current position
  int getPosition(){ return currentIndex;}
  //! Create playlist file
  static void createPlaylistFile(const std::string& name, PlayList * pl){
    std::ofstream out(playlistsDirectory + "//" + name + ".txt");
    auto vec = pl->getSongList();
    for (int i = 0; i < vec->size(); ++i){
        out << vec->at(i).getName() + "." +vec->at(i).getExt() << (i == vec->size() - 1 ? "" : "\n");
      }
    out.flush();
    out.close();
  }
  //! Create playlist file contains all tracks
  static void createBasePlaylistFile(){
    std::string name = "All tracks";
    createPlaylistFile(name, new PlayList(defaultTrackDirectory));

  }
  //! Return playlist contains all tracks
  static PlayList * getBaseTrackPlaylist(){
    return new PlayList(defaultTrackDirectory);
  }
  //! @sa createBasePlaylistFile()
  static void updatePlaylistFile(PlayList * pl){
    createPlaylistFile(pl->playlistName, pl);
  }
  /*!
   * \brief delete playlist by its name
   * \param name playlist name
   * \return 0 if operation was successfully done or code of error otherwise
   */
  static int deletePlaylistFile(const std::string & name){
    return std::remove( ( playlistsDirectory + "//" + name + ".txt").c_str() );
  }
  bool operator==(const PlayList &) const;
  bool operator!=(const PlayList &) const;

private:
  PlayList();
  //! Check whether filename is supported format or not
  bool isSupportedFile(const std::string & fileName);
  //! Return default directory name contains playlists
  static std::string playlistsDirectory;
  //! Return default directory name contains tracks
  static std::string defaultTrackDirectory;
  //! Vector of tracks
  //! @sa TrackInfo
  std::vector<TrackInfo> list;
  //! Supported formats
  std::string supportedFormats[2] = {"wav", "mp3"};
  //! Current track index
  int currentIndex;
  // For future updates
  std::string trackDirectory;
  std::string playlistName;
};

#endif // PLAYLIST_H
