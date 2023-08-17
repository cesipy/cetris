#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "main.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

int ch;
int gameBoard[BOARD_HEIGHT][BOARD_WIDTH]; // 2D array to represent the game board


int main (int argc, char* argv[])
{
    initscr();
    noecho();
    resize_term(22, 22);
    timeout(0);
    curs_set(0);
    box(stdscr, 0, 0);

    main_loop();
    endwin();
}

/*  ------------------------------  */

void main_loop()
{
    while (!hit_bottom())
    {
        usleep(10000);
        ch = getchar();
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

