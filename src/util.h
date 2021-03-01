#ifndef UTIL_H
#define UTIL_H

#include <string>
using std::string;

enum Color {
    RED =  31 ,
    GREEN =  32 ,
    YELLOW =  33 ,
    BLUE =  34 ,
    MAGENTA =  35 ,
    CYAN =  36 ,
    WHITE =  37 ,
    BRIGHT_RED =  91 ,
    BRIGHT_GREEN =  92 ,
    BRIGHT_YELLOW =  93 ,
    BRIGHT_BLUE =  94 ,
    BRIGHT_MAGENTA =  95 ,
    BRIGHT_CYAN =  96 ,
    BRIGHT_WHITE =  97
};

enum Modificator {
    REGULAR =  0 ,
    BOLD =  1 ,
    UNDERLINE =  4 ,
    BLINK =  5 ,
    REVERSE =  7
};

bool file_exists(string filepath);
bool even(int n);
bool odd(int n);
string tcolor(const string msg, Color color=WHITE, Modificator modificator=REGULAR);

#endif

