#include "common.hpp"


TextureManager *texturetitle = nullptr;
SDL_Texture *UET = NULL;
SDL_Texture *Escape = NULL;


void initTitle()
{   
    app.delegate.logic = logic;
    app.delegate.draw = draw;   

    memset(app.keyboard, 0, sizeof(int)*MAX_KEYBOARD_KEYS);
    if(!texturetitle)
    {
        texturetitle = new TextureManager(app.renderer);   
    }
    UET = texturetitle->loadTexture("Graphic/UET.png");
    Escape = texturetitle->loadTexture("Graphic/escape.png");
}



void exitTitle()
{
    if(UET != NULL)
    {
        SDL_DestroyTexture(UET);
        UET = NULL;
    }

    if(Escape != NULL)
    {
        SDL_DestroyTexture(Escape);
        Escape = NULL;
    }
    return;
}

