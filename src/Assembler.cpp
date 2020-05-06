#include "../include/Assembler.h"

Assembler::Assembler(std::string src, std::string out, bool warnings) : 
    srcFilename(src), outFilename(out), warnings(warnings) {
    //ctor
}

Assembler::~Assembler() {
    //dtor
}

bool Assembler::isValidLabel(const std::string& s) {
    // Label must start with either '_' or letter.
    if (((s[0] < 'A') || (s[0] > 'Z')) &&  (s[0] != '_')) return false;
    
    // Following characters need to be alphanumeric, '-' or '_'.
    for (int i = 1; i < s.length()-1; i++) {
        char c = s[i];
        if ((c < '0') || (c > '9')) {         // Not digit
            if ((c < 'A') || (c > 'Z')) {     // Not letter
                if ((c != '-') && (c != '_')) // Not '-' or '_'
                    return false;
            }
        }
    }

    // Must end with ':'
    if (s[s.length()-1] != ':') return false;

    return true;
}

bool Assembler::isValidMnemonic(const std::string& s) {
    std::map<std::string,mnemonicData>::const_iterator it = mTable.find(s);

    if (it == mTable.end()) {
        return false;
    }

    return true;
}

void Assembler::printUsage() {
    std::cerr << "Usage: assembler <option(s)> source\n\n"
              << "Options: -o <destination>\n"
              << "         -v"
              << std::endl;
}

void Assembler::printSTable() {
    std::cout << "Symbol Table:\n";
    
    std::cout << std::internal << std::setfill('0');
    
    std::for_each(sTable.begin(), sTable.end(),
        [](std::pair<std::string, symbolData > element)
        {
            std::cout << "0x" << std::setw(3) <<  std::hex << element.second.address << ": " << element.first << "\n";
        });
    std::cout << "\n";
}

bool Assembler::firstStep(std:: ifstream& src) {
    // Utils
    uint16_t lc = 0;                  // Line Counter
    uint16_t pc = 0;                  // Instruction Counter
    
    std::string line;                 // Current line
    std::vector<std::string> words;   // Vector with words from line
    
    std::regex regComment("(;.*)");   // Matches a comment
    
    bool isEnd = false;               // Program End flag
    bool isError = false;             // Error flag
    bool hasLabel = false;

    while (getline(src, line) && !isEnd) {
        isEnd = false;
        hasLabel = false;
        lc++;

        // Line pre-processing:
        line = std::regex_replace(line, regComment, "");                    // Remove Comments
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);  // Uppercase line
        words = split(line, ' ');                                           // Extract word vector

        // Skip empty lines:
        if (words.size() == 0) continue;

        // There should only be two or three words: 
        if (words.size() > 3) {
            if (warnings) std::cerr << "Warning(" << lc << "): too many operands.\n";
        }

        // If firs word is a label, it ends with "1":
        if (words[0].back() == ':') {
            hasLabel = true;
            std::string newLabel = words[0];

            if (!isValidLabel(newLabel)) {
                std::cerr << "Error(" << lc << "): invalid label.\n";
                isError = true;
                continue;
            }

            // Now we remove the ':' at the end:
            newLabel.pop_back();

            // Is it already in the Symbol Table?
            std::map<std::string,symbolData>::const_iterator it = sTable.find(newLabel);

            if (it == sTable.end()) {
                
                // It isn't, then we add it to the Symbol Table:
                symbolData data = {pc, lc, true, false};
                sTable[newLabel] = data;

                // And remove it from word vector:
                words.erase(words.begin()); // It is inefficient, but our vector is small.

                //std::cout << labelName << "at line " << lc << "\n";
            }
            else {
                // It could be used but still not defined.
                if (it->second.isDefined) {
                    std::cerr << "Error(" << lc << "): label redefined , previously defined at line ("
                            << it->second.line << ").\n";
                    isError = true;
                continue;
                }
                else
                {
                    // In this case, we have to update the label entry:
                    sTable[newLabel].isDefined = true;
                    sTable[newLabel].address =  pc;
                    sTable[newLabel].line = lc;

                    // And remove it from word vector:
                    words.erase(words.begin()); // It is inefficient, but our vector is small.
                }
                
            }
        }

        // Next word should be a mnemonic:
        std::string mnemonic = words[0];

        if (!isValidMnemonic(mnemonic)) {
            std::cerr << "Error(" << lc << "): invalid mnemonic \"" << mnemonic << "\".\n";
            isError = true;
            continue;
        }

        // Check if instruction is at valid address
        if (mTable[mnemonic].size > 0) {
            if (pc > 0xFFF) {
                std::cerr <<"Error(" << lc << "): invalid instruction address (0x" << std::hex << pc << ").\n";
                continue;
            }
        }

        if (mnemonic == "END") {
            isEnd = true; // Important to end step 1.
            if (hasLabel) {
                std::cerr <<"Error(" << lc << "): END cannot have label.\n";
                isError = true;
            }
        }
        if (mnemonic == "ORG" && hasLabel) {
            std::cerr <<"Error(" << lc << "): ORG cannot have label.\n";
            isError = true;
        }

        // Does it require an operand?
        if(mTable[mnemonic].param) {
            // There needs to be an operand:
            if (words.size() < 2) {
                std::cerr << "Error(" << lc << "): instruction \"" << mnemonic << "\" requires an operand.\n";
                isError = true;
                continue;
            }

            std::string operand = words[1];

            // Is the operand a number?
            if (isNumber(operand)) {
                
                uint32_t addr;
                if (operand[0] == '$') addr = htoi(operand);  // operand in HEX
                else addr = std::stoi(operand);               // operand in DEC

                // Checks if valid operand                
                if (addr > 0xFFF) {
                    std::cerr << "Error(" << lc << "): invalid operand value.\n";
                    isError = true;
                    continue;
                }

                // If it is, it could be the ORG operand. Then, we update PC.
                if (mnemonic == "ORG") pc = addr;

            }
            // if not, it is a label:
            else {
                // Sees if it is already defined:
                std::map<std::string,symbolData>::const_iterator it = sTable.find(operand);

                if (it == sTable.end()) {
                    // It wasnt, then we add it to the Symbol Table;
                    symbolData data = {0, 0, false, true};
                    sTable[operand] = data;
                }
                else {
                    // It was, then we set referenced to true;
                    sTable[operand].isReferenced = true;
                }

                // ORG cannot receive a label as operand.
                if (mnemonic == "ORG") {
                    std::cerr << "Error(" << lc << "): ORG cannot have a label.\n";
                    isError = true;
                    continue;
                }
            }
        }
            
        pc += mTable[mnemonic].size;
        //std::cout << "\n";

    }

    // Check if every label was defined and referenced
    bool undefinedLabel = false;

    std::map<std::string, symbolData>::iterator it;

    for (it = sTable.begin(); it != sTable.end(); it++) {
        if (it->first.length() > labelSize) labelSize = it->first.length();
        if (!it->second.isDefined) {
            std::cerr << "Error(" << it->second.line << "): label \"" << it->first << "\" used but not defined.\n";
            isError = true;
        }
        if (!it->second.isReferenced) {
            if (warnings) std::cerr << "Warning(" << it->second.line << "): label \"" << it->first << "\" is defined but not referenced.\n";
        }
    }
    
    return(!isError);
}

void Assembler::secondStep(std::ifstream& src, std::ofstream& lst, std::ofstream& vnc) {

    //if (!lst.is_open()) std::cout << "lst not open\n";
    //if (!vnc.is_open()) std::cout << "vnc not open\n";

    // Utils
    uint16_t lc = 0;                  // Line Counter
    uint16_t pc = 0;                  // Instruction Counter
    
    std::string line;                 // Current line
    std::vector<std::string> words;   // Vector with words from line
    
    std::regex regComment("(;.*)");   // Matches a comment
    
    bool isEnd = false;               // Program End flag
    bool isError = false;             // Error flag

    lst << "Listing for source file \"" << srcFilename << "\".\n";
    lst << "Note: memory is Little-Endian.\n\n";

    // Hex output variables
    std::vector<uint8_t> data;
    uint16_t addr = 0;
    uint16_t size = 0;
    uint16_t chks = 0;

    while (getline(src, line) && !isEnd) {
        isEnd = false;
        lc++;
        
        // Listing output variables
        std::string comment  = "";
        std::string label    = "";
        std::string mnemonic = "";
        std::string operand  = "";
        
        // Save Comment
        std::smatch match;
        std::regex_search(line, match, regComment);
        comment = match[1];

        // Line pre-processing:
        line = std::regex_replace(line, regComment, "");                    // Remove Comments
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);  // Uppercase all words
        words = split(line, ' ');                                           // Get word vector

        if (words.size() == 0 ) continue;                                   // ignore blank lines

        // save and delete labels
        if (words[0].back() == ':') {
            label = words[0];
            label.pop_back();
            words.erase(words.begin());
        }

        // Extract mnemonic
        mnemonic = words[0];

        if (words.size() > 1) {
            // Extract operand if it exists
            operand  = words[1];
        }

        // Change PC on ORG
        if (mnemonic == "ORG") {
            if (operand[0] == '$') pc = htoi(operand); // operand in HEX
            else pc = std::stoi(operand);              // operand in DEC
        }

        if (mnemonic == "END") isEnd = true;

        // Calculate machine code
        if (mnemonic == "CON") {
            if (isNumber(operand)) {
                if (operand[0] == '$') data.push_back(htoi(operand));
                else data.push_back(std::stoi(operand));
            }
        }

        else if (mTable[mnemonic].size == 2) {
            data.push_back(mTable[mnemonic].opcode << 4);
            
            if (isNumber(operand)) {
                if (operand[0] == '$') {
                    data[data.size()-1] |= (htoi(operand) >> 8);
                    data.push_back(htoi(operand) & 0xFF);
                }
                else {
                    data[data.size()-1] |= (std::stoi(operand) >> 8);
                    data.push_back(std::stoi(operand) & 0xFF);
                }
            }
            // label as operand
            else {
                data[data.size()-1] |= sTable[operand].address >> 8;
                data.push_back(sTable[operand].address & 0xFF);
            }
        }
        
        // Output to lst file:
        // If it doensn't occupy memory space:
        if (mTable[mnemonic].size == 0) {
            // Blank space for address and code
            lst << "            ";
            
            // Blankspace for label
            for (int i = 0; i < labelSize; i++) {
                lst << " "; 
            }
            lst << "  ";
        }
        else if (mTable[mnemonic].size == 1) {
            // Output memory location
            lst << std::right << std::setfill('0') << std::setw(3) << std::uppercase
                << std::hex  << pc << " ";
            // Output machine code
            lst << std::setw(2) << (uint)data[data.size()-1] << "      "; // First Byte

            // Output label;
            lst << std::right << std::setfill(' ') << std::setw(labelSize) << label << "  "; 
        }
        else if (mTable[mnemonic].size == 2) {
            // Output memory location
            lst << std::right << std::setfill('0') << std::setw(3) << std::uppercase
                << std::hex  << pc << " ";

            // Output machine code
            lst << std::setw(2) << (uint)data[data.size()-2] << " ";    // First Byte
            lst << std::setw(2) << (uint)data[data.size()-1] << "   ";  // Second Byte
            
            // Output label;
            lst << std::right << std::setfill(' ') << std::setw(labelSize) << label << "  "; 
        }

        // Output mnemonic
        lst << std::left << std::setfill(' ') << std::setw(3) << mnemonic << "  ";

        // Output operand
        lst << std::setw(labelSize) << operand << " ";

        // Output comment
        lst << comment << "\n";

        // Only real instructions are generated
        if (mTable[mnemonic].size > 0) {
            if (addr == 0) {
                addr = pc;
            }
        }
        
        // End block and write to vnc file
        if ((data.size() >= 15 || mnemonic == "ORG" || isEnd) && addr) {
            //std::cout << "ENTRI\n";
            //std::cout << size << "\n";
            if (data.size() > 15) size = 15;
            else size = data.size();
            vnc << std::right << std::setfill('0') <<  std::uppercase
                << std::setw(4) << std::hex  << addr  // write address
                << std::setw(2) << size;              // write size

            for (int i = 0; i <= size; i++) {
                vnc << std::setw(2) << std::hex << (uint16_t)data[i];
            }

            // 
            data.erase(data.begin(), data.begin() + size);

            //chks = calculateCheksum(addr, size, data);
            
            // Write checksum
            // vnc << std::right << std::setfill('0') <<  std::uppercase
            //     << std::setw(2) << std::hex  << chks << "\n";
            
            chks = 0;
            addr = 0;
            size = 0;
        }

        pc += mTable[mnemonic].size;
    }
    vnc << "0000";

    return;
}

bool Assembler::assemble() {
    // Open source file
    std::ifstream src;
    src.open(srcFilename);

    // Check if file opened
    if (!src.is_open()) {
        std::string fileName = eraseSubStr(fileName, "./filesystem/");
        std::cerr << "Could not open \"" << fileName << "\". Aborting.\n";
        return (false);
    }

    if (!firstStep(src)) {
        src.close();
        return false;
    }

    // Go back to start of file
    src.clear();
    src.seekg(0, std::ios::beg);

    // Open listing output file
    std::ofstream lst;
    lst.open(outFilename + ".lst");

    // Do first step
    if (!lst.is_open()) {
        std::cerr << "Could not create \"" << outFilename << ".lst\"; Exiting program.\n";
        return(false);
    }

    // Open hex output file
    std::ofstream vnc;
    vnc.open(outFilename + ".vnc");

    if (!vnc.is_open()) {
        std::cerr << "Could not create \"" << outFilename << ".o\"; Exiting program.\n";
        return(false);
    }

    // Do second step
    secondStep(src, lst, vnc);
    
    src.close();
    lst.close();
    vnc.close();
    
    return true;
    
}

