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

#include "../include/MyUtils.h"

#define MEM_SIZE 4096 // 4KB

class VonNeumannMachine {
private:
    // Memory
    uint8_t mem[MEM_SIZE] = {0}; // note: Little Endian
    
    // Registers
    enum {
        ac = 0, // Accumulator
        pc,     // Program Counter
        sp,
        reg_num
    };
    
    uint16_t reg[reg_num] = {0};
    
    // Halt flags
    bool halted = false;

    // Breakpoints
    std::list<uint16_t>* breakpoints;
    
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

    uint16_t input();
    void executeInstruction(uint16_t instr, bool debug);

public:
    VonNeumannMachine();
    void run(uint16_t addr);
    void step(uint16_t addr);
    void halt();
    uint16_t* getRegisters();
    uint16_t memRead(uint16_t addr);
    void memWrite_w(uint16_t addr, uint16_t data);
};

#endif