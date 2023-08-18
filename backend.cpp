#include "backend.h"

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
