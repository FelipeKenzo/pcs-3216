#include <stdint.h>
#include <iostream>
#include <string>

#include "../include/Interface.h"
#include "../include/VonNeumannMachine.h"
#include "../include/Assembler.h"

int main (int argc, char* argv[]) {
    
    Interface* i = new Interface();
    i->input();

    return 0;
}