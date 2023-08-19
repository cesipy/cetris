#include <ncurses.h>
#include "main.h"

#include <stdlib.h>
#include <unistd.h>

void game_init(Game*, int rows, int cols);
void display_board(WINDOW* win, Game* g);

void gravity(Falling* f);

int ch;

WINDOW* board, * falling, *hold, *score;
Game* game;


int main (int argc, char* argv[])
{
    game = new Game;    // alloc memory
    falling_piece = new Falling ;
    game_init(game, BOARD_HEIGHT, BOARD_WIDTH);

    initscr();
    init_colors();
    noecho();
    resize_term(BOARD_HEIGHT,  BOARD_WIDTH);
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);       // allow for arrow keys

    board = newwin(game->rows, game->cols, 0, 0);
    falling_piece->win = newwin(falling_piece->rows, falling_piece->cols, 0, 0);

    main_loop();

    // free allocated objects
    delete game;
    delete falling_piece;
    endwin();
}

/*  ------------------------------  */

void main_loop()
{
    int tick = 0;

    while (!hit_bottom())
    {
        int input = getch();

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


        display_board(board, game);

        // Update Tetromino position and draw it
        draw_tetromino_window(falling_piece);

        if (tick % GRAVITY_TICKS == 0) {
            gravity(falling_piece);
        }


        doupdate();

        usleep(100000);

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



void display_board(WINDOW* win, Game* g)
{
    box(win, 0, 0);

    for(int i=0; i < g->rows; i++)
    {
        wmove(win, i +1, 1);
        for (int j = 0; j < g->cols; j++)
        {
            if (g->game_board[i][j] != EMPTY_CELL)
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

void gravity(Falling* f)
{
    for (int i=0; i<f->rows;i++)
    {
        for (int j=0; j<f->cols;j++)
        {
            if(f->game_board[i][j] == 1 && i < BOARD_HEIGHT - 1)
            {
                f->game_board[i][j] = 0;
                f->game_board[i+1][j] = 1;
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
            if(j == 1) { g->game_board[i][j] = CELL;}
            else if(i == 10) {g->game_board[i][j] = CELL; }
            // fill game board with empty cells at start -> '0' is emtpy
            else {
                g->game_board[i][j] = EMPTY_CELL;
            }
        }
    }
}

void init_falling_piece(Falling* f, int rows, int cols, type type)
{
    f->rows = rows;
    f->cols = cols;

    for (int i=0; i<f->rows; i++)
    {
        for (int j=0; j<f->cols;j++)
        {
            f->game_board[i][j] = 0;
        }
    }

    if (type == I)
    {
        int x = 10;
        int y = BOARD_WIDTH / 2;

        f->game_board[x][y-1]= 1;
        f->game_board[x][y]= 1;
        f->game_board[x][y+1]= 1;
        f->game_board[x][y+2]= 1;
    }
}

