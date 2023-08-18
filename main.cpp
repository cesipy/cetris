#include <curses.h>
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
    resize_term(50, 50);
    timeout(0);
    curs_set(0);
    //keypad(stdscr, TRUE);
    box(stdscr, 0, 0);

    main_loop();
    endwin();
}

/*  ------------------------------  */

void main_loop()
{
    while (!hit_bottom())
    {
        refresh();
        //printw("press 'q' to exit!\n");
        
        move(40, 10);         // first is row, second is column
        attron(A_BOLD | A_UNDERLINE);

        printw("This is bold and underlined text.");

        // Turn off the attributes
        attroff(A_BOLD | A_UNDERLINE);

        refresh();
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

