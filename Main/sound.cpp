#include "common.hpp"

Mix_Chunk *sounds[MAX_SND_CHANNELS];
Mix_Music *music;


void loadMusic(const char *filename) // Thay đổi từ char* thành const char*
{
    
    music = Mix_LoadMUS(filename);
    if (music == NULL)
    {
        SDL_Log("Failed to load music: %s", Mix_GetError());
    }
    Mix_VolumeMusic(128);
    
}

void initSounds()
{
    memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);
    
    loadMusic("Sounds Effect/nanana.mp3");

    if (music != NULL) // Chỉ phát nhạc nếu tải thành công
    {
        playmusic(-1);
    }
    else
    {
        SDL_Log("Music not loaded. Skipping playback.");
    }

    loadSounds();
}

void loadSounds()
{   
    sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("Sounds Effect/player_shot.wav");
    sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("Sounds Effect/enemy_shot.flac");
    sounds[SND_PLAYER_DIE] = Mix_LoadWAV("Sounds Effect/player_explosion.wav");
    sounds[SND_ALIEN_DIE] = Mix_LoadWAV("Sounds Effect/enemy_explosion.wav");

    Mix_VolumeChunk(sounds[SND_PLAYER_FIRE], 32);
    Mix_VolumeChunk(sounds[SND_ALIEN_DIE], 8);
    Mix_VolumeChunk(sounds[SND_ALIEN_FIRE], 32);
    Mix_VolumeChunk(sounds[SND_PLAYER_DIE], 32);

}


void playmusic(int loop)
{
    Mix_PlayMusic(music, (loop) ? -1 : 0); 
} 

void playsound(int id, int channel)
{
    Mix_PlayChannel(channel, sounds[id], 0);
}

void cleanupSound()
{
    for(int i = 0; i < SND_MAX; i++)
    {
        if(sounds[i] != nullptr)
        {
            Mix_FreeChunk(sounds[i]);
            sounds[i] = nullptr;
        }
    }   

    if(music != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }
}
