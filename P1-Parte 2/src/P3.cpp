#include "../include/P3.h"

P3::P3() {

}

void P3::setReferences(P4* p4) {
    this->p4 = p4;
}

void P3::sendSequence() {
    //std::cout << "P3: enviando: " << sequence << "\n";
    p4->receiveSequence(firstOfLine, sequence, lineNumber);
}

void P3::receiveSequence(bool fol, std::string seq, int line) {
    firstOfLine = fol;
    sequence = seq;
    lineNumber = line;

    insertEvent(0);
}

void P3::notifyEndOfT() {
    p4->insertEvent(1); // Tells P4 to notify P5
}

void P3::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: sendSequence();
            break;
        case 1: notifyEndOfT();
            break;
        default:
            break;
    }

    eventQueue.pop();
}