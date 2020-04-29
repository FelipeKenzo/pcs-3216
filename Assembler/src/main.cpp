#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <regex>

#include "../include/DataArea.h"

static void printUsage();
static void printSTable();
static std::string uppercase(std::string s);
static std::uint16_t htoi(std::string hex);
int16_t pow16 (int16_t exp);

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
    uint16_t    lc = 0;                     // Line Counter
    uint16_t    pc = 0;                     // Instruction Counter
    std::string line;                       // Current line
    std::regex whites("\\s+");
    std::regex label("([_*\\w]+\\:)");      // Matches a label
    std::regex alphanum("(\\$?[A-Z0-9]+)"); // Matches an alphanumeric word
    std::regex number("(\\$?[0-9]+)");      // Matches a number
    std::regex word("([A-Z]+)");
    std::regex comment("(;.*)");            // Matches a comment
    std::smatch match;                      // Match results
    bool isOrigin = false;
    bool isEnd = false;

    // First Pass -> Generate Symbol and Constant Tables, report syntax errors.
    while (getline(src, line) && !isEnd) {
        isOrigin = false;
        isEnd = false;

        lc++;

        // Pre-Process:
        line = std::regex_replace(line, comment, ""); // Remove Comments
        line = std::regex_replace(line, whites, " "); // Reduce whitespaces
        line = uppercase(line);
        
        // Search for label
        if (std::regex_search(line, match, label)) { 
            
            std::string labelName = match[1];
            labelName.pop_back(); // removes the ":""
            //labelName = uppercase(labelName);

            // Was it already defined?
            std::unordered_map<std::string,symbolData>::const_iterator it = sTable.find(labelName);

            if (it == sTable.end()) {
                // It wasnt, then we add it to the Symbol Table
                symbolData s = {pc, lc, true, false};
                sTable[labelName] = s;

                //std::cout << labelName << "at line " << lc << "\n";

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

        // Searches for mnemonic:
        if (std::regex_search(line, match, alphanum)) {
            
            //std::cout <<"line " << lc << ":" <<  line << "\n";
            //std::cout << match[1] << "\n";
            
            //std::string mnemoName = uppercase(match[1]);
            std::string mnemoName = match[1];
            
            // Is it a valid mnemonic?
            std::unordered_map<std::string,mnemonicData>::const_iterator it = mTable.find(mnemoName);

            if (it == mTable.end()) {
                std::cerr << "Error: invalid mnemonic \"" << mnemoName << "\" at line (" << lc << ").\n";
                return(-1);
            }

            if (mnemoName == "ORG") isOrigin = true; // Important to alter PC.
            if (mnemoName == "END") isEnd = true;    // Important to end step.

            // Remove mnemonic from line
            size_t pos = line.find(mnemoName);
            line.erase(pos, mnemoName.length());

            // Does it requires a parameter?
            if (it->second.param) {
                // Then it needs a parameter.
                if (std::regex_search(line, match, alphanum)) {

                    //std::string paramVal = uppercase(match[1]);
                    std::string paramVal = match[1];
                    // std::cout << match[1] << "\n";
                    
                    // And it might be a label, if it isn't a number:
                    if (!std::regex_search(paramVal, match, number)) {

                        // ORG does not support a label.
                        if (isOrigin) {
                            std::cerr << "Error: ORG label at line (" << lc << ") doesnt accept label as argument.\n";
                            return(-1);
                        }
                        
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

                    //std::cout << match[1] << "\n";

                    // ORG changes PC
                    if (isOrigin) {
                        std::string addr = match[1];
                        //std::cout << addr << "\n";
                        if (addr[0] == '$') pc = htoi(addr);
                        else pc = std::stoi(addr);
                        
                        //std::cout << "ORG: pc set to " << pc << "\n";
                    }
                }
                // No parameter found :(
                else {
                    std::cerr << "Error: instruction \"" << mnemoName << "\" at line (" << lc << ") requires a parameter.\n";
                    return(-1);
                }
            }

            pc += it->second.size;

        }

        //std::cout << "\n";

    }

    // Check if every label was defined
    bool unidefinedLabel = false;

    std::unordered_map<std::string, symbolData>::iterator it;

    for (it = sTable.begin(); it != sTable.end(); it++) {
        if (!it->second.isDefined) {
            std::cerr << "Error: label \"" << it->first << "\" used but not defined. At line (" << it->second.line
                          << ").\n";
            unidefinedLabel = true;
        }
    }
    
    if (unidefinedLabel) return(-1);


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
              << "Options: -o <destination>"
              << std::endl;
}

static void printSTable() {
    std::cout << "Symbol Table:\n";
    
    std::cout << std::internal
              << std::setfill('0');
    
    std::for_each(sTable.begin(), sTable.end(),
        [](std::pair<std::string, symbolData > element)
        {
            std::cout << "0x" << std::setw(4) <<  std::hex << element.second.address << ": " << element.first << "\n";
        });
    std::cout << "\n";
}

static std::string uppercase(std::string s) {
    for (int i = 0; i < s.length(); i++) {
        s[i] = std::toupper(s[i]);
    }
    return s;
}

// Converts ascii hex to integer
static std::uint16_t htoi(std::string hex) {
    //std::cout << "hex number: (" << hex << ")\n";
    
    uint16_t intNum = 0;
    uint16_t len = hex.length();
    
    for (int i = 0; i < len; i++) {

        if ((hex[i] > 0x29) && (hex[i] < 0x3A)) { // 0-9
            intNum += (hex[i] - 0x30)*pow16(len-i-1);
            //std::cout << (int)(hex[i] - 0x30) << " x " << pow16(len-i-1) << " = " << (hex[i] - 0x30)*pow16(len-i-1) << "\n";
        }
        else if (hex[i] > 0x40 && hex[i] < 0x47) { //A-F
            intNum += (hex[i] - 0x37)*pow16(len-i-1);
        }
    }
    std::cout << "\n";
    return intNum;
}

// Returns 16^exp
int16_t pow16 (int16_t exp) {
    int16_t x = 1;
    for (int i = 0; i < exp; i++) {
        x *= 16;
    }
    //std::cout << "16^" << exp << " = " << x << "\n";
    return x;
}