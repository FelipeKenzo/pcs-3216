#include "../include/VonNeumannMachine.h"

VonNeumannMachine::VonNeumannMachine(std::ifstream* input, std::ofstream* output) : 
    input(input), output(output){
}

VonNeumannMachine::VonNeumannMachine() {
}

void VonNeumannMachine::setInput(std::ifstream* input) {
    this->input = input;
}

void VonNeumannMachine::setOutput(std::ofstream* output) {
    this->output = output;
}

void VonNeumannMachine::executeInstruction(uint16_t instr, bool debug) {
    
    // Decode instruction
    uint16_t op = (instr >> 12);    // opcode
    uint16_t pa = (instr & 0x0FFF); // parameter

    if(debug) {
        std::cout << "---------------------------\n"
                  << "$" << std::right << std::setfill('0') 
                  << std::setw(3) << std::hex << pc + of -2 << "  "
                  << std::setw(2) << std::hex << (uint16_t)mem[pc + of - 2] << " "
                  << std::setw(2) << std::hex << (uint16_t)mem[pc + of - 1] << "        ";
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
            if (pa == 0) { // Load Overlay
                loadOverlay(memRead_b(pc+of), memRead_w(pc+of+1));
            }
            else if (pa == 1) {
                endOverlay();
            }
            else std::cout << "System call not implemented.\n";
            break;
            
    }

    if (debug) {
        std::cout << std::right << std::setfill('0') << std::setw(3) << std::uppercase
                  << std::hex << pa << "\n\n"
                  << "ac: $" << std::right << std::setfill('0') << std::setw(2)
                  << std::hex << (uint16_t)ac << "  "
                  << "pc: $" << std::right << std::setfill('0') << std::setw(3)
                  << std::hex << pc << "  "
                  << "of: $" << std::right << std::setfill('0') << std::setw(3)
                  << std::hex << of << "\n"
                  << "---------------------------\n";
    }

}

void VonNeumannMachine::run() {
    halted = false;
    of = 0;

    while (!halted) {
        if (pc+of >= 0xFFF) {
            std::cerr << "run: invalid memory address (" << std::uppercase << std::hex << pc << ").\n";
            return;
        }

        // Fetch instruction
        uint16_t instr = memRead_w(pc+of);
        pc += 2;

        executeInstruction(instr, false); 
    }

}

void VonNeumannMachine::step() {
    halted = false;

    std::string input;

    do {
        if (pc+of >= 0xFFF) {
            std::cerr << "step: invalid memory address (" << std::uppercase << std::hex << pc << ").\n";
            return;
        }

        // Fetch instruction
        uint16_t instr = memRead_w(pc+of);
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

    if (input == NULL) {
        std::cerr << "VNM: Error: no input file. Halting machine.\n";
        halt();
        return 0;
    }

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
    if (output == NULL) {
        std::cerr << "VNM: Error: no output file. Halting machine.\n";
        halt();
        return;
    }

    uint16_t ac16 = (uint16_t)ac;
    (*output) << std::right << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (uint)ac << "\n";
    (*output).flush();
}

void VonNeumannMachine::loadOverlay(uint8_t block, uint16_t offset) {

    GotoLine(input, block); // Search for overlay block code

    // Create aux file with corrected addresses
    std::ofstream auxOut("auxFile");
    while (true) {
        // read address
        uint16_t upper = readInput();
        if (upper == 0xFF) {
            auxOut << "FF";
            break;
        }
        uint16_t lower = readInput();

        uint16_t address = (upper << 8) + lower;
        
        // correct address
        address += offset + of;

        // output corrected address to aux file
        auxOut << std::right << std::setfill('0') <<  std::uppercase
               << std::setw(4) << std::hex  << address;
        
        // read size
        uint16_t size = readInput();
        auxOut << std::right << std::setfill('0') <<  std::uppercase
               << std::setw(2) << std::hex  << size;

        // copy data and checksum to aux
        std::string data = "";
        for (int i = 0; i < (size+1)*2; i++) {
            char c = (*input).get();
            data.push_back(c);
        }
        auxOut << data;
    }
    auxOut.close();

    // Change input file to auxFile
    std::ifstream* savedInput = input;
    std::ifstream auxIn("auxFile");
    input = &auxIn;
    
    savedAc = ac;
    returns.push(pc + 3);
    offsets.push(offset);

    // std::cout << "\nLoading overlay " << (uint)block << "\n";
    // std::cout << "Offset: " << offset << "\n";
    pc = 0; // loader
    uint16_t savedOf = of;
    this->run();  // run loader
    
    pc = 0;
    of = offset + savedOf;
    halted = false;
    
    auxIn.close();
    input = savedInput;
    // std::cout << "\nOverlay loaded\n";
}

void VonNeumannMachine::endOverlay() {
    // Remove PC offset
    of = of - offsets.top();
    offsets.pop();

    // Return to father node
    pc = returns.top();
    returns.pop();
}