#include "common.hpp"
#include <cstring>
#include <cmath>
#include <algorithm>

// Định nghĩa các biến toàn cục
App app;
Entity *player;
Stage stage;
SDL_Texture *bulletTexture;
SDL_Texture *enemyTexture;
SDL_Texture *playerTexture;
SDL_Texture *enemybulletTexture;
SDL_Texture *background;
SDL_Texture *explosionTexture;
int main(int argc, char *argv[])
{     
    long then;
    float remain;

    memset(&app, 0, sizeof(App));
    // Khởi tạo SDL
    InitSDL();

    // Đăng ký hàm cleanup để gọi khi chương trình kết thúc
    atexit(cleanup);
    
    // Khởi tạo stage
    initStage();
    // Khởi tạo sound
    initSounds();
    // Vòng lặp chính của trò chơi
    then = SDL_GetTicks();
    remain = 0;
    while (true)
    {
        prepareScene();

        doInput();

        app.delegate.logic();

        app.delegate.draw();

        presentScene();

        capFrameRate(&then, &remain);
    }
    cleanupSound();

    return 0;
}