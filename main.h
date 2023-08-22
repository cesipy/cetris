#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#define GRAVITY_TICKS 80
#define BOARD_WIDTH  30
#define BOARD_HEIGHT 30
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
    int value;
    bool falling_piece;
    bool fixed_piece;
    bool moved_in_prev_iteration;
}Block;

typedef struct {
    // for board:
    int rows;
    int cols;

    Block game_board[BOARD_WIDTH][BOARD_HEIGHT];
    WINDOW* win;
    //further add
}Game;


typedef struct {
    int rows;
    int cols;

    WINDOW* win;
    int type;
}Piece;

typedef struct {
    int x;
    int y;
}Coordinate;

void insert_falling_piece(type type, Game* g);
void insert_block(Game* g, int r, int c, bool is_fixed);

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