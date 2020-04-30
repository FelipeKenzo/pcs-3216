#ifndef MYUTILS_H
#define MYUTILS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

// Converts string to uppercase
std::string uppercase(const std::string& s);

// Converts ascci hex to int
std::uint16_t htoi(const std::string& hex);

// Returns 16^exp
int16_t pow16 (int16_t exp);

// Splits string into vector of words
std::vector<std::string> split(const std::string& str, char delim);

// Returns if string ias a number (either hex or dec)
bool isNumber(const std::string& s);

#endif