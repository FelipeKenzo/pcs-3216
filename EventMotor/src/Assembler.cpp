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
                if ((c != '-') || (c != '_')) // Not '-' or '_'
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
        line = std::regex_replace(line, regComment, ""); // Remove Comments
        line = uppercase(line);                          // Uppercase all words
        words = split(line, ' ');                        // Extract word vector

        // Skip empty lines:
        if (words.size() == 0) continue;

        // There should only be two or three words: 
        if (words.size() > 3) {
            if (warnings) std::cerr << "Warning(" << lc << "): too many operands.\n";
        }

        // If more than two words, first is a label:
        if (words.size() > 2) {
            hasLabel = true;
            std::string newLabel = words[0];
            //std::cout << newLabel << "\n";

            if (!isValidLabel(newLabel)) {
                std::cerr << "Error(" << lc << "): invalid label .\n";
                isError = true;
                continue;
            }

            // Now we remove the ':' at the end:
            newLabel.pop_back();

            // Was it already defined?
            std::map<std::string,symbolData>::const_iterator it = sTable.find(newLabel);

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
                std::cerr << "Error(" << lc << "): label redefined , previously defined at line ("
                          << it->second.line << ").\n";
                isError = true;
                continue;
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
                
                int16_t addr;
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
                    symbolData data = {pc, lc, false, true};
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
    uint16_t code = 0;
    uint16_t addr = 0;
    uint16_t size = 0;
    uint64_t data = 0;
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
        line = std::regex_replace(line, regComment, ""); // Remove Comments
        line = uppercase(line);                          // Uppercase all words
        words = split(line, ' ');                        // Get word vector

        if (words.size() == 0 ) continue;                // ignore white lines

        // save and delete labels
        if (words.size() > 2) {
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
        if (mTable[mnemonic].size == 2) {
            code = mTable[mnemonic].opcode << 12;

            if (isNumber(operand)) {
                if (operand[0] == '$') code = code | htoi(operand);
                else code = code | std::stoi(operand);
            }
            else {
                code = code | sTable[operand].address;
            }

            // To Little Endian
            code = (code << 8) | (code >> 8);
        }

        // Output to vnc file:
        // Only real instructions are generated
        if (mTable[mnemonic].size == 2) {
            // Write address if not written yet;
            // std::cout << (addr == 0) << "\n";
            if (addr == 0) {
                addr = pc;
                //std::cout << "addr = " << std::hex << addr << "\n";
            }
            // fill data
            data = (data << 16) | code;
            size += 4;
            //std::cout << "data = " <<std::setfill('0') << std::setw(16) << std::hex << data << "\n";
        }
        
        // End block and write to vnc file
        if ((size == 16 || mnemonic == "ORG" || isEnd) && addr) {
            //std::cout << "ENTRI\n";
            //std::cout << size << "\n";
            vnc << std::right << std::setfill('0') <<  std::uppercase
                << std::setw(3) << std::hex  << addr // write address
                << std::setw(1) << size              // write size
                << std::setw(size) << data;          // write data

            // Calculate checksum
            // Sum all nibbles
            while (addr) {
                chks += addr & 0xF;
                addr = addr >> 4;
            }
            while(size) {
                chks += size & 0xF;
                size = size >> 4;
            }
            while (data) {
                chks += data & 0xF;
                data = data >> 4;
            }
            
            //std::cout << std::hex << chks << "\n";

            // One's complement
            chks = 0xFFFF - chks;
            // Isolate last byte
            chks = chks & 0x00FF;
            
            // Write checksum
            vnc << std::right << std::setfill('0') <<  std::uppercase
                << std::setw(2) << std::hex  << chks << "\n";
            
            chks = 0;
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
        else {
            // Output memory location
            lst << std::right << std::setfill('0') << std::setw(3) << std::uppercase
                << std::hex  << pc << " ";

            // Output machine code
            lst << std::setw(2) << (code >> 8) << " ";       // First Byte
            lst << std::setw(2) << (code & 0x00FF) << "   "; // Second Byte
            
            // Output label;
            lst << std::right << std::setfill(' ') << std::setw(labelSize) << label << "  "; 
        }

        // Output mnemonic
        lst << std::left << std::setfill(' ') << std::setw(3) << mnemonic << "  ";

        // Output operand
        lst << std::setw(labelSize) << operand << " ";

        // Output comment
        lst << comment << "\n";

        pc += mTable[mnemonic].size;
    }

    return;
}

bool Assembler::assemble() {
    // Open source file
    std::ifstream src;
    src.open(srcFilename);

    // Check if file opened
    if (!src.is_open()) {
        std::cerr << "Could not open \"" << srcFilename << "\". Aborting.\n";
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

