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
    example_fill_board(game);
    initscr();
    init_colors();
    noecho();
    resize_term(BOARD_HEIGHT,  BOARD_WIDTH);
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);       // allow for arrow keys

    board = newwin(game->rows, game->cols, 0, 0);
    game->win = board;
    main_loop();

    // free allocated objects
    delete game;
    endwin();
}

/*  ------------------------------  */

void main_loop()
{
    int tick = 0;

    while (!hit_bottom())
    {
        int input = getch();
        // check for 'q' to quit the game
        if (input=='q') {break;}

        // handle input
        switch (input)
        {
            case KEY_LEFT:
                // Move Tetromino left
                break;
            case KEY_RIGHT:
                // Move Tetromino right
                //tetrominoX++;
                break;
            case KEY_UP:
                // Rotate Tetromino (implement the rotation logic)
                // Example: currentTetromino = rotate_tetromino(currentTetromino);
                break;
            case KEY_DOWN:
                //tetrominoY++;
                // Move Tetromino down faster (if desired)
                break;
            default:
                break;
        }


        display_board(game);

        // Update Tetromino position and draw it
        //draw_tetromino_window(falling_piece);

        if (tick % GRAVITY_TICKS == 0) {
            gravity(game);
        }


        doupdate();


        usleep(SLEEP_TIME);

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
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
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
                ADD_BLOCK(win, 1);
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
    for (int i=0; i < g->rows; i++)
    {
        for (int j=0; j<g->cols;j++)
        {
            /* checks for falling pieces/blocks in order to move them every tick */
            bool condition = g->game_board[i][j].falling_piece &&
                    !g->game_board[i][j].moved_in_prev_iteration &&
                    i < (BOARD_HEIGHT - 2);
            if(g->game_board[i][j].value == 1 && condition)
            {
                // update position of the falling piece on the board
                g->game_board[i][j].value = 0;
                g->game_board[i][j].falling_piece = false;

                // update new position and set as falling piece
                g->game_board[i+1][j].value = 1;
                g->game_board[i + 1][j].falling_piece = true;
                g->game_board[i + 1][j].moved_in_prev_iteration = true;
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

    // further implementation

    for (int i=0; i<g->rows;i++)
    {
        for (int j=0; j < g->cols;j++)
        {
            //if(j == 1) { g->game_board[i][j] = CELL;}
            if(i == 10)
            {
                g->game_board[i][j].value = CELL;
                g->game_board[i][j].fixed_piece = true;
            }
            // fill game board with empty cells at start -> '0' is emtpy
            g->game_board[i][j].value = EMPTY_CELL;
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



