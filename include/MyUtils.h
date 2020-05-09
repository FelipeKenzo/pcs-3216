#ifndef MYUTILS_H
#define MYUTILS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

// Converts ascci hex to int
std::uint64_t htoi(const std::string& hex);

// Splits string into vector of words
std::vector<std::string> split(const std::string& str, char delim);

// Returns true if given string representas a number (either Hex our Dec)
bool isNumber(const std::string& s);

// Returns true if given string represents a decimal number
bool isDec(const std::string& s);

// Returns true if given string represents a hexadecimal number
bool isHex(const std::string& s);

// returns a main string without a substring
std::string eraseSubStr(const std::string& mainStr, const std::string &toErase);

// Calculates cheksum
uint16_t calculateChecksum(uint16_t addr, uint16_t size, std::vector<uint8_t> data);

#endif