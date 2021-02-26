#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;
using std::stoi;

#include <fstream>
using std::ifstream;

const int    DEFAULT_NCHECK_NUM        = 3;
const string DEFAULT_INPUT_PUZZLE_FILE ("input.txt");

struct Config {
    int error; // codigo de erro caso programa seja mal invocado. 0 = tudo ok
    bool help;
    int ncheck;
    string input_puzzle_file;
};

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
                conf.error = -1;
                return conf;
            }
            // caminhando por argv para pegar parametro de ncheck
            i++;
                try {
                    conf.ncheck = stoi(string(argv[i])); }
                catch (const std::invalid_argument& ia) {
                    cout << "Aviso:`--ncheck` nao recebeu argumento valido (deve ser numero inteiro). "
                         << "usando valor padrao " << DEFAULT_NCHECK_NUM << "." << endl; }
        } else {
            conf.input_puzzle_file = arg;
            // checando caso usuario tenha passado argumentos "a mais"
            if (i < argc - 1)
                cout << "Aviso: recebidos argumentos depois do <input_puzzle_file>. estes argumentos serao ignorados." << endl;
            return conf;
        }
    }
    return conf; // evitando warning do compilador
}

/**
 * printa ajuda no stdout
 */
void print_help() {
    cout << "Usage: sudoku [<options>] [<input_puzzle_file>]\n"
         << "  Game options:\n"
         << "    --ncheck    <num> Number of checks per game. Default = 3.\n"
         << "    --help      Print this help text."
         << endl;
    return;
}

int main(int argc, char **argv) 
{
    Config config = make_config(argc, argv);
    // lidando com erros avisados pela config
    if (config.error != 0) {
        if (config.error == -1) {
            cout << "Erro: `--ncheck` espera argumento." << endl;
            return -1;
        } else {
            cout << "Erro: `config.erro` acusa erro " << config.error << "." << endl;
            return -1;
        }
    }
    if (config.help) {
        print_help();
        return 0;
    }
    // carregando arquivo com puzzles e saindo caso nao consiga achar o arquivo
    // de entrada ou nao consiga parsear os puzzles
    if (!file_exists(config.input_puzzle_file)) {
        cout << "Erro: nao eh possivel acessar o arquivo " << config.input_puzzle_file << endl;
        return -1;
    }
    cout << "config gerada" << endl << config.error << endl << config.help << endl << config.ncheck << endl << config.input_puzzle_file << endl;
    return 0;
}

