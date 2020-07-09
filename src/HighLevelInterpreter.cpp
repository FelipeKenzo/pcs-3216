#include "../include/HighLevelInterpreter.h"

HighLevelInterpreter::HighLevelInterpreter(std::string scriptFile, std::ifstream* infile, std::ofstream* outfile) {
    std::map<std::vector<int>,int> routinesTable = {
        {{0,13}, 0}, {{1,2}, 3}, {{2,3}, 6}, {{3,4}, 7},
        {{4,3}, 9}, {{0,3}, 5}, {{7,8}, 1}, {{8,0}, 2},
        {{8,9}, 11}, {{9,10}, 7}, {{10,9}, 9}, {{10,11}, 10},
        {{11,12}, 7}, {{12, 11}, 9}, {{12,0}, 10}, {{4,0}, 10},
        {{5,6}, 12}
    };

    std::map<std::string, int> state0 = {
        {"LET", 1}, {"WRITE", 3}, {"READ", 5}, {"GOTO", 7},
        {"Id", 13} 
    };

    std::map<std::string, int> state1 = {
        {"Id", 2} 
    };

    std::map<std::string, int> state2 = {
        {"=", 3} 
    };

    std::map<std::string, int> state3 = { 
        {"Id", 4}, {"Num", 4}
    };

    std::map<std::string, int> state4 = {
        {"/", 3},  {"*", 3},  {"-", 3},  {"+", 3}
    };

    std::map<std::string, int> state5 = {
        {"Id", 6}
    };

    std::map<std::string, int> state6 = {
    };

    std::map<std::string, int> state7 = {
        {"Id", 8}
    };

    std::map<std::string, int> state8 = {
        {"IF", 9}
    };

    std::map<std::string, int> state9 = {
        {"Id", 10}, {"Num", 10} 
    };
    std::map<std::string, int> state10 = {
        {"/", 9},  {"*", 9},  {"-", 9},  {"+", 9},
        {">", 11}, {"=", 11}, {"<", 11}
    };

    std::map<std::string, int> state11 = {
        {"Id", 12}, {"Num", 12}
    };

    std::map<std::string, int> state12 = {
        {"/", 11},  {"*", 11},  {"-", 11},  {"+", 11}
    };

    std::map<std::string, int> state13 = {
        {":", 0} 
    };


    std::vector<std::map<std::string, int>> transitionTable = {
        state0, state1, state2, state3, state4, state5, state6,
        state7, state8, state9, state10, state11, state12,
        state13
    };

    std::vector<std::string> keywords = {
        "LET",
        "GOTO",
        "WRITE",
        "READ",
        ":",
        "=",
        "/",
        "*",
        "-",
        "+",
        ">",
        "<"
    };

    std::ifstream src;
    src.open(scriptFile);
    std::string lineRead;

    std::vector<std::string>* vectorSrc = new std::vector<std::string>();

    while (std::getline(src, lineRead)) {
        (*vectorSrc).push_back(lineRead);
    }

    e = new HliEventTreater(vectorSrc, infile, outfile);
    dm = new DecisionMaker(routinesTable, e);
    r = new Recognizer(transitionTable, dm);
    c = new Classifier(vectorSrc, "listing.txt", keywords, r, e);
    c->insertEvent(0);
}

void HighLevelInterpreter::executeScript() {
    while (!c->eof && !r->syntaxError && !e->error) {
        c->execute();
        r->execute();
        dm->execute();
        e->execute();
    }
}