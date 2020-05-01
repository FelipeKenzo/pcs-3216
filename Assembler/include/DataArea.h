#ifndef DATAAREA_H
#define DATAAREA_H

#include <cstdint>
#include <map>

enum type {
    instruction = 0,
    pseudo
};

struct mnemonicData {
    uint8_t opcode; // Opcode value
    type t;         // Type of mnemonic
    uint8_t size;   // Size in memory
    bool param;     // If requires parameter
};

struct symbolData {
    uint16_t address;
    uint16_t line;
    bool isDefined;
    bool isReferenced;
};


#endif