#include "../include/VonNeumannMachine.h"

VonNeumannMachine::VonNeumannMachine() {
    breakpoints = new std::list<uint16_t>();
    reg[sp] = 0xFFE;
}

void VonNeumannMachine::executeInstruction(uint16_t instr, bool debug) {
    
    // Decode instruction
    uint16_t op = (instr >> 12);    // opcode
    uint16_t pa = (instr & 0x0FFF); // parameter

    if(debug) {
        std::cout << "---------------------------\n"
                  << "0x" << std::right << std::setfill('0') 
                  << std::setw(3) << std::hex << reg[pc]-2 << "  "
                  << std::setw(2) << std::hex << (uint16_t)mem[reg[pc] - 2] << " "
                  << std::setw(2) << std::hex << (uint16_t)mem[reg[pc] - 1] << "  ";
    }

    switch(op) {
        case JP:
            if (debug) std::cout << "JP  ";
            reg[pc] = pa;
            break;
        case JZ:
            if (debug) std::cout << "JZ  ";
            if (reg[ac] == 0) reg[pc] = pa;
            break;
        case JN:
            if (debug) std::cout << "JN  ";
            if (reg[ac < 0]) reg[pc] = pa;
            break;
        case LV:
            if (debug) std::cout << "LV  ";
            reg[ac]  = memRead(pa);
            break;
        case ADD:
            if (debug) std::cout << "ADD ";
            reg[ac] += memRead(pa);
            break;
        case SUB:
            if (debug) std::cout << "SUB ";
            reg[ac] -= memRead(pa);
            break;
        case MUL:
            if (debug) std::cout << "MUL ";
            reg[ac] *= memRead(pa);
            break;
        case DIV:
            if (debug) std::cout << "DIV ";
            reg[ac] /= memRead(pa);
            break;
        case LD:
            if (debug) std::cout << "LD  ";
            reg[ac]  = memRead(pa);
            break;
        case MM:
            if (debug) std::cout << "MM  ";
            memWrite_w(pa, reg[ac]);
            break;
        case SC:
            if (debug) std::cout << "SC  ";
            memWrite_w(reg[sp], reg[pc]);
            reg[sp] -= 2;
            reg[pc] = pa;
            break;
        case RS:
            if (debug) std::cout << "RS  ";
            reg[sp] += 2;
            // std::cout << std::hex << reg[sp] << "\n";
            // std::cout << std::hex << memRead(reg[sp]) << "\n";
            reg[pc] = memRead(reg[sp]);
            break;
        case HM:
            if (debug) std::cout << "HM  ";
            halt();
            reg[pc] = pa;
            break;
        case GD:
            if (debug) std::cout << "GD  ";
            reg[ac] = input();
            break;
        case PD:
            if (debug) std::cout << "PD  ";
            std::cout << std::internal << std::setfill('0');
            std::cout << "Accumulator: 0x" << std::uppercase
                        << std::setw(4) << std::hex << reg[ac]
                        << std::endl;
            break;
        case OS:
            if (debug) std::cout << "OS  ";
            std::cout << "System call not implemented.\n";
            break;
            
    }

    if (debug) {
        std::cout << std::right << std::setfill('0') << std::setw(3)
                  << std::hex << pa << "\n"
                  << "pc: 0x" << std::right << std::setfill('0') << std::setw(3)
                  << std::hex << reg[pc] << "\n"
                  << "sp: 0x" << std::right << std::setfill('0') << std::setw(3)
                  << std::hex << reg[sp] << "\n"
                  << "ac: 0x" << std::right << std::setfill('0') << std::setw(3)
                  << std::hex << reg[ac] << "\n"
                  << "---------------------------";
    }
}

void VonNeumannMachine::run(uint16_t addr) {
    halted = false;

    reg[pc] = addr;

    while (!halted) {
        // Fetch instruction
        uint16_t instr = memRead(reg[pc]);
        reg[pc] += 2;

       executeInstruction(instr, false); 
    }
}

void VonNeumannMachine::step(uint16_t addr) {
    halted = false;
    reg[pc] = addr;

    std::string input;

    do {
        // Fetch instruction
        uint16_t instr = memRead(reg[pc]);
        reg[pc] += 2;

        executeInstruction(instr, true);


    } while(std::getline(std::cin, input) && !halted && input != "halt");
}

void VonNeumannMachine::halt() {
    halted = true;
}

uint16_t VonNeumannMachine::memRead(uint16_t addr) {
    return *((uint16_t *)&mem[addr]);
}

void VonNeumannMachine::memWrite_w(uint16_t addr, uint16_t data) {
    mem[addr]   = data;
    mem[addr+1] = data>>8;
}

uint16_t* VonNeumannMachine::getRegisters() {
    return reg;
}

uint16_t VonNeumannMachine::input() {
    bool validInput = false;
    uint intNumber = 0;

    while (!validInput) {
        std::cout << "Input (hex starts with $): ";
        std::string input;
        std::cin >> input;

        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        
        if (isNumber(input)) {
            validInput = true;
            if (input[0] == '$') intNumber = htoi(input);
            else intNumber = std::stoi(input);
        }
    }

    return intNumber;
}