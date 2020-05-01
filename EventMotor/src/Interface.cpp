#include "../include/Interface.h"

Interface::Interface() {
    commandMap = {
        {"exit", exit},
        {"assemble", assemble},
        {"help", help},
        {"ls", ls},
        {"rm", rm}
    };

    std::cout << "Event Motor v0.5\n"
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
                    std::cout << "assemble [source] [-w | --warning] [-o][outname].\n";
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
                break;
            
            default:
                std::cout << "\"" << argv[0] << "\" is not a valid command.\n";
                break;
        }

    }
}

void Interface::assembleSrc(std::string src, std::string out, bool w) {

    Assembler* a = new Assembler(src, out, w);
    a->assemble();
}

void Interface::helpMessage() {
    std::cout << "\n-----Help-----\n\n"
              << "Valid commands ('*' indicates mandatory parameter):\n\n"
              << "help: displays this help message.\n\n"
              << "assemble: assembles a .txt source file into a .vnc hex file and .lst list file.\n"
              << "      [source]*        source file name (with extension)\n"
              << "      [-w | --warning] show warning messages\n"
              << "      [-o][outname]    output name (without extension)\n\n"
              << "ls: list files in filesystem.\n\n"
              << "exit: exits program.\n\n"; 
}

void Interface::exitMessage() {
    std::cout << "\nExiting program...\n";
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
}