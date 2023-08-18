#ifndef MAIN_H
#define MAIN_H

#include <curses.h>
#include "block.h"
#include "backend.h"
#define GRAVITY_TICKS 10


#define BOARD_WIDTH 50
#define BOARD_HEIGHT 50
#define EMPTY_CELL 0
#define CELL 1
#define TETROMINO_SIZE 4

#define ADD_BLOCK(w,x) waddch((w),' '|A_REVERSE|COLOR_PAIR(x));     \
                       waddch((w),' '|A_REVERSE|COLOR_PAIR(x))
#define ADD_EMPTY(w) waddch((w), ' '); waddch((w), ' ')

void main_loop();
int hit_bottom();
void init_colors();


enum type {
     I, J, L
};
typedef struct {
    // for board:
    int rows;
    int cols;

    int game_board[BOARD_WIDTH][BOARD_HEIGHT];
    //further add

}Game;

typedef struct {
    int rows;
    int cols;

    int game_board[BOARD_WIDTH][BOARD_HEIGHT];
    WINDOW* win;
}Falling;

typedef struct {
    int rows;
    int cols;

    WINDOW* win;
    int type;
}Piece;

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

const int* types[3] = {&TETROMINO_I[0][0], &TETROMINO_J[0][0], &TETROMINO_L[0][0]};

#endif