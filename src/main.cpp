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

// TODO const nos paramtros de funcoes
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
        return gametables;
    }
    tablecount = numcount / 81;
    // tables eh ponteiro para array de `tablecount` `GameTable`s vazios (?) 
    GameTable* tables     { new GameTable[tablecount]{} }; 
    gametables.tables     = tables; // LEMBRAR de deletar usando `delete[] tables`
    gametables.tablecount = tablecount;

    int table_index{}, line_index{}, row_index{};
    for (table_index = 0; table_index < tablecount; table_index++) {
        for (line_index = 0; line_index < 9; line_index++) { 
            for (row_index = 0; row_index < 9; row_index++) {
                input_stream >> tables[table_index].table[line_index][row_index];
            }
        }
    }
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
                         << "usando valor padrao " << DEFAULT_NCHECK_NUM << "." << endl; 
                }
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
void print_gametable(GameTable gametable, int offset = 3) {
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
            if (gametable.table[line_index][row_index] < 0) {
                cout << " ";
            } else {
                cout << gametable.table[line_index][row_index];
            }
        }
        cout << "|" << endl;
    }
    cout << padding << "+---+---+---+" << endl;
    return;
}


struct GameState {
    int chosen_puzzle; // -1 indica que nenhuma puzzle foi selecionada ainda
};

/**
 * Apresenta interativamente as puzzles no GameTables e retorna o indice da puzzle
 * escolhida pelo usuario.
 *
 * @param   gametables  
 * @param   chosen_puzzle   -1 indica que nenhuma puzzle foi escolhida ainda  
 * @param   shown_puzzle    puzzle que vamos printar e oferecer ao usuario
 * @return  indice da puzzle selecionada
 */
int select_puzzle(GameTables gametables, int chosen_puzzle, int shown_puzzle) {
    bool exit_flag = false;
    while (!exit_flag) {
        cin.clear();
        if (chosen_puzzle == -1) {
            cout << "Nenhuma puzzle foi escolhida ainda." << endl;
        } else {
            cout << "Puzzle " << chosen_puzzle << " escolhida." << endl;
        }
        cout << "Puzzle " << shown_puzzle << " exibida a seguir: " << endl;
        print_gametable(gametables.tables[shown_puzzle]);
        cout << "0-" << gametables.tablecount - 1 << ": mostrar puzzle com esse indice" << endl;
        cout << "a: mostrar proxima puzzle" << endl;
        cout << "b: mostrar puzzle anterior" << endl;
        cout << "c: escolher puzzle exibida" << endl;
        cout << "d: voltar para menu inicial" << endl;
        cout << "0-" << gametables.tablecount - 1 << "abcd? ";
        string user_input{};
        getline(cin, user_input);
        if (user_input == "") {
            cout << "Nenhum opcao recebida." << endl;

        } else if (user_input.front() == 'a') {
            shown_puzzle++;
            // "dando a volta" na lista de puzzle
            if (shown_puzzle == gametables.tablecount)
                shown_puzzle = 0;
        } else if (user_input.front() == 'b') {
            shown_puzzle--;
            // "dando a volta" na lista de puzzle
            if (shown_puzzle == -1) {
                shown_puzzle = gametables.tablecount - 1;
            }
        } else if (user_input.front() == 'c') {
            chosen_puzzle = shown_puzzle;
        } else if (user_input.front() == 'd') {
            return chosen_puzzle;
        } else {
            try {
                shown_puzzle = stoi(user_input);
            }
            catch (const std::invalid_argument& ia) {
                cout << "Esta opcao nao era esperada." << endl;
            }
        }
        cout << endl;
    }
    return chosen_puzzle;
}

/**
 * Entra no fluxo principal do jogo. Oferece o menu principal e nao devolve controle
 * para o main() ate receber ordem para sair do jogo.
 *
 * @param   config      configuracao gerada com make_config()
 * @param   gametables  gametables gerada com gen_gametables()
 * @param   gamestate   
 */
void enter_main_menu(Config config, GameTables gametables, GameState gamestate) {
    bool exit_flag = false;
    while (!exit_flag) {
        cout << "SUDOKU INTERATIVO" << endl;
        if (gamestate.chosen_puzzle == -1) {
            cout << "Nenhuma puzzle escolhida ainda! A opcao 1 te permite visualizar puzzles e escolher uma." << endl;
        } else {
            cout << "Puzzle " << gamestate.chosen_puzzle << " selecionada." << endl;
            print_gametable(gametables.tables[gamestate.chosen_puzzle]);
        }
        cout << endl << endl;
        cout << "a - Visualizar puzzles e escolher uma" << endl;
        cout << "b - Jogar puzzle escolhido" << endl;
        cout << "c - Mostrar regras e ajuda" << endl;
        cout << "d - Sair do jogo" << endl;
        cout << "abcd?  " << std::flush;
        char menu_escolha{};
        cin.clear();
        cin >> menu_escolha;
        switch (menu_escolha) {
            case 'a':
                cout << endl << endl;
                gamestate.chosen_puzzle = select_puzzle(gametables, gamestate.chosen_puzzle, 0);
                break;
            case 'b':
                break;
            case 'c':
                break;
            case 'd':
                exit_flag = true;
            default:
                cout << "Opcao nao reconhecida!" << endl;
                break;
        }
    }
    cout << "Saindo do programa." << endl;
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
    cout << "Info: " << gametables.tablecount << " tables gerados" << endl;
    GameState gamestate { -1 };
    enter_main_menu(config, gametables, gamestate);
    destroy_gametables(gametables);
    return 0;
}

