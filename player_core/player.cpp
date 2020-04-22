#include "player.h"

void Player::listAudioDevice(const ALCchar* devices)
{
	const ALCchar* device = devices, * next = devices + 1;
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

QString Player::getCurrentTrackName()
{
  if (currentPlayList == NULL) return "";
  return currentPlayList->getSong();
}

QString Player::getRandTrackName()
{
  if (currentPlayList == NULL) return "";
  int len = currentPlayList->getSongCount();
  int index = rand() % len;
  currentPlayList->setPosition(index);
  return currentPlayList->getSong();
}

MusicFile *Player::getMusicFile()
{
  return musicFile;
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
  _loadNextTrack();
  play();
}

void Player::playPrevTrack()
{
  if (!isReady) return;
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
  if (!isReady) return;
  _volume = v;
  alSourcef(source, AL_GAIN, _volume);
}

void Player::start()
{
  loadTrack(currentPlayList->getSong());
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

void Player::loadMusicFile(MusicFile &musicFile)
{
  if (isReady) {
      clear();
  }
  createSource();
  isReady = true;

  alGenBuffers((ALuint)1, &buffer);
  // check for errors
  alBufferData(buffer, AL_FORMAT_STEREO16, musicFile.samplesBuffer.arr, musicFile.samplesBuffer.size, musicFile.header.sampleRate);
  alSourcei(source, AL_BUFFER, buffer);
  ALint source_state;
  alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  // check for errors
  if (this->musicFile != NULL) delete this->musicFile;
  this->musicFile = &musicFile;
}

void Player::_loadNextTrack()
{
  if (!isReady) return;
  if(_isLoopedTrack){
      return;
    }

  QString name;

  if (_isRandTrack){
      name = getRandTrackName();
    }
  else{
      name = currentPlayList->getNextSong();
      if (name == ""){
          if(isLoopedPlaylist()){
              currentPlayList->toFirstSong();
              name = currentPlayList->getSong();
            }
          else return;
        }
    }

  loadTrack(name);
}

void Player::_loadPrevTrack()
{
  if (!isReady) return;
  if(_isLoopedTrack){
      return;
    }
  QString name;
  if (_isRandTrack){
      name = getRandTrackName();
    }
  else{
      name = currentPlayList->getPrevSong();
      if (name == ""){
          if(isLoopedPlaylist()){
              currentPlayList->toLastSong();
              name = currentPlayList->getSong();
            }
          else return;
        }
    }
  //qDebug() << name;
  loadTrack(name);
}

void Player::loadTrack(QString name)
{
  if (name == "") return;
  MusicFile* musicFile = new MusicFile;


  Decoder::DecodeFile(
        *musicFile,
        err,
        QString("music/").append(name).toLocal8Bit().data()
        );

  loadMusicFile(*musicFile);
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
  alSourcei(source, AL_LOOPING, ( _isLoopedTrack ? AL_TRUE : AL_FALSE ));
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
  this->setPlaylist(NULL);
  deinitOAL();
}
