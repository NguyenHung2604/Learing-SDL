#include "common.hpp"
#include "structs.hpp"
#include <cmath>
#include <algorithm>
#include "util.hpp"
   

int enemyremainingTime;
int stageResetTimer;
int backgroundX;
int highscore = 0;
Star stars[MAX_STARS];

void doPlayer()
{
    if (player != nullptr)
    {

        player->dx = player->dy = 0;

        if (player->reload > 0)
        {
            player->reload--;
        }

        if (app.keyboard[SDL_SCANCODE_UP])
        {
            player->dy = -PLAYER_SPEED;
        }
        if (app.keyboard[SDL_SCANCODE_DOWN])
        {
            
            player->dy = PLAYER_SPEED;
        }
        if (app.keyboard[SDL_SCANCODE_LEFT])
        {
            
            player->dx = -PLAYER_SPEED;
        }
        if (app.keyboard[SDL_SCANCODE_RIGHT])
        {
            
            player->dx = PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
        {
            
            fireBullet();
            playsound(SND_PLAYER_FIRE, CH_PLAYER);
            player->reload = RELOAD_SPEED; // Đặt lại thời gian nạp đạn
        }

        player->x += player->dx;
        player->y += player->dy;

        // Giới hạn vị trí của người chơi trong màn hình
        clipPlayer();
    }
}

void doFighter()
{
    Entity *e, *pre;

    pre = &stage.fighterhead;
    for (e = stage.fighterhead.next; e != nullptr; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (e == player && e->health == 0)
        {
            
            addExplosion(e->x, e->y, 2);
            addDebris(e);
            player = nullptr;
        }
        if (e != player && (e->x < -e->w || e->health == 0 || collision(e->x, e->y, e->w, e->h, player->x, player->y, player->w, player->h)))
        {
            if (e == stage.fightertail)
            {
                stage.fightertail = pre;
            }
            pre->next = e->next;
            
            addDebris(e);
            delete e;
            e = pre;
        }
        else
        {
            pre = e;
        }
    }
}

void doBackground()
{
    std::cout << backgroundX << std::endl;
    if(--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}

void doStarfield()
{
    for(int i = 0; i < MAX_STARS; i++)
    {
        stars[i].x -= stars[i].speed;

        // Reset vi tri cua nhung ngoi sao
        if(stars[i].x < 0)
        {
            stars[i].x += SCREEN_WIDTH ;
        }
    }
}

void doExplosion()
{
    Explosion *ex, *pre;
    
    pre = &stage.explosionhead;

    for(ex = stage.explosionhead.next; ex != nullptr; ex = ex->next)
    {
        ex->x += ex->dx;
        ex->y += ex->dy;

        if(--ex->a <= 0)
        {
            if(ex == stage.explosiontail)
            {
                stage.explosiontail = pre;
            }

            pre->next = ex->next;
            delete ex;
            ex = pre;
        }
        pre = ex;
    }
}

void addExplosion(int x, int y, int num)
{
    Explosion *e;
    for(int i = 0; i < num; i++)
    {   
        e = new Explosion;
        memset(e, 0, sizeof(Explosion));
        stage.explosiontail->next = e;
        stage.explosiontail = e;

        e->x = x;
        e->y = y;
        e->dx = (rand() % 10) - (rand() % 10);
        e->dy = (rand() % 10) - (rand() % 10);

        e->dx /= 10;
        e->dy /= 10;


        // random 1 trong 4
        switch(rand() % 4)
        {
            case 0:
                e->r = 255; // mau do
                break;
                
            case 1:
                e->r = 255;
                e->g = 128;
                break;
            
            case 2:
                e->r = 255;
                e->g = 255;
                break;

            default:
                e->r = 255;
                e->g = 255;
                e->b = 255;
                break;
        }   

        e->a = rand() % FPS/2;
    }

}

void addDebris(Entity *e)
{   
    Debris *d;
    int x, y, w, h;

    w = e->w/2;
    h = e->h/2;

    for(y = 0; y <= h; y+= h)
    {
        for(x = 0; x <= w; x+= w)
        {
            d = new Debris;
            memset(d, 0, sizeof(Debris));
            stage.debristail->next = d;
            stage.debristail = d;

            d->x = e->x + e->w/2;
            d->y = e->y + e->h/2;
            d->dx = (rand() % 5) - (rand() % 5);
            d->dy = -(5 + (rand() % 12));
            d->life = FPS/2;
            d->texture = e->texture;


            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }   
    }
}

void doDebris()
{
    Debris *d, *pre;
    pre = &stage.debrishead;

    for(d = stage.debrishead.next; d != nullptr; d = d->next)
    {
        d->x += d->dx;
        d->y += d->dy;

        d->dy += 0.5;
        if(--d->life <= 0)
        {
            if(d == stage.debristail)
            {
                stage.debristail = pre;
            }
            pre->next = d->next;
            delete d;
            d = pre;
        }

        pre = d;
    }
}

void spawnEnemy()
{
    if (--enemyremainingTime <= 0)
    {
        Entity *enemy = new Entity();
        if (!enemy)
        {
            SDL_Log("Failed to allocate memory for enemy!");
            return;
        }
        memset(enemy, 0, sizeof(Entity));

        enemy->side = SIDE_ENEMY;
        enemy->health = ENEMY_HEALTH;
        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % (SCREEN_HEIGHT - enemy->h);
        enemy->texture = enemyTexture;

        if (!enemy->texture)
        {
            SDL_Log("Failed to load enemy texture!");
            delete enemy;
            return;
        }

        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
        enemy->dx = -(2 + (rand() % 4));
        enemyremainingTime = 30 + (rand() % 60);

        stage.fightertail->next = enemy;
        stage.fightertail = enemy;

        
    }
}

void logic()
{
    
    doBackground();
    doStarfield();

    doPlayer();
    doFighter();
    doBullet();
    spawnEnemy();
    doEnemies();
    if (player == nullptr && --stageResetTimer <= 0)
    {
        SDL_Log("Resetting stage...");
        resetStage();
    }

    doExplosion();
    doDebris();
}

void clipPlayer()
{
    if (player != NULL)
    {
        if (player->x < 0)
        {
            player->x = 0;
        }

        if (player->y < 0)
        {
            player->y = 0;
        }

        if (player->x > SCREEN_WIDTH / 2)
        {
            player->x = SCREEN_WIDTH / 2;
        }

        if (player->y > SCREEN_HEIGHT - player->h)
        {
            player->y = SCREEN_HEIGHT - player->h;
        }
    }
}

void resetStage()
{
    SDL_Log("Resetting stage...");
    Entity *e;
    Explosion *ex;
    Debris *d;

    stage.score = 0;
    while (stage.fighterhead.next)
    {
        e = stage.fighterhead.next;
        stage.fighterhead.next = e->next;
        delete e;
    }

    while (stage.bullethead.next)
    {
        e = stage.bullethead.next;
        stage.bullethead.next = e->next;
        delete e;
    }
    
    while (stage.explosionhead.next)
    {
        ex = stage.explosionhead.next;
        stage.explosionhead.next = ex->next;
        delete ex;
    }
    

    while(stage.debrishead.next)
    {
        d = stage.debrishead.next;
        stage.debrishead.next = d->next;
        delete d;
    }

    memset(&stage, 0, sizeof(Stage));
    stage.fightertail = &stage.fighterhead;
    stage.bullettail = &stage.bullethead;
    stage.explosiontail = &stage.explosionhead;
    stage.debristail = &stage.debrishead;
    initPlayer();
    if (!player)
    {
        SDL_Log("Player failed to initialize!");
        exit(1);
    }
    initStarField();

    enemyremainingTime = 0;
    stageResetTimer = 3 * FPS;
    
}

void initStarField()
{
    for(int i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = 1 + rand() % 8;
    }
}

void initStage()
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    memset(&stage, 0, sizeof(Stage));
    stage.fightertail = &stage.fighterhead;
    stage.bullettail = &stage.bullethead;
    stage.explosiontail = &stage.explosionhead;
    stage.debristail = &stage.debrishead;

    // Khởi tạo các texture trước khi gọi initPlayer
    playerTexture = loadTexture("Graphic/ship2new.png");
    bulletTexture = loadTexture("Graphic/dan1.png");
    enemyTexture = loadTexture("Graphic/enemynew.png");
    enemybulletTexture = loadTexture("Graphic/dan2.png");
    background = loadTexture("Graphic/map1.jpg");
    explosionTexture = loadTexture("Graphic/explosion.png");
    

    // Đặt lại stage
    resetStage();
}

void fireBullet()
{
    if (!player) return;

    // Tạo viên đạn bên trái
    Entity *bullet = new Entity;
    memset(bullet, 0, sizeof(Entity)); // Khởi tạo viên đạn mới
    bullet->side = SIDE_PLAYER;
    bullet->x = player->x;
    bullet->y = player->y + (player->h / 2); // Đặt ở giữa
    bullet->dx = PLAYER_BULLET_SPEED;
    bullet->health = 1;
    bullet->texture = bulletTexture;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    stage.bullettail->next = bullet;
    stage.bullettail = bullet;

}

int bulletHitFighter(Entity *b)
{
    Entity *e;

    for (e = stage.fighterhead.next; e != nullptr; e = e->next)
    {
        if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
        {
            b->health = 0;
            e->health--;
            if(e->health == 0)
            {
                addExplosion(e->x +(e->w/2), e->y + (e->h)/2, 10);
                if(e == player)
                {
                    playsound(SND_PLAYER_DIE, CH_PLAYER);
                }
                else{
                    playsound(SND_ALIEN_DIE, CH_ANY);
                    stage.score ++;
                    highscore = std::max(highscore, stage.score);
                    
                }
            }
            addDebris(e);
            return 1;
        }
    }
    return 0;
}

void enemyFireBullet(Entity *e) // bắn một viên đạn từ enemy
{
    Entity *bullet = new Entity;
    memset(bullet, 0, sizeof(Entity));
    // thêm viên đạn vào cuối linked list
    stage.bullettail->next = bullet;
    stage.bullettail = bullet;

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->texture = enemybulletTexture;
    bullet->side = SIDE_ENEMY;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);
    velocityVector(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ENEMY_BULLET_SPEED;
    bullet->dy *= ENEMY_BULLET_SPEED;

    e->reload = (rand() % (FPS * 2));
}

void doEnemies()
{
    Entity *e;
    for (e = stage.fighterhead.next; e != nullptr; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0)
        {
            enemyFireBullet(e);
            playsound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
        }
    }
}

void doBullet()
{
    Entity *b, *pre;

    pre = &stage.bullethead; // pre trỏ đến đầu danh sách viên đạn
    for (b = stage.bullethead.next; b != nullptr; b = b->next) // Duyệt qua danh sách viên đạn
    {
        b->x += b->dx; // Cập nhật vị trí x của viên đạn
        b->y += b->dy; // Cập nhật vị trí y của viên đạn

        if (b->x > SCREEN_WIDTH || bulletHitFighter(b) || b->y > SCREEN_HEIGHT || b->x < -b->w || b->y < -b->h) // Kiểm tra nếu viên đạn vượt quá màn hình
        {
            if (b == stage.bullettail) // Nếu viên đạn là viên đạn cuối cùng
            {
                stage.bullettail = pre; // Cập nhật bullettail để trỏ đến viên đạn trước đó
            }
            pre->next = b->next; // Bỏ qua viên đạn hiện tại trong danh sách liên kết
            delete b; // Giải phóng bộ nhớ của viên đạn hiện tại
            b = pre; // Đặt b trỏ lại về viên đạn trước đó để tiếp tục vòng lặp
        }
        else
        {
            pre = b; // Cập nhật pre để trỏ đến viên đạn hiện tại
        }
    }
}

void draw()
{   drawBackground();
    drawStarfield();
    drawHud();

    drawFighter();
    drawBullet();

    drawDebris();
    drawExplosions();
}

void drawHud()
{
    drawText(10, 30, 255, 255, 255, "SCORE: %03d", stage.score);

    if (stage.score > 0 && stage.score == highscore)
    {
        drawText(960, 30, 0, 255, 0, "HIGH SCORE: %03d", highscore);
    }
    else
    {
        drawText(960, 30, 255, 255, 255, "HIGH SCORE: %03d", highscore);
    }
}
void drawBackground()
{
    SDL_Rect dest;
    
    for(int x = backgroundX; x < SCREEN_WIDTH; x+= SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }
}

void drawStarfield()
{
    int c;
    for(int i = 0 ; i < MAX_STARS; i++)
    {
        c = 32*stars[i].speed;

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

        SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y); // ve 1 duong sao cach 3 frames 
    }
}

void drawDebris()
{
    Debris *d;
    for(d = stage.debrishead.next; d != nullptr; d = d->next)
    {
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
}

void drawExplosions()
{
    Explosion *e;

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    for(e = stage.explosionhead.next; e != nullptr; e = e->next)
    {
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
        SDL_SetTextureAlphaMod(explosionTexture, e->r);

        // blit hinh anh thu cong
        SDL_Rect rect;
        rect.x = e->x - EXPLOSION_WIDTH / 2;
        rect.y = e->y - EXPLOSION_HEIGHT / 2;
        rect.w = EXPLOSION_WIDTH;
        rect.h = EXPLOSION_HEIGHT;
        SDL_RenderCopy(app.renderer, explosionTexture, NULL, &rect);

    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}


void drawFighter()
{
    Entity *e;
    for (e = stage.fighterhead.next; e != nullptr; e = e->next)
    {
        blit(e->texture, e->x, e->y);
    }
}

void drawBullet()
{
    Entity *b;
    for (b = stage.bullethead.next; b != nullptr; b = b->next)
    {
        blit(b->texture, b->x , b->y - b->h/2);
    }
}

void initPlayer()
{
    player = new Entity;
    if (!player)
    {
        SDL_Log("Failed to allocate memory for player!");
        exit(1);
    }
    memset(player, 0, sizeof(Entity));
    player->side = SIDE_PLAYER;
    player->health = PLAYER_HEALTH;
    stage.fightertail->next = player;
    stage.fightertail = player;

    player->x = 100;
    player->y = 100;
    player->texture = playerTexture;
    if (!player->texture)
    {
        SDL_Log("Failed to load player texture!");
        exit(1);
    }
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime;

    wait = 16 + *remainder;
    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;
    *then = SDL_GetTicks();
}