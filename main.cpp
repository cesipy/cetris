#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


int main (int argc, char* argv[])
{
    initscr();
    WINDOW* win = newwin(100, 100, 40, 30 );

    sleep(10);

    endwin();
}