#ifndef COMMON_H
#define COMMON_H
#define SDL_MAIN_HANDLED
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include "structs.hpp"
#include "defs.hpp"
#include "init.hpp"
#include "draw.hpp"
#include "input.hpp"
#include "stage.hpp"
#include <stdlib.h>
#include <time.h>
#include "sound.hpp"
#include "text.hpp"
#include "highscore.hpp"
#include "Texture.hpp"
#include "title.hpp"
extern App app;
extern Entity *player;
extern Stage stage;
extern SDL_Texture *bulletTexture;
extern SDL_Texture *enemyTexture;
extern SDL_Texture *enemybulletTexture;
extern SDL_Texture *playerTexture;
extern SDL_Texture *background;
extern SDL_Texture *explosionTexture;
extern SDL_Texture *fontTexture;
extern SDL_Texture *UET;
extern SDL_Texture *Escape;
void InitSDL();
void cleanup();
#endif