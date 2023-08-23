#include <ncurses.h>
#include "main.h"

#include <stdlib.h>
#include <unistd.h>

void game_init(Game*, int rows, int cols);
void display_board(Game* g);
void gravity(Game* g);
void example_fill_board(Game* g);

int ch;

WINDOW* board, * falling, *hold, *score;
Game* game;

#define SLEEP_TIME 10000

int main (int argc, char* argv[])
{
    game = new Game;    // alloc memory
    game_init(game, BOARD_HEIGHT, BOARD_WIDTH); // initialize the board struct and all its members

    //fill the board with some blocks to draw
    // example_fill_board(game);

    initscr();
    init_colors();
    noecho();
    resize_term(BOARD_HEIGHT,  BOARD_WIDTH);
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);       // allow for arrow keys

    board = newwin(game->rows, game->cols, 0, 0);
    game->win = board;
    
    insert_falling_piece(L, game);

    main_loop();

    // free allocated objects
    delete game;
    endwin();
}

/*  ------------------------------  */

void main_loop()
{
    int tick = 0;

    while (game->running)
    {
        int input = getch();
        // check for 'q' to quit the game
        if (input=='q') {break;}

        // handle input
        switch (input)
        {
            case KEY_LEFT:
                // Move Tetromino left
                move_piece(left);
                break;
            case KEY_RIGHT:
                // Move Tetromino right
                move_piece(right);
                break;
            case KEY_UP:
                // Rotate Tetromino (implement the rotation logic)
                // Example: currentTetromino = rotate_tetromino(currentTetromino);
                break;
            case KEY_DOWN:
                // Move Tetromino down faster (if desired)
                break;
            default:
                break;
        }


        display_board(game);

        // update position if a falling piece aka gravity
        if (tick % GRAVITY_TICKS == 0) {
            gravity(game);
        }

        doupdate();             // update all windows

        usleep(SLEEP_TIME);     // sleep for a bit

        tick++;
    }
}


int hit_bottom()
{
    return 0;
}


void init_colors()
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLUE); // Example color pair
    init_pair(2, COLOR_BLUE, COLOR_RED);
    // Define other color pairs
}


void display_board(Game* g)
{
    WINDOW* win = g->win;
    box(win, 0, 0);

    for(int i=0; i < g->rows; i++)
    {
        wmove(win, i +1, 1);
        for (int j = 0; j < g->cols; j++)
        {
            if (g->game_board[i][j].value != EMPTY_CELL)
            {
                if (g->game_board[i][j].falling_piece)
                { ADD_BLOCK(win, 1); }

                else
                {
                    ADD_BLOCK(win, 2);
                }
            }

            else
            {
                ADD_EMPTY(win);
            }
        }
    }
    wnoutrefresh(win);
}


void gravity(Game* g)
{
    for (int i=g->rows-1; i > 0; i--)
    {
        for (int j= g->cols-1; j > 0; j--)
        {
            /* checks for falling pieces/blocks in order to move them every tick */
            bool condition =
                    g->game_board[i][j].value == 1 &&
                    g->game_board[i][j].falling_piece &&
                    !g->game_board[i][j].moved_in_prev_iteration &&
                    i < (BOARD_HEIGHT - 2);
            if(condition)
            {
                // update position of the falling piece on the board
                set_block(i, j, 0, false, false );

                // update new position and set as falling piece
                set_block(i+1, j, 1, true, true);
            }
            else if (g->game_board[i][j].value == 1 && g->game_board[i][j].moved_in_prev_iteration)
            {
                // update if block was moved
                g->game_board[i][j].moved_in_prev_iteration = false;
            }
        }
    }
}

void game_init(Game* g, int rows, int cols)
{
    g->rows = rows;
    g->cols = cols;
    g->running = true;

    // further implementation

    for (int i=0; i<g->rows;i++)
    {
        for (int j=0; j < g->cols;j++)
        {
            // fill game board with empty cells at start -> '0' is emtpy
            g->game_board[i][j].value = EMPTY_CELL;
            //if(j == 1) { g->game_board[i][j] = CELL;}

            // example init for fixed blocks
            if(i == 20)
            {
                set_block(i, j, CELL, false, false);

            }
        }
    }
}

void example_fill_board(Game* g)
{
    int rows = g->cols;
    int cols = g->cols;

    for (int i=0; i<rows; i++)
    {
        for (int j=0; j< cols; j++)
        {
            if (i == 6 && j < BOARD_WIDTH -1 && j >= 1)
            {
                g->game_board[i][j].value = CELL;
                g->game_board[i][j].falling_piece = true;
            }
        }
    }
}



void insert_falling_piece(type type, Game* g)
{
    int mid = 10;

    if (type == L)
    {
        insert_block(game, 1, mid - 1 , false);
        insert_block(game, 1, mid     , false);
        insert_block(game, 1, mid + 1 , false);
        insert_block(game, 2, mid + 1 , false);
    }

    else if (type == I)
    {
        insert_block(game, 0, mid - 1 , false);
        insert_block(game, 0, mid     , false);
        insert_block(game, 0, mid + 1 , false);
        insert_block(game, 0, mid + 2 , false);
    }

    else if (type == J)
    {
        insert_block(game, 0, mid - 1 , false);
        insert_block(game, 0, mid     , false);
        insert_block(game, 0, mid + 1 , false);
        insert_block(game, 1, mid - 1 , false);
    }
}


void insert_block(Game* g, int r, int c, bool is_fixed)
{
    g->game_board[r][c].value = CELL;
    g->game_board[r][c].falling_piece = !is_fixed;
    g->game_board[r][c].fixed_piece = is_fixed;
    g->game_board[r][c].moved_in_prev_iteration = false;
}


void move_piece(direction dir)
{
    for (int i=0; i < game->rows; i++)
    {
        for (int j=0; j<game->cols; j++)
        {
            bool condition = game->game_board[i][j].value == CELL &&
                             game->game_board[i][j].falling_piece;

            if (condition)
            {
                // check if new positions for each block are valid
                int new_j = (dir == left) ? j-1 : j+1;
                bool is_available = is_valid_block(i, new_j);

                if (!is_available) { return; }
            }
        }
    }


    // run again through game board to move each block
    for (int i=0; i<game->rows; i++)
    {
        for (int j=0; j<game->cols; j++)
        {
            bool condition = game->game_board[i][j].value == CELL &&
                             game->game_board[i][j].falling_piece;

            if (condition)
            {
                // delete old block
                set_block(i, j, EMPTY_CELL, false, false);

                // set new block
                int new_j = (dir == left) ? j-1 : j+1;
                set_block(i, new_j, CELL, true, true);
            }
        }
    }
}

bool is_valid_block(int rows, int cols)
{
    return true;
}

/**
 * sets a block in a given row and column to a specified value with several attributes
 *
 * @param row
 * @param col
 * @param value
 * @param is_falling
 * @param moved_in_prev_iteration
 */
void set_block(int row, int col, int value, bool is_falling, bool moved_in_prev_iteration)
{
    game->game_board[row][col].value         = value;
    game->game_board[row][col].falling_piece = is_falling;
    game->game_board[row][col].fixed_piece   =  !is_falling;
    game->game_board[row][col].moved_in_prev_iteration  =  moved_in_prev_iteration;
}
