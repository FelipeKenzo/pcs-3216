#include "../include/CommandLineInterpreter.h"

CommandLineInterpreter::CommandLineInterpreter(std::string scriptFile) {

    std::map<std::vector<int>, int> routinesTable = {
        {{2,3},0},  {{4,5},1},  {{1,6},2},  {{7,8},3},
        {{9,10},4}, {{11,12},5}, {{13,14},6}, {{15,16},7}, 
        {{17,18},8}, {{19,20},9}, {{21,22},10}
    };

    std::map<std::string, int> state0 = {
        {"$", 1} 
    };

    std::map<std::string, int> state1 = {
        {"JOB", 2}, {"DISK", 4}, {"DIRECTORY", 6}, {"CREATE", 7},
        {"DELETE", 9}, {"LIST", 11}, {"INFILE", 13}, {"OUTFILE", 15},
        {"DISKFILE", 17}, {"RUN", 19}, {"ENDJOB", 21} 
    };

    std::map<std::string, int> state2 = {
        {"Id", 3} 
    };

    std::map<std::string, int> state3 = { 
    };

    std::map<std::string, int> state4 = {
        {"Id", 5} 
    };

    std::map<std::string, int> state5 = {
    };

    std::map<std::string, int> state6 = {
    };

    std::map<std::string, int> state7 = {
        {"Id", 8} 
    };

    std::map<std::string, int> state8 = {
    };

    std::map<std::string, int> state9 = {
        {"Id", 10} 
    };
    std::map<std::string, int> state10 = {
    };

    std::map<std::string, int> state11 = {
        {"Id", 12} 
    };

    std::map<std::string, int> state12 = { 
    };

    std::map<std::string, int> state13 = {
        {"Id", 14} 
    };

    std::map<std::string, int> state14 = { 
    };

    std::map<std::string, int> state15 = {
        {"Id", 16} 
    };

    std::map<std::string, int> state16 = { 
    };

    std::map<std::string, int> state17 = {
        {"Id", 18} 
    };

    std::map<std::string, int> state18 = { 
    };

    std::map<std::string, int> state19 = {
        {"Id", 20} 
    };

    std::map<std::string, int> state20 = { 
    };

    std::map<std::string, int> state21 = {
        {"Id", 22} 
    };

    std::map<std::string, int> state22 = {
    };


    std::vector<std::map<std::string, int>> transitionTable = {
        state0, state1, state2, state3, state4, state5, state6,
        state7, state8, state9, state10, state11, state12,
        state13, state14, state15, state16, state17, state18,
        state19, state20, state21, state22
    };

    std::vector<std::string> keywords = {
        "JOB",
        "DISK",
        "DIRECTORY",
        "CREATE",
        "DELETE",
        "LIST",
        "INFILE",
        "OUTFILE",
        "DISKFILE",
        "RUN",
        "ENDJOB"
    };

    std::ifstream src;
    src.open(scriptFile);
    std::string lineRead;

    std::vector<std::string>* vectorSrc = new std::vector<std::string>();

    while (std::getline(src, lineRead)) {
        (*vectorSrc).push_back(lineRead);
    }

    e = new CliEventTreater();
    dm = new DecisionMaker(routinesTable, e);
    r = new Recognizer(transitionTable, dm);
    c = new Classifier(vectorSrc, "listing.txt", keywords, r, e);
    c->insertEvent(0);
}

void CommandLineInterpreter::executeScript() {
    while (!c->eof && !r->syntaxError) {
        c->execute();
        r->execute();
        dm->execute();
        e->execute();
    }
}