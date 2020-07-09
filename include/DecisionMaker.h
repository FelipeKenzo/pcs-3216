#ifndef DECISIONMAKER_H
#define DECISIONMAKER_H

#include <map>

#include "EventMotor.h"
#include "Structs.h"

#include "EventTreater.h"

class DecisionMaker : public EventMotor {
public:
    DecisionMaker(std::map<std::vector<int>, int> routinesTable, EventTreater* e);

    // ===============
    // == Interface ==
    // ===============
    
    void execute();
    void receiveState(transition t);

private:
    // ================
    // == Attributes ==
    // ================

    EventTreater* e;

    transition t;
    std::map<std::vector<int>, int> routinesTable;

    // ========================
    // == Treatment routines ==
    // ========================

    void decideRoutine();
};

#endif