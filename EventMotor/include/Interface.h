#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <filesystem>
#include <cstdio>

#include "VonNeumannMachine.h"
#include "Assembler.h"

class Interface {
private:
    VonNeumannMachine* vnm;
    void exitMessage();
    void helpMessage();
    void assembleSrc(std::string src, std::string out, bool w);
    void run(std::string file);
    void listFiles();
    void removeFiles(std::vector<std::string> files);

    enum command {
        invalid,
        exit,
        assemble,
        help,
        ls,
        rm
    };

    std::map<std::string, command> commandMap; 

public:
    Interface();
    void input();
};

#endif