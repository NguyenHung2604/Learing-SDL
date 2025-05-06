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

struct Stage{
    Entity fighterhead, *fightertail;
    Entity bullethead, *bullettail;
    Explosion explosionhead, *explosiontail;
    Debris debrishead, *debristail;
    int score;
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




