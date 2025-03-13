#pragma once


#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>

class SoundClip
{
public:
    SoundClip(const char *filename, int loop=0);
    ~SoundClip();

    void play();
    void stop();
    void setMaxPlaying(int num);
    void setVolume(int volume);
private:
    int mLoop;
    int mMaxPlaying;
    Mix_Chunk *mAudio;
    int mChannel;

    SoundClip();
};

