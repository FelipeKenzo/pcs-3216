#ifndef P6_H
#define P6_H

#include <fstream>
#include <string>

#include "EventMotor.h"

class P6 : public EventMotor {
private:
    std::queue<std::string> printQueue;
    std::ofstream listing;
    std::ofstream report;

    void printToListing();
    void printToReport();
public:
    P6(std::string listingPath, std::string reportPath);
    void addToQueue(const std::string& toPrint, bool toReport);
    void execute();
};

#endif