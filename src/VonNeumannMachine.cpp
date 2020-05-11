#include "../include/VonNeumannMachine.h"

VonNeumannMachine::VonNeumannMachine(std::ifstream* input, std::ofstream* output) : 
    input(input), output(output){
}

void VonNeumannMachine::executeInstruction(uint16_t instr, bool debug) {
    
    // Decode instruction
    uint16_t op = (instr >> 12);    // opcode
    uint16_t pa = (instr & 0x0FFF); // parameter

    if(debug) {
        std::cout << "---------------------------\n"
                  << "$" << std::right << std::setfill('0') 
                  << std::setw(3) << std::hex << pc-2 << "  "
                  << std::setw(2) << std::hex << (uint16_t)mem[pc - 2] << " "
                  << std::setw(2) << std::hex << (uint16_t)mem[pc - 1] << "        ";
    }

    switch(op) {
        case JP:
            if (debug) std::cout << "JP  ";
            pc = pa;
            break;
        case JZ:
            if (debug) std::cout << "JZ  ";
            if (ac == 0) pc = pa;
            break;
        case JN:
            if (debug) std::cout << "JN  ";
            if (ac >> 7) pc = pa;
            break;
        case LV:
            if (debug) std::cout << "LV  ";
            ac = pa;
            break;
        case ADD:
            if (debug) std::cout << "ADD ";
            ac += memRead_b(pa);
            break;
        case SUB:
            if (debug) std::cout << "SUB ";
            ac -= memRead_b(pa);
            break;
        case MUL:
            if (debug) std::cout << "MUL ";
            ac *= memRead_b(pa);
            break;
        case DIV:
            if (debug) std::cout << "DIV ";
            ac /= memRead_b(pa);
            break;
        case LD:
            if (debug) std::cout << "LD  ";
            ac  = memRead_b(pa);
            break;
        case MM:
            if (debug) std::cout << "MM  ";
            memWrite_b(pa, ac);
            break;
        case SC:
            if (debug) std::cout << "SC  ";
            memWrite_w(pa, pc);
            pc = pa + 2;
            break;
        case RS:
            if (debug) std::cout << "RS  ";
            pc = pa;
            break;
        case HM:
            if (debug) std::cout << "HM  ";
            halt();
            pc = pa;
            break;
        case GD:
            if (debug) std::cout << "GD  ";
            ac = readInput();
            break;
        case PD:
            if (debug) std::cout << "PD  ";
            writeOutput();
            break;
        case OS:
            if (debug) std::cout << "OS  ";
            std::cout << "System call not implemented.\n";
            break;
            
    }

    if (debug) {
        std::cout << std::right << std::setfill('0') << std::setw(3) << std::uppercase
                  << std::hex << pa << "\n\n"
                  << "ac: $" << std::right << std::setfill('0') << std::setw(2)
                  << std::hex << (uint16_t)ac << "    "
                  << "pc: $" << std::right << std::setfill('0') << std::setw(3)
                  << std::hex << pc << "\n"
                  << "---------------------------\n";
    }

}

void VonNeumannMachine::run() {
    halted = false;

    while (!halted) {
        if (pc >= 0xFFF) {
            std::cerr << "run: invalid memory address (" << std::uppercase << std::hex << pc << ").\n";
            return;
        }

        // Fetch instruction
        uint16_t instr = memRead_w(pc);
        pc += 2;

        executeInstruction(instr, false); 
    }

}

void VonNeumannMachine::step() {
    halted = false;

    std::string input;

    do {
        if (pc >= 0xFFF) {
            std::cerr << "step: invalid memory address (" << std::uppercase << std::hex << pc << ").\n";
            return;
        }

        // Fetch instruction
        uint16_t instr = memRead_w(pc);
        pc += 2;

        executeInstruction(instr, true);


    } while(std::getline(std::cin, input) && !halted && input != "halt");
}

void VonNeumannMachine::halt() {
    halted = true;
}

void VonNeumannMachine::setPC(uint16_t data) {
    pc = data;
}

void VonNeumannMachine::setAC(uint8_t data) {
    ac = data;
}

uint16_t VonNeumannMachine::getPC() {
    return pc;
}

uint16_t VonNeumannMachine::getAC() {
    return (uint16_t)ac;
}

uint16_t VonNeumannMachine::memRead_w(uint16_t addr) {
    return ((uint16_t)mem[addr] << 8) | (uint16_t)mem[addr+1];
}

uint8_t VonNeumannMachine::memRead_b(uint16_t addr) {
    return mem[addr];
}

void VonNeumannMachine::memWrite_w(uint16_t addr, uint16_t data) {
    mem[addr+1]  = data;
    mem[addr]    = data>>8;
}

void VonNeumannMachine::memWrite_b(uint16_t addr, uint16_t data) {
    mem[addr] = data;
}

uint16_t VonNeumannMachine::readInput() {

    char firstNibble = (*input).get();
    char secondNibble = (*input).get();

    std::string byte = "";

    byte.push_back(firstNibble);
    byte.push_back(secondNibble);

    uint intByte;
    
    if (isHex(byte)) {
        intByte = htoi(byte);
        return intByte;
    }
    else {
        return 0;
    }
}

void VonNeumannMachine::writeOutput() {
    (*output) << std::setfill('0') << std::setw(2)
              << std::right << std::uppercase << std::hex << (uint)ac;
    (*output).flush();
}