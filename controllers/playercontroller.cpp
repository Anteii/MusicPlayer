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
          while(!player->isPlaying() && !pc->_clickFlag){
            //qDebug() << "waiting ";
            std::this_thread::sleep_for(std::chrono::milliseconds(60));
          }

          // while playing
          while(player->isPlaying()){
              pos = player->getCurrentPosition();
              emit pc->trackPositionChanged(pos);
              std::this_thread::sleep_for(std::chrono::milliseconds(60));
            }
          if (pc->_clickFlag) {
            qDebug() << "Was clicked";
            player->start();
            emit pc->trackDurationChanged(player->getDurationOfTrack());
            pc->_clickFlag = false;
            continue;
          }
          if (player->isStopped()){
              emit pc->trackEnded();
            }
        }
    }, player, this);

  updater->detach();
}

PlayList* PlayerController::getPlaylist()
{
  return player->getPlaylist();
}

TrackFile *PlayerController::getTrackFile()
{
  return player->getTrackFile();
}

QString PlayerController::getCurrentTrackName()
{
  return player->getCurrentTrackName();
}

bool PlayerController::isLoopedTrack()
{
  return player->isLoopedTrack();
}

bool PlayerController::isRandTrack()
{
  return player->isRandTrack();
}

void PlayerController::setRandTrack(bool flag)
{
  player->setRandTrack(flag);
}

void PlayerController::setLoopedTrack(bool flag)
{
  return player->setLoopedTrack(flag);
}

bool PlayerController::isPlaying()
{
  return player->isPlaying();
}

int PlayerController::getCurrentPosition()
{
  return player->getCurrentPosition();
}

void PlayerController::setLogger(Logger *_logger)
{
  logger = _logger;
}

PlayerController::~PlayerController()
{
  delete updater;
}

void PlayerController::setTime(int time)
{
  player->setTime(time);
}

void PlayerController::start()
{
  _clickFlag = true;
  player->pause();
}

void PlayerController::setVolume(float v)
{
  player->setVolume(v);
}

void PlayerController::pause()
{
  player->pause();
}

void PlayerController::playPause()
{
  if (!isInited) return;
  player->playPause();
}

void PlayerController::playNextTrack()
{
  if (!isInited) return;
  player->pause();
  player->playNextTrack();
  emit trackDurationChanged(player->getDurationOfTrack());
}

void PlayerController::playPrevTrack()
{
  if (!isInited) return;
  player->pause();
  player->playPrevTrack();
  emit trackDurationChanged(player->getDurationOfTrack());
}
