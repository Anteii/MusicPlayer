#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <thread>
#include "player_core/player.h"
#include "logger/logger.h"
#include "forms/errorwindow.h"

class PlayerController : public QObject
{
  Q_OBJECT
public:
  explicit PlayerController(QObject *parent = nullptr);
  void initPlayer(Player * player);

  PlayList* getPlaylist();
  void setPlaylist(PlayList * pl);
  TrackFile * getTrackFile();
  TrackInfo getCurrentTrack();
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
  int start();
  void setVolume(float);
  void pause();
  void playPause();
  int playNextTrack();
  int playPrevTrack();

private:
  ErrorWindow * errWin;

  int showErrorBox();
  void initUpdater();
  bool _clickFlag = false;
  bool isInited = false;
  bool synhronizedFlag = false;
  Player * player = NULL;
  std::thread * updater = NULL;
  Logger * logger = NULL;
};

#endif // PLAYERCONTROLLER_H
