#pragma once


void initTablescore();
void initHighscore();

void drawHighscore();
int scorecomparator(const void *a, const void *b);
void addHighscore(int score);