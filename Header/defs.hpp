
#ifndef _DEFS_HPP
#define _DEFS_HPP

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int MAX_KEYBOARD_KEYS = 350;
const int PLAYER_SPEED = 4;
const int PLAYER_BULLET_SPEED = 16;
const int RELOAD_SPEED = 6;
const int SIDE_PLAYER = 0;
const int SIDE_ENEMY = 1;
const int ENEMY_HEALTH = 2;
const int PLAYER_HEALTH = 10000;
const int FPS = 60;
const int ENEMY_BULLET_SPEED = 8;
const int MAX_STARS = 500;
const int EXPLOSION_WIDTH = 500;
const int EXPLOSION_HEIGHT = 500;
const int MAX_SND_CHANNELS = 8;

enum  
{
    CH_ANY = -1,
    CH_PLAYER,
    CH_ALIEN_FIRE
};


enum
{
    SND_PLAYER_FIRE,
    SND_ALIEN_FIRE,
    SND_PLAYER_DIE,
    SND_ALIEN_DIE,
    SND_MAX
};

#endif