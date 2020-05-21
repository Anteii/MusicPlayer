#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <thread>
#include "player_core/player.h"
#include "logger/logger.h"

class PlayerController : public QObject
{
  Q_OBJECT
public:
  explicit PlayerController(QObject *parent = nullptr);
  void initPlayer(Player * player);

  PlayList* getPlaylist();
  MusicFile * getMusicFile();
  TrackFile * getTrackFile();
  QString getCurrentTrackName();
  bool isLoopedTrack();
  bool isRandTrack();
  void setRandTrack(bool);
  void setLoopedTrack(bool);
  bool isPlaying();
  int getCurrentPosition();

  void setLogger(Logger * _logger);
  ~PlayerController();

signals:
  void trackPositionChanged(int pos);
  void trackDurationChanged(int duration);
  void trackEnded();
public slots:
  void setTime(int);
  void start();
  void setVolume(float);
  void pause();
  void playPause();
  void playNextTrack();
  void playPrevTrack();

private:
  void initUpdater();
  bool _clickFlag = false;
  bool isInited = false;
  Player * player = NULL;
  std::thread * updater = NULL;
  Logger * logger = NULL;
};

#endif // PLAYERCONTROLLER_H
