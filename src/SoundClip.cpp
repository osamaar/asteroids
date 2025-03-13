#include "SoundClip.h"


SoundClip::SoundClip(const char * filename, int loop)
        : mLoop(loop)
        , mMaxPlaying(5)
        , mChannel(-2) {
    const char *err = Mix_GetError();
    mAudio = Mix_LoadWAV(filename);
    err = Mix_GetError();
}

SoundClip::~SoundClip() {
    if (mAudio) {
        Mix_FreeChunk(mAudio);
    }
}

void SoundClip::play() {
    Mix_HaltChannel(mChannel);
    mChannel = Mix_PlayChannel(-1, mAudio, 0);
}

void SoundClip::stop() {
    Mix_HaltChannel(mChannel);
}

void SoundClip::setMaxPlaying(int num) {
    mMaxPlaying = num;
}

void SoundClip::setVolume(int volume) {
    Mix_VolumeChunk(mAudio, volume);
}
