#pragma once
#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2/SDL.h>
#include "defs.hpp"

struct Delegate{
    void (*logic)();
    void (*draw)(); 
}; 

struct App{
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr; 
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
};

struct Explosion{
    float x;
    float y;
    float dx;
    float dy;
    float w;
    float h;
    int r, g, b, a;
    Explosion *next;
};

struct Debris{
    float x;
    float y;
    float dx;
    float dy;
    SDL_Rect rect;
    SDL_Texture *texture;
    int life;
    Debris *next;
};

struct Entity{
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;
    int health;
    int reload;
    int side;
    SDL_Texture *texture;
    Entity *next;
};  

struct Star{
    int x;
    int y;
    int speed;
};

struct Highscore{
    int recent;
    int score;
};

struct HIGHSCORES
{
    Highscore highscore[NUM_HIGHSCORES];  
};


// Boss

typedef enum{
    BOSS_PHASE_1,
    BOSS_PHASE_2
} BossPhase;

typedef enum{
    BOSS_ATTACK_NORMAL,
    BOSS_ATTACK_SPREAD,
    BOSS_ATTACK_LASER,
    BOSS_ATTACK_SPRIAL
} BossAttackType;

typedef struct Boss{
    Entity entity;
    BossPhase CurrentPhase;
    int phaseChangeHealth;
    BossAttackType attacktype;
    int attackcooldown; // hoi chieu
    int attackPattern; // mau tan cong
    int movementPattern; // cach thuc di chuyen
    float specialAttacktimer; // bo dem cho tan cong dac biet cua boss
    int invincibleFrames;
    int lasertime;
    int laserwarningtime;
    int lasertargetY;
   
    SDL_Texture * texture;
    struct Boss *next;
} Boss;


struct Stage{
    Entity fighterhead, *fightertail;
    Entity bullethead, *bullettail;
    Explosion explosionhead, *explosiontail;
    Debris debrishead, *debristail;
    Boss bosshead, *bosstail;
    int score;
};
