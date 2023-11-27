#include "tetris.hpp"

/**
 * parses command line arguments for the game.
 * default is 1, error is 0. 
 * no graphics is 2.
 * 
*/
short parse_agruments(int argc, char* argv[]) 
{
    if (argc == 1) {
        return 1;
    }
    else if (strcmp (argv[1], "--no-graphics") == 0)
    {
        return 2;
    }
    else if (strcmp(argv [1], "--graphics") == 0)
    {
        return 1;
    }
    else
    {
        std::cerr << "incorrect usage! Usage: "<< argv[0] << " [--no-graphics/--graphics]" << "\n";
        return 0;
    }
}

/*  ------------------------------  */

int main (int argc, char* argv[])
{
    short arg = parse_agruments(argc, argv);

    if (arg == 1) {
        // defaut, graphics are activated
        initialize_graphics();   
    }
    
    Game* game = new Game;    // alloc memory
    initialize_game(game);
    // example_fill_board(game);
    main_loop(game);

    endwin();
    // free allocated objects
    delete game;
}

/*  ------------------------------  */

