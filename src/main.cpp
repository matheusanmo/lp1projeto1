#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;
using std::stoi;

const int    DEFAULT_NCHECK_NUM        = 3;
const string DEFAULT_INPUT_PUZZLE_FILE ("input.txt");

struct Config {
    int error; // codigo de erro caso programa seja mal invocado. 0 = tudo ok
    bool help;
    int ncheck;
    string input_puzzle_file;
};

/**
 * recebe argc e argv como os do main e retorna Config equivalente
 *
 * @param   argc    argc como passado para main()
 * @param   argv    argv como passado para main()
 * @returns config padrao sobrescrita pelo que eh passado
 */
Config make_config(int argc, char** argv) {
    // esta eh a configuracao padrao
    Config conf = { 0, false, DEFAULT_NCHECK_NUM, DEFAULT_INPUT_PUZZLE_FILE };
    if (argc == 1) {
        // invocado sem argumentos
        return conf;
    }
    for (int i=1; i<argc; i++) { // i=1 para pular nome do programa
        string arg (argv[i]);
        if (arg == "--help") {
            conf.help = true;
        } else if (arg == "--ncheck") {
            // checar caso esteja faltando o <num> para nao gerar segfault
            // tentando acessar argv[i+1]
            if (i == argc - 1) {
                cout << "`--ncheck` espera argumento" << endl;
                conf.error = -1;
                return conf;
            }
            i++;
            conf.ncheck = stoi(string(argv[i]));
        } else {
            //conf.input_puzzle_file = arg;
            // checando caso usuario tenha passado argumentos "a mais"
            if (i < argc - 1)
                cout << "recebidos argumentos depois do <input_puzzle_file>. estes argumentos serao ignorados." << endl;
            return conf;
        }
    }
    return conf; // evitando warning do compilador
}

/* Trata corretamente os argumentos de linha de comando (5 pts);
    ** Usage: sudoku [<options>] [<input_puzzle_file>]
         Game options:
           --ncheck    <num> Number of checks per game. Default = 3.
           --help      Print this help text
*/

int main(int argc, char **argv) 
{
    Config config = make_config(argc, argv);
    if (config.error != 0) {
        if (config.error == -1) {
            cout << "`--ncheck` espera argumento." << endl;
            return -1;
        } else {
            cout << "config.erro acusa algum erro." << endl;
            return -1;
        }
    }
    cout << "config gerada" << endl << config.error << endl << config.help << endl << config.ncheck << endl << config.input_puzzle_file << endl;
    return 0;
}

