#include "../include/MyUtils.h"

// Converts given string to uppercase
std::string uppercase(const std::string& s) {
    std::string upper = s;
    for (int i = 0; i < s.length(); i++) {
        upper[i] = std::toupper(s[i]);
    }
    return upper;
}

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

int16_t pow16 (int16_t exp) {
    int16_t x = 1;
    for (int i = 0; i < exp; i++) {
        x *= 16;
    }
    //std::cout << "16^" << exp << " = " << x << "\n";
    return x;
}

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

bool isNumber(const std::string& s) {
    // Should start with '$' if hex or a digit if decimal.
    if (((s[0] < '0') || (s[0] > '9')) && (s[0] != '$')) return false;

    // All other characters 
    for (int i = 1; i < s.length()-1; i++) {
        if (((s[0] < '0') || (s[0] > '9')) && (s[0] != '$')) return false;
    }

    return true;
}

std::string eraseSubStr(const std::string & mainStr, const std::string & toErase)
{
    std::string newStr = mainStr;

	// Search for the substring in string
	size_t pos = newStr.find(toErase);
 
	if (pos != std::string::npos)
	{
		// If found then erase it from string
		newStr.erase(pos, toErase.length());
	}

    return newStr;
}