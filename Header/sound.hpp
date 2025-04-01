#ifndef SOUND_HPP
#define SOUND_HPP

void initSounds();
void loadSounds();
void loadMusic(const char *filename);
void playmusic(int loop);
void playsound(int id, int channel);
void cleanupSound();
#endif
