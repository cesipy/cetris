#ifndef MAIN_H
#define MAIN_H

#include <curses.h>
#include "block.h"
#include "backend.h"


#define BOARD_WIDTH 50
#define BOARD_HEIGHT 50
#define EMPTY_CELL 0
#define TETROMINO_SIZE 4

void main_loop();
int hit_bottom();
void init_colors();
void draw_tetromino_window(WINDOW *, const int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int, int);


typedef struct {
    // for board:
    int rows;
    int cols;

    int game_board[BOARD_WIDTH][BOARD_HEIGHT];
    //further add

}Game;

const int TETROMINO_I[TETROMINO_SIZE][TETROMINO_SIZE] = {
        {0,0,0,0},
        {1, 1, 1, 1},
        {0,0,0,0},
        {0,0,0,0}
};


const int TETROMINO_J[TETROMINO_SIZE][TETROMINO_SIZE] = {
        {1,0,0,0},
        {1, 1, 1, 0},
        {0,0,0,0},
        {0,0,0,0}
};

const int TETROMINO_L[TETROMINO_SIZE][TETROMINO_SIZE] = {
        {1,0,0,0},
        {1, 1, 1, 0},
        {0,0,0,0},
        {0,0,0,0}
};

#endif