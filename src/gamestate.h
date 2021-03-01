#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gametable.h"
#include "gamestate.h"
#include "play.h"

struct Config {
    int    error; // codigo de erro caso programa seja mal invocado. 0 = tudo ok
    bool   help;
    int    ncheck;
    string input_puzzle_file;
};

struct GameState {
    Config       config;
    int          chosen_puzzle; // -1 indica que nenhuma puzzle foi selecionada ainda
    PuzzleTables puzzletables;
    PlayState    playstate;
};

void destroy_gamestate(GameState gamestate);

#endif
