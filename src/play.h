#ifndef PLAY_H
#define PLAY_H

#include "gametable.h"

struct PlayState {
    int         puzzle_index;
    int         checks_used;
    PuzzleTable input_table; /** contem os digitos escritos pelo usuario. 0=nenhum input do usuario */
    int         last_move_pos[2]; /** posicao (linha, coluna) da ultima jogada. */
    int         last_move_input;  /** ultimo digito inserido. -1 quer dizer que um digito foi apagado */ 
    bool        has_last_move;   /** alguma jogada ja foi realizada para falar-se da utima jogada? */
};

struct PlayMove {
    int  movepos[2];  // (linha, coluna)
    bool is_deletion; // true=a jogada foi remocao de digito ja colocado
};

struct UndoHistory {
    int undo_count = 0;
    Move* moves;
};

const PlayState default_playstate {-1, 0, empty_table, 0, 0, 0, false };

#endif
