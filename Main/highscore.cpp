#include "common.hpp"

HIGHSCORES highscores;

void initTablescore()
{
    memset(&highscores, 0, sizeof(HIGHSCORES));
    for(int i = 0; i < NUM_HIGHSCORES; i++)
    {
        highscores.highscore[i].score = 0;
    }
}


void initHighscore()
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;
    
    memset(app.keyboard, 0, sizeof(int)*MAX_KEYBOARD_KEYS);
}

void drawHighscore()
{
    int y;

    y = 150; 

    drawText(425, 70, 255, 255, 255, "HIGHSCORES");

    for(int i = 0; i < NUM_HIGHSCORES; i++)
    {
        if(highscores.highscore[i].recent)
        {
            drawText(425, y, 255, 255, 0, "#%d............... %03d", (1 + i), highscores.highscore[i].score);
        }
        else{
            drawText(425, y, 255, 255, 255, "#%d............... %03d", (i + 1), highscores.highscore[i].score);
        }
        y += 50;
    }

}

int scorecomparator(const void *a, const void *b)
{
    Highscore *h1 = ((Highscore*) a);
    Highscore *h2 = ((Highscore*) b);
    return h2->score - h1 ->score;
}

void addHighscore(int score)
{
    
    // xoa trang thai recent cho tat ca cac diem cu
    for(int i = 0; i < NUM_HIGHSCORES; i++)
    {
        highscores.highscore[i].recent = 0;
    }

    if(score > highscores.highscore[NUM_HIGHSCORES - 1].score || highscores.highscore[NUM_HIGHSCORES - 1].score == 0)
    {
        highscores.highscore[NUM_HIGHSCORES - 1].score = score;
        highscores.highscore[NUM_HIGHSCORES - 1].recent = 1;
    }
    
    qsort(highscores.highscore, NUM_HIGHSCORES, sizeof(Highscore), scorecomparator);

    for(int i = 0; i < NUM_HIGHSCORES; i++)
    {
        if(highscores.highscore[i].score == score)
        {
            highscores.highscore[i].recent = 1;
            break;
        }
    }

    
}
