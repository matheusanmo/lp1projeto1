#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gametable.h"

struct GameState {
    Config       config;
    int          chosen_puzzle; // -1 indica que nenhuma puzzle foi selecionada ainda
    PuzzleTables puzzletables;
    PlayState    playstate;
};

void destroy_gamestate(GameState gamestate);

#endif
