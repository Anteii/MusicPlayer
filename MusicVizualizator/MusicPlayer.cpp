#include "MusicPlayer.h"
#include <Thread>

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

int MusicPlayer::play(MusicFile& musicFile)
{
	ALCdevice* device;
	device = alcOpenDevice(NULL);
	if (!device) {}
	ALboolean enumeration;

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE) {}		// enumeration not supported
	else {}								// enumeration supported
	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	ALCenum error;

	error = alGetError();
	if (error != AL_NO_ERROR) {}

	ALCcontext* context;

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {}
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	alListener3f(AL_POSITION, 0, 0, 1.0f);
	// check for errors
	alListener3f(AL_VELOCITY, 0, 0, 0);
	// check for errors
	alListenerfv(AL_ORIENTATION, listenerOri);
	// check for errors
	ALuint source;


	alGenSources((ALuint)1, &source);
	// check for errors

	alSourcef(source, AL_PITCH, 1);
	// check for errors
	alSourcef(source, AL_GAIN, 1);
	// check for errors
	alSource3f(source, AL_POSITION, 0, 0, 0);
	// check for errors
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(source, AL_LOOPING, AL_TRUE);
	// check for errros

	ALuint buffer;

	alGenBuffers((ALuint)1, &buffer);
	// check for errors
	alBufferData(buffer, AL_FORMAT_STEREO16, musicFile.samplesBuffer.arr, musicFile.samplesBuffer.size, 44100);
	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);
	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	// check for errors
	bool flag = true;
	std::cout << "Controls: " << std::endl << "play, pause, stop" << std::endl;
	std::thread reader([source, &flag]() {
		std::string command;
		while (std::cin >> command) {
			if (command == "stop") {
				flag = false;
				break;
			}
			else if (command == "play") {
				alSourcePlay(source);
			}
			else if (command == "pause") {
				alSourcePause(source);
			}
		}
	});
	
	while (flag) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		// check for errors
	}
	reader.join();

	alSourceStop(source);
	
	//free resources

	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	return 0;
}
