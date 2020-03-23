#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QDebug>
#include <thread>
#include <chrono>

#include "al.h"
#include "alc.h"

#include "musicfile.h"
#include "musicfiledecoder.h"
#include "playlist.h"


class Player : public QObject
{
  Q_OBJECT
public:
  explicit Player(QObject *parent = nullptr);

  // get track info
  int getCurrentPosition();
  int getDurationOfSong();
  QString getTrackName();
  QString getRandTrackName();

  // playlist manipulating
  void setPlaylist(PlayList * pl);
  PlayList* getPlayList();

  // player controls
  void start();
  void play();
  void pause();
  void setTime(int);
  void setLoopedTrack(bool);
  void setLoopedPlaylist(bool);
  void setRandTrack(bool);
  void setVolume(float);

  // player properties
  inline bool isReadyToPlay() { return isReady; }
  bool isPlaying();
  inline bool isLoopedTrack() { return _isLoopedTrack; }
  inline bool isLoopedPlaylist() { return _isLoopedPlaylist; }
  inline bool isRandTrack() { return _isRandTrack; }
  ~Player();

signals:
  void positionChanged(int pos);
  void durationChanged(int duration);
  void songChanged(QString);
  void songEnded(void);

public slots:
  void playPause();
  void playNextTrack();
  void playPrevTrack();
private:
  // set up connection
  void makeConnections();

  // set up updating thread
  void createUpdaterThread();

  // low-level controlling
  inline void clear();
  void loadMusic(MusicFile& musicFile);
  void _loadNextTrack();
  void _loadPrevTrack();
  void loadSong(QString name);
  void initOAL();
  void createSource();
  void createDevice();
  void configureListner();
  void deinitOAL();

  // private properties
  PlayList * currentPlayList = NULL;
  std::thread * updatingThread = NULL;
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
