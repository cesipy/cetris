#include <curses.h>
#include "main.h"
#include "backend.h"
#include "block.h"
#include <stdlib.h>
#include <unistd.h>

void game_init(Game*, int rows, int cols);
void display_board(WINDOW* win, Game* game);

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
    falling = newwin(44, 44, BOARD_HEIGHT / 2, BOARD_WIDTH / 2);
    box(board, 0, 0);
    wrefresh(board);

    main_loop();


    delete game;
    endwin();
}

/*  ------------------------------  */

void main_loop()
{
    int tetrominoX = 10;
    int tetrominoY = 0; // Start at the top

    int tempx = BOARD_HEIGHT / 2;
    int tempy = BOARD_WIDTH / 2;


    while (!hit_bottom())
    {
        int input = getch();

        if (input=='q') {break;}

        // Handle user input
        switch (input)
        {
            case KEY_LEFT:
                // Move Tetromino left
                tetrominoX--;

                break;
            case KEY_RIGHT:
                // Move Tetromino right
                tetrominoX++;
                break;
            case KEY_UP:
                // Rotate Tetromino (implement the rotation logic)
                // Example: currentTetromino = rotate_tetromino(currentTetromino);
                break;
            case KEY_DOWN:
                tetrominoY++;
                // Move Tetromino down faster (if desired)
                break;
            default:
                break;
        }



        // Clear Tetromino window
        werase(falling);

        // Update Tetromino position and draw it
        draw_tetromino_window(falling, TETROMINO_I, tetrominoY, tetrominoX);
        //mvprintw(tetrominoY, tetrominoX * 2, "[]");
        tempy++;
        tempx++;
        mvwin(falling, tempy, tempx);

        display_board(board, game);

        // Refresh Tetromino window
        wrefresh(falling);
        wrefresh(board);

        // Sleep for a short duration
        usleep(100000);

        // Handle additional game logic (collision detection, row clearing, etc.)
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
                // Set the color pair for the Tetromino block (adjust color pair number)
                wattron(win, COLOR_PAIR(1));
                // Print a block character at the calculated coordinates
                waddch(win, ' '); // Add your desired character representation
                wattroff(win, COLOR_PAIR(1));
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
            if(i == j) { game->game_board[i][j] = CELL;}
            // fill game board with empty cells at start -> '0' is emtpy
            else {
                game->game_board[i][j] = EMPTY_CELL;
            }
        }
    }
}

void display_board(WINDOW* win, Game* game)
{
    box(win, 0, 0);

    for(int i=0; i< game->rows;i++)
    {
        wmove(win, i +1, 1);
        for (int j = 0; j < game->cols;j++)
        {
            if (game->game_board[i][j] != EMPTY_CELL)
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