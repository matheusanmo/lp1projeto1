#include "gametable.h"

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <fstream>
using std::ifstream;

/**
 * Destroi um `PuzzleTables` deletando seu membro que eh ponteiro para lista de 
 * `PuzzleTable`s.
 * 
 * @param   puzzletables  NAO recebe ponteiro mas sim a variavel em si
 */
void destroy_puzzletables(PuzzleTables puzzletables) {
    delete[] puzzletables.tables;
    puzzletables.tables = nullptr;
    return;
}

/**
 * Recebe string com caminho de um arquivo a parseado como PuzzleTables. Descarta
 * tables que nao consegue parsear e avisa, mas nao gera erros ou para a execucao do 
 * programa. Nao checa se o arquivo existe ou eh acessivel; isto deve ser feito
 * de antemao. Espera-se que os tabuleiros estejam no seguinte formato:
 * 9 inteiros, com ou sem sinal, separados por whitespace (fora \n) seguidos
 * por uma linha vazia (mesmo sendo o ultimo tabuleiro do arquivo). O arquivo
 * deve comecar imediatamente com a primeira fileira do primeiro tabuleiro, isto
 * eh, nada de whitespace ou comentarios antes.  Se o membro `puzzletables` do retorno 
 * eh nullptr, um erro ocorreu.
 * 
 * @param   input_puzzle_file   caminho do arquivo
 * @returns PuzzleTables com tables que foram parseadas com sucesso
 */
PuzzleTables gen_puzzletables(string input_puzzle_file) {
    PuzzleTables puzzletables { 0, nullptr };
    ifstream input_stream (input_puzzle_file);
    int numcount = 0; // qtd inteiros no arquivo de texto
    int throwaway{};  // variavel inutil p forcar `>>` a procurar inteiros
    int tablecount = 0;
    while (input_stream >> throwaway) {
        numcount++;
    }
    // resetando posicao no arquivo
    input_stream.clear();
    input_stream.seekg(0);
    // checando se faltam/sobram numeros e quantas tables leremos
    if (numcount % 81 != 0) {
        cout << "Erro: `input_puzzle_file` contem tabuleiro mal formado (faltam ou sobram numeros)." << endl;
        return puzzletables;
    }
    tablecount = numcount / 81;
    // tables eh ponteiro para array de `tablecount` `PuzzleTable`s vazios (?) 
    PuzzleTable* tables     { new PuzzleTable[tablecount]{} }; 
    puzzletables.tables     = tables; // LEMBRAR de deletar usando `delete[] tables`
    puzzletables.tablecount = tablecount;

    int table_index{}, line_index{}, row_index{};
    for (table_index = 0; table_index < tablecount; table_index++) {
        for (line_index = 0; line_index < 9; line_index++) { 
            for (row_index = 0; row_index < 9; row_index++) {
                input_stream >> tables[table_index].table[line_index][row_index];
            }
        }
    }
    return puzzletables;
}

/**
 * printa na tela a puzzletable indentada com `offset` espacos antes do primeiro
 * numero (padrao 3).
 *
 * @params  puzzletable   tabela que sera printada
 * @params  offset      indentacao da table (padrao 3)
 */
void print_puzzletable(PuzzleTable puzzletable, int offset) {
    string padding (offset, ' ');
    for (int line_index = 0; line_index < 9; line_index++) {
        cout << padding;
        if (line_index % 3 == 0) {
            cout << "+---+---+---+" << endl << padding;
        }
        for (int row_index = 0; row_index < 9; row_index++) {
            if (row_index % 3 == 0) { 
                cout << "|";
            }
            if (puzzletable.table[line_index][row_index] < 0) {
                cout << " ";
            } else {
                cout << puzzletable.table[line_index][row_index];
            }
        }
        cout << "|" << endl;
    }
    cout << padding << "+---+---+---+" << endl;
    return;
}

