#include "../include/P4.h"

P4::P4(std::map<std::string, std::list<int>*>* refTable) : refTable(refTable) {
    //ctor
}

void P4::setReferences(P5* p5) {
    this->p5 = p5;
}

void P4::receiveSequence(bool DEF, std::string seq, int line) {
    this->DEF = DEF;
    alphabeticalSequence = seq;
    if (DEF) lineNumber = -line;
    else lineNumber = line;


    insertEvent(0);
}

void P4::notifyEndOfT() {
    p5->insertEvent(0); // Tells P5 to order the table
}

void P4::processSequence() {
    std::map<std::string, std::list<int>*>::iterator it;

    // Searches for the sequence in the table
    it = (*refTable).find(alphabeticalSequence);
    if (it != (*refTable).end()) {
        //std::cout << "P4: ref achada para " << alphabeticalSequence << "\n";
        // Add new usage line
        (*it->second).push_back(lineNumber);
    }
    else {
        // Adds new entry to refTable
        //std::cout << "P4: ref criada para " << alphabeticalSequence << "\n";
        std::list<int>* lines = new std::list<int>();
        (*lines).push_back(lineNumber);
        (*refTable)[alphabeticalSequence] = lines;
    }
}

void P4::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: processSequence();
            break;
        case 1: notifyEndOfT();
            break;
        default:
            break;
    }

    eventQueue.pop();
}