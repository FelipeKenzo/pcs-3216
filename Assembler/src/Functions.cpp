#include "../include/Functions.h"

extern std::map<std::string, mnemonicData> mTable;
extern std::map<std::string, symbolData> sTable;

//*----------------------------------*//
//*---- Function Implementations ----*//
//*----------------------------------*//

void printUsage() {
    std::cerr << "Usage: assembler <option(s)> source\n\n"
              << "Options: -o <destination>\n"
              << "         -v"
              << std::endl;
}

// Prints constructed symbol table
void printSTable() {
    std::cout << "Symbol Table:\n";
    
    std::cout << std::internal << std::setfill('0');
    
    std::for_each(sTable.begin(), sTable.end(),
        [](std::pair<std::string, symbolData > element)
        {
            std::cout << "0x" << std::setw(3) <<  std::hex << element.second.address << ": " << element.first << "\n";
        });
    std::cout << "\n";
}

// Converts given string to uppercase
std::string uppercase(const std::string& s) {
    std::string upper = s;
    for (int i = 0; i < s.length(); i++) {
        upper[i] = std::toupper(s[i]);
    }
    return upper;
}

// Converts ascii hex to integer
std::uint16_t htoi(const std::string& hex) {
    //std::cout << "hex number: (" << hex << ")\n";
    
    uint16_t intNum = 0;
    uint16_t len = hex.length();
    
    for (int i = 1; i < len; i++) {

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
int16_t pow16 (int16_t exp) {
    int16_t x = 1;
    for (int i = 0; i < exp; i++) {
        x *= 16;
    }
    //std::cout << "16^" << exp << " = " << x << "\n";
    return x;
}

// Splits string into vector of strings.
std::vector<std::string> split(const std::string& str, char delim) {
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
bool isValidLabel(const std::string& s) {
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
bool isValidMnemonic(const std::string& s) {
    std::map<std::string,mnemonicData>::const_iterator it = mTable.find(s);

    if (it == mTable.end()) {
        return false;
    }

    return true;
}

// Checks if given string is a number
bool isNumber(const std::string& s) {
    // Should start with '$' if hex or a digit if decimal.
    if (((s[0] < '0') || (s[0] > '9')) && (s[0] != '$')) return false;

    // All other characters 
    for (int i = 1; i < s.length()-1; i++) {
        if (((s[0] < '0') || (s[0] > '9')) && (s[0] != '$')) return false;
    }

    return true;
}