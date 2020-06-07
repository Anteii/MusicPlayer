#include "playercontroller.h"

#include <QIcon>
#include <QMessageBox>

PlayerController::PlayerController(QObject *parent) : QObject(parent)
{
  errWin = new ErrorWindow((QWidget*)parent);
  errWin->setModal(true);
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
            bool isErrOccur = false;
            try {
              player->start();
            } catch (std::logic_error er) {
              isErrOccur = true;
              pc->synhronizedFlag = true;
              while(pc->synhronizedFlag)
                std::this_thread::sleep_for(std::chrono::milliseconds(60));
            }
            pc->_clickFlag = false;
            if (isErrOccur) continue;
            emit pc->trackDurationChanged(player->getDurationOfTrack());
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

void PlayerController::setPlaylist(PlayList *pl)
{
  player->setPlaylist(pl);
}

TrackFile *PlayerController::getTrackFile()
{
  return player->getTrackFile();
}

TrackInfo PlayerController::getCurrentTrack()
{
  return player->getCurrentTrack();
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
  delete errWin;
}

void PlayerController::setTime(int time)
{
  player->setTime(time);
}

int PlayerController::start()
{
  if (!isInited) return 1;
  player->pause();
  _clickFlag = true;
  int errCode = 0;
  while(_clickFlag){
      if (synhronizedFlag){
          errCode = 1;
          showErrorBox();
          synhronizedFlag = false;
        }
    }
  return errCode;
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

int PlayerController::playNextTrack()
{
  if (!isInited) return 1;
  bool isErrOccur = false;
  player->pause();
  try {
    player->playNextTrack();
  } catch (std::out_of_range err) {
    isErrOccur = true;
    showErrorBox();
  } catch (std::logic_error err){
    isErrOccur = true;
    showErrorBox();
  }
  if (isErrOccur) return 1;
  emit trackDurationChanged(player->getDurationOfTrack());
  return 0;
}

int PlayerController::playPrevTrack()
{
  if (!isInited) return 1;
  bool isErrOccur = false;
  player->pause();
  try {
    player->playPrevTrack();
  } catch (std::out_of_range err) {
    isErrOccur = true;
    showErrorBox();
  } catch (std::logic_error err){
    isErrOccur = true;
    showErrorBox();
  }
  if (isErrOccur) return 1;
  emit trackDurationChanged(player->getDurationOfTrack());
  return 0;
}

int PlayerController::showErrorBox()
{
  QMessageBox msgBox;
  msgBox.setWindowTitle("Error");
  msgBox.setIcon(QMessageBox::Icon::Warning);
  msgBox.setWindowIcon(QIcon(":/Windows/resources/icons/windows/errorWindow.png"));
  msgBox.setText("Error");
  //msgBox.setStyleSheet(QString::fromUtf8(("QPushButton{ width:200px; height:50px; background-color: rgb(56, 75, 44); color: white;} QMessageBox{ background-color: rgb(0, 75, 141); color: white; } ")));
  msgBox.setInformativeText("File can't be found. This track will be deleted from this playlist.");
  return msgBox.exec();
}
