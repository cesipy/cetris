#include <ncurses.h>
#include "main.h"
#include <unistd.h>
#include <iostream>

int ch;

WINDOW* board, * falling, *hold, *score;
Game* game;
int piece_counter = 0;

#define SLEEP_TIME 10000

/*  ------------------------------  */

int main (int argc, char* argv[])
{
    game = new Game;    // alloc memory
    game_init(game, BOARD_HEIGHT, BOARD_WIDTH); // initialize the board struct and all its members

    //fill the board with some blocks to draw
    // example_fill_board(game);

    initscr();
    init_colors();
    noecho();
    resize_term(BOARD_HEIGHT,  BOARD_WIDTH);
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);       // allow for arrow keys
    srand(time(0));

    board = newwin(game->rows, game->cols, 0, 0);
    game->win = board;

    // example_fill_board(game);
    main_loop();

    // free allocated objects
    delete game;
    endwin();
}

/*  ------------------------------  */

void main_loop()
{
    int tick = 0;

    while (game->running)
    {

        if (game->need_new_piece)
        {
            int random_piece = generate_random_number(0, 2);
            insert_falling_piece(static_cast<type>(random_piece), game);
            piece_counter_increase();

            game->need_new_piece = false;
        }

        int input = getch();
        // check for 'q' to quit the game
        if (input=='q') {break;}

        // handle input
        switch (input)
        {
            case KEY_LEFT:
                // Move Tetromino left
                move_piece(left);
                break;
            case KEY_RIGHT:
                // Move Tetromino right
                move_piece(right);
                break;
            case KEY_UP:
                // rotate
                can_piece_rotate(left);
                break;
            case KEY_DOWN:
                skip_tick_gravity();
                // Move Tetromino down faster (if desired)
                break;
            default:
                break;
        }

        display_board(game);

        // update position if a falling piece aka gravity
        if (tick % GRAVITY_TICKS == 0) {
            gravity(game);
            // std::cout<<"row: " <<game->middle_coordinate.row<<",col: " << game->middle_coordinate.col<<"\n";
        }

        doupdate();             // update all windows

        usleep(SLEEP_TIME);     // sleep for a bit

        tick++;

        check_game_state();

    }
}


int hit_bottom()
{
    return 0;
}


void init_colors()
{
    start_color();
    for (int i = 1; i < 8; i++) 
    {
        init_pair(i, i, 0);
    }
    // Define other color pairs
}


void display_board(Game* g)
{
    WINDOW* win = g->win;

    //temp for debugging
    Position middle = game->middle_coordinate;

    for(int i=0; i < g->rows; i++)
    {
        wmove(win, i +1, 1);
        for (int j = 0; j < g->cols; j++)
        {
            if (g->game_board[i][j].value != EMPTY_CELL)
            {


                //debugging: show middle coordinate
                if (i == middle.row && j == middle.col)
                {
                    ADD_BLOCK(win, 1);
                }
                else
                {
                    ADD_BLOCK(win, game->game_board[i][j].color);
                }
            }

            else
            {
                ADD_EMPTY(win);
            }
        }
    }
    box(win, 0, 0);
    wnoutrefresh(win);
}


int gravity(Game* g)
{
    // check if the blocks below are free
    bool can_move_down = can_piece_move(down);
    if (!can_move_down)
    {
        falling_to_fixed();
        game->need_new_piece = true;
        return 1;       // ultimate gravity turn
    }

    // save middle point
    int middle_row = game->middle_coordinate.row;
    int middle_col = game->middle_coordinate.col;

    // piece is moved down
    for (int i=g->rows-1; i > 0; i--)
    {
        for (int j= g->cols-1; j > 0; j--)
        {
            /* checks for falling pieces/blocks in order to move them every tick */
            bool condition =
                    g->game_board[i][j].value == 1 &&
                    g->game_board[i][j].falling_piece &&
                    !g->game_board[i][j].moved_in_prev_iteration;

            if(condition && i < game->bottom_height)
            {

                // save the color
                short color = game->game_board[i][j].color;

                // update position of the falling piece on the board
                set_block(i, j, EMPTY_CELL, false, false, 8 );

                // update new position and set as falling piece
                set_block(i+1, j, CELL, true, true, color);

                if (i == middle_row && j == middle_col)
                {
                    game->middle_coordinate.row++;
                }
            }

            else if (g->game_board[i][j].value == 1 && g->game_board[i][j].moved_in_prev_iteration)
            {
                // update if block was moved
                g->game_board[i][j].moved_in_prev_iteration = false;
            }

            else if (condition && i+1 >= game->bottom_height)
            {
                falling_to_fixed();
                game->need_new_piece = true;
                return 1;       // return needed for skip_tick_gravity if down arrow is pressed
            }
        }
    }


    return 0;                    // return needed for skip_tick_gravity if down arrow is pressed
}


void game_init(Game* g, int rows, int cols)
{
    // Position for middle coordinate
    Position position = {.row=0, .col=0};

    g->rows                = rows;
    g->cols                = cols;
    g->running             = true;
    g->bottom_height       = BOARD_HEIGHT - 2;
    g->need_new_piece      = true;               // start with a new falling piece
    g->highest_fixed_block = 0;
    g->middle_coordinate   = position;

    // further implementation

    for (int i=0; i<g->rows;i++)
    {
        for (int j=0; j < g->cols;j++)
        {
            // fill game board with empty cells at start -> '0' is emtpy

            set_block(i, j, EMPTY_CELL, false, false, 8);
        }
    }
}


void example_fill_board(Game* g)
{
    int rows = g->cols;
    int cols = g->cols;

    for (int i=0; i<rows; i++)
    {
        for (int j=0; j< cols; j++)
        {
            if (i == 13 && j < BOARD_EDGE_RIGHT  && j >= 0)
            {
                set_block(i, j, CELL, false, false, 4);
            }
        }
    }
}


void insert_falling_piece(type type, Game* g)
{
    int mid = 8;
    short color = generate_random_number(0, 7);
    Position pos;

    if (type == J)
    {
        set_block(1, mid - 1, CELL, true, false, color);
        set_block(1, mid    , CELL, true, false, color);
        set_block(1, mid + 1, CELL, true, false, color);
        set_block(2, mid + 1, CELL, true, false, color);

        // assign middle position
        pos.row = 1;
        pos.col = mid + 1;
    }

    else if (type == I)
    {
        set_block(1, mid - 1, CELL, true, false, color);
        set_block(1, mid    , CELL, true, false, color);
        set_block(1, mid + 1, CELL, true, false, color);
        set_block(1, mid + 2, CELL, true, false, color);

        // assign middle position
        pos.row = 1;
        pos.col = mid + 1;
    }

    else if (type == L)
    {
        set_block(1, mid - 1, CELL, true, false, color);
        set_block(1, mid    , CELL, true, false, color);
        set_block(1, mid + 1, CELL, true, false, color);
        set_block(2, mid - 1, CELL, true, false, color);

        // assign middle position
        pos.row = 1;
        pos.col = mid - 1;
    }

    game->middle_coordinate = pos;
}


void move_piece(direction dir) {
    bool can_move = can_piece_move(dir);

    if (!can_move)
    {
        return;
    }

    // traverse the game board in the direction based on the dir parameter
    if (dir == right)
    {
        for (int i = game->rows -1; i >= 0; i--)
        {
            for (int j = game->cols - 1; j >= 0; j--)
            {
                bool condition = game->game_board[i][j].value == CELL &&
                                 game->game_board[i][j].falling_piece;

                if (condition)
                {
                    // calculate the new column index
                    int new_j = j + 1;

                    if (is_valid_block(i, new_j) )
                    {
                        // save the color
                        short color = game->game_board[i][j].color;
                        // delete old block
                        set_block(i, j, EMPTY_CELL, false, false, 8);

                        // set new block at the updated position
                        set_block(i, new_j, CELL, true, false, color);

                    }
                }
            }
        }
        // update position of middle coordinate to the right
        game->middle_coordinate.col++;
    }

    // if dir == left, traverse the list the way around
    else
    {
        for (int i = 0; i < game->rows; i++)
        {
            for (int j = 0; j < game->cols; j++)
            {
                bool condition = game->game_board[i][j].value == CELL &&
                                 game->game_board[i][j].falling_piece;

                if (condition)
                {
                    // calculate the new column index
                    int new_j = j - 1;

                    if (is_valid_block(i, new_j))
                    {

                        // save the color
                        short color = game->game_board[i][j].color;

                        // delete old block
                        set_block(i, j, EMPTY_CELL, false, false, 8);

                        // set new block at the updated position
                        set_block(i, new_j, CELL, true, false, color);
                    }
                }
            }
        }
        // update position of middle coordinate to the left
        game->middle_coordinate.col--;
    }

}



bool is_valid_block(int rows, int cols)
{
    bool condition1 = rows >= 0 && rows < game->rows - 2;
    bool condition2 = cols >= 0 && cols <= BOARD_EDGE_RIGHT;

    if (condition1 && condition2)
    { return true; }

    return false;
}


void set_block(int row, int col, int value, bool is_falling, bool moved_in_prev_iteration, short color)
{
    game->game_board[row][col].value                        = value;
    game->game_board[row][col].color                        = color;
    if (value == EMPTY_CELL)
    {
        game->game_board[row][col].falling_piece            = false;
        game->game_board[row][col].fixed_piece              =  false;
        game->game_board[row][col].moved_in_prev_iteration  =  false;
    }

    else
    {
        game->game_board[row][col].falling_piece = is_falling;
        game->game_board[row][col].fixed_piece = !is_falling;
        game->game_board[row][col].moved_in_prev_iteration = moved_in_prev_iteration;
    }
}


void piece_counter_increase()
{
    piece_counter++;
    piece_counter = piece_counter % (AMOUNT_OF_PIECES + 1);
}


void falling_to_fixed()
{
    for (int i=0; i<game->rows;i++)
    {
        for (int j=0; j<game->cols;j++)
        {
            bool condition = game->game_board[i][j].value == CELL &&
                    game->game_board[i][j].falling_piece;

            if (condition)
            {
                // save the color
                short color = game->game_board[i][j].color;

                // set to falling piece
                set_block(i, j, CELL, false, false, color);
                int height = BOARD_HEIGHT - i;

                bool condition_height = height > game->highest_fixed_block;

                if (condition_height)
                {
                    // set new highest current block
                    game->highest_fixed_block = height;
                }
            }
        }
    }
}


bool can_piece_move(direction dir)
{
    for (int i = 0; i < game->rows; i++)
    {
        for (int j = 0; j < game->cols; j++)
        {
            bool condition = game->game_board[i][j].value == CELL &&
                             game->game_board[i][j].falling_piece;

            if (condition)
            {
                int new_i = i;
                int new_j = j;

                if (dir == left)
                {
                    new_j = j - 1;
                }

                else if (dir == right)
                {
                    new_j = j + 1;
                }

                else if (dir == down)
                {
                    new_i = i + 1;
                }
                bool valid_block = is_valid_block(new_i, new_j);

                bool empty_block = is_empty_block(new_i, new_j);

                // check if the new position is valid and not colliding with other pieces
                if (!is_valid_block(new_i, new_j) || !is_empty_block(new_i, new_j))
                {
                    return false; // collision detected
                }
            }
        }
    }
    return true; // no collision detected
}


bool is_empty_block(int row, int col)
{
    bool condition = !game->game_board[row][col].fixed_piece;
    if (condition)
    {
        return true;
    }
    return false;
}


void skip_tick_gravity()
{
    int status_gravity = gravity(game);
    while (status_gravity != 1)
    {
        status_gravity = gravity(game);
    }
}


// check for highest current block
void check_game_state(void)
{
    // check if highest current block reaches top -> game-over
    if (game->highest_fixed_block >= BOARD_HEIGHT-1)
    {
        game->running = false;
    }
}

int generate_random_number(int min, int max)
{
    int random_number = rand() % (max+1);
    return random_number;
}


Position block_position_after_rotation(int row, int col, direction dir)
{
    Position pos;

    Position middle_position = game->middle_coordinate;
    int delta_row, delta_col;

    delta_row = row - middle_position.row;
    delta_col = col - middle_position.row;

    // turn clockwise
    if (dir == right)
    {
        // block is to the right/right of middle
        if (delta_col != 0 && delta_row == 0)
        {
            pos.row = row + delta_col;
            pos.col = middle_position.col;
        }
        // block is above/under middle
        else if (delta_row != 0 && delta_col == 0)
        {
            pos.col = col - delta_row;
            pos.row = middle_position.row;
        }
        // block is middle
        else
        {
            pos.row = middle_position.row;
            pos.col = middle_position.col;
        }
    }

    else if (dir == left)
    {
        // block is to the left/right of middle
        if (delta_col != 0 && delta_row == 0)
        {
            pos.row = row - delta_col;
            pos.col = middle_position.col;
        }
        // block is above/under middle
        else if (delta_row != 0 && delta_col == 0)
        {
            pos.col = col - delta_row;
            pos.row = middle_position.row;
        }
        // block is middle
        else
        {
            pos.row = middle_position.row;
            pos.col = middle_position.col;
        }
    }
    return pos;
}

bool can_piece_rotate(direction dir)
{
    for (int i=0; i<game->rows;i++)
    {
        for(int j=0; j<game->cols; j++)
        {
            bool condition =
                    game->game_board[i][j].falling_piece && game->game_board[i][j].value == CELL;

            if (condition)
            {
                Position pos = block_position_after_rotation(i, j, dir);

                // is position valid?
                bool valid_position = is_valid_block(pos.row, pos.col) && is_empty_block(pos.row, pos.col);
                if (!valid_position) { //std::cout<<"cant rotate";
                    return false; }
            }
        }
    }
    //std::cout << "can rotate!";
    return true;
}


// rotate the piece
void rotate_piece()
{
    return;
}