#include <stdint.h>
#include <iostream>
#include <string>

#include "../include/VonNeumannMachine.h"
#include "../include/Assembler.h"

int main (int argc, char* argv[]) {

    Assembler* a = new Assembler("test.txt", "test", false);

    if(a->assemble()) std::cout << "Success.\n";
    else std::cout << "Failed.\n";

    return 0;
}