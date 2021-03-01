#include "gametable.h"
#include "util.h"

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
void print_puzzletable(PuzzleTable puzzletable, int offset, int line_marker, int row_marker) {
    auto table = puzzletable.table;
    string padding (offset + 1, ' ');
    int puzzle_line = 0;
    int line_marker_index = -1;
    if (line_marker != -1) {
        line_marker_index = 2 + (line_marker - 1) + (line_marker - 1) / 3;
    }
    if (row_marker > 0) {
        int row_marker_padding = 4 + 2 * ((row_marker - 1) % 3) + 8 * ((row_marker - 1 ) / 3);
        cout << padding << string(row_marker_padding, ' ') << "V" << endl;
    }
    for (int line_index = 0; line_index < 14; line_index++) {
        if (line_index  == line_marker_index) {
            cout << ">" << string(offset, ' ');
        } else {
            cout << padding;
        }
        if (line_index == 0) {
            cout << "    1 2 3   4 5 6   7 8 9"   << endl;
        } else if (line_index == 1 || line_index == 13) {
            cout << "  +-------+-------+-------+" << endl;
        } else if (line_index == 5 || line_index == 9) {
            cout << "  |-------+-------+-------|" << endl;
        } else {
            int puzzle_row = 0;
            cout << puzzle_line + 1 << " | ";
            for (int i = 4; i < 27; i++) {
                if (i == 10 || i == 18 || i == 26 ){
                    cout << "|";
                } else if (odd(i)) {
                    cout << " ";
                } else {
                    if (table[puzzle_line][puzzle_row] < 0)
                        cout << " ";
                    if (table[puzzle_line][puzzle_row] > 0)
                        cout << table[puzzle_line][puzzle_row];
                    puzzle_row++;
                }
            }
            cout << endl;
            puzzle_line++;
        }
    }
    return;
}

