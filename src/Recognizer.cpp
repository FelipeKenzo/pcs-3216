#include "../include/Recognizer.h"

Recognizer::Recognizer(std::vector<std::map<std::string, int>> transitionTable,  DecisionMaker* dm) : 
    transitionTable(transitionTable), dm(dm) {

    commandSequence = new std::vector<std::string>();
}

void Recognizer::processToken() {
    // transition state
    std::map<std::string,int>::iterator it;
    
    if (t.sequence == ";") {
        requestingToken = true;
        insertEvent(2);
        return;
    }

    it = transitionTable[state].find(t.sequence);
    
    // didn't find sequence in transition table
    if (it == transitionTable[state].end()) {
        if (t.type == integer) {
            it = transitionTable[state].find("Num");
        }
        else if (t.type == identifier) {
            it = transitionTable[state].find("Id");
        }
    }

    // There was a match
    if (it != transitionTable[state].end()) {
        (*commandSequence).push_back(t.sequence);
        
        transition tr = {
            {state,(*it).second},
            t
        };
        dm->receiveState(tr);

        requestingToken = true; // requests next token
        state = (*it).second;
        
        // Recognizes an end state, restarts automaton
        if (transitionTable[state].empty()) {
            insertEvent(2);
        }
    }
    else {
        insertEvent(1); // error message
    }

}

void Recognizer::printError() {
    std::cerr << "Syntax error at line (" << t.line << ").\n";
    syntaxError = true;
}

void Recognizer::restartAutomaton() {
    transition tr = {
            {state, 0},
            t
        };
    
    (*commandSequence).clear();
    state = 0;
    dm->receiveState(tr);
}

void Recognizer::receiveToken(token t) {
    this->t = t;
    requestingToken = false;
    insertEvent(0); // process received token
    return;
}

bool Recognizer::isRequestingToken() {
    return requestingToken;
}

void Recognizer::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: processToken();
            break;
        case 1: printError();
            break;
        case 2: restartAutomaton();
            break;
        default:
            break;
    }

    eventQueue.pop();
}