#include "util.h"

#include <string>
using std::string;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::ostringstream;

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

/**
 * Codigo para colorir texto adaptado do disponivel no SIGAA. `color` e `modificator` sao
 * parametros opcionais, cujo padrao eh deixar o texto branco e sem modificador.
 *
 * @param   msg          string na qual serao aplicados cor e modificadores
 * @param   color        variavel que define a cor do texto. opcoes definidas no namespace `Color`
 * @param   modificator  modificador aplicado ao texto. opcoes definidos no namespace `Color`
 * @return  a string     recebida com as cores e modificadores aplicados
 */
string tcolor(const string msg, Color color, Modificator modificator) {
    ostringstream oss;
    oss << "\e[" << (short)modificator << ";" << (short)color << "m" << msg << "\e[0m";
    return oss.str();
}
