#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <thread>
#include "player_core/player.h"


class PlayerController : public QObject
{
  Q_OBJECT
public:
  explicit PlayerController(QObject *parent = nullptr);
  void initPlayer(Player * player);
  void initUpdater();
  MusicFile * getMusicFile();
  ~PlayerController();

signals:
  void trackPositionChanged(int pos);
  void trackDurationChanged(int duration);
  void trackChanged();
  void trackEnded(void);

public slots:
  void setTime(int);
  void start();
  void setVolume(float);
  void playPause();
  void playNextTrack();
  void playPrevTrack();

private:
  bool isInited = false;
  Player * player = NULL;
  std::thread * updater = NULL;
};

#endif // PLAYERCONTROLLER_H
