#include "player.h"

void Player::listAudioDevice(const ALCchar* devices)
{
	const ALCchar* device = devices;
	const ALCchar* next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}

Player::Player()
{
  decoder = new MusicFileDecoder;
  initOAL();
}

int Player::getCurrentPosition()
{
  if (!isReady) return 0;
  ALfloat pos;
  alGetSourcef(source, AL_SEC_OFFSET, &pos);
  // to ms
  pos *= 1000;
  return pos;
}

int Player::getDurationOfTrack()
{
  if (!isReady) return 0;
  ALint sizeInBytes;
  ALint channels;
  ALint bits;

  alGetBufferi(buffer, AL_SIZE, &sizeInBytes);
  alGetBufferi(buffer, AL_CHANNELS, &channels);
  alGetBufferi(buffer, AL_BITS, &bits);

  float lengthInSamples = sizeInBytes * 8 / (channels * bits);

  ALint frequency;

  alGetBufferi(buffer, AL_FREQUENCY, &frequency);
  // in ms
  int duration = (float)lengthInSamples / (float)frequency * 1000;
  return duration;
}

TrackInfo Player::getCurrentTrack()
{
  if (currentPlayList == NULL)
    throw std::logic_error(std::string("Playlist isn't set"));

  return currentPlayList->getCurrentSong();
}

TrackInfo Player::getRandTrack()
{
  if (currentPlayList == NULL)
    throw std::logic_error(std::string("Playlist isn't set"));

  int len = currentPlayList->size();
  int index = rand() % len;
  currentPlayList->setPosition(index);
  return currentPlayList->getCurrentSong();
}

TrackFile *Player::getTrackFile()
{
  return trackFile;
}

void Player::setPlaylist(PlayList * pl)
{
  if (currentPlayList != NULL) delete currentPlayList;
  currentPlayList = pl;
}

PlayList* Player::getPlaylist()
{
  return currentPlayList;
}

void Player::playPause(){
  if (isReady && !isPlaying()){
      this->play();
    }
  else if (isReady && isPlaying()){
      this->pause();
    }
}

void Player::playNextTrack()
{
  if (!isReady) return;
  pause();
  _loadNextTrack();
  play();
}

void Player::playPrevTrack()
{
  if (!isReady) return;
  pause();
  _loadPrevTrack();
  play();
}

void Player::play()
{
  if (!isReady) return;
  alSourcePlay(source);
}

void Player::pause()
{
  if (!isReady) return;
  alSourcePause(source);
}

void Player::setTime(int time)
{
  if (!isReady) return;
  alSourcef(source, AL_SEC_OFFSET, (float)time / 1000);
}

void Player::setLoopedTrack(bool flag)
{
  _isLoopedTrack = flag;
}

void Player::setLoopedPlaylist(bool flag)
{
  _isLoopedPlaylist = flag;
}

void Player::setRandTrack(bool flag)
{
  _isRandTrack = flag;
}

void Player::setVolume(float v)
{
  _volume = v;
  alSourcef(source, AL_GAIN, _volume);
}

void Player::start()
{
  pause();
  loadTrack(currentPlayList->getCurrentSong());
  play();
}

bool Player::isPlaying()
{
  if (!isReady) return false;
  ALint state;
  alGetSourcei(source, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

bool Player::isPaused()
{
  if (!isReady) return false;
  ALint state;
  alGetSourcei(source, AL_SOURCE_STATE, &state);
  return state == AL_PAUSED;
}

bool Player::isStopped()
{
  if (!isReady) return false;
  ALint state;
  alGetSourcei(source, AL_SOURCE_STATE, &state);
  return state == AL_STOPPED;
}

void Player::loadTrackFile()
{
  if (isReady) {
      clear();
  }
  createSource();
  isReady = true;

  alGenBuffers((ALuint)1, &buffer);
  // check for errors
  alBufferData(buffer, AL_FORMAT_STEREO16, trackFile->getData(), trackFile->getSize(), trackFile->getSampleRate());
  alSourcei(source, AL_BUFFER, buffer);
  ALint source_state;
  alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  // check for errors
}

void Player::_loadNextTrack()
{
  if (!isReady) return;

  try {
    if (_isLoopedTrack)
      setTime(0);
    else if (_isRandTrack){
        loadTrack(getRandTrack());
      }
    else{
        loadTrack(currentPlayList->getNextSong());
      }
  } catch (std::out_of_range) {
    if (_isLoopedPlaylist){
        currentPlayList->toFirstSong();
        loadTrack(currentPlayList->getCurrentSong());
      }
  }
}

void Player::_loadPrevTrack()
{
  if (!isReady) return;

  try {
    if (_isLoopedTrack)
      setTime(0);
    else if (_isRandTrack){
        loadTrack(getRandTrack());
      }
    else{
        loadTrack(currentPlayList->getPrevSong());
      }
  } catch (std::out_of_range) {
    if (_isLoopedPlaylist){
        currentPlayList->toLastSong();
        loadTrack(currentPlayList->getCurrentSong());
      }
  }
}

void Player::loadTrack(const TrackInfo& track)
{
  TrackFile * tmp = trackFile;
  trackFile = decoder->decodeFile(track.getPath());
  qDebug() << trackFile->getBitsPerSample();
  if (tmp != NULL) {
      delete tmp;
    }
  loadTrackFile();
}

void Player::initOAL()
{
  createDevice();

  ALboolean enumeration;

  enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
  if (enumeration == AL_FALSE) {}		// enumeration not supported
  else {}                           		// enumeration supported
  listAudioDevice(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
  ALCenum error;

  error = alGetError();
  if (error != AL_NO_ERROR) {}

  context = alcCreateContext(device, NULL);
  if (!alcMakeContextCurrent(context)) {}

  configureListner();
}

void Player::createSource()
{
  alGenSources((ALuint)1, &source);
  // check for errors

  alSourcef(source, AL_PITCH, 1); // speed
  // check for errors
  alSourcef(source, AL_GAIN, _volume); // volume
  // check for errors
  alSource3f(source, AL_POSITION, 0, 0, 0);
  // check for errors
  alSource3f(source, AL_VELOCITY, 0, 0, 0);
  // check for errors
  alSourcei(source, AL_LOOPING, AL_FALSE);
  // check for errros
}

void Player::createDevice()
{
  device = alcOpenDevice(NULL);
  if (!device) {}
}

void Player::configureListner()
{
  ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
  alListener3f(AL_POSITION, 0, 0, 1.0f);
  // check for errors
  alListener3f(AL_VELOCITY, 0, 0, 0);
  // check for errors
  alListenerfv(AL_ORIENTATION, listenerOri);
  // check for errors
}

void Player::deinitOAL()
{
  clear();
  device = alcGetContextsDevice(context);
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context);
  alcCloseDevice(device);

  isReady = false;
}

void Player::clear()
{
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);

  isReady = false;
}

Player::~Player()
{
  delete decoder;
  if (trackFile != NULL) delete trackFile;
  pause();
  this->setPlaylist(NULL);
  deinitOAL();
}

void Player::setLogger(Logger *_logger)
{
  logger = _logger;
}
