#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <string>
using std::string;

struct PuzzleTable {
    int table[9][9];
};

struct PuzzleTables {
    int  tablecount;
    PuzzleTable* tables;
};

const PuzzleTable empty_table{};

PuzzleTables gen_puzzletables(string input_puzzle_file);
void destroy_puzzletables(PuzzleTables puzzletables);
void print_puzzletable(PuzzleTable puzzletable, int offset = 3);

#endif

