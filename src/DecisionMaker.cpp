#include "../include/DecisionMaker.h"

DecisionMaker::DecisionMaker(std::map<std::vector<int>, int> routinesTable, EventTreater* e) : 
    routinesTable(routinesTable), e(e) {

}

void DecisionMaker::decideRoutine() {
    std::map<std::vector<int>, int>::iterator it;
    it = routinesTable.begin();
    
    while (it != routinesTable.end()) {
        if ((*it).first[0] == t.states[0]){
            if((*it).first[1] == t.states[1]) {
                treatment routine = {
                    (*it).second,
                    t.t
                };
                
                e->receiveRoutine(routine);
                return;
            }
        }
        it++;
    }
}

void DecisionMaker::receiveState(transition t) {
    this-> t = t;
    insertEvent(0);
}

void DecisionMaker::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: decideRoutine();
            break;
        default:
            break;
    }

    eventQueue.pop();
}