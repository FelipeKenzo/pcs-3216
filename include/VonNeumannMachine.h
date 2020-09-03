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
#include <fstream>
#include <stack>

#include "MyUtils.h"

#define MEM_SIZE 4096 // 4KB

class VonNeumannMachine {
private:
    // Memory
    uint8_t mem[MEM_SIZE] = {0};
    
    // Registers
    uint8_t  ac = 0; // Accumulator
    uint16_t pc = 0; // Program Counter
    uint16_t of = 0; // Offset Register
    
    // Input/Output
    std::ifstream* input = NULL;
    std::ofstream* output = NULL;

    // Halt flags
    bool halted = false;

    // Breakpoints
    std::list<uint16_t> breakpoints;
    
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

    uint16_t readInput();
    void writeOutput();
    void executeInstruction(uint16_t instr, bool debug);

public:
    VonNeumannMachine(std::ifstream* input, std::ofstream* output);
    VonNeumannMachine();
    void setInput(std::ifstream* input);
    void setOutput(std::ofstream* output);
    void run();
    void step();
    void halt();
    void setPC(uint16_t data);
    void setAC(uint8_t data);
    uint16_t getPC();
    uint16_t getAC();
    uint16_t memRead_w(uint16_t addr);
    uint8_t  memRead_b(uint16_t addr);
    void memWrite_w(uint16_t addr, uint16_t data);
    void memWrite_b(uint16_t addr, uint16_t data);

    // Overlay Driver
    void loadOverlay(uint8_t block, uint16_t offset);
    void endOverlay();
    uint8_t savedAc;
    std::stack<uint16_t> returns;
    std::stack<uint16_t> offsets;
};

#endif