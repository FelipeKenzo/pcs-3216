#include "../include/DataArea.h"

// Instructions
mnemonicData jp  = {0x0, instruction, 2, true};
mnemonicData jz  = {0x1, instruction, 2, true};
mnemonicData jne = {0x2, instruction, 2, true};
mnemonicData lv  = {0x3, instruction, 2, true};
mnemonicData add = {0x4, instruction, 2, true};
mnemonicData sub = {0x5, instruction, 2, true};
mnemonicData mul = {0x6, instruction, 2, true};
mnemonicData dv  = {0x7, instruction, 2, true};
mnemonicData ld  = {0x8, instruction, 2, true};
mnemonicData mm  = {0x9, instruction, 2, true};
mnemonicData sc  = {0xA, instruction, 2, true};
mnemonicData rs  = {0xB, instruction, 2, true};
mnemonicData hm  = {0xC, instruction, 2, true};
mnemonicData gd  = {0xD, instruction, 2, true};
mnemonicData pd  = {0xE, instruction, 2, true};
mnemonicData os  = {0xF, instruction, 2, true};

// Pseudoinstructions
mnemonicData org = {16, pseudo, 0, true };
mnemonicData end = {16, pseudo, 0, false};
mnemonicData con = {16, pseudo, 2, true };

// Mnemonic Table
std::map<std::string, mnemonicData> mTable = {
    {"JP" ,  jp}, {"JZ" ,  jz}, {"JN" , jne}, {"LV" ,  lv},
    {"ADD", add}, {"SUB", sub}, {"MUL", mul}, {"DIV",  dv},
    {"LD" ,  ld}, {"MM" ,  mm}, {"SC" ,  sc}, {"RS" ,  jp},
    {"HM" ,  hm}, {"GD" ,  gd}, {"PD" ,  pd}, {"OS" ,  os},
    {"ORG", org}, {"END", end}, {"CON", con}
};

// Symbol Table
std::map<std::string, symbolData> sTable;