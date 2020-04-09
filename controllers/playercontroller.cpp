#include "playercontroller.h"

PlayerController::PlayerController(QObject *parent) : QObject(parent)
{

}

void PlayerController::initPlayer(Player *player)
{
  this->player = player;
  isInited = true;
  initUpdater();
}

void PlayerController::initUpdater()
{

  updater = new std::thread([](Player * player, PlayerController* pc){

      while(true){

          // init parameters
          int pos;

          // while waiting for playing
          while(!player->isPlaying()){
            //qDebug() << "waiting ";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
          }

          // while playing
          while(player->isPlaying()){
              pos = player->getCurrentPosition();
              emit pc->trackPositionChanged(pos);
              std::this_thread::sleep_for(std::chrono::milliseconds(10));
              //qDebug() << "playing";
            }
          qDebug() << "iteration sdox ";
          if (player->isStopped()){

              pc->playNextTrack();
              qDebug() << "next";
            }
        }
    }, player, this);

  updater->detach();
}

MusicFile *PlayerController::getMusicFile()
{
  return player->getMusicFile();
}

PlayerController::~PlayerController()
{
  delete updater;
}

void PlayerController::setTime(int time)
{
  player->setTime(time);
  emit trackDurationChanged(player->getDurationOfTrack());
  emit trackChanged();
}

void PlayerController::start()
{
  player->start();
  emit trackDurationChanged(player->getDurationOfTrack());
  emit trackChanged();
}

void PlayerController::setVolume(float v)
{
  player->setVolume(v);
}

void PlayerController::playPause()
{
  if (!isInited) return;
  player->playPause();
}

void PlayerController::playNextTrack()
{
  if (!isInited) return;
  player->playNextTrack();
  emit trackDurationChanged(player->getDurationOfTrack());
  emit trackChanged();
}

void PlayerController::playPrevTrack()
{
  if (!isInited) return;
  player->playPrevTrack();
  emit trackDurationChanged(player->getDurationOfTrack());
  emit trackChanged();
}
