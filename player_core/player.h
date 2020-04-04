#ifndef PLAYER_H
#define PLAYER_H

#include <QDebug>
#include <thread>
#include <chrono>

#include "al.h"
#include "alc.h"

#include "global_types/musicfile.h"
#include "decoder/musicfiledecoder.h"
#include "global_types/playlist.h"


class Player
{
public:
  Player();
  // get track info
  int getCurrentPosition();
  int getDurationOfTrack();
  QString getCurrentTrackName();
  QString getRandTrackName();

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


private:
  // low-level controlling
  inline void clear();
  void loadMusicFile(MusicFile& musicFile);
  void _loadNextTrack();
  void _loadPrevTrack();
  void loadTrack(QString name);
  void listAudioDevice(const ALCchar* devices);
  void initOAL();
  void createSource();
  void createDevice();
  void configureListner();
  void deinitOAL();

  // private properties
  PlayList * currentPlayList = NULL;
  errno_t err;

  // playing properties
  bool isReplay = false;
  bool isReady = false;
  bool _isRandTrack = false;
  bool _isLoopedTrack = false;
  bool _isLoopedPlaylist = true;
  ALfloat _volume = ALfloat(1);

  // OpenAL variables
  MusicFile * musicFile = NULL;
  ALCcontext* context = NULL;
  ALuint source;
  ALuint buffer;
  ALCdevice* device = NULL;
};


#endif // PLAYER_H
