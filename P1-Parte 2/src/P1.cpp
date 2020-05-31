#include "../include/P1.h"

P1::P1(std::string T) {
    this->T.open(T);
    if (!this->T.is_open()) {
        std::cerr << "Could not open file.\n";
    }

}

void P1::setReferences(P2* p2, P6* p6) {
    this->p2 = p2;
    this->p6 = p6;
}

void P1::readLine() {
    if (std::getline(T, currentLine)) {
        cindex = 0;
        lineNumber++;
        //std::cout << "P1: leu: " << currentLine << std::endl;
        currentLine += "\n";
        p6->addToQueue((std::to_string(lineNumber) + " " + currentLine), false);
        std::transform(currentLine.begin(), currentLine.end(), currentLine.begin(), std::ptr_fun<int, int>(std::toupper));
        insertEvent(1);
    }
    else { // End of file
        p2->insertEvent(1); // tells P2 to notify P3
    }
    
}

void P1::readCharacter() {
    bool fol = (cindex == 0);

    if (cindex >= currentLine.length()) {
        insertEvent(0);
        return;
    }


    char c = currentLine[cindex++];
    if (c == ';') {
        insertEvent(0);
        return;
    }
    bool isa = isAlphabetical(c);

    // sends char to p2
    p2->receiveChar(fol, isa, c, lineNumber);
    
}

bool P1::isAlphabetical(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
}

void P1::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: readLine();
            break;
        case 1: readCharacter();
            break;
        default:
            break;
    }

    eventQueue.pop();
}