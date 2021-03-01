#ifndef PLAY_H
#define PLAY_H

#include "gametable.h"

struct PlayMove {
    int  movepos[2];  /** (linha, coluna) */
    int  num;
    bool is_deletion; 
};

/**
 * nao mexer diretamente em objetos UndoHistory. Use history_push e history_pop 
 * que foram implementados em play.cpp
 */
struct UndoHistory {
    int       undo_count;
    PlayMove* moves;
};

struct PlayState {
    int         puzzle_index;
    int         checks_used;
    PuzzleTable input_table; /** contem os digitos escritos pelo usuario. 0=nenhum input do usuario */
    UndoHistory undohistory; 
};

const UndoHistory default_undohistory { 0, nullptr };
const PlayState default_playstate { -1, 0, empty_table, default_undohistory };

void play_puzzle(const PuzzleTable puzzletable, PlayState* playstate);

#endif
