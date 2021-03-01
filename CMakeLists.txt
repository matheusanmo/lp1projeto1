# cmake --version
cmake_minimum_required(VERSION 3.16.3)

# nome do projeto
project(Sudoku)

# exigir suporte C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# exibir mais warnings
add_compile_options(-Wall -Wextra)

# definir nome de executavel e arquivos que o constroem
add_executable(sudoku src/main.cpp src/util.cpp src/gametable.cpp src/play.cpp src/gamestate.cpp)

