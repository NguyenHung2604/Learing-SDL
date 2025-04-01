#ifndef DRAW_HPP
#define DRAW_HPP

void prepareScene();
void presentScene();
void blit(SDL_Texture *texture, int x, int y);
void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y);
SDL_Texture *loadTexture(const char *filename);

#endif