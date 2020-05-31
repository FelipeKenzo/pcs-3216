#include "../include/P6.h"

P6::P6(std::string listingPath, std::string reportPath) {
    listing.open(listingPath);
    if (!listing.is_open()) std::cerr << "could not open listing.\n";
    
    report.open(reportPath);
    if (!report.is_open()) std::cerr << "could not open report.\n";
}

void P6::addToQueue(const std::string& toPrint, bool toReport) {
    printQueue.push(toPrint);
    //std::cout << "printing: " << toPrint << "\n";
    if (toReport) insertEvent(1);
    else insertEvent(0);
}

void P6::printToListing() {
    listing << printQueue.front();
    listing.flush();
    printQueue.pop();
}

void P6::printToReport() {
    report << printQueue.front();
    report.flush();
    printQueue.pop();
}

void P6::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: printToListing();
            break;
        case 1: printToReport();
            break;
        default:
            break;
    }

    eventQueue.pop();
}