#include "play.h"
#include "gametable.h"

#include <iostream>
using std::cout;

/**
 * entra no fluxo de jogo de puzzle
 */
void play_puzzle(const PuzzleTable puzzletable, GameState* gamestate) {
    bool exit_flag = false;
    while (!exit_flag) {
        cout << "a: Inserir digito" << endl;
        cout << "b: Desfazer ultima jogada" << endl;
        cout << "c: Remover digito inserido" << endl;
        cout << "d: Solicitar verificação" << endl;
        cout << "e: Retornar ao menu principal" << endl;

    }
    return;
}

