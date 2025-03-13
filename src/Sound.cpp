#include "Sound.h"


Sound::Sound(const char * filename, int loop)
        : mLoop(loop)
        , mMaxPlaying(5)
        , mChannel(-2) {
    const char *err = Mix_GetError();
    mAudio = Mix_LoadWAV(filename);
    err = Mix_GetError();
}

Sound::~Sound() {
    if (mAudio) {
        Mix_FreeChunk(mAudio);
    }
}

void Sound::play() {
    Mix_HaltChannel(mChannel);
    mChannel = Mix_PlayChannel(-1, mAudio, 0);
}

void Sound::stop() {
    Mix_HaltChannel(mChannel);
}

void Sound::setMaxPlaying(int num) {
    mMaxPlaying = num;
}

void Sound::setVolume(int volume) {
    Mix_VolumeChunk(mAudio, volume);
}
