#include "Texture.hpp"
#include <SDL2/SDL_log.h>

TextureManager::TextureManager(SDL_Renderer *renderer)
    :renderer(renderer){}

SDL_Texture *TextureManager::loadTexture(const std::string &filename)
{
    SDL_Texture *texture = getTexture(filename);

    if(!texture)
    {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "loading %s", filename.c_str());
        texture = IMG_LoadTexture(renderer, filename.c_str());  
        if(texture)
        {
            addTexturetoCache(filename, texture);
        }
    }

    return texture;
}   

SDL_Texture *TextureManager::getTexture(const std::string &name)const{
    auto it = texturecache.find(name);
    if(it != texturecache.end())
    {
        return it->second;
    }
    return nullptr;
}

void TextureManager::addTexturetoCache(const std::string &name, SDL_Texture *texture)
{
    texturecache[name] = texture;
}