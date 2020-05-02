#include "../include/Interface.h"

Interface::Interface() {
    vnm = new VonNeumannMachine();
    std::cout << "__     __          _   _                                         __     ____  __ \n"
              << "\\ \\   / /__  _ __ | \\ | | ___ _   _ _ __ ___   __ _ _ __  _ __   \\ \\   / /  \\/  |\n"
              << " \\ \\ / / _ \\| '_ \\|  \\| |/ _ \\ | | | '_ ` _ \\ / _` | '_ \\| '_ \\   \\ \\ / /| |\\/| |\n"
              << "  \\ V / (_) | | | | |\\  |  __/ |_| | | | | | | (_| | | | | | | |   \\ V / | |  | |\n"
              << "   \\_/ \\___/|_| |_|_| \\_|\\___|\\__,_|_| |_| |_|\\__,_|_| |_|_| |_|    \\_/  |_|  |_|\n"
              << "\n"                                                                    
              << "                                                                     --Ver. 1.0--\n\n"
              << "Type 'help' for available commands.\n\n";
}

void Interface::input() {

    std::map<std::string, command>::iterator i;
    std::string input;
    std::vector<std::string> argv;
    int argc;

    while(1) {
        
        std::cout << "~ $ ";
        std::getline(std::cin, input);

        argv = split(input, ' ');
        argc = argv.size();

        if (argc < 1) continue;
        i = commandMap.find(argv[0]);

        switch(i->second) {
            
            case exit:
                exitMessage();
                return;
            
            case assemble: {
                std::string out = "./filesystem/a";
                std::string src = "./filesystem/";
                bool warnings;
                if (argc > 1) {
                    std::string arg; 
                    for (int i = 1; i < argc; i++) {
                        arg = argv[i];
                        if (arg == "-o") {
                            out = "./filesystem/" + argv[++i];
                        }
                        else if (arg == "-w" || arg == "--warnings") {
                            warnings = true;
                        }
                        else {
                            src += argv[i];
                        }
                    }
                }
                else {
                    std::cout << "assemble [source]* [-w|--warning] [-o outname].\n";
                    break;
                }

                assembleSrc(src, out, warnings);
                break;
            }
            
            case help:
                helpMessage();
                break;

            case ls:
                listFiles();
                break;
            
            case rm:
                if (argc > 1) {
                    argv.erase(argv.begin());

                    removeFiles(argv);
                }
                else {
                    std::cerr << "turn [file(1)]* ... [file(n)]\n";
                }
                break;
            
            case status:
                showVnmStatus();
                break;
            
            case turn:
                if (argc == 2) {
                    if (argv[1] == "on") vnmTurnOn();
                    else if (argv[1] == "off") vnmTurnOff();
                }
                else {
                    std::cerr << "turn [on|off]*\n";
                }
                break;

            case load:
                if (argc == 2) {
                    loadProgram("./filesystem/" + argv[1]);
                }
                else {
                    std::cerr << "load [program]*\n";
                }
                break;

            case run:
                if (argc == 2) {
                    runVnm(argv[1]);
                }
                else {
                    std::cerr << "run [startAddress]*\n";
                }
                break;

            case step:
                if (argc == 2) {
                    stepVnm(argv[1]);
                }
                else {
                    std::cerr << "step [startAddress]*\n";
                }
                break;

            case print:
                if (argc == 2) {
                    printFile("./filesystem/" + argv[1]);
                }
                else {
                    std::cerr << "print [file]\n";
                }
                break;
            
            default:
                std::cerr << "\"" << argv[0] << "\" is not a valid command.\n";
                break;
        }

    }
}

void Interface::assembleSrc(std::string src, std::string out, bool w) {

    Assembler* a = new Assembler(src, out, w);
    a->assemble();
    delete a;
}

void Interface::helpMessage() {
    std::cout << "Note: parameters with '*' are mandatory.\n\n"
              << "assemble: assembles a .asm source file into a .vnc hex file and .lst list file.\n"
              << "usage: assemble [source]* [-w|--warning] [-o outname]\n"
              << "      [source]*        source file name (with extension)\n"
              << "      [-w | --warning] show warning messages\n"
              << "      [-o][outname]    output name (without extension)\n\n"
              << "exit: exits program.\n\n" 
              << "help: displays this help message.\n\n"
              << "load: loads a program into the VNM.\n"
              << "usage: load [program]\n"
              << "      [program]*       program file name (with extension)\n\n"
              << "ls: list files in the filesystem.\n\n"
              << "print: prints the contents of a file to the terminal.\n"
              << "usage: print [file]*\n"
              << "      [file]*          file name (with extension)\n\n"
              << "rm: removes (deletes) file(s) from the filesystem.\n"
              << "usage: rm [file(1)]* ... [file(n)]\n"
              << "      [file(n)]*       file names to be deleted.\n\n"
              << "run: runs the VM at the the memory location specified.\n"
              << "usage: run [startAddress]*\n"
              << "      [startAddress]   start address for the simulation. Must be lower than $FFE.\n"
              << "                       can be in etiher decimal or hexadecimal (starting with '$').\n\n"
              << "status: shows the current status of the Von Neumann Machine.\n\n"
              << "step: steps throught the execution of the Von Neumann Machine. Typing 'halt' aborts.\n"
              << "      [startAddress]   start address for the simulation. Must be lower than $FFE.\n"
              << "                       can be in etiher decimal or hexadecimal (starting with '$').\n\n"
              << "turn: turns the Von Neumann Machine ON or OFF.\n"
              << "usage: turn [ON|OFF]*\n"
              << "      [ON|OFF]         state for the Machine. Must be either one of the options.\n\n";
}

void Interface::exitMessage() {
    std::cout << "\nExiting program...\n";
    return;
}

void Interface::listFiles() {
    std::string files = std::filesystem::current_path();
    files += "/filesystem/";
    std::string fileName;
    for (const auto & entry : std::filesystem::directory_iterator(files)) {
        fileName = entry.path();
        fileName = eraseSubStr(fileName, files);
        std::cout << fileName << "\n";
    }
    return;
}

void Interface::removeFiles(std::vector<std::string> files) {
    for (int i = 0; i < files.size(); i++) {
        std::string filename = "./filesystem/" + files[i];
        // char cfilename[filename.length()];
        // std::strcpy(cfilename, filename.c_str)
        if (std::remove(filename.c_str())) {
            std::cerr << "Cannot not remove \"" << files[i] << "\": No such file\n";
        }
    }
    return;
}

void Interface::vnmTurnOn() {
    if (vnm == NULL) {
        vnm = new VonNeumannMachine();
        std::cout << "Von Neumann Machine was turned ON.\n";
    }
    else {
        std::cout << "Von Neumann Machine is already ON.\n";
    }
    return;
}

void Interface::vnmTurnOff() {
    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is already OFF.\n";
    }
    else {
        delete vnm;
        vnm = NULL;
        std::cout << "Von Neumann Machine was turned ON.\n";
    }
    return;
}

void Interface::showVnmStatus() {
    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is OFF.\n";
    }
    else {
        std::cout << "Von Neumann Machine is ON.\n";
        uint16_t* reg = vnm->getRegisters();
        std::cout << std::hex << std::right <<std::setw(4) << std::uppercase
                  << "ac: 0x" << reg[0] << "\n";
        std::cout << std::hex << std::right <<std::setw(4) << std::uppercase
                  << "pc: 0x" << reg[1] << "\n";
        std::cout << std::hex << std::right <<std::setw(4) << std::uppercase
                  << "sp: 0x" << reg[2] << "\n";
    }
    return;
}

void Interface::loadProgram(std::string vnc) {
    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is OFF.\n";
        return;
    }
    Loader* l = new Loader();
    l->load(vnm, vnc);
}

void Interface::runVnm(std::string addr) {
    uint16_t start;

    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is OFF.\n";
        return;
    }

    if(!isNumber(addr)) {
        std::cerr << "run: invalid start address.\n";
        return;
    }

    if (addr[0] == '$') start = htoi(addr);
    else start = std::stoi(addr);

    if (start > 0xFFF) {
        std::cerr << "run: invalid start address.\n";
        return;
    }

    vnm->run(start);
}

void Interface::stepVnm(std::string addr) {
    uint16_t start;
    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is OFF.\n";
        return;
    }

    if(!isNumber(addr)) {
        std::cerr << "step: invalid start address.\n";
        return;
    }

    if (addr[0] == '$') start = htoi(addr);
    else start = std::stoi(addr);

    if (start > 0xFFF) {
        std::cerr << "step: invalid start address.\n";
        return;
    }

    vnm->step(start);
}


void Interface::printFile(std::string path) {
    std::ifstream file;
    file.open(path);

    if (!file.is_open()) {
        std::cerr << "print: file \"" << path << "\" could not be opened.\n";
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }

    return;
}