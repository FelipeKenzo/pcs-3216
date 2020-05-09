#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <filesystem>
#include <cstdio>
#include <fstream>

#include "../include/VonNeumannMachine.h"
#include "Assembler.h"

class Interface {
private:
    VonNeumannMachine* vnm = NULL;
    void loadLoader();
    void exitMessage();
    void helpMessage();
    void assembleSrc(std::string src, std::string out, bool w);
    void buildSrc(std::string src, std::string out, bool w);
    void runVnm(std::string addr);
    void stepVnm(std::string addr);
    void listFiles();
    void removeFiles(std::vector<std::string> files);
    void showVnmStatus();
    void vnmTurnOn();
    void vnmTurnOff();
    void loadProgram(std::string vnc);
    void printFile(std::string file);
    void setReg(std::string reg, std::string data);
    void displayMemory(std::string addr, std::string range);
    void modifyMemory(std::string addr, std::string data);

    std::ifstream inFile;
    std::ofstream outFile;

    enum command {
        invalid, exit, assemble, help, ls,
        rm, status, turn, load, run, print,
        step, set, md, mm, build
    };

    std::map<std::string, command> commandMap = {
        {"exit", exit},
        {"assemble", assemble},
        {"build", build},
        {"help", help},
        {"ls", ls},
        {"rm", rm},
        {"status", status},
        {"turn", turn},
        {"load", load},
        {"run", run},
        {"print", print},
        {"step", step},
        {"set", set},
        {"md", md},
        {"mm", mm}
    }; 

public:
    Interface();
    void input();
};

#endif