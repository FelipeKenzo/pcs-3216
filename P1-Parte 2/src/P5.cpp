#include "../include/P5.h"

P5::P5(std::map<std::string, std::list<int>*>*  refTable) : refTable(refTable){
    // ctor
}

void P5::setReferences(P0* p0, P6* p6) {
    this->p0 = p0;
    this->p6 = p6;
}

void P5::orderTable() {
    // a map is already ordered.
    // std::cout << "P5: ordering table\n";
    insertEvent(1); // to print table
}

void P5::printTable() {
    std::map<std::string, std::list<int>*>::iterator it = (*refTable).begin();
    
    while (it != (*refTable).end()) {
        bool wasDefined = false;
        std::string lineToPrint = it->first;
        std::list<int>::iterator defLine = (*it->second).begin();
        while (defLine != (*it->second).end()) {
            if ((*defLine) < 0) {
                lineToPrint += (" " + std::to_string(-(*defLine)) + "*");
                wasDefined = true;
            }  
            else {
                lineToPrint += (" " + std::to_string(*defLine));
            }
            defLine++; 
        }
        lineToPrint += "\n";
        
        if (wasDefined) {
            p6->addToQueue(lineToPrint, true);
            insertEvent(-1); // "NOP" to wait for p6
        }
        it++;
    }
    insertEvent(2); // Signals end to p0
}

void P5::signalEnd() {
    p0->stop();
}

void P5::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: orderTable();
            break;
        case 1: printTable();
            break;
        case 2: signalEnd();
            break;
        default:
            break;
    }

    eventQueue.pop();
}