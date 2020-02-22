#include "MusicFile.h"
#include "MusicFileDecoder.h"
#include "MusicPlayer.h"


int main() {
	MusicFile musicFile;
	errno_t err;
	Decoder::DecodeFile(musicFile, err, "Resources\\Music\\ass.wav\0");
	MusicPlayer::play(musicFile);

	return 0;
}