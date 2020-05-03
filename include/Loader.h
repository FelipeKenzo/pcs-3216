#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <string>
#include <istream>

#include "VonNeumannMachine.h"

class Loader {
public:
    void load(VonNeumannMachine* vnm, std::string vnc);

};

#endif