#include "../include/VonNeumannMachine.h"

VonNeumannMachine::VonNeumannMachine() {
    breakpoints = new std::list<uint16_t>();
    reg[sp] = 0xFFE;
}

void VonNeumannMachine::executeInstruction(uint16_t instr, bool debug) {
    
    // Decode instruction
    uint16_t op = (instr >> 12);    // opcode
    uint16_t pa = (instr & 0x0FFF); // parameter

    switch(op) {
        case JP:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "JP   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            reg[pc] = pa;
            break;
        case JZ:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "JZ   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            if (reg[ac] == 0) reg[pc] = pa;
            break;
        case JN:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "JN   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            if (reg[ac < 0]) reg[pc] = pa;
            break;
        case LV:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "LV   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            reg[ac]  = memRead(pa);
            break;
        case ADD:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "ADD  " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            reg[ac] += memRead(pa);
            break;
        case SUB:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "SUB  " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            reg[ac] -= memRead(pa);
            break;
        case MUL:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "MUL  " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            reg[ac] *= memRead(pa);
            break;
        case DIV:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "DIV  " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            reg[ac] /= memRead(pa);
            break;
        case LD:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "LD   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            reg[ac]  = memRead(pa);
            break;
        case MM:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "MM   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            memWrite_w(pa, reg[ac]);
            break;
        case SC:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "SC   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            memWrite_w(reg[sp], reg[pc]);
            reg[sp] -= 2;
            reg[pc] = pa;
            break;
        case RS:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "RS   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            reg[sp] += 2;
            reg[pc] = memRead(reg[sp]);
            break;
        case HM:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "HM   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            halt();
            reg[pc] = pa;
            break;
        case GD:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "GD   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            reg[ac] = input();
            break;
        case PD:
            if (debug) {
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::right << std::setfill('0') << std::setw(4) << std::hex << reg[pc]-2 << "  "
                          << "PD   "    << std::right << std::setfill('0') << std::setw(4) << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            std::cout << std::internal << std::setfill('0');
            std::cout << "Accumulator: 0x" << std::uppercase
                        << std::setw(4) << std::hex << reg[ac]
                        << std::endl;
            break;
        case OS:
            if (debug) {
                std::cout << std::right << std::setfill('0') << std::setw(4);
                std::cout << "---------------------------\n"
                          << "addr: 0x" << std::hex << reg[pc]-2 << "  "
                          << "OS   " << std::hex << pa << "\n"
                          << "---------------------------\n";
            }
            std::cout << "System call not implemented.\n";
            break;
            
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

        std::cin >> input;

    } while (!halted && input != "halt");
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