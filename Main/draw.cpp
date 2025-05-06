#include "common.hpp"

using namespace std;

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer , 32, 32, 32, 255);
    SDL_RenderClear(app.renderer);
}



void blit(SDL_Texture *texture, int x, int y)
{
    SDL_Rect des;
    des.x = x;
    des.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &des.w, &des.h);

    SDL_RenderCopy(app.renderer, texture, NULL, &des);
}

void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
{
    SDL_Rect des;
    des.x = x;
    des.y = y;
    des.w = src->w;
    des.h = src->h;

    SDL_RenderCopy(app.renderer, texture, src, &des);
}


void presentScene()
{
    SDL_RenderPresent(app.renderer);
}


