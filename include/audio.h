#ifndef _AUDIO_H
#define _AUDIO_H

#include "miniaudio.h" // Include the header file for ma_engine


class Audio
{
private:
    static ma_engine engine;
    static ma_sound ambientSound;

    Audio() = default; 

public:
    static void Init(const char* ambientFile, float volume = 1.0f);
    static void PlayAmbient();
    static void StopAmbient();
    static void Destroy();
};

#endif
