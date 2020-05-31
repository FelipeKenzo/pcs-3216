#ifndef P1_H
#define P1_H

#include <fstream>
#include <algorithm>

#include "Classes.h"
#include "EventMotor.h"
#include "P2.h"
#include "P6.h"

class P1 : public EventMotor {
private:
    std::ifstream T;
    int lineNumber = 0;
    int cindex = 0;
    std::string currentLine;

    P2* p2;
    P6* p6;

    void readLine();
    void readCharacter();
    bool isAlphabetical(char c);
public:
    P1(std::string T);
    ~P1();
    void setReferences(P2* p2, P6* p6);
    void execute();
};

#endif