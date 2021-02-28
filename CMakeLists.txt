cmake_minimum_required(VERSION 3.16.3)

# set the project name
project(Sudoku)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(sudoku src/main.cpp src/util.cpp src/gametable.cpp)

