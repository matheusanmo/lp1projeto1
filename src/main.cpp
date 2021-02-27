#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;
using std::stoi;
using std::getline;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::stringstream;

const int    DEFAULT_NCHECK_NUM        = 3;
const string DEFAULT_INPUT_PUZZLE_FILE ("input.txt");

struct Config {
    int    error; // codigo de erro caso programa seja mal invocado. 0 = tudo ok
    bool   help;
    int    ncheck;
    string input_puzzle_file;
};

struct GameTable {
    int table[9][9];
};

struct GameTables {
    int  tablecount;
    GameTable* tables;
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
 * Destroi um `GameTables` deletando seu membro que eh ponteiro para lista de 
 * `GameTable`s.
 * 
 * @param   gametables  NAO recebe ponteiro mas sim a variavel em si
 */
void destroy_gametables(GameTables gametables) {
    delete[] gametables.tables;
    return;
}

/**
 * Recebe string com caminho de um arquivo a parseado como GameTables. Descarta
 * tables que nao consegue parsear e avisa, mas nao gera erros ou para a execucao do 
 * programa. Nao checa se o arquivo existe ou eh acessivel; isto deve ser feito
 * de antemao. Espera-se que os tabuleiros estejam no seguinte formato:
 * 9 inteiros, com ou sem sinal, separados por whitespace (fora \n) seguidos
 * por uma linha vazia (mesmo sendo o ultimo tabuleiro do arquivo). O arquivo
 * deve comecar imediatamente com a primeira fileira do primeiro tabuleiro, isto
 * eh, nada de whitespace ou comentarios antes.  Se o membro `gametables` do retorno 
 * eh nullptr, um erro ocorreu.
 * 
 * @param   input_puzzle_file   caminho do arquivo
 * @returns GameTables com tables que foram parseadas com sucesso
 */
GameTables gen_gametables(string input_puzzle_file) {
    GameTables gametables { 0, nullptr };
    int tablecount = 1;
    int linecount  = 1;
    ifstream input_stream (input_puzzle_file);
    string line;
    while (getline(input_stream, line)) {
        linecount++;
    }
    // caso tenhamos recebido input mal formatado, esta eh uma forma de descobrir
    // que tambem nos diz a quantidade de tabuleiros declarados.
    if (linecount % 10 != 0) {
        cout << "Erro: input_puzzle_file contem algum tabuleiro mal formatado (linhas a mais ou a menos). `linecount` = " << linecount << endl;
        return gametables;
    }
    tablecount            = linecount / 10; // 10 linhas por tabuleiro (9 de numero + linha vazia)
    // tables eh ponteiro para array de `tablecount` `GameTable`s vazios (?) 
    GameTable* tables     { new GameTable[tablecount]{} }; 
    gametables.tables     = tables; // LEMBRAR de deletar usando `delete[] tables`
    gametables.tablecount = tablecount;

    // resetando posicao no arquivo (nao consegui com `seekg` por alguma razao)
    input_stream.close();
    input_stream = ifstream(input_puzzle_file);
    int table_index{}, line_index{}, row_index{};
    for (table_index = 0; table_index < tablecount; table_index++) {
        for (line_index = 0; line_index < 9; line_index++) { 
            for (row_index = 0; row_index < 9; row_index++) {
                int readnumber = 0;
                input_stream >> readnumber;
                cout << readnumber << " ";
                tables[tablecount].table[line_index][row_index] = readnumber;
            }
            int throwaway{};
            //input_stream >> throwaway;
        }
    }
    //input_stream.close();
    return gametables;
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
    Config conf { 0, false, DEFAULT_NCHECK_NUM, DEFAULT_INPUT_PUZZLE_FILE };
    if (argc == 1) {
        // invocado sem argumentos
        return conf;
    }
    for (int i = 1; i < argc; i++) { // i=1 para pular nome do programa
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
    return conf; // fluxo nao deve chegar aqui mas evita warning do compilador
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

/**
 * printa na tela a gametable indentada com `offset` espacos antes do primeiro
 * numero (padrao 0).
 *
 * @params  gametable   tabela que sera printada
 * @params  offset      indentacao da table (padrao 0)
 */
void print_gametable(GameTable gametable, int offset = 0) {
    string padding (offset, ' ');
    for (int line_index = 0; line_index < 9; line_index++) {
        cout << padding;
        for (int row_index = 0; row_index < 9; row_index++) {
            cout << gametable.table[line_index][row_index] << " ";
        }
        cout << endl;
    }
    return;
}

int main(int argc, char **argv) 
{
    Config config = make_config(argc, argv);
    if (config.error != 0) {
        if (config.error == -1) {
            cout << "Erro: `--ncheck` espera argumento." << endl;
            return -1;
        }
        cout << "Erro: `config.erro` acusa erro " << config.error << "." << endl;
        return -1;
    }
    if (config.help) {
        print_help();
        return 0;
    }
    if (!file_exists(config.input_puzzle_file)) {
        cout << "Erro: nao eh possivel acessar o arquivo " << config.input_puzzle_file << endl;
        return -1;
    }
    GameTables gametables = gen_gametables(config.input_puzzle_file);
    if (gametables.tablecount == 0) {
        cout << "Erro: `init_gametables` nao foi capaz de inicializar nenhuma gametable." << endl;
        return -1;
    }
    print_gametable(gametables.tables[0]);
    destroy_gametables(gametables);
    cout << "config gerada" << endl << config.error << endl << config.help << endl << config.ncheck << endl << config.input_puzzle_file << endl;
    cout << gametables.tablecount << " tables gerados" << endl;
    return 0;
}

