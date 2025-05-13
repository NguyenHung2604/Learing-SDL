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
SDL_Texture *bossTexture;
SDL_Texture *enemybulletTexture;
SDL_Texture *background;
SDL_Texture *explosionTexture;
SDL_Texture *fontTexture;
SDL_Texture *bossbullet;
GameState currentGameState = BEGINNING; // Thêm biến trạng thái game
int main(int argc, char *argv[])
{     
    long then;
    float remain;

    memset(&app, 0, sizeof(App));
    // Khởi tạo SDL
    InitSDL();

    // Đăng ký hàm cleanup để gọi khi chương trình kết thúc
    atexit(cleanup);
    
    initTitle();
    // khoi tao diem cao
    initHighscore();
    // Khởi tạo stag
    initStage();
    // Khoi tao game
    initGame();
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