#include "play.h"
#include "gametable.h"
#include "gamestate.h"

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

PlayMove history_pop(UndoHistory* undohistory) {
    if (undohistory->undo_count == 0) {
        cout << "Erro: tentou `history_pop` sem jogadas na fila" << endl;
        return { 0, 0, 0, false };
    }
    PlayMove lastmove = undohistory->moves[undohistory->undo_count - 1];
    undohistory->undo_count--;
    PlayMove* newmoves = new PlayMove[undohistory->undo_count];
    for (int i = 0; i < undohistory->undo_count; i++) {
        newmoves[i] = undohistory->moves[i];
    }
    delete undohistory->moves;
    undohistory->moves = newmoves;
    return lastmove;
}

void history_push(UndoHistory* undohistory, PlayMove newmove) {
    if (undohistory->undo_count == 0) {
        undohistory->undo_count = 1;
        undohistory->moves = new PlayMove[1];
        undohistory->moves[0] = newmove;
        return;
    }
    undohistory->undo_count++;
    PlayMove* newmoves = new PlayMove[undohistory->undo_count];
    for (int i = 0; i < undohistory->undo_count - 1; i++) {
        newmoves[i] = undohistory->moves[i];
    }
    delete undohistory->moves;
    undohistory->moves = newmoves;
    undohistory->moves[undohistory->undo_count - 1] = newmove;

    cout << endl << "undocout " << undohistory->undo_count << endl; 
    return;
}

/**
 * Testa se faz sentido inserir este digito nesse espaco. Retorna 1 quando o numero
 * eh inserido com sucesso; numeros negativos indicam erros.
 * 
 * @param[in,out]   playstate
 */
int insert_digit(PuzzleTable puzzletable, PlayState* playstate, int line, int row, int num) {
    // subtraindo 1 pois para o jogador as linhas/colunas sao 1-indexadas
    if (puzzletable.table[line-1][row-1] > 0) {
        // tentou colocar digito aonde ja vem na puzzle
        return -1;
    }
    if (playstate->input_table.table[line-1][row-1] > 0) {
        // tentou colocar digito aonde ja tinha colocado
        return -2;
    }
    if (line < 1 || line > 9 || row < 1 || row > 9) {
        // tentou escrever fora do tabuleiro
        return -3;
    }
    if (num < 1 || num > 9) {
        // tentou escrever numero que nao pode
        return -4;
    }
    playstate->input_table.table[line-1][row-1] = num;
    history_push(&playstate->undohistory, { line, row, num, false });
    return 0;
}


/**
 * entra no fluxo de jogo de puzzle
 */
void play_puzzle(const PuzzleTable puzzletable, PlayState* playstate) {
    bool exit_flag = false;
    int line_marker = -1;
    int row_marker = -1;
    while (!exit_flag) {
        print_puzzletable(playstate->input_table, puzzletable, 3, line_marker, row_marker);
        cout << "a: Inserir digito         " << "b: Desfazer ultima jogada" << endl;
        cout << "c: Remover digito inserido" << "d: Solicitar verificação" << endl;
        cout << "e: Retornar ao menu principal" << endl;
        cin.clear();
        char play_input{};
        cin >> play_input;
        switch (play_input) {
            case 'a': 
                {
                    int line_num{};
                    cin.clear();
                    cout << "linha? ";
                    cin >> line_num;
                    int row_num{};
                    cin.clear();
                    cout << "fileira? ";
                    cin >> row_num;
                    int num{};
                    cin.clear();
                    cout << "numero? ";
                    cin >> num;
                    int insert_return{};
                    insert_return = insert_digit(puzzletable, playstate, line_num, row_num, num);
                    if (insert_return < 0) {
                        cout << "Erro " << insert_return << ": erro inserindo numero na puzzle. Comando desconsiderado." << endl;
                    } else {
                        line_marker = line_num;
                        row_marker = row_num;
                    }
                }
                break;
            case 'b':
                break;
            case 'c':
                break;
            case 'd':
                break;
            case 'e':
                break;
            default:
                break;
        }
    }
    return;
}

