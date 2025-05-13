#include "common.hpp"
#include "structs.hpp"
#include <cmath>
#include <algorithm>
#include "util.hpp"

bool hasWon = false;
int enemyremainingTime;
int stageResetTimer;
int backgroundX;
int highscore = 0;
int timeout = 300; // thoi gian xuat hien cai title
int reveal = 0; // thoi gian chay de xuat hien cai title
int bossSpawnScore = BOSS_SPAWN_SCORE;
bool bossActive = false; // danh dau boss co hoat dong hay khong
 // Đánh dấu đang phát nhạc nào
TextureManager *texManager = nullptr;
Star stars[MAX_STARS];


void bossFireBullet(Boss *e)
{
    if(e->CurrentPhase == BOSS_PHASE_1)
    {
        if(player == nullptr) return;
        Entity *bullet = new Entity;
        memset(bullet, 0, sizeof(Entity));
        // thêm viên đạn vào cuối linked list
        stage.bullettail->next = bullet;
        stage.bullettail = bullet;

        bullet->x = e->entity.x;
        bullet->y = e->entity.y;
        bullet->health = 1;
        bullet->texture = bossbullet;
        bullet->side = SIDE_ENEMY;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

        bullet->x += (e->entity.w / 2) - (bullet->w / 2);
        bullet->y += (e->entity.h / 2) - (bullet->h / 2);
        velocityVector(player->x + (player->w / 2), player->y + (player->h / 2), e->entity.x, e->entity.y, &bullet->dx, &bullet->dy);

        bullet->dx *= ENEMY_BULLET_SPEED;
        bullet->dy *= ENEMY_BULLET_SPEED;

        // Khi tạo enemy, đặt reload time
        e->entity.reload = FPS*(1 + (rand() % 3)); 
        }
    else if(e->CurrentPhase == BOSS_PHASE_2)
{
    if(player == nullptr) return;
        if(e->attacktype == BOSS_ATTACK_SPREAD){
        // Bắn đạn spread (3 viên đạn cùng lúc)
        for(int i = -1; i <= 1; i++)
        {
            Entity *bullet = new Entity;
            memset(bullet, 0, sizeof(Entity));
            stage.bullettail->next = bullet;
            stage.bullettail = bullet;

            bullet->x = e->entity.x;
            bullet->y = e->entity.y;
            bullet->health = 1;
            bullet->texture = bossbullet;
            bullet->side = SIDE_ENEMY;
            SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

            bullet->x += (e->entity.w / 2) - (bullet->w / 2);
            bullet->y += (e->entity.h / 2) - (bullet->h / 2);
            
            // Tính vector vận tốc hướng về player
            float dx, dy;
            velocityVector(player->x + (player->w / 2), player->y + (player->h / 2), 
                        bullet->x, bullet->y, &dx, &dy);
            
            // Điều chỉnh góc bắn tạo hiệu ứng spread
            float angle = atan2(dy, dx);
            angle += i * 0.3; // Điều chỉnh góc bắn (khoảng 17 độ mỗi viên)
            
            bullet->dx = cos(angle) * ENEMY_BULLET_SPEED * 1.5; // Tăng tốc đạn 50%
            bullet->dy = sin(angle) * ENEMY_BULLET_SPEED * 1.5;
        }
        
        // Thời gian nạp đạn nhanh hơn Phase 1
        e->entity.reload = FPS*(2 + (rand() % 3)); // 0.3-0.5 giây
        
        // Đôi khi thêm một đợt đạn đặc biệt
        if (rand() % 2 == 0) { // 20% cơ hội
            // Chờ một chút trước khi bắn đợt đặc biệt
            SDL_Delay(100);
            
            // Bắn một viên đạn lớn, mạnh hơn
            Entity *bigBullet = new Entity;
            memset(bigBullet, 0, sizeof(Entity));
            stage.bullettail->next = bigBullet;
            stage.bullettail = bigBullet;
            
            bigBullet->x = e->entity.x;
            bigBullet->y = e->entity.y + (rand() % e->entity.h); // Vị trí ngẫu nhiên
            bigBullet->health = 2; // Cần 2 hit để phá
            bigBullet->texture = enemybulletTexture;
            bigBullet->side = SIDE_ENEMY;
            bigBullet->w *= 4;
            bigBullet->h *= 4;
            SDL_QueryTexture(bigBullet->texture, NULL, NULL, &bigBullet->w, &bigBullet->h);
            
            // Làm cho viên đạn to hơn
            
            // Nhắm thẳng vào player với tốc độ cao
            velocityVector(player->x + (player->w / 2), player->y + (player->h / 2),
                        bigBullet->x, bigBullet->y, &bigBullet->dx, &bigBullet->dy);
            bigBullet->dx *= ENEMY_BULLET_SPEED * 1.5;
            bigBullet->dy *= ENEMY_BULLET_SPEED * 1.5;
        }
    }
    else if(e->attacktype == BOSS_ATTACK_LASER)
    {   
        if(player == nullptr) return;
        if(e->attackcooldown == FPS/2)
        {
            e->lasertargetY = player->y + (player->w/2);
        }
        if(e->attackcooldown > 0)
        {
            return;
        }

        playsound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
        // ban laser
        for(int i = 0; i < 15; i++)
        {
            Entity *laserpart = new Entity;
            memset(laserpart, 0, sizeof(Entity));
            stage.bullettail->next = laserpart;
            stage.bullettail = laserpart;

            laserpart->x = e->entity.x - 40*i;
            laserpart->y = e->lasertargetY - 15;
            laserpart->health = 1;
            laserpart->texture = enemybulletTexture;
            laserpart->side = SIDE_ENEMY;
            SDL_QueryTexture(laserpart->texture, NULL, NULL, &laserpart->w, &laserpart->h);

            laserpart->w *= 4;
            laserpart->h  = 30;

            laserpart->dx = -25;
            laserpart->dy = 0;

        }
        e->attackcooldown = FPS*3;
    }
}
}

void doBoss(){
    Boss *b, *prev;
    
    prev = &stage.bosshead;
    
    for (b = stage.bosshead.next; b != nullptr; b = b->next) {
        // Di chuyển boss
        b->entity.x += b->entity.dx;
        b->entity.y += b->entity.dy;

        if (b->invincibleFrames > 0) {
            b->invincibleFrames--;
        }
       
        if (b->CurrentPhase == BOSS_PHASE_2) {
            // Di chuyển theo hình sin để khó đoán hơn
            if (b->entity.dx == 0) {
                b->entity.dy = 3 * sin(SDL_GetTicks() / 500.0); // Di chuyển theo hình sin
            }
            
            
            // if (rand() % 180 == 0) { // Khoảng mỗi 3 giây với FPS=60
            //     b->entity.y = 50 + rand() % (SCREEN_HEIGHT - b->entity.h - 50);
            //     // Tạo hiệu ứng teleport
            //     addExplosion(b->entity.x + b->entity.w/2, b->entity.y + b->entity.h/2, 3);
            // }

            // if (rand() % 20 == 0) { // 5% cơ hội né đạn mỗi frame
            //      b->entity.y = rand() % (SCREEN_HEIGHT - b->entity.h - 100) + 50;
            // }

            // chuyen doi hai kieu tan cong
            if(rand() % 180 == 0) 
            {
                if(b->attacktype == BOSS_ATTACK_SPREAD)
                {
                    b->attacktype = BOSS_ATTACK_LASER;
                    b->lasertime = FPS*2;
                    b->attackcooldown = FPS/2;
                    b->laserwarningtime = 30;
                    // Cập nhật vị trí mục tiêu ngay tại đây khi chuyển sang mode laser
                    if(player != nullptr) {
                     b->lasertargetY = player->y + (player->h/2);
                    }
                }
                else{
                    b->attacktype = BOSS_ATTACK_SPREAD;
                }
            }   

                if(b->attacktype == BOSS_ATTACK_LASER)
            {
                if(b->lasertime > 0)
                {
                    b->lasertime--;
                }
            }    
        }
        else{
        // Logic di chuyển lên xuống
        if (b->entity.y <= 50 || b->entity.y >= SCREEN_HEIGHT - b->entity.h - 50) {
            b->entity.dy *= -1;
        }
        
        // Logic khi boss mới xuất hiện
        if (b->entity.x > SCREEN_WIDTH - b->entity.w - 100) {
            // Tiếp tục di chuyển từ phải sang, vi b->entity.dx = -1;
            b->entity.x = std::fmax(b->entity.x, SCREEN_WIDTH/2);
        } 
        else {
            // Khi đến vị trí, chỉ di chuyển lên xuống
            b->entity.dx = 0;
            if (b->entity.dy == 0) {
                b->entity.dy = 2;
            }
        }
    }
        

        // Boss tấn công
        if (--b->attackcooldown <= 0) {
            bossFireBullet(b);
            b->attackcooldown = FPS/(1 + (rand() % 2));
        }
        
        // Xử lý chuyển phase
        if (b->entity.health <= b->phaseChangeHealth && b->CurrentPhase == BOSS_PHASE_1) {
            b->CurrentPhase = BOSS_PHASE_2;
            b->attacktype = BOSS_ATTACK_SPREAD;
            // addExplosion(b->entity.x + b->entity.w/2, b->entity.y + b->entity.h/2, 5);
        }
    
        // Xử lý khi boss bị tiêu diệt
        if (b->entity.health <= 0) {
            addExplosion(b->entity.x + b->entity.w/2, b->entity.y + b->entity.h/2, 20);
            playmusic(SND_ALIEN_DIE);
            switchMusic(false);
            stage.score += 200;
            hasWon = true;
            
            if (b == stage.bosstail) {
                stage.bosstail = prev;
            }
            
            prev->next = b->next;
            delete b;
            b = prev;
            bossActive = false;
            
            // Thêm dòng này để chuyển sang màn hình highscore sau khi giết boss
            currentGameState = SCORE_SCREEN;
            
            // Lưu điểm trước khi chuyển màn hình
            addHighscore(stage.score);
            initHighscore();
        }
    }
}



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

        if (e == player && e->health <= 0)
        {
            addExplosion(e->x, e->y, 2);
            addDebris(e);
            
            // Thay đổi trạng thái game
            currentGameState = GAME_OVER;
            // SDL_LOG(("Player died, switching to GAME_OVER state");
            
            // Vẫn xóa player khỏi danh sách và đặt player = nullptr
            if (e == stage.fightertail)
            {
                stage.fightertail = pre;
            }
            pre->next = e->next;
            delete e;
            player = nullptr;
            e = pre;  // Cần thiết để không bị truy cập con trỏ đã giải phóng
            continue;
        }
        else if (e != player && (e->x < -e->w || e->health == 0 || (player != nullptr && collision(e->x, e->y, e->w, e->h, player->x, player->y, player->w, player->h))))
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
            // SDL_LOG(("Failed to allocate memory for enemy!");
            return;
        }
        memset(enemy, 0, sizeof(Entity));

        enemy->side = SIDE_ENEMY;
        enemy->health = ENEMY_HEALTH;
        enemy->x = SCREEN_WIDTH;
        enemy->texture = enemyTexture;
        
        // Kiểm tra texture
        if (!enemy->texture)
        {
            // SDL_LOG(("Enemy texture is NULL! Cannot spawn enemy.");
            delete enemy;
            return;
        }

        // Lấy kích thước trước khi tính y
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
        
        // Giờ mới tính y
        enemy->y = rand() % (SCREEN_HEIGHT - enemy->h);
        
        // Thiết lập các giá trị khác
        enemy->dx = -(2 + (rand() % 4));
        enemy->reload = FPS/(1 + (rand() % 3)); // Thêm reload time
        
        // Cập nhật remaining time
        enemyremainingTime = 30 + (rand() % 60);

        // Thêm vào danh sách
        stage.fightertail->next = enemy;
        stage.fightertail = enemy;
        
        // SDL_LOG(("Enemy spawned at (%f, %f) with health %d", enemy->x, enemy->y, enemy->health);
    }
}


void logic()
{   
    doBackground();
    doStarfield();

    // Debug để xem trạng thái game hiện tại
    // SDL_LOG(("Current game state: %d", currentGameState);
    
    // Xử lý tất cả Enter key press ở một chỗ 
    if (app.keyboard[SDL_SCANCODE_KP_ENTER] || app.keyboard[SDL_SCANCODE_RETURN])
    {   
        app.keyboard[SDL_SCANCODE_KP_ENTER] = 0;
        app.keyboard[SDL_SCANCODE_RETURN] = 0;
        
        // Xử lý dựa trên trạng thái
        if (currentGameState == BEGINNING)
        {
            // SDL_LOG(("Chuyển từ BEGINNING sang SCORE_SCREEN");
            currentGameState = SCORE_SCREEN;
            exitTitle();
            return;
        }
        else if (currentGameState == SCORE_SCREEN)
        {
            // SDL_LOG(("Chuyển từ SCORE_SCREEN sang GAME_PLAYING");
            currentGameState = GAME_PLAYING;
            resetStage();
            enemyremainingTime = 1;  // Đặt thời gian spawn enemy ngắn để enemies xuất hiện ngay
            bossSpawnScore = BOSS_SPAWN_SCORE;  // Reset điểm spawn boss
            bossActive = false;  // Đảm bảo boss không active khi bắt đầu game mới
            return;
        }
        else if (currentGameState == GAME_OVER)
        {
            // SDL_LOG(("Chuyển từ GAME_OVER sang GAME_PLAYING");
            currentGameState = GAME_PLAYING;
            resetStage();
            return;
        }
    }
    
    // Xử lý theo trạng thái game
    if (currentGameState == BEGINNING)
    {
        if (reveal < SCREEN_HEIGHT)
        {
            reveal++;
        }
        
        if (--timeout <= 0)
        {
            initTitle();
        }
    }
    else if (currentGameState == SCORE_SCREEN)
    {
        // Xử lý cho SCORE_SCREEN
        return;
    }
    else if (currentGameState == GAME_PLAYING)
    {
        // Logic game khi đang chơi
        doFighter();
        
        if (player != nullptr)
        {
            doPlayer();
        }
        
            // Kiểm tra điều kiện spawn boss
        if (!bossActive && stage.score >= bossSpawnScore) {
            initBoss();
            
            
        }
        
        // Gọi doBoss nếu đang active
        if (bossActive) {
            doBoss();
        } else {
            spawnEnemy();
        }

        
        doEnemies();
        doBullet();
        if (bossActive) {
        bulletHitBoss();
    }
        doExplosion();
        doDebris();
    }
    else if (currentGameState == GAME_OVER)
    {
        // Xử lý cho GAME_OVER
        if (player == nullptr && --stageResetTimer <= 0)
        {   
            addHighscore(stage.score);
            initHighscore();
            initPlayer();
        }
    }
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
    // SDL_LOG(("Resetting stage...");
    Entity *e;
    Explosion *ex;
    Debris *d;

    // Lưu lại các texture trước khi reset
    SDL_Texture* playerTex = playerTexture;
    SDL_Texture* bulletTex = bulletTexture;
    SDL_Texture* enemyTex = enemyTexture;
    SDL_Texture* enemyBulletTex = enemybulletTexture;
    SDL_Texture* bg = background;
    SDL_Texture* explosionTex = explosionTexture;
    SDL_Texture* bosstex = bossTexture;
    SDL_Texture* bulletboss = bossbullet;
    stage.score = 0;
    
    // Giải phóng các đối tượng
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

    while (stage.bosshead.next)
    {
        Boss* b = stage.bosshead.next;
        stage.bosshead.next = b->next;
        delete b;
    }
    // Reset cấu trúc stage
    memset(&stage, 0, sizeof(Stage));
    
    // Khôi phục các texture
    playerTexture = playerTex;
    bulletTexture = bulletTex;
    enemyTexture = enemyTex;
    enemybulletTexture = enemyBulletTex;
    background = bg;
    explosionTexture = explosionTex;
    bossTexture = bosstex;
    bossbullet = bulletboss;

    // Khởi tạo lại các đầu danh sách
    stage.fightertail = &stage.fighterhead;
    stage.bullettail = &stage.bullethead;
    stage.explosiontail = &stage.explosionhead;
    stage.debristail = &stage.debrishead;
    stage.bosstail = &stage.bosshead;
    

    bossActive = false;
    stage.score = 0;
    // Khởi tạo lại người chơi và các thành phần khác
    initPlayer();
    if (!player)
    {
        //SDL_LOG(("Player failed to initialize!");
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
    stage.bosstail = &stage.bosshead;
    // Khởi tạo các texture trước khi gọi initPlayer
    if (!texManager) {
        texManager = new TextureManager(app.renderer);
    }
    bossbullet = texManager->loadTexture("Graphic/bossbul.png");
    bossTexture = texManager->loadTexture("Graphic/bosss.png");
    playerTexture = texManager->loadTexture("Graphic/ship2new.png");
    bulletTexture = texManager->loadTexture("Graphic/dan1.png");
    enemyTexture = texManager->loadTexture("Graphic/enemynew.png");
    enemybulletTexture = texManager->loadTexture("Graphic/dan2.png");
    background = texManager->loadTexture("Graphic/map1.jpg");
    explosionTexture = texManager->loadTexture("Graphic/explosion.png");
    

    // Đặt lại stage
    resetStage();
}

void initBoss(){
    Boss *boss = new Boss;
    memset(boss, 0, sizeof(Boss));

    // thiet lap thuoc tinh
    switchMusic(true);
    boss->entity.side = SIDE_ENEMY;
    boss->entity.health = BOSS_HEALTH;
    boss->entity.x = SCREEN_WIDTH;
    boss->entity.y = SCREEN_HEIGHT/2 - 100;
    boss->entity.dx = -1; // di chuyen cham lai
    boss->lasertargetY = SCREEN_HEIGHT/2;
    boss->entity.texture = bossTexture;
    SDL_QueryTexture(boss->entity.texture, NULL, NULL, &boss->entity.w, &boss->entity.h);

    // thiet lap thuoc tinh dac biet cua boss
   
    boss->CurrentPhase = BOSS_PHASE_1;
    boss->phaseChangeHealth = 100; // se chuyen sang trang thai khac khi mau xuong 300
    boss->attacktype = BOSS_ATTACK_NORMAL;
    boss->attackcooldown = FPS;

    // Them boss vao danh sach
    stage.bosstail->next = boss;
    stage.bosstail = boss;
    player->health = std::min(PLAYER_HEALTH, player->health + 30);

    bossActive = true;
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
                    switchMusic(false);
                }
                else{
                    playsound(SND_ALIEN_DIE, CH_ANY);
                    stage.score ++;
                    highscore = std::max(highscore, stage.score);
                    if(stage.score % 10 == 0 && stage.score != 0)
                    {
                        player->health = std::min(PLAYER_HEALTH, player->health + 3);
                    }
                    
                }
            }
            addDebris(e);
            return 1;
        }
    }
    return 0;
}

void bulletHitBoss()
{
    if (!bossActive) return;
    
    Entity *b;
    Boss *boss;
    
    for (b = stage.bullethead.next; b != nullptr; b = b->next)
    {
        // Chỉ xét đạn của người chơi
        if (b->side == SIDE_PLAYER && b->health > 0) // Kiểm tra đạn còn sống
        {
            for (boss = stage.bosshead.next; boss != nullptr; boss = boss->next)
            {
                if (collision(b->x, b->y, b->w, b->h, boss->entity.x, boss->entity.y, boss->entity.w, boss->entity.h))
                {   
                    // Đánh dấu đạn đã trúng mục tiêu
                    b->health = 0;
                    
                    // Tạo hiệu ứng nổ, vo vun
                    addDebris(b);
                    
                    // Phát âm thanh
                    playsound(SND_ALIEN_DIE, CH_ANY);
                    
                    // Boss chỉ mất máu nếu không trong trạng thái miễn nhiễm
                    if (boss->invincibleFrames <= 0) {
                        // Có 33% cơ hội gây 2 sát thương, 67% gây 1 sát thương
                        if(rand() % 3 == 0) {
                            boss->entity.health -= 2;
                        } else {
                            boss->entity.health--;
                        }
                        
                        // Đặt thời gian miễn nhiễm
                        boss->invincibleFrames = 10;
                    }
                    break;
                }
            }
        }
    }
}

void enemyFireBullet(Entity *e) // bắn một viên đạn từ enemy
{   
    if(player == nullptr) return;
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

    // Khi tạo enemy, đặt reload time
    e->reload = FPS/(1 + (rand() % 3)); 
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

void drawLogo()
{
    SDL_Rect r;
    r.x = 0;
    r.y = 0;

    SDL_QueryTexture(Escape, NULL, NULL, &r.w, &r.h);
    r.h =  std::min(reveal, r.h);

    blitRect(Escape, &r, (SCREEN_WIDTH/2) - 2*(r.h), 100);

    SDL_QueryTexture(UET, NULL, NULL, &r.w, &r.h);

    r.h = std::min(reveal, r.h);
    blitRect(UET, &r, (SCREEN_WIDTH/2) - (r.h) - 10, 250);
}

void drawBoss() {
    Boss* b;
    for (b = stage.bosshead.next; b != nullptr; b = b->next) {
        blit(b->entity.texture, b->entity.x, b->entity.y);
        
        // Vẽ thanh máu cho boss
        SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
        SDL_Rect healthBar = {
            (int)b->entity.x, 
            (int)b->entity.y - 10, 
            (int)(b->entity.w * b->entity.health / BOSS_HEALTH), 
            5
        };
        SDL_RenderFillRect(app.renderer, &healthBar);

        // Vẽ đường laser cảnh báo
        if(b->attacktype == BOSS_ATTACK_LASER && b->attackcooldown > 0 && b->laserwarningtime > 0)
        {
            // Vẽ đường nhắm laser đậm hơn
            b->laserwarningtime --;
            SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
            SDL_SetRenderDrawColor(app.renderer, 255, 0, 255, 200);
            
            // Vẽ nhiều đường để tạo hiệu ứng đậm hơn
            for(int i = -5; i <= 5; i++) {
                SDL_RenderDrawLine(app.renderer, 
                    b->entity.x + b->entity.w/2, 
                    b->lasertargetY + i, 
                    0, 
                    b->lasertargetY  + i);
            }
            
            // Thêm text cảnh báo khi đếm ngược
            if(b->attackcooldown % 20 < 12) {
                drawText(1000, 35, 255, 0 , 0, "FAIL EXAM LASER");
            }
            
            SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
        }
    }
}
void draw()
{
    drawBackground();
    drawStarfield();
    drawLogo();

    if(currentGameState == BEGINNING){
        if(timeout % 40 < 20)
        {
            drawText(470, 600, 255, 255, 255, "PRESS ENTER TO START");
        }
    }
    else if (currentGameState == SCORE_SCREEN)
    {
        // Vẽ màn hình tiêu đề
        // drawText(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 30, 255, 255, 255, "GAME TITLE");
        // drawText(SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 + 30, 255, 255, 255, "PRESS ENTER TO START");
        drawHighscore();
        if(hasWon)
        {
            drawText(570, 580, 255, 255, 0, "YOU WIN!");
            drawText(420, 600, 255, 255, 255, "PRESS ENTER TO PLAY AGAIN");
        }
        else{
        drawText(450, 600, 255, 255, 255, "PRESS ENTER TO PLAY");
        }
    }
    else if (currentGameState == GAME_PLAYING)
    {
        // Vẽ màn hình gameplay
        drawHP();
        drawHud();
        drawFighter();
        drawBullet();
        if(bossActive)
        {
            drawBoss();
        }
        drawDebris();
        drawExplosions();
    }
    else if (currentGameState == GAME_OVER)
    {
        // Vẽ màn hình game over
        drawHighscore();
        drawText(SCREEN_WIDTH/2 - 110, 600, 255, 255, 255, "GAME OVER");
        drawText(425, 640, 255, 255, 255, "PRESS ENTER TO RESTART");
        drawText(395, 680, 255, 255, 255, "CLICK THE X SYMBOL TO EXIT");
    }
}

void drawHud()
{
    drawText(10, 15, 255, 255, 255, "SCORE: %05d", stage.score);
    if (stage.score > 0 && stage.score == highscore)
    {
        drawText(1000, 15, 0, 255, 0, "HIGH SCORE: %05d", highscore);
    }
    else
    {
        drawText(1000, 15, 255, 255, 255, "HIGH SCORE: %05d", highscore);
    }
}
void drawHP()
{   if(player != nullptr){
    drawText(10, 35, 255, 0, 0, "HP: %d", player->health);
    drawText(10, 55, 255, 0, 125, "HEAL 3 HP FOR EACH 10 POINTS");
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
        // Trong drawBullet() cải thiện cách hiển thị laser:
        if (b->h > 25 && b->side == SIDE_ENEMY) {
            // Đây là laser - vẽ với hiệu ứng đặc biệt
            SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
            SDL_SetTextureColorMod(b->texture, 255, 20, 60); // Màu đỏ rực cho laser
            
            // Vẽ laser to hơn
            SDL_Rect dest;
            dest.x = b->x;
            dest.y = b->y - b->h/4; // Đặt vị trí y chính xác
            dest.w = b->w;
            dest.h = b->h;
            SDL_RenderCopy(app.renderer, b->texture, NULL, &dest);
            
            // Vẽ thêm viền sáng
            SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 150);
            SDL_Rect laserRect = {(int)b->x, (int)b->y - b->h/4, b->w, b->h};
            SDL_RenderDrawRect(app.renderer, &laserRect);
            
            // Thêm hiệu ứng nổ nhỏ để laser dễ thấy hơn
            // if (rand() % 3 == 0) {
            //     addExplosion(b->x, b->y, 1);
            // }
            
            SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
            SDL_SetTextureColorMod(b->texture, 255, 255, 255);
        } else {
            // Vẽ bình thường cho đạn thường
            blit(b->texture, b->x, b->y);
        }
    }
}

void initPlayer()
{
    player = new Entity;
    if (!player)
    {
        //SDL_LOG((("Failed to allocate memory for player!");
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
        //dSDL_LOG((("Failed to load player texture!");
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