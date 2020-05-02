#include "../include/Loader.h"

void Loader::load(VonNeumannMachine* vnm, std::string filepath) {
    std::ifstream vnc;
    vnc.open(filepath);

    if(!vnc.is_open()) {
        std::cerr << "load: could not open \"" << filepath << "\".\n";
        return;
    }

    std::string line;
    while(getline(vnc, line)) {
        uint16_t start = 0;
        uint16_t size = 0;
        uint64_t data = 0;
        uint16_t chks = 0;

        //std::cout << line.length() << "\n";
        if (line.length() < 10) {
            std::cerr << "load: file \"" << filepath << "\" is corrupted.\n";
            return;
        }

        //std::cout << isNumber(line) << "\n";
        if (!isNumber(line)) {
            std::cerr << "load: file \"" << filepath << "\" is corrupted.\n";
            return;
        }

        //std::cout << line << "\n";

        start = htoi(line.substr(0,3));
        size  = htoi(line.substr(3,1));
        data  = htoi(line.substr(4,size));
        chks  = htoi(line.substr(line.length()-2, 2));

        // std::cout << line << "\n";

        // std::cout << std::hex << start << "\n";
        // std::cout << std::hex << size  << "\n";
        // std::cout << std::hex << data  << "\n";
        // std::cout << std::hex << chks  << " and " << std::hex << calculateCheksum(start, size, data) << "\n";

        if (chks != calculateCheksum(start, size, data)) {
            std::cerr << "load: file \"" << filepath << "\" is corrupted.\n";
            return;
        }

        for (int i = 0; i < size; i += 2) {
            //std::cout << std::hex << start << "\n";
            start += i;
            vnm->memWrite_w(start, (data >> (8*i)) & 0xFFFF);
        }
    }
}