#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <regex>

#include "../include/DataArea.h"

static void printUsage();
static void printSTable();
static std::string uppercase(std::string s);

int main(int argc, char* argv[]) {
    std::string filename;
    std::string output = "a.o";
    
    // Arguments parsing
    if (argc > 1) {
        std::string arg; 
        for (int i = 1; i < argc; i++) {
            arg = argv[i];
            if (arg == "-o") {
                output = argv[++i];
            }
            else {
                filename = argv[i];
            }
        }
    }
    else {
        printUsage();
        return(-1);
    }
    // Opening file
    std::ifstream src;
    src.open(filename);

    // Check if file opened
    if (!src.is_open()) {
        std::cerr << "Error: Could not open \"" << filename << "\". Exiting Program."
                  << std::endl;
        return(-1);
    }

    // Utils
    uint16_t    lc = 0; // Line Counter
    uint16_t    pc = 0; // Instruction Counter
    std::string line;
    std::regex   label("([_A-Z0-9]+\\:)");
    std::regex    word("([A-Z0-9]+)");
    std::regex  number("([0-9])+");
    std::regex comment("(;.*)");
    std::smatch match;

    // First Pass -> Generate Symbol and Constant Tables, report syntax errors.
    while (getline(src, line)) {
        lc++;

        // Remove comments
        line = std::regex_replace(line, comment, "");
        
        // Search for label
        if (std::regex_search(line, match, label)) { 
            
            std::string labelName = match[1];
            labelName.pop_back(); // removes the ":""

            // Was it already defined?
            std::unordered_map<std::string,symbolData>::const_iterator it = sTable.find(labelName);

            if (it == sTable.end()) {
                // It wasnt, then we add it to the Symbol Table
                symbolData s = {pc, lc, true, false};
                sTable[labelName] = s;

                // And remove it from the line
                size_t pos = line.find(labelName + ":");
                line.erase(pos, labelName.length());
            }
            else {
                // It was, error!
                std::cerr << "Error: label redefined at line (" << lc << "), previously defined at line ("
                          << it->second.line << ").\n";
                return(-1);
            }
        }

        // Searches for Mnemonic and Parameter
        if (std::regex_search(line, match, word)) {
            
            std::cout << match[1] << "\n";
            
            std::string mnemoName = uppercase(match[1]);
            
            // Is it a valid mnemonic?
            std::unordered_map<std::string,mnemonicData>::const_iterator it = mTable.find(mnemoName);

            if (it == mTable.end()) {
                std::cerr << "Error: invalid mnemonic \"" << mnemoName << "\" at line (" << lc << ").\n";
                return(-1);
            }

            // Does it requires a parameter?
            if (it->second.param) {
                // Then it needs exactly one parameter.
                for (int i = 0; i < match.length(); i++)
                    std::cout << match[1] << " ";
                std::cout << "\n";

                if (match.length() != 2) {
                    std::cerr << "Error: instruction \"" << mnemoName << "\" at line (" << lc << ") requires *one* parameter.\n";
                    return(-1);
                }

                std::string paramVal = uppercase(match[2]);
                
                // And it might be a label, if it isn't a number:
                if (!std::regex_search(paramVal, match, number)) {
                    
                    // Sees if it is already defined:
                    std::unordered_map<std::string,symbolData>::const_iterator it = sTable.find(mnemoName);

                    if (it == sTable.end()) {
                        // It wasnt, then we add it to the Symbol Table
                        symbolData s = {pc, lc, false, true};
                        sTable[paramVal] = s;
                    }
                    else {
                        // It was, then we set referenced to true
                        sTable[paramVal].isReferenced = true;
                    }
                }
            }

            pc += it->second.size;

        }

    }

    printSTable();

    // Go back to start of file
    src.clear();
    src.seekg(0, std::ios::beg);

    // Second Pass -> Generate output code.
    std::ofstream out;
    out.open(output);

    return(0);
}

static void printUsage() {
    std::cerr << "Usage: assembler <option(s)> source"
              << std::endl;
}

static void printSTable() {
    std::for_each(sTable.begin(), sTable.end(),
        [](std::pair<std::string, symbolData > element)
        {
            std::cout << element.first << ": " << element.second.address << "\n";
        });
}

static std::string uppercase(std::string s) {
    for (int i = 1; i < s.length(); i++) {
        s[i] = std::toupper(s[i]);
    }
    return s;
}