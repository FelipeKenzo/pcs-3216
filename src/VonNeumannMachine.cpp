#include "../include/VonNeumannMachine.h"

uint16_t VonNeumannMachine::input() {
    bool validInput = false;
    uint16_t intNumber = 0;

    while (!validInput) {
        std::cout << "Input (up to 4 Hex digits): ";
        std::string input;
        std::cin >> input;

        std::transform(input.begin(), input.end(), input.begin(), ::toupper);

        std::regex inputParser("([0-9A-F]){1,4}");
        std::smatch match;
        if (std::regex_search(input, match, inputParser)) {
            validInput = true;
            std::string asciiNumber = match[1];
            for (int i = asciiNumber.length(); i > 0; i--) {
                if (asciiNumber[i] > 0x29 && asciiNumber[i] < 0x3A) intNumber += (asciiNumber[i] - 0x30)*intPow16(i); // 0-9
                else if (asciiNumber[i] > 0x40 && asciiNumber[i] < 0x47) intNumber += (asciiNumber[i] - 0x37)*intPow16(i); //A-F
            }
        }
    }

    return intNumber;
}

int16_t VonNeumannMachine::intPow16 (int16_t exp) {
    int16_t x = 0;
    for (int i = 0; i < exp; i++) {
        x *= 16;
    }
    return x;
}

VonNeumannMachine::VonNeumannMachine() {
    breakpoints = new std::list<int16_t>();
    reg[sp] = 0xFFE;
}

void VonNeumannMachine::run() {
    halted = false;
    while (!halted) {
        // Fetch instruction
        int16_t instr = memRead(reg[pc]);
        reg[pc] += 2;

        // Decode instruction
        int16_t op = (instr >> 12);    // opcode
        int16_t pa = (instr & 0x0FFF); // parameter

        switch(op) {
            case JP:
                reg[pc] = pa;
                break;
            case JZ:
                if (reg[ac] == 0) reg[pc] = pa;
                break;
            case JN:
                if (reg[ac < 0]) reg[pc] = pa;
                break;
            case LV:
                reg[ac]  = memRead(pa);
                break;
            case ADD:
                reg[ac] += memRead(pa);
                break;
            case SUB:
                reg[ac] -= memRead(pa);
                break;
            case MUL:
                reg[ac] *= memRead(pa);
                break;
            case DIV:
                reg[ac] /= memRead(pa);
                break;
            case LD:
                reg[ac]  = memRead(pa);
                break;
            case MM:
                memWrite_w(pa, reg[ac]);
                break;
            case SC:
                memWrite_w(reg[sp], reg[pc]);
                reg[sp] -= 2;
                reg[pc] = pa;
                break;
            case RS:
                reg[sp] += 2;
                reg[pc] = memRead(reg[sp]);
                break;
            case HM:
                halt();
                reg[pc] = pa;
                break;
            case GD:
                reg[ac] = input();
                break;
            case PD:
                std::cout << std::internal << std::setfill('0');
                std::cout << "Accumulator: 0x" << std::uppercase
                          << std::setw(4) << std::hex << reg[ac]
                          << std::endl;
                break;
            case OS:
                std::cout << "System call not implemented.\n";
                break;
                
        }   

    }
}

void VonNeumannMachine::halt() {
    halted = true;
}

int16_t VonNeumannMachine::memRead(int16_t addr) {
    return *((int16_t *)&mem[addr]);
}

void VonNeumannMachine::memWrite_w(int16_t addr, int16_t data) {
    mem[addr]   = data;
    mem[addr+1] = data>>8;
}