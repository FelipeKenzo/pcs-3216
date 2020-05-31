#ifndef P4_H
#define P4_H

#include <map>
#include <list>
#include "EventMotor.h"

#include "P5.h"

class P4 : public EventMotor {
private:
    bool DEF = false;
    std::string alphabeticalSequence = "";
    int lineNumber = 0;

    std::map<std::string, std::list<int>*>*  refTable;
    P5* p5;

    void processSequence();
    void notifyEndOfT();
public:
    P4(std::map<std::string, std::list<int>*>*  refTable);
    void setReferences(P5* p5);

    void receiveSequence(bool DEF, std::string seq, int line); 
    void execute();
    void receiveEndOfT();
};

#endif