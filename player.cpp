#include "player.h"

void list_audio_devices(const ALCchar* devices)
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

Player::Player(QObject *parent) : QObject(parent)
{
  initOAL();
  createUpdaterThread();
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

int Player::getDurationOfSong()
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

QString Player::getTrackName()
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

void Player::setPlaylist(PlayList * pl)
{
  if (currentPlayList != NULL) delete currentPlayList;
  currentPlayList = pl;
}

PlayList* Player::getPlayList()
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
  bool flag = isPlaying();
  _loadNextTrack();
  if (flag) play();
}

void Player::playPrevTrack()
{
  if (!isReady) return;
  bool flag = isPlaying();
  _loadPrevTrack();
  if (flag) play();
}

void Player::play()
{
  if (!isReady) return;
  //qDebug() << "play from player.play()";
  alSourcePlay(source);
}

void Player::pause()
{
  //qDebug() << "pause from player.pause()";
  alSourcePause(source);
}

void Player::setTime(int time)
{
  alSourcef(source, AL_SEC_OFFSET, (float)time / 1000);
}

void Player::setLoopedTrack(bool flag)
{
  qDebug() << ( flag ? "Y" : "N" );
  _isLoopedTrack = flag;
}

void Player::setLoopedPlaylist(bool flag)
{
  _isLoopedPlaylist = flag;
}

void Player::setRandTrack(bool flag)
{
  qDebug() << ( flag ? "Y" : "N" );
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
  //qDebug() << "Started play plailist";
  //qDebug() << currentPlayList->getSong();
  loadSong(currentPlayList->getSong());
  play();
}

bool Player::isPlaying()
{
  if (!isReady) return false;
  ALint state;
  alGetSourcei(source, AL_SOURCE_STATE, &state);
  return state == AL_PLAYING;
}

void Player::makeConnections()
{
  connect(this,
          &Player::songEnded,
          this,
          &Player::playNextTrack
          );
}

void Player::createUpdaterThread()
{

  int ptr = (int)this;

  updatingThread = new std::thread([]( int ptr){
      Player * player = (Player*)ptr;

      while(true){

          // init parameters
          int pos;
          ALint state = AL_NONE;

          // while waiting for playing
          while(alGetSourcei(player->source, AL_SOURCE_STATE, &state), state != AL_PLAYING){
            //qDebug() << "waiting " << state;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
          }
          // while playing
          while(state == AL_PLAYING){
              pos = player->getCurrentPosition();
              emit player->positionChanged(pos);
              std::this_thread::sleep_for(std::chrono::milliseconds(10));
              alGetSourcei(player->source, AL_SOURCE_STATE, &state);
              //qDebug() << "playing";
            }
          //qDebug() << "iteration sdox " << state;
          if (state == AL_STOPPED){
              //emit player->songEnded();
              player->_loadNextTrack();
              player->play();
              qDebug() << "next";
            }
        }
    }, ptr);

  updatingThread->detach();
}

void Player::loadMusic(MusicFile &musicFile)
{
  if (isReady) {
      clear();
  }

  createSource();
  isReady = true;
  emit songChanged(currentPlayList->getSong());

  alGenBuffers((ALuint)1, &buffer);
  // check for errors
  alBufferData(buffer, AL_FORMAT_STEREO16, musicFile.samplesBuffer.arr, musicFile.samplesBuffer.size, musicFile.header.sampleRate);
  alSourcei(source, AL_BUFFER, buffer);
  ALint source_state;
  alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  // check for errors

  emit durationChanged(getDurationOfSong());
  emit positionChanged(0);

  //qDebug() << "Song duration: " << getDurationOfSong();
}

void Player::_loadNextTrack()
{
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
  //qDebug() << name;
  loadSong(name);
}

void Player::_loadPrevTrack()
{
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
  loadSong(name);
}

void Player::loadSong(QString name)
{
  //qDebug() << "Started loading " << name;
  MusicFile musicFile;
  if (name == "") return;

  Decoder::DecodeFile(
        musicFile,
        err,
        QString("music/").append(name).toLocal8Bit().data()
        );

  loadMusic(musicFile);
  //qDebug() << "Ended loading " << name;
}

void Player::initOAL()
{
  createDevice();

  ALboolean enumeration;

  enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
  if (enumeration == AL_FALSE) {}		// enumeration not supported
  else {}                           		// enumeration supported
  list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
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
  emit positionChanged(0);
}

Player::~Player()
{
  updatingThread->~thread();
  this->setPlaylist(NULL);
  deinitOAL();
}
