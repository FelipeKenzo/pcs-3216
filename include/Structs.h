#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>

#include "Enums.h"

struct token {
    std::string sequence;
    tokenType type;
    int line;
};

struct transition {
    int states[2];
    token t;
};

struct treatment {
    int routine;
    token t;
};

#endif