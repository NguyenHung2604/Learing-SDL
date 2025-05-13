#include "common.hpp"

Mix_Chunk *sounds[MAX_SND_CHANNELS];
Mix_Music *music;
Mix_Music *bossMusic = NULL;
bool isPlayingBossMusic = false;


void loadMusic(const char *filename) // Thay đổi từ char* thành const char*
{
    
    music = Mix_LoadMUS(filename);
    if (music == NULL)
    {
        SDL_Log("Failed to load music: %s", Mix_GetError());
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    
}

void loadBossMusic(const char *filename)
{
    bossMusic = Mix_LoadMUS(filename);
    if (bossMusic == NULL)
    {
        SDL_Log("Failed to load boss music: %s", Mix_GetError());
    }
    Mix_VolumeMusic(50);
}

void initSounds()
{
    memset(sounds, 0, sizeof(Mix_Chunk*) * SND_MAX);
    
    // Load main theme
    loadMusic("Sounds Effect/main theme.mp3");
    
    // Load boss theme
    loadBossMusic("Sounds Effect/ban phase.mp3");

    if (music != NULL) // Chỉ phát nhạc main theme khi bắt đầu
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


// Thêm vào file sound.cpp
void switchMusic(bool toBossMusic)
{
    if (toBossMusic && !isPlayingBossMusic)
    {
        Mix_FadeOutMusic(500); // Fade out nhạc hiện tại trong 500ms
        SDL_Delay(550); // Đợi nhạc fade out xong
        
        if (bossMusic != NULL)
        {
            Mix_PlayMusic(bossMusic, -1); // Lặp vô hạn
            Mix_VolumeMusic(50);
            isPlayingBossMusic = true;
        }
        else
        {
            SDL_Log("Boss music not loaded!");
        }
    }
    else if (!toBossMusic && isPlayingBossMusic)
    {
        Mix_FadeOutMusic(500);
        SDL_Delay(550);
        
        if (music != NULL)
        {
            Mix_PlayMusic(music, -1);
            Mix_VolumeMusic(50);
            isPlayingBossMusic = false;
        }
        else
        {
            SDL_Log("Main music not loaded!");
        }
    }
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
    
    if(bossMusic != NULL)
    {
        Mix_FreeMusic(bossMusic);
        bossMusic = NULL;
    }
}
