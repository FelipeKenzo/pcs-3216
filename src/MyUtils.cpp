#include "../include/MyUtils.h"
#include <limits>

std::uint64_t htoi(const std::string& hex) {
    //std::cout << "hex number: (" << hex << ")\n";
    
    std::string toint;
    if (hex[0] == '$') toint = hex.substr(1,hex.length()-1);
    else toint = hex;

    uint64_t intNum = 0;
    uint16_t len = toint.length();
    
    for (int i = 0; i < len; i++) {
        //std::cout << (toint[i]) << "\n";

        if ((toint[i] > 0x29) && (toint[i] < 0x3A)) { // 0-9
            intNum |= (uint64_t)(toint[i] - 0x30)<<((len-i-1)*4);
            //std::cout << std::right << std::setfill('0') << std::setw(len) << ((toint[i] - 0x30)<<((len-i-1)*4)) << "\n";
        }
        else if (toint[i] > 0x40 && toint[i] < 0x47) { //A-F
            intNum |= (uint64_t)(toint[i] - 0x37)<<((len-i-1)*4);
            //std::cout << std::right << std::setfill('0') << std::setw(len) << std::hex << ((toint[i] - 0x37)<<((len-i-1)*4)) << "\n";
        }

        //std::cout << std::hex << intNum << "\n";
    }

    //std::cout << "result: " << std::hex << intNum << "\n";
    return intNum;
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
    // std::cout << s << ": ";
    // Should start with '$' if hex or a digit if decimal.
    if (((s[0] < '0') || (s[0] > '9')) && ((s[0] < 'A')||(s[0] > 'F')) && (s[0] != '$')) {
        // std::cout << s[0] << "\n";
        return false;
    }

    // All other characters can range from (0-9,A-F)
    for (int i = 1; i < s.length()-1; i++) {
        if (((s[i] < '0') || (s[i] > '9')) && ((s[i] < 'A')||(s[i] > 'F'))) {
            // std::cout << ((s[0] < '0') || (s[0] > '9')) << " ";
            // std::cout << ((s[0] < 'A') || (s[0] > 'F')) << " ";
            // std::cout << (((s[0] < '0') || (s[0] > '9')) && ((s[0] < 'A')||(s[0] > 'F'))) << " ";
            // std::cout << "\"" << s[i] << "\"\n";
            return false;
        
        }
    }

    return true;
}

bool isDec(const std::string& s) {
    for (int i = 0; i < s.length(); i++) {
        if ((s[i] < '0') || s[i] > '9') return false;
    }
    return true;
}

bool isHex(const std::string& s) {
    int i = 0;
    if (s[0] == '$') {
        i++;
    }

    for (i; i < s.length(); i++) {
        if (((s[i] < '0') || (s[i] > '9')) && ((s[i] < 'A')||(s[i] > 'F')))
            return false;
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

uint16_t calculateChecksum(uint16_t addr, uint16_t size, std::vector<uint8_t> data) {
    uint16_t chks = 0;
    
    chks += (addr >> 8);
    // std::cout << std::hex << chks << "\n";
    chks += (addr & 0xFF);
    // std::cout << std::hex << chks << "\n";

    chks += (size & 0xFF);
    // std::cout << std::hex << chks << "\n";

    for (int i = 0; i < size; i++) {
        chks += (uint16_t)data[i];
        // std::cout << std::hex << chks << "\n";
    }
    

    // One's complement
    chks = 0xFFFF - chks;
    
    // Isolate last byte
    chks = chks & 0x00FF;
    
    return chks;
}

void GotoLine(std::ifstream* file, unsigned int num) {
    (*file).seekg(std::ios::beg);
    std::string trash;
    for (uint line = 0; line < num; line++) {
        char c = 'a';
        while (c != '\n') {
            c = (*file).get();
        }
    }
    return;
}