#include "common.hpp"
char drawTextBuffer[MAX_LINE_LENGTH];

void initFont()
{   TextureManager texManager(app.renderer);  
    fontTexture = texManager.loadTexture("Graphic/fontreal.png");
}

void drawText(int x, int y, int r, int g, int b, const char *format, ...)
{
    int i, len, c;
    SDL_Rect rect;
    va_list args;

    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    SDL_SetTextureColorMod(fontTexture, r, g, b);

    for (i = 0 ; i < len ; i++)
    {
        c = drawTextBuffer[i];

        if (c >= ' ' && c <= 'z')
        {
            rect.x = ((c - ' ') % GLYPHS_PER_ROW) * GLYPH_WIDTH;
            rect.y = ((c - ' ') / GLYPHS_PER_ROW) * GLYPH_HEIGHT;
            blitRect(fontTexture, &rect, x, y);

            x += GLYPH_WIDTH;
        
        }
    }
}
