#include "../include/P2.h"

P2::P2() {
    // ctor
}

void P2::setReferences(P1* p1, P3* p3) {
    this->p1 = p1;
    this->p3 = p3;
}

void P2::receiveChar(bool fol, bool isa, char c, int line){
    if (fol) {
        firstOfLine = fol;
        wordNumber = 1;
        labeledLine = false;
    }
    isAlphabetic = isa;
    charToBeProcessed = c;
    lineNumber = line;
    insertEvent(0);
    // std::cout << "P2: recebeu caractere " << charToBeProcessed << "\n";
}

void P2::notifyEndOfT() {
    p3->insertEvent(1); // Tells P3 to notify P4
}

void P2::processChar() {
    if (isAlphabetic) {
        charSequence += charToBeProcessed;
        // std::cout << "P2: caractere: " << charToBeProcessed << "\n";
    }
    else {
        std::cout << "P2: sequencia: " << charSequence << " numero: " << wordNumber << "fol: " << firstOfLine <<"\n";
        
        if (charSequence != "") {
            if (firstOfLine && wordNumber == 1) labeledLine = true;

            if (!(!labeledLine && wordNumber == 1)) // can't be mnemonic 
                if (!(labeledLine && wordNumber == 2)) // can't be mnemonic
                    p3->receiveSequence(firstOfLine, charSequence, lineNumber);
            wordNumber++;
        }
        charSequence = "";
        firstOfLine = false;
        isAlphabetic = false;
    }
    p1->insertEvent(1); // asks for next char
}

void P2::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: processChar();
            break;
        case 1:  notifyEndOfT();
            break;
        default:
            break;
    }

    eventQueue.pop();
}