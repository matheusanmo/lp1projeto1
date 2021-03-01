#include <string>
using std::string;

#include <fstream>
using std::ifstream;

#include "util.h"

/**
 * recebe string que deve ser caminho de um arquivo. checa se ele pode ser aberto como
 * stream e retorna true em caso positivo
 * 
 * @param   filepath    caminho do arquivo a ser acessado com `std::ifstream`
 * @returns `true` caso o arquivo possa ser acessado
 */
bool file_exists(string filepath) {
    ifstream input_stream (filepath);
    return input_stream.good();
}

bool even(int n) {
    return n % 2 == 0;
}

bool odd(int n) {
    return !even(n);
}
