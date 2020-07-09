#ifndef COMMANDLINEINTERPRETER
#define COMMANDLINEINTERPRETER

#include <string>
#include <vector>
#include <fstream>

#include "../include/Interface.h"
#include "../include/VonNeumannMachine.h"
#include "../include/Assembler.h"

#include "../include/Classifier.h"
#include "../include/Recognizer.h"
#include "../include/DecisionMaker.h"
#include "../include/CliEventTreater.h"

class CommandLineInterpreter {
public:
    CommandLineInterpreter(std::string scriptFile);
    void executeScript();
private:
    CliEventTreater* e;
    DecisionMaker* dm;
    Recognizer* r;
    Classifier* c;
};

#endif