#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <string>
#include <fstream>

#include "EventMotor.h"
#include "Enums.h"
#include "Structs.h"

#include "Recognizer.h"
#include "EventTreater.h"

class Classifier : public EventMotor {
public:
    Classifier(std::vector<std::string>* src, std::string lstPath, std::vector<std::string>keywords, Recognizer* r, EventTreater* e);
    void execute();
    
    bool eof = false;
private:
    // ================
    // == Attributes ==
    // ================

    std::vector<std::string> keywords;
    Recognizer* r;
    EventTreater* e;

    std::vector<std::string>* src;
    std::ofstream lst;
    std::string sequence = "";
    std::string currentLine = "";
    tokenType candidateType = identifier;
    int lineNumber = 0;
    int cindex = 0;

    // ========================
    // == Treatment routines ==
    // ========================

    void readLine();     // 0
    void processChar();  // 1
    void printLine();    // 2
    void sendToken();    // 3
    void waitDep();      // 4

    // ===============
    // == Utilities ==
    // ===============

    bool isAlphabetical(char c);
    bool isNumeric(char c);
};

#endif