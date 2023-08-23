#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#define GRAVITY_TICKS 35
#define BOARD_WIDTH  30
#define BOARD_HEIGHT 30
#define EMPTY_CELL 0
#define CELL 1

#define ADD_BLOCK(w,x) waddch((w),' '|A_REVERSE|COLOR_PAIR(x));     \
                       waddch((w),' '|A_REVERSE|COLOR_PAIR(x))
#define ADD_EMPTY(w) waddch((w), ' '); waddch((w), ' ')

/* ----------------------------------------------------------- */

enum type {
     I, J, L
};


enum direction {
    left, right
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
    bool running;
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

/* ----------------------------------------------------------- */

void insert_falling_piece(type type, Game* g);
void insert_block(Game* g, int r, int c, bool is_fixed);
void main_loop();
int hit_bottom();
void init_colors();

void move_piece(direction);
bool is_valid_block(int rows, int cols);
void set_block(int row, int col, int value, bool is_falling, bool moved_in_prev_iteration);

#endif