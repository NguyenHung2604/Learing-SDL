#include "common.hpp"

using namespace std;

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer , 32, 32, 32, 255);
    SDL_RenderClear(app.renderer);
}

SDL_Texture *loadTexture(const char *filename)
{
    SDL_Texture *texture;

    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    
    texture = IMG_LoadTexture(app.renderer, filename);

    return texture;
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