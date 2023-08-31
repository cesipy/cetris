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
    short color; // color from 0 to 7
}Block;


typedef struct {
    // for board:
    int rows;
    int cols;

    Block game_board[BOARD_HEIGHT][BOARD_WIDTH];
    WINDOW* win;
    bool running;
    char bottom_height;         // number of bottom row
    bool need_new_piece;        // is a piece still falling, or is a new one needed
    int highest_fixed_block;    // height of the highest current block. (to check for game-over)
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
void game_init(Game*, int rows, int cols);
void display_board(Game* g);

/**
 * Apply gravity to the falling piece on the game board.
 *
 * This function checks if the blocks below the falling piece are free. If they are,
 * it moves the falling piece down one row. If the falling piece reaches the bottom
 * or encounters obstacles below, it sets the piece as a fixed piece on the board
 * and triggers the need for a new piece.
 *
 * @param g A pointer to the Game struct containing game information.
 * @return 1 if the falling piece has reached the bottom and needs a new piece, 0 otherwise.
 * Is used for skip_gravity function
 */
int gravity(Game* g);
void example_fill_board(Game* g);

/**
 * move a piece in direction 'dir'.
 */
void move_piece(direction);

/**
 * Check if a block at the specified row and column is within the valid game board boundaries.
 *
 * @param row The row of the block to check.
 * @param col The column of the block to check.
 * @return true if the block is within the valid game board boundaries, false otherwise.
 */
bool is_valid_block(int rows, int cols);


/**
 * @brief Set a block in a given row and column to a specified value with several attributes.
 *
 * @param row The row in which to set the block.
 * @param col The column in which to set the block.
 * @param value The value to set for the block (e.g., EMPTY_CELL or CELL).
 * @param is_falling Whether the block is part of a falling piece.
 * @param moved_in_prev_iteration Whether the block was moved in the previous iteration.
 * @param color Value from 0 to 7, indicates the color. 8 means field is free.
 */
void set_block(int row, int col, int value, bool is_falling, bool moved_in_prev_iteration, short color);
void piece_counter_increase(void);
void falling_to_fixed(void);
bool can_piece_move(direction);
bool can_piece_rotate(void);
bool is_empty_block(int, int);
void skip_tick_gravity(void);

void check_game_state(void);
int generate_random_number(int min, int max);


void alloc_game_board();
void dealloc_game_board();

#endif