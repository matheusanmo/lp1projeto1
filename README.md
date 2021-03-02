# SUDOKU

`sudoku` é um programa em C++ que lê puzzles de sudoku e permite ao usuário jogar estes puzzles.

## Compilação
`sudoku` foi desenvolvido e testado em uma máquina Ubuntu 20.04. Compilar `sudoku` depende do CMake e de um compilador C++ com suporte a C++11. Segue o processo de compilação do programa:
- Crie um diretório para o CMake trabalhar usando o comando `mkdir build`.
- Vá para o diretório criado usando o comando `cd build`.
- Configure o projeto e prepare o Makefile com o comando `cmake ../`.
- Construa o programa com o comando `cmake --build .`.
- Se tudo deu certo, invoque a ajuda do programa usando `./sudoku --help`.

## Utilização
`sudoku --help` imprime a ajuda do programa.

## Pontos implementados
Os seguintes pontos listados na descrição do projeto foram implementados:
- Trata corretamente os argumentos de linha de comando.
- Lê e armazena os vários puzzles contidos em um arquivo de entrada, no formato indicado na seção 4.
- Exibe corretamente a tela principal com o menu de ações.
- Permite a escolha dos vários puzzles lidos do arquivo.
- Executa corretamente a ação de posicionamento de dígitos no tabuleiro (comando posicionar) com a identificação de jogadas inválidas.
- Executa corretamente a remoção de digitos posicionados pelo jogador (comando remover).
- Executa corretamente o comando de desfazer ação.
- Apresenta as indicações com ‘setas‘ da coordenada da célula que foi alvo da última ação (5 pts).
