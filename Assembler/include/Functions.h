#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <regex>

#include "DataArea.h"

void printUsage();
void printSTable();
std::string uppercase(const std::string& s);
std::uint16_t htoi(const std::string& hex);
int16_t pow16 (int16_t exp);
std::vector<std::string> split(const std::string& str, char delim); // Taken from github (https://stackoverflow.com/questions/5607589/right-way-to-split-an-stdstring-into-a-vectorstring)
bool isValidLabel(const std::string& s);
bool isValidMnemonic(const std::string& s);
bool isNumber(const std::string& s); 

#endif