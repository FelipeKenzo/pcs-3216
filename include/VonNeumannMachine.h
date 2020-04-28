#ifndef VonNeumannMachine_H
#define VonNeumannMachine_H

#include <stdint.h>   // for fixed size integer types
#include <iostream>   // for cin and cout
#include <string>     // strings
#include <iomanip>    // stream formatting
#include <list>       // breakpoints list
#include <regex>      // for input parsing (valid inputs)
#include <locale>     // for input parsing (uppercase)
#include <algorithm>  // for input parsing (transform)

#define MEM_SIZE 4096 // 4KB

class VonNeumannMachine {
private:
    // Memory
    int8_t mem[MEM_SIZE] = {0}; // note: Little Endian
    
    // Registers
    int16_t reg[2] = {0};
    enum {
        ac = 0, // Accumulator
        pc,     // Program Counter
        sp
    };
    
    // Flags
    bool halted = false;


    // Breakpoints
    std::list<int16_t>* breakpoints;
    
    // Opcodes
    enum {
        JP = 0, // JUMP UNCONDITIONAL
        JZ,     // JUMP IF ZERO
        JN,     // JUMP IF NEGATIVE
        LV,     // LOAD VALUE
        ADD,    // ADD
        SUB,    // SUBTRACT
        MUL,    // MULTIPLY
        DIV,    // DIVIDE
        LD,     // LOAD FROM MEMORY
        MM,     // MOVE TO MEMORY
        SC,     // SUBROUTINE CALL
        RS,     // RETURN FROM SUBROUTINE
        HM,     // HALT MACHINE
        GD,     // GET DATA
        PD,     // PUT DATA
        OS      // OPERATING SYSTEM CALL
    };

    // Utilities
    uint16_t input();
    int16_t intPow16(int16_t exp);

public:
    VonNeumannMachine();
    void run();
    void halt();
    int16_t memRead(int16_t addr);
    void memWrite_w(int16_t addr, int16_t data);
};

#endif