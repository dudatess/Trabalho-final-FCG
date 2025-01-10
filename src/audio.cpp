#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#define MINIAUDIO_IMPLEMENTATION
#define strcasecmp strcmp
#include <miniaudio.h>

ma_engine Audio::engine;
ma_sound Audio::ambientSound;

void Audio::Init(const char* ambientFile, float volume)
{
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        printf("Falha ao inicializar o motor de áudio.\n");
        exit(1);
    }

    // Inicializa o som ambiente
    result = ma_sound_init_from_file(&engine, ambientFile, MA_SOUND_FLAG_STREAM, NULL, NULL, &ambientSound);
    if (result != MA_SUCCESS)
    {
        printf("Falha ao carregar o som ambiente: %s\n", ambientFile);
        ma_engine_uninit(&engine);
        exit(1);
    }

    // Configura volume e looping
    ma_sound_set_volume(&ambientSound, volume);
    ma_sound_set_looping(&ambientSound, MA_TRUE);
}

void Audio::PlayAmbient()
{
    ma_sound_start(&ambientSound);
}

void Audio::StopAmbient()
{
    ma_sound_stop(&ambientSound);
}

void Audio::Destroy()
{
    ma_sound_uninit(&ambientSound);
    ma_engine_uninit(&engine);
}
