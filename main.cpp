#include <ncurses.h>
#include "main.h"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>

void game_init(Game*, int rows, int cols);
void display_board(Game* g);
void gravity(Game* g);
void example_fill_board(Game* g);

int ch;

WINDOW* board, * falling, *hold, *score;
Game* game;
int piece_counter = 0;

#define SLEEP_TIME 10000

/*  ------------------------------  */

int main (int argc, char* argv[])
{
    game = new Game;    // alloc memory
    game_init(game, BOARD_HEIGHT, BOARD_WIDTH); // initialize the board struct and all its members

    //fill the board with some blocks to draw
    // example_fill_board(game);

    initscr();
    init_colors();
    resizeterm(BOARD_HEIGHT, BOARD_WIDTH);
    noecho();
    resize_term(BOARD_HEIGHT,  BOARD_WIDTH);
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);       // allow for arrow keys

    board = newwin(game->rows, game->cols, 0, 0);
    game->win = board;

    example_fill_board(game);
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
        if (game->need_new_piece)
        {
            insert_falling_piece(I, game);
            piece_counter_increase();

            game->need_new_piece = false;
        }

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
    // check if the blocks below are free
    bool can_move_down = can_piece_move(down);
    if (!can_move_down)
    {
        falling_to_fixed();
        game->need_new_piece = true;
        return;
    }
    for (int i=g->rows-1; i > 0; i--)
    {
        for (int j= g->cols-1; j > 0; j--)
        {
            /* checks for falling pieces/blocks in order to move them every tick */
            bool condition =
                    g->game_board[i][j].value == 1 &&
                    g->game_board[i][j].falling_piece &&
                    !g->game_board[i][j].moved_in_prev_iteration;

            if(condition && i < game->bottom_height)
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

            else if (condition && i+1 >= game->bottom_height)
            {
                falling_to_fixed();
                game->need_new_piece = true;
                return;
            }
        }
    }
}


void game_init(Game* g, int rows, int cols)
{
    g->rows = rows;
    g->cols = cols;
    g->running = true;
    g->bottom_height = BOARD_HEIGHT - 2;
    g->need_new_piece = true;               // start with a new falling piece

    //alloc_game_board();
    // further implementation

    for (int i=0; i<g->rows;i++)
    {
        for (int j=0; j < g->cols;j++)
        {
            // fill game board with empty cells at start -> '0' is emtpy

            set_block(i, j, EMPTY_CELL, false, false);
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
            if (i == 13 && j < BOARD_EDGE_RIGHT  && j >= 0)
            {
                set_block(i, j, CELL, false, false);
            }
        }
    }
}


void insert_falling_piece(type type, Game* g)
{
    int mid = 8;

    if (type == L)
    {
        set_block(1, mid - 1, CELL, true, false);
        set_block(1, mid    , CELL, true, false);
        set_block(1, mid + 1, CELL, true, false);
        set_block(2, mid + 1, CELL, true, false);
    }

    else if (type == I)
    {
        set_block(1, mid - 1, CELL, true, false);
        set_block(1, mid    , CELL, true, false);
        set_block(1, mid + 1, CELL, true, false);
        set_block(1, mid + 2, CELL, true, false);
    }

    else if (type == J)
    {
        set_block(1, mid - 1, CELL, true, false);
        set_block(1, mid    , CELL, true, false);
        set_block(1, mid + 1, CELL, true, false);
        set_block(2, mid - 1, CELL, true, false);
    }
}


void move_piece(direction dir) {
    bool can_move = can_piece_move(dir);

    if (!can_move) { printf("ey"); return; }
    printf("reach");

    // traverse the game board in the direction based on the dir parameter
    if (dir == right)
    {
        for (int i = game->rows -1; i >= 0; i--)
        {
            for (int j = game->cols - 1; j >= 0; j--)
            {
                bool condition = game->game_board[i][j].value == CELL &&
                                 game->game_board[i][j].falling_piece;

                if (condition)
                {
                    // calculate the new column index
                    int new_j = j + 1;

                    if (is_valid_block(i, new_j) )
                    {
                        // delete old block
                        set_block(i, j, EMPTY_CELL, false, false);

                        // set new block at the updated position
                        set_block(i, new_j, CELL, true, false);
                    }
                }
            }
        }
    }

    // if dir == left, traverse the list the way around
    else
    {
        for (int i = 0; i < game->rows; i++)
        {
            for (int j = 0; j < game->cols; j++)
            {
                bool condition = game->game_board[i][j].value == CELL &&
                                 game->game_board[i][j].falling_piece;

                if (condition)
                {
                    // calculate the new column index
                    int new_j = j - 1;

                    if (is_valid_block(i, new_j))
                    {
                        // delete old block
                        set_block(i, j, EMPTY_CELL, false, false);

                        // set new block at the updated position
                        set_block(i, new_j, CELL, true, false);
                    }
                }
            }
        }
    }

}



bool is_valid_block(int rows, int cols)
{
    bool condition1 = rows >= 0 && rows < game->rows - 2;
    bool condition2 = cols >= 0 && cols <= BOARD_EDGE_RIGHT;

    if (condition1 && condition2)
    { return true; }

    return false;
}


void set_block(int row, int col, int value, bool is_falling, bool moved_in_prev_iteration)
{
    game->game_board[row][col].value         = value;
    if (value == EMPTY_CELL)
    {
        game->game_board[row][col].falling_piece = false;
        game->game_board[row][col].fixed_piece   =  false;
        game->game_board[row][col].moved_in_prev_iteration  =  false;
    }

    else
    {
        game->game_board[row][col].falling_piece = is_falling;
        game->game_board[row][col].fixed_piece = !is_falling;
        game->game_board[row][col].moved_in_prev_iteration = moved_in_prev_iteration;
    }
}


void piece_counter_increase()
{
    piece_counter++;
    piece_counter = piece_counter % (AMOUNT_OF_PIECES + 1);
}


void falling_to_fixed()
{
    for (int i=0; i<game->rows;i++)
    {
        for (int j=0; j<game->cols;j++)
        {
            bool condition = game->game_board[i][j].value == CELL &&
                    game->game_board[i][j].falling_piece;

            if (condition)
            {
                set_block(i, j, CELL, false, false);
            }
        }
    }
}


bool can_piece_move(direction dir)
{
    for (int i = 0; i < game->rows; i++)
    {
        for (int j = 0; j < game->cols; j++)
        {
            bool condition = game->game_board[i][j].value == CELL &&
                             game->game_board[i][j].falling_piece;

            if (condition)
            {
                int new_i = i;
                int new_j = j;

                if (dir == left)
                {
                    new_j = j - 1;
                }

                else if (dir == right)
                {
                    new_j = j + 1;
                }

                else if (dir == down)
                {
                    new_i = i + 1;
                }
                bool valid_block = is_valid_block(new_i, new_j);

                bool empty_block = is_empty_block(new_i, new_j);

                //std::cout<<"valid block: " << valid_block << ", empty block: "<<empty_block<<"\n";

                // check if the new position is valid and not colliding with other pieces
                if (!is_valid_block(new_i, new_j) || !is_empty_block(new_i, new_j))
                {
                    return false; // collision detected
                }
            }
        }
    }
    return true; // no collision detected
}


bool is_empty_block(int row, int col)
{
    bool condition = !game->game_board[row][col].fixed_piece;
    if (condition)
    {
        return true;
    }
    return false;
}