#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include <vector>
#include <map>

#include "EventMotor.h"
#include "Structs.h"

#include "DecisionMaker.h"

class Recognizer : public EventMotor {
public:
    Recognizer(std::vector<std::map<std::string, int>> transitionTable, DecisionMaker* dm);

    // ===============
    // == Interface ==
    // ===============

    void execute();
    void receiveToken(token t);
    bool isRequestingToken();
    bool syntaxError = false;
private:
    // ================
    // == Attributes ==
    // ================

    DecisionMaker* dm;

    int state = 0;
    token t;
    bool requestingToken = true;
    
    std::vector<std::map<std::string, int>> transitionTable;
    std::vector<std::string>* commandSequence;

    // ========================
    // == Treatment routines ==
    // ========================
    void processToken();    // 0
    void printError();      // 1
    void restartAutomaton(); // 2

};

#endif