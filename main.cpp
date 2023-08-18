#include <curses.h>
#include <unistd.h>
#include <cstdlib>

#include "main.h"
#include "backend.h"
#include "block.h"


int ch;
int gameBoard[BOARD_HEIGHT][BOARD_WIDTH]; // 2D array to represent the game board

WINDOW* board, * next, *hold, *score;
Game* game;

int main (int argc, char* argv[])
{
    game = new Game;    // alloc memory
    game_init(game, BOARD_HEIGHT, BOARD_WIDTH);
    initscr();
    init_colors();
    noecho();
    resize_term(BOARD_HEIGHT,  BOARD_WIDTH);
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);       // allow for arrow keys

    board = newwin(game->rows, game->cols, 0, 0);

    main_loop();

    delete game;
    endwin();
}

/*  ------------------------------  */

void main_loop()
{
    while (!hit_bottom())
    {
        // Implement Tetris game logic here
        // Update game state and windows

        box(board, 0, 0); // Draw a border around the board window
        wrefresh(board); // Refresh the board window

        usleep(10000);
        ch = getch();
        if (ch == 'q')
        {
            break;
        }
    }
}


int hit_bottom()
{
    return 0;
}

void init_colors()
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Example color pair
    // Define other color pairs
}
