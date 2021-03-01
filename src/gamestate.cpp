#include "gamestate.h"
#include "gametable.h"

/**
 * delete as alocacoes dinamicas feitas por um gamestate. Acessar certos membros de uma 
 * gametable destruida pode gerar erros de memoria.
 */
void destroy_gamestate(GameState gamestate) {
    destroy_puzzletables(gamestate.puzzletables);
    return;
}

