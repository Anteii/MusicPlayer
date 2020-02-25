#include "MusicFile.h"
#include "MusicFileDecoder.h"
#include "MusicPlayer.h"


int main() {

	MusicFile musicFile;
	errno_t err;

	Decoder::DecodeFile(musicFile, err, "Resources\\Music\\sleep.wav\0");

	std::cout << "Frequency: " << musicFile.header.sampleRate << std::endl;
	std::cout << "Channels number: " << musicFile.header.numChannels << std::endl;
	std::cout << "Audio format: " << musicFile.header.audioFormat << std::endl;
	std::cout << "Data size: " << musicFile.header.subchunk2Size << std::endl;
	std::cout << "Samples number: " << musicFile.samplesBuffer.length << std::endl;

	MusicPlayer::play(musicFile);

	return 0;
}