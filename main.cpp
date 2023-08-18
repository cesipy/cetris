#include <curses.h>
#include "main.h"
#include "backend.h"
#include "block.h"
#include <stdlib.h>
#include <unistd.h>

void game_init(Game*, int rows, int cols);

int ch;

WINDOW* board, * falling, *hold, *score;
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
    falling = newwin(4, 4, 10, 10);

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
        // Clear the Tetromino window
        wclear(falling);

        // Draw the Tetromino on the Tetromino window
        draw_tetromino_window(falling,  TETROMINO_I, 10, 10);

        // Refresh the Tetromino window
        wrefresh(falling);

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

// Draw a Tetromino on the specified window at the given row and column
void draw_tetromino_window(WINDOW* win, const int tetromino[TETROMINO_SIZE][TETROMINO_SIZE], int row, int col)
{
    for (int i = 0; i < TETROMINO_SIZE; i++) {
        for (int j = 0; j < TETROMINO_SIZE; j++) {
            // Calculate the coordinates on the window
            int y = row + i;
            int x = col + j;

            // Check if the current cell in the Tetromino is a block (1)
            if (tetromino[i][j] == 1) {
                // Print a block character at the calculated coordinates
                mvwprintw(win, y, x * 2, "[]"); // Multiply x by 2 to leave space between blocks
            }
        }
    }
}

void game_init(Game* game, int rows, int cols)
{
    game->rows = rows;
    game->cols = cols;

    // further implementation

    for (int i=0; i<game->rows;i++)
    {
        for (int j=0; j < game->cols;j++)
        {
            // fill game board with empty cells at start -> '0' is emtpy
            game->game_board[i][j] = EMPTY_CELL;
        }
    }
}
