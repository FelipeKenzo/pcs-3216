#ifndef HIGHLEVELINTERPRETER_H
#define HIGHLEVELINTERPRETER_H

#include <string>
#include <vector>
#include <fstream>

#include "../include/Classifier.h"
#include "../include/Recognizer.h"
#include "../include/DecisionMaker.h"
#include "../include/HliEventTreater.h"

class HighLevelInterpreter {
public:
    HighLevelInterpreter(std::string script, std::ifstream* infile, std::ofstream* outfile);
    void executeScript();
private:

    HliEventTreater* e;
    DecisionMaker* dm;
    Recognizer* r;
    Classifier* c;
};

#endif