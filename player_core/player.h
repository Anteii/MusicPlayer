#ifndef PLAYER_H
#define PLAYER_H

#include <QDebug>
#include <chrono>

#include "al.h"
#include "alc.h"

#include "global_types/musicfile.h"
#include "decoder/musicfiledecoder.h"
#include "global_types/playlist.h"
#include "logger/logger.h"

/*!
* @brief Basic music player class
*
* Uses cross-platform OpenAL library.
*
* Supported formats:
*   - WAV
*
* Support basic controlls:
*   - Play / pause
*   - Play next / previous track
*   - Set current time
*   - Make track looped or random selected
*   - Make playlist looped
*   - Volume control
*
* To control music streaming flow use playlists.
*
* @see playlist.h
* @see musicfiledecoder.h
*/
class Player
{
public:
  Player();
  // get track info
  int getCurrentPosition();
  int getDurationOfTrack();
  TrackInfo getCurrentTrack();
  TrackInfo getRandTrack();
  TrackFile * getTrackFile();

  // playlist manipulating
  void setPlaylist(PlayList*);
  PlayList* getPlaylist();

  // player controls
  void start();
  void playPause();
  void play();
  void pause();
  void playNextTrack();
  void playPrevTrack();
  void setTime(int);
  void setLoopedTrack(bool);
  void setLoopedPlaylist(bool);
  void setRandTrack(bool);
  void setVolume(float);

  // player properties
  bool isPlaying();
  bool isPaused();
  bool isStopped();
  inline bool isReadyToPlay() { return isReady; }
  inline bool isLoopedTrack() { return _isLoopedTrack; }
  inline bool isLoopedPlaylist() { return _isLoopedPlaylist; }
  inline bool isRandTrack() { return _isRandTrack; }
  ~Player();

  void setLogger(Logger * _logger);

private:
  // Decoder
  MusicFileDecoder * decoder = NULL;
  // low-level controlling
  void clear();
  void loadTrackFile();
  void _loadNextTrack();
  void _loadPrevTrack();
  void loadTrack(const TrackInfo& rack);
  void listAudioDevice(const ALCchar* devices);
  void initOAL();
  void createSource();
  void createDevice();
  void configureListner();
  void deinitOAL();

  // private properties
  PlayList * currentPlayList = NULL;
  TrackFile * trackFile = NULL;
  errno_t err;
  Logger * logger = NULL;

  // playing properties
  bool isReady = false;
  bool _isRandTrack = false;
  bool _isLoopedTrack = false;
  bool _isLoopedPlaylist = true;
  ALfloat _volume = ALfloat(1);

  // OpenAL variables
  ALCcontext* context = NULL;
  ALuint source;
  ALuint buffer;
  ALCdevice* device = NULL;
};


#endif // PLAYER_H
