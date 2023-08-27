#ifndef MAIN_H
#define MAIN_H

#include <ncurses.h>
#define GRAVITY_TICKS 35
#define BOARD_WIDTH  30
#define BOARD_HEIGHT 30
#define EMPTY_CELL 0
#define CELL 1
#define AMOUNT_OF_PIECES 2
#define BOARD_EDGE_RIGHT (BOARD_WIDTH-17)

#define ADD_BLOCK(w,x) waddch((w),' '|A_REVERSE|COLOR_PAIR(x));     \
                       waddch((w),' '|A_REVERSE|COLOR_PAIR(x))
#define ADD_EMPTY(w) waddch((w), ' '); waddch((w), ' ')

/* ----------------------------------------------------------- */

enum type {
     I, J, L
};


enum direction {
    left, right, down
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

    Block game_board[BOARD_HEIGHT][BOARD_WIDTH];
    WINDOW* win;
    bool running;
    char bottom_height;     // number of bottom row
    bool need_new_piece;    // is a piece still falling, or is a new one needed
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
void main_loop();
int hit_bottom();
void init_colors();

/**
 * move a piece in direction 'dir'.
 */
void move_piece(direction);
bool is_valid_block(int rows, int cols);

/**
 * sets a block in a given row and column to a specified value with several attributes
 *
 * @param row
 * @param col
 * @param value
 * @param is_falling
 * @param moved_in_prev_iteration
 */
void set_block(int row, int col, int value, bool is_falling, bool moved_in_prev_iteration);
void piece_counter_increase();
void falling_to_fixed();
bool can_piece_move(direction);
bool can_piece_rotate();
bool is_empty_block(int, int);

void alloc_game_board();
void dealloc_game_board();

#endif