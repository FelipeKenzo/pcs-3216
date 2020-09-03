#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>

#include "../include/Interface.h"
#include "../include/VonNeumannMachine.h"
#include "../include/Assembler.h"

#include "../include/CommandLineInterpreter.h"
#include "../include/HighLevelInterpreter.h"

int main (int argc, char* argv[]) {
    
    std::string script;

    if (argc > 1) {
        script = argv[1];
        CommandLineInterpreter* cli = new CommandLineInterpreter(script);
        cli->executeScript();
    }
    else {
        std::cerr << "Missing script file.\n";
    }


    return 0;
}