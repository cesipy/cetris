#include "tetris.hpp"


void initialize_graphics()
{
    initscr();
    noecho();
    resize_term(BOARD_HEIGHT,  BOARD_WIDTH);
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);       // allow for arrow keys
}