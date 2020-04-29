#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <regex>

#include "../include/DataArea.h"

// Utility functions
static void printUsage();
static void printSTable();
static std::string uppercase(const std::string& s);
static std::uint16_t htoi(const std::string& hex);
static int16_t pow16 (int16_t exp);
static std::vector<std::string> split(const std::string& str, char delim); // Taken from github (https://stackoverflow.com/questions/5607589/right-way-to-split-an-stdstring-into-a-vectorstring)
static bool isValidLabel(const std::string& s);
static bool isValidMnemonic(const std::string& s);
static bool isNumber(const std::string& s); 

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
            else if (arg == "-h" || arg == "--help") {
                printUsage();
                return(0);
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
    std::vector<std::string> words;         // Vector with words from line
    
    std::regex whites("\\s+");              // Matches whitespaces
    std::regex comment("(;.*)");            // Matches a comment
    
    bool isEnd = false;
    bool isError = false;

    /*---- First Pass -> Generate Symbol and Constant Tables, report syntax errors. ----*/
    while (getline(src, line) && !isEnd) {
        isEnd = false;

        lc++;

        // Line pre-processing:
        line = std::regex_replace(line, comment, ""); // Remove Comments
        line = std::regex_replace(line, whites, " "); // Reduce whitespaces
        line = uppercase(line);                       // Uppercase all words
        words = split(line, ' ');                     // Extract word vector

        // Skip empty lines:
        if (words.size() == 0) continue;

        // There should only be two or three words: 
        if (words.size() > 3) {
            std::cerr << "Error: too many parameters at line (" << lc << ").\n";
            isError = true;
            continue;
        }

        // If more than two words, first is a label:
        if (words.size() > 2) {
            std::string newLabel = words[0];
            //std::cout << newLabel << "\n";

            if (!isValidLabel(newLabel)) {
                std::cerr << "Error: invalid label at line (" << lc << ").\n";
                isError = true;
                continue;
            }

            // Now we remove the ':' at the end:
            newLabel.pop_back();

            // Was it already defined?
            std::unordered_map<std::string,symbolData>::const_iterator it = sTable.find(newLabel);

            if (it == sTable.end()) {
                
                // It wasnt, then we add it to the Symbol Table:
                symbolData data = {pc, lc, true, false};
                sTable[newLabel] = data;

                // And remove it from word vector:
                words.erase(words.begin()); // It is inefficient, but our vector is small.

                //std::cout << labelName << "at line " << lc << "\n";
            }
            else {
                // label redefinition!
                std::cerr << "Error: label redefined at line (" << lc << "), previously defined at line ("
                          << it->second.line << ").\n";
                isError = true;
                continue;
            }
        }

        // Next word should be a mnemonic:
        std::string mnemonic = words[0];

        if (!isValidMnemonic(mnemonic)) {
            std::cerr << "Error: invalid mnemonic \"" << mnemonic << "\" at line (" << lc << ").\n";
            isError = true;
            continue;
        }

        // Check if instruction is at valid address
        if (mTable[mnemonic].size > 0) {
            if (pc > 0xFFF) {
                std::cerr <<"Error: invalid instruction address (0x" << std::hex << pc << ") at line (" << std::dec << lc << ").\n";
                continue;
            }
        }

        if (mnemonic == "END") isEnd = true; // Important to end step 1.

        // Does it require a parameter?
        if(mTable[mnemonic].param) {
            // There needs to be a parameter:
            if (words.size() < 2) {
                std::cerr << "Error: instruction \"" << mnemonic << "\" at line (" << lc << ") requires a parameter.\n";
                isError = true;
                continue;
            }

            std::string parameter = words[1];

            // Is the parameter a number?
            if (isNumber(parameter)) {
                
                // If it is, it could be the ORG parameter. Then, we update PC.
                if (mnemonic == "ORG") {
                    int16_t addr;
                    if (parameter[0] == '$') addr = htoi(parameter); // parameter in HEX
                    else addr = std::stoi(parameter);                // parameter in DEC

                    // Checks if valid address
                    if (addr > 0xFFF) {
                        std::cerr << "Error: invalid ORG address at line (" << lc <<").\n";
                        isError = true;
                        continue;
                    }

                    pc = addr;
                }
            }
            // if not, it is a label:
            else {
                // Sees if it is already defined:
                std::unordered_map<std::string,symbolData>::const_iterator it = sTable.find(parameter);

                if (it == sTable.end()) {
                    // It wasnt, then we add it to the Symbol Table;
                    symbolData data = {pc, lc, false, true};
                    sTable[parameter] = data;
                }
                else {
                    // It was, then we set referenced to true;
                    sTable[parameter].isReferenced = true;
                }

                // ORG cannot receive a label as parameter.
                if (mnemonic == "ORG") {
                    std::cerr << "Error: ORG address at line (" << lc <<") cannot be a label.\n";
                    isError = true;
                    continue;
                }
            }
        }
            
        pc += mTable[mnemonic].size;
        //std::cout << "\n";

    }

    // Check if every label was defined
    bool undefinedLabel = false;

    std::unordered_map<std::string, symbolData>::iterator it;

    for (it = sTable.begin(); it != sTable.end(); it++) {
        if (!it->second.isDefined) {
            std::cerr << "Error: label \"" << it->first << "\" used but not defined. At line (" << it->second.line
                          << ").\n";
            undefinedLabel = true;
        }
    }
    
    if (undefinedLabel) return(-1);


    printSTable();

    // Go back to start of file
    src.clear();
    src.seekg(0, std::ios::beg);
    pc = 0;
    lc = 0;
    isEnd = false;

    /*---- Second Pass -> Generate output code and listing.          ----*/
    /*---- It doesn't checks for erros, since Step 1 already did it. ----*/
    
    // Go back to start of file
    std::ofstream out;
    out.open(output);

    while (getline(src, line) && !isEnd) {
        isEnd = false;

        lc++;

        // Line pre-processing:
        line = std::regex_replace(line, comment, ""); // Remove Comments
        line = std::regex_replace(line, whites, " "); // Reduce whitespaces
        line = uppercase(line);                       // Uppercase all words
        words = split(line, ' ');                     // Get word vector


        // for (int i = 0; i < words.size(); i++) {
        //     std::cout << words[i] << " ";
        // }

        //std::cout << "\n";
        //std::cout << std::setw(2) << lc << ": " << line << "\n";
    }

    return(0);
}

static void printUsage() {
    std::cerr << "Usage: assembler <option(s)> source\n\n"
              << "Options: -o <destination>"
              << std::endl;
}

// Prints constructed symbol table
static void printSTable() {
    std::cout << "Symbol Table:\n";
    
    std::cout << std::internal
              << std::setfill('0');
    
    std::for_each(sTable.begin(), sTable.end(),
        [](std::pair<std::string, symbolData > element)
        {
            std::cout << "0x" << std::setw(3) <<  std::hex << element.second.address << ": " << element.first << "\n";
        });
    std::cout << "\n";
}

// Converts given string to uppercase
static std::string uppercase(const std::string& s) {
    std::string upper = s;
    for (int i = 0; i < s.length(); i++) {
        upper[i] = std::toupper(s[i]);
    }
    return upper;
}

// Converts ascii hex to integer
static std::uint16_t htoi(const std::string& hex) {
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
    return intNum;
}

// Returns 16^exp
static int16_t pow16 (int16_t exp) {
    int16_t x = 1;
    for (int i = 0; i < exp; i++) {
        x *= 16;
    }
    //std::cout << "16^" << exp << " = " << x << "\n";
    return x;
}

// Splits string into vector of strings.
static std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}

// Checks if given string is a valid label name
static bool isValidLabel(const std::string& s) {
    // Label must start with either '_' or letter.
    if (((s[0] < 'A') || (s[0] > 'Z')) &&  (s[0] != '_')) return false;
    
    // Following characters need to be alphanumeric, '-' or '_'.
    for (int i = 1; i < s.length()-1; i++) {
        char c = s[i];
        if ((c < '0') || (c > '9')) {         // Not digit
            if ((c < 'A') || (c > 'Z')) {     // Not letter
                if ((c != '-') || (c != '_')) // Not '-' or '_'
                    return false;
            }
        }
    }

    // Must end with ':'
    if (s[s.length()-1] != ':') return false;

    return true;
}

// Checks if given string is a valid mnemonic
static bool isValidMnemonic(const std::string& s) {
    std::unordered_map<std::string,mnemonicData>::const_iterator it = mTable.find(s);

    if (it == mTable.end()) {
        return false;
    }

    return true;
}

// Checks if given string is a number
static bool isNumber(const std::string& s) {
    // Should start with '$' if hex or a digit if decimal.
    if (((s[0] < '0') || (s[0] > '9')) && (s[0] != '$')) return false;

    // All other characters 
    for (int i = 1; i < s.length()-1; i++) {
        if (((s[0] < '0') || (s[0] > '9')) && (s[0] != '$')) return false;
    }

    return true;
}