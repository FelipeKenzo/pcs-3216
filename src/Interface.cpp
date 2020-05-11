#include "../include/Interface.h"

Interface::Interface() {

    inFile.open("./filesystem/test.vnc");
    outFile.open("./filesystem/output.txt");

    vnm = new VonNeumannMachine(&inFile, &outFile);
    loadLoader();

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
                    loadProgram(argv[1]);
                }
                else {
                    std::cerr << "load [program]*\n";
                }
                break;

            case build: {
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
                    std::cout << "load [source]* [-w|--warning] [-o outname].\n";
                    break;
                }

                buildSrc(src, out, warnings);
                break;
            }

            case run:
                if (argc == 2) {
                    runVnm(argv[1]);
                }
                else if (argc == 1){
                    runVnm("");
                }
                else {
                    std::cerr << "run [startAddress]\n";
                }
                break;

            case step:
                if (argc == 2) {
                    stepVnm(argv[1]);
                }
                else if (argc == 1) {
                    stepVnm("");
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

            case set:
                if (argc == 3) {
                    setReg(argv[1], argv[2]);
                }
                else {
                    std::cerr << "set [reg] [data]\n";
                }
                break;
            
            case md: {
                std::string range = "0";
                if (argc == 1) {
                    std::cerr << "md [address]* [range].\n";
                    break;
                }
                else if (argc == 3) {
                    range = argv[2];
                }
                else if (argc > 3) {
                    std::cerr << "md [address]* [range].\n";
                    break;
                }
                displayMemory(argv[1], range);
                break;
            }

            case mm: 
                if (argc == 1) {
                    std::cerr << "mm [address]* [data]\n";
                    break;
                }
                else if (argc == 2) modifyMemory(argv[1], "");
                else if (argc == 3) modifyMemory(argv[1], argv[2]);
                else std::cerr << "mm [address]* [data]\n";
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

void Interface::buildSrc(std::string src, std::string out, bool w) {
    Assembler* a = new Assembler(src, out, w);
    if (a->assemble()){
        loadProgram(eraseSubStr(out, "./filesystem/") + ".vnc");
    }
    delete a;
}

void Interface::helpMessage() {
    std::cout << "\nNote: parameters with '*' are mandatory.\n\n"
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
              << "md: displays the contents of a memory address and it's chunk. If a range is provided, displays\n"
              << "    the contents until address + range, also in chunks.\n"
              << "usage: md [address]* [range]\n"
              << "      [address*]       memory address to be displayed.\n"
              << "      [range]          the range of addresses to be displayed.\n\n"
              << "mm: modifies the contents of a memory address. If no data is given, it asks the user until\n"
              << "    they enter '.'\n"
              << "usage: mm [address]* [data]\n"
              << "      [address]*       memory address to be modified.\n"
              << "      [data]           value to be stored at the address.\n\n"
              << "usage: print [file]*\n"
              << "      [file]*          file name (with extension)\n\n"
              << "rm: removes (deletes) file(s) from the filesystem.\n"
              << "usage: rm [file(1)]* ... [file(n)]\n"
              << "      [file(n)]*       file names to be deleted.\n\n"
              << "run: runs the VM at the the memory location specified.\n"
              << "usage: run [startAddress]\n"
              << "      [startAddress]   start address for the simulation. Must be lower than $FFE.\n"
              << "                       can be in etiher decimal or hexadecimal (starting with '$').\n\n"
              << "set: sets the contents of a register.\n"
              << "usage: set [register] [data]\n"
              << "      [register]       the register to be written. Either 'ac, 'pc' or 'sp'.\n"
              << "      [data]           the data to be written in the register. Mus be lower tha $FFFF.\n\n"
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
        inFile.open("./filesystem/input.txt");
        outFile.open("./filesystem/output.txt");
        vnm = new VonNeumannMachine(&inFile, &outFile);
        loadLoader();
        std::cout << "Von Neumann Machine was turned on.\n";
    }
    else {
        std::cout << "Von Neumann Machine is already on.\n";
    }
    return;
}

void Interface::vnmTurnOff() {
    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is already off.\n";
    }
    else {
        inFile.close();
        outFile.close();
        delete vnm;
        vnm = NULL;
        std::cout << "Von Neumann Machine was turned off.\n";
    }
    return;
}

void Interface::showVnmStatus() {
    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is OFF.\n";
    }
    else {
        std::cout << "\n";
        std::cout << "ac: $" << std::setfill('0') << std::setw(2) << std::uppercase
                  << std::hex << vnm->getAC() << "    ";
        std::cout << "pc: $" << std::setfill('0') << std::setw(3) << std::uppercase
                  << std::hex << vnm->getPC() << "\n\n";
    }
    return;
}

void Interface::loadProgram(std::string vnc) {
    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is OFF.\n";
        return;
    }

    inFile.close();
    inFile.open("./filesystem/" + vnc);

    if (!inFile.is_open()) {
        std::cerr << "load: file \"" << vnc << "\" could not be opened.\n";
    }

    vnm->setPC(0x000);
    vnm->run();

    if (vnm->memRead_b(0x0B)) {
        std::cerr << "load: corrupted file!\n";
    }

    inFile.close();
    inFile.open("./filesystem/input.txt");
}

void Interface::runVnm(std::string addr) {
    uint16_t start;

    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is OFF.\n";
        return;
    }

    if (addr == "") {
        vnm->run();
        return;
    }


    if (isDec(addr)) start = std::stoi(addr);
    else if (addr[0] == '$' && isHex(addr)) start = htoi(addr);
    else {
        std::cerr << "run: invalid start address.\n";
        return;
    }

    if (start > 0xFFF) {
        std::cerr << "run: invalid start address.\n";
        return;
    }

    vnm->setPC(start);
    vnm->run();
}

void Interface::stepVnm(std::string addr) {
    uint16_t start;
    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is OFF.\n";
        return;
    }

    if (addr == "") {
        vnm->step();
        return;
    }

    if (isDec(addr)) start = std::stoi(addr);
    else if (addr[0] == '$' && isHex(addr)) start = htoi(addr);
    else {
        std::cerr << "step: invalid start address.\n";
        return;
    }

    if (start > 0xFFF) {
        std::cerr << "step: invalid start address.\n";
        return;
    }

    vnm->setPC(start);
    vnm->step();
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

void Interface::setReg(std::string reg, std::string data) {
    uint32_t dataValue;
    
    if (vnm == NULL) {
        std::cout << "Von Neumann Machine is OFF.\n";
        return;
    }

    if (isDec(data)) dataValue = std::stoi(data);   
    else if (data[0] == '$' && isHex(data)) dataValue = htoi(data);
    else {
        std::cerr << "setReg: invalid data value\n";
        return;
    }

    if (reg == "ac") {
        if (dataValue > 0xFFFF) {
            std::cerr << "set: invalid data value\n";
            return;
        }
        vnm->setAC(dataValue);
    }
    else if (reg == "pc") {
        if (dataValue > 0x0FFF) {
            std::cerr << "set: invalid data value\n";
            return;
        }
        vnm->setPC(dataValue);
    }
    else {
        std::cerr << "setReg: invalid register.\n";
        return;
    }
}

void Interface::loadLoader() {
    vnm->memWrite_w(0x00, 0x800B);
    vnm->memWrite_w(0x02, 0x500B);
    vnm->memWrite_w(0x04, 0x900B);
    vnm->memWrite_w(0x06, 0x000E);
    vnm->memWrite_w(0x08, 0x0001);
    vnm->memWrite_w(0x0A, 0x9000);
    vnm->memWrite_w(0x0C, 0x00FF);
    vnm->memWrite_w(0x0E, 0x800C);
    vnm->memWrite_w(0x10, 0x500C);
    vnm->memWrite_w(0x12, 0x900C);
    vnm->memWrite_w(0x14, 0xA05A);
    vnm->memWrite_w(0x16, 0x400A);
    vnm->memWrite_w(0x18, 0x902E);
    vnm->memWrite_w(0x1A, 0xA05A);
    vnm->memWrite_w(0x1C, 0x902F);
    vnm->memWrite_w(0x1E, 0x1022);
    vnm->memWrite_w(0x20, 0x0028);
    vnm->memWrite_w(0x22, 0x802E);
    vnm->memWrite_w(0x24, 0x500A);
    vnm->memWrite_w(0x26, 0x1058);
    vnm->memWrite_w(0x28, 0xA05A);
    vnm->memWrite_w(0x2A, 0x9008);
    vnm->memWrite_w(0x2C, 0xA05A);
    vnm->memWrite_w(0x2E, 0x9000);
    vnm->memWrite_w(0x30, 0x802F);
    vnm->memWrite_w(0x32, 0x4009);
    vnm->memWrite_w(0x34, 0x902F);
    vnm->memWrite_w(0x36, 0x1042);
    vnm->memWrite_w(0x38, 0x8008);
    vnm->memWrite_w(0x3A, 0x5009);
    vnm->memWrite_w(0x3C, 0x9008);
    vnm->memWrite_w(0x3E, 0x104A);
    vnm->memWrite_w(0x40, 0x002C);
    vnm->memWrite_w(0x42, 0x802E);
    vnm->memWrite_w(0x44, 0x4009);
    vnm->memWrite_w(0x46, 0x902E);
    vnm->memWrite_w(0x48, 0x0038);
    vnm->memWrite_w(0x4A, 0x800D);
    vnm->memWrite_w(0x4C, 0x500C);
    vnm->memWrite_w(0x4E, 0x900C);
    vnm->memWrite_w(0x50, 0xD000);
    vnm->memWrite_w(0x52, 0x500C);
    vnm->memWrite_w(0x54, 0x100E);
    vnm->memWrite_w(0x56, 0x900B);
    vnm->memWrite_w(0x58, 0xC00E);
    vnm->memWrite_w(0x5A, 0x0000);
    vnm->memWrite_w(0x5C, 0xD000);
    vnm->memWrite_w(0x5E, 0x9068);
    vnm->memWrite_w(0x60, 0x400C);
    vnm->memWrite_w(0x62, 0x900C);
    vnm->memWrite_w(0x64, 0x8068);
    vnm->memWrite_w(0x66, 0xB05A);
    
}

void Interface::displayMemory(std::string addr, std::string range) {
    uint16_t numAddr;

    if (isDec(addr)) numAddr = std::stoi(addr);   
    else if (addr[0] == '$' && isHex(addr)) numAddr = htoi(addr);
    else {
        std::cerr << "md: invalid memory address.\n";
        return;
    }

    if (numAddr > 0xFFF) {
        std::cerr << "md: invalid memory address.\n";
        return;
    }

    uint16_t baseAddr = numAddr & 0xFF0;

    uint16_t numRange;

    if (isDec(range)) numRange = std::stoi(range);   
    else if (range[0] == '$' && isHex(range)) numRange = htoi(range);
    else {
        std::cerr << "md: invalid range value.\n";
        return;
    }


    uint16_t endAddr = numAddr + numRange;
    //std::cout << std::hex << endAddr << "\n";

    //std::cout << std::hex << (((endAddr & 0xFF0) - baseAddr) >> 4);

    uint16_t nlines = (((endAddr & 0xFF0) - baseAddr) >> 4) + 1;

    std::cout << "\n     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"
              << "     -----------------------------------------------\n";

    for (int i = 0; i < nlines; i++) {
        std::cout << std::setfill('0') << std::setw(3) << std::uppercase
                  << std::hex << baseAddr + 0x10 * i << ": ";
        for (int j = 0; j < 16; j++) {
            std::cout << std::setfill('0') << std::setw(2) << std::uppercase
                  << std::hex << (uint)vnm->memRead_b((baseAddr + 0x10 * i)+j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Interface::modifyMemory(std::string addr, std::string data) {
    uint16_t numAddr;

    if (isDec(addr)) numAddr = std::stoi(addr);
    else if (addr[0] == '$' && isHex(addr)) numAddr = htoi(addr);
    else {
        std::cerr << "mm: invalid memory address.\n";
        return;
    }

    if (numAddr > 0xFFF) {
        std::cerr << "mm: invalid memory address.\n";
        return;
    }

    if (data != "") {
        uint16_t numData;
        if (isDec(data)) numData = std::stoi(data);
        else if (data[0] == '$' && isHex(data)) numData = htoi(data);
        else {
            std::cerr << "mm: invalid data value.\n";
            return;
        }

        if (numData > 0xFF) {
            std::cerr << "mm: invalid data value.\n";
            return;
        }

        vnm->memWrite_b(numAddr, numData);
        return;
    }
    else {
        std::string in;

        while(numAddr < 0xFFF) {
            std::cout << std::setfill('0') << std::setw(3) << std::uppercase
                    << std::hex << addr << "    " << std::setw(2)
                    << (uint16_t)vnm->memRead_b(numAddr) << "  ? ";

            std::cin >> in;

            if (in == ".") break;

            uint16_t numData;
            if (isDec(in)) numData = std::stoi(in);
            else if (in[0] == '$' && isHex(in)) numData = htoi(in);
            else {
                std::cerr << "mm: invalid data value.\n";
                return;
            }

            if (numData > 0xFF) {
                std::cerr << "mm: invalid data value.\n";
                return;
            }

            vnm->memWrite_b(numAddr, numData);
            numAddr++;
        }

        std::cout << "\n";
        return;
    }


}