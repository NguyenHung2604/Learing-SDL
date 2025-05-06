#pragma once

#include <iostream>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class TextureManager{
    public:
    TextureManager(SDL_Renderer *renderer);
    SDL_Texture *loadTexture(const std::string &filename);

    private:
    SDL_Texture *getTexture(const std::string &name)const;
    void addTexturetoCache(const std::string &name, SDL_Texture *texture);

    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> texturecache;
};
