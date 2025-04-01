#include "common.hpp"

using namespace std;

void doKeyDown(SDL_KeyboardEvent *event)
{
    if(event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) // nhan giu phim thi khong tinh them su kien
    {
        app.keyboard[event->keysym.scancode] = 1;
    }
}

void doKeyUp(SDL_KeyboardEvent *event)
{
    if(event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 0;
    }  
}

void doInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;
        
        case SDL_KEYDOWN:
            doKeyDown(&event.key);
            break;
        
        case SDL_KEYUP:
            doKeyUp(&event.key);
            break;
        
        default:
            break;
        }
    }
}