#ifndef STAGE_HPP
#define STAGE_HPP

// Khởi tạo và reset stage
void initStage();
void initPlayer();
void resetStage();

// Xử lý logic chính
void logic();
void doBackground();
void doStarfield();
void doPlayer();
void doFighter();
void doEnemies();
void doBullet();
void doExplosion();
void addExplosion(int x, int y, int num);
void addDebris(Entity *e);
void doDebris();
void spawnEnemy();

// Điều khiển và bắn đạn
void fireBullet();
void enemyFireBullet(Entity *e);

// Vẽ game
void draw();
void drawFighter();
void drawBullet();
void initStarField();
void drawBackground();
void drawStarfield();
void drawDebris();
void drawExplosions();
void drawFighter();
void drawBullet();
void drawHud();
void drawHP();
void drawLogo();
// Hỗ trợ
void clipPlayer();
void capFrameRate(long *then, float *remainder);

#endif
