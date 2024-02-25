#include "../include/tetris.hpp"


void initialize_graphics()
{
    initscr();
    noecho();
    resize_term(BOARD_HEIGHT,  BOARD_WIDTH);
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);       // allow for arrow keys
}


void display_board(Game* g)
{
    WINDOW* win = g->win;

    for(int i=0; i < g->rows; i++)
    {
        wmove(win, i +1, 1);
        for (int j = 0; j < g->cols; j++)
        {

            if (g->game_board[i][j].value != EMPTY_CELL)
            {
                // draw block with saved color
                ADD_BLOCK(win, g->game_board[i][j].color);
            }

            else
            {
                // draw empty block
                ADD_EMPTY(win);
            }
        }
    }

    box(win, 0, 0);
    wnoutrefresh(win);
}

void display_score(Game* g)
{
    mvwprintw(g->win, 0, 0, "Score: %d", g->score);
    wrefresh(g->win);
}