#ifndef MAIN_H
#define MAIN_H

#define BOARD_WIDTH 50
#define BOARD_HEIGHT 50
#define EMPTY_CELL 0

void main_loop();
int hit_bottom();
void init_colors();

typedef struct {
    // for board:
    int rows;
    int cols;

    int game_board[BOARD_WIDTH][BOARD_HEIGHT];
    //further add

}Game;

#endif