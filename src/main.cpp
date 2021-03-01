#include "gametable.h"
#include "gamestate.h"
#include "play.h"
#include "util.h"

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
const int         DEFAULT_NCHECK_NUM        = 3;
const string      DEFAULT_INPUT_PUZZLE_FILE ("input.txt");

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
 * Apresenta interativamente as puzzles no PuzzleTables e retorna o indice da puzzle
 * escolhida pelo usuario.
 *
 * @param   puzzletables  
 * @param   chosen_puzzle   -1 indica que nenhuma puzzle foi escolhida ainda  
 * @param   shown_puzzle    puzzle que vamos printar e oferecer ao usuario
 * @return  indice da puzzle selecionada
 */
int select_puzzle(PuzzleTables puzzletables, int chosen_puzzle, int shown_puzzle) {
    bool exit_flag = false;
    while (!exit_flag) {
        cin.clear();
        if (chosen_puzzle == -1) {
            cout << "Nenhuma puzzle foi escolhida ainda." << endl;
        } else {
            cout << "Puzzle " << chosen_puzzle << " escolhida." << endl;
        }
        cout << "Puzzle " << shown_puzzle << " exibida a seguir: " << endl;
        print_puzzletable(empty_table, puzzletables.tables[shown_puzzle]);
        cout << "0-" << puzzletables.tablecount - 1 << ": mostrar puzzle com esse indice" << endl;
        cout << "a: mostrar proxima puzzle " << "    " << "b: mostrar puzzle anterior" << endl;
        cout << "c: escolher puzzle exibida" << "    " << "d: voltar para menu inicial" << endl;
        cout << "0-" << puzzletables.tablecount - 1 << "abcd? ";
        string user_input{};
        getline(cin, user_input);
        if (user_input == "") {
            cout << "Nenhum opcao recebida." << endl;

        } else if (user_input.front() == 'a') {
            shown_puzzle++;
            // "dando a volta" na lista de puzzle
            if (shown_puzzle == puzzletables.tablecount)
                shown_puzzle = 0;
        } else if (user_input.front() == 'b') {
            shown_puzzle--;
            // "dando a volta" na lista de puzzle
            if (shown_puzzle == -1) {
                shown_puzzle = puzzletables.tablecount - 1;
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
 * @param   puzzletables  puzzletables gerada com gen_puzzletables()
 * @param   gamestate   
 */
void main_menu(Config config, PuzzleTables puzzletables, GameState* gamestate) {
    bool exit_flag = false;
    while (!exit_flag) {
        cout << "SUDOKU INTERATIVO" << endl;
        if (gamestate->chosen_puzzle == -1) {
            cout << "Nenhuma puzzle escolhida ainda! A opcao 1 te permite visualizar puzzles e escolher uma." << endl;
        } else {
            cout << "Puzzle " << gamestate->chosen_puzzle << " selecionada." << endl;
            print_puzzletable(empty_table, puzzletables.tables[gamestate->chosen_puzzle]);
        }
        cout << endl << endl;
        cout << "a - Visualizar puzzles e escolher uma" << endl;
        cout << "b - Jogar puzzle escolhido" << endl;
        cout << "c - Mostrar regras e ajuda" << endl;
        cout << "d - Sair do jogo" << endl;
        cout << "abcd? " << std::flush;
        char menu_escolha{};
        cin.clear();
        cin >> menu_escolha;
        switch (menu_escolha) {
            case 'a':
                cout << endl << endl;
                gamestate->chosen_puzzle = select_puzzle(puzzletables, gamestate->chosen_puzzle, 0);
                break;
            case 'b':
                play_puzzle(puzzletables.tables[gamestate->chosen_puzzle], &gamestate->playstate);
                break;
            case 'c':
                break;
            case 'd':
                exit_flag = true;
                break;
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
    PuzzleTables puzzletables = gen_puzzletables(config.input_puzzle_file);
    if (puzzletables.tablecount == 0) {
        cout << "Erro: `init_puzzletables` nao foi capaz de inicializar nenhuma puzzletable." << endl;
        return -1;
    }
    cout << "Info: " << puzzletables.tablecount << " tables gerados" << endl;
    PlayState playstate = default_playstate; 
    GameState gamestate { config, -1, puzzletables, playstate };
    main_menu(config, puzzletables, &gamestate);
    destroy_gamestate(gamestate);
    return 0;
}

