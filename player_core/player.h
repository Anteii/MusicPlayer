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
*   - MP3
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
* @sa playlist.h
* @sa musicfiledecoder.h
*/
class Player
{
public:
  //! Constructor
  Player();
  //! Get current position in playlist
  int getCurrentPosition();
  //! Get current track duration
  int getDurationOfTrack();
  //! Get current track
  //! @sa TrackInfo
  TrackInfo getCurrentTrack();
  //! Get random track
  //! @sa TrackInfo
  TrackInfo getRandTrack();
  //! Get current track (Check warning in TrackFile class)
  //! @sa TrackFile
  TrackFile * getTrackFile();

  //! Set playlist and old
  void setPlaylist(PlayList*);
  //! Return playlist or NULL
  PlayList* getPlaylist();

  //! Load and play current track
  void start();
  //! Play if player in pause state, pause it otherwise
  void playPause();
  //! Play
  void play();
  //! Pause
  void pause();
  //! Play next track
  void playNextTrack();
  //! Play previous track
  void playPrevTrack();
  //! Set time offset
  void setTime(int);
  //! Enable| disable repeat of track
  void setLoopedTrack(bool);
  //! Enable| disable repeat of playlist
  void setLoopedPlaylist(bool);
  //! Enable| disable random track selection
  void setRandTrack(bool);
  //! Set volume
  void setVolume(float);

  //! Is playing
  bool isPlaying();
  //! Is paused
  bool isPaused();
  //! Is stopped
  bool isStopped();
  /*!
   * \brief check if player ready to play
   * \return true if player have loaded track, false otherwise
   */
  inline bool isReadyToPlay() { return isReady; }
  //! Is track should repeat after its end
  inline bool isLoopedTrack() { return _isLoopedTrack; }
  //! Is playlist should repeat after its end
  inline bool isLoopedPlaylist() { return _isLoopedPlaylist; }
  //! Is random track selection enabled
  inline bool isRandTrack() { return _isRandTrack; }
  //! Destructor
  ~Player();
  //! Set logger
  void setLogger(Logger * _logger);

private:
  //! Decoder
  MusicFileDecoder * decoder = NULL;
  //! Delete OAL source and buffer
  void clear();
  //! Create OAL source and buffer and load track in it
  void loadTrackFile();
  //! Load next track
  void _loadNextTrack();
  //! Load Previous track
  void _loadPrevTrack();
  //! Decode track and call loadTrackFile()
  void loadTrack(const TrackInfo& rack);
  //! Output list of devices
  void listAudioDevice(const ALCchar* devices);
  //! Create OAL device and listner
  void initOAL();
  //! Create OAL source
  void createSource();
  //! Create OAL device
  void createDevice();
  //! Configure OAL listner
  void configureListner();
  //! call clrear() and delete OAL context, close device
  void deinitOAL();

  //! Current playlist
  //! @sa PlayList
  PlayList * currentPlayList = NULL;
  //! Current trackFile
  TrackFile * trackFile = NULL;
  //! Error code
  errno_t err;
  //! Logger
  Logger * logger = NULL;

  //! Is track was loaded in OAL buffer
  bool isReady = false;
  //! Is random track selection enabled
  bool _isRandTrack = false;
  //! Is track should repeat after its end
  bool _isLoopedTrack = false;
  //! Is playlist should repeat after its end
  bool _isLoopedPlaylist = true;
  //! Volume level
  ALfloat _volume = ALfloat(1);

  //! OAL context
  ALCcontext* context = NULL;
  //! OAL source
  ALuint source;
  //! OAL buffer
  ALuint buffer;
  //! OAL device
  ALCdevice* device = NULL;
};


#endif // PLAYER_H
