#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <thread>
#include "player_core/player.h"
#include "logger/logger.h"
#include "forms/errorwindow.h"

/*!
 * \brief The player controller class
 *
 * Controller for player class
 */
class PlayerController : public QObject
{
  Q_OBJECT

public:
  //! Constructor
  explicit PlayerController(QObject *parent = nullptr);
  //! Set player
  void initPlayer(Player * player);
  //! Return current playlist
  PlayList* getPlaylist();
  //! Set playlist
  void setPlaylist(PlayList * pl);
  //! Return current track file
  //! @sa TrackFile
  TrackFile * getTrackFile();
  //! Return current track information
  //! @sa TrackInfo
  TrackInfo getCurrentTrack();
  //! Is track should repeat after its end
  bool isLoopedTrack();
  //! Is random track selection enabled
  bool isRandTrack();
  //! Enable| disable random track selection
  void setRandTrack(bool);
  //! Enable| disable repeat of track
  void setLoopedTrack(bool);
  //! Is player play track right now
  bool isPlaying();
  //! Return offset in milliseconds
  int getCurrentPosition();
  //! Set logger
  void setLogger(Logger * _logger);
  //! Destructor
  ~PlayerController();

signals:
  void trackPositionChanged(int pos);
  void trackDurationChanged(int duration);
  void trackEnded();

public slots:
  //! Set time offset
  void setTime(int);
  /*!
   * \brief play selected in playlist track
   *
   * If track is unable to play, then delete it from playlist
   *  and show message box with info
   *
   * \return 0 if all is good, -1 otherwise
   */
  int start();
  //! Set volume
  void setVolume(float);
  //! Pause
  void pause();
  //! Play if player in pause state, pause it otherwise
  void playPause();
  /*!
   * \brief Try to play next track
   *
   * If error occure, then delete track from playlist
   *  and show message box with info
   *
   * \return 0 if all is good, -1 otherwise
   */
  int playNextTrack();
  /*!
   * \brief Try to play previous track
   *
   * If error occure, then delete track from playlist
   *  and show message box with info
   *
   * \return 0 if all is good, -1 otherwise
   */
  int playPrevTrack();

private:
  //! Message box with error info
  ErrorWindow * errWin;
  //! Display message box
  int showErrorBox();
  //! Init updater thread
  void initUpdater();
  //! This flag is rised when click was done
  bool _clickFlag = false;
  //! Is player was set
  bool isInited = false;
  //! For some thread operations
  bool synhronizedFlag = false;
  Player * player = NULL;
  std::thread * updater = NULL;
  Logger * logger = NULL;
};

#endif // PLAYERCONTROLLER_H
