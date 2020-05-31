#ifndef P2_H
#define P2_H

#include "EventMotor.h"
class P1;
#include "P1.h"
#include "P3.h"

class P2 : public EventMotor {
private:
    std::string charSequence = "";
    bool firstOfLine = false;
    bool isAlphabetic = false;
    bool labeledLine = false;
    int lineNumber = 0;
    int wordNumber = 1;
    char charToBeProcessed;

    P1* p1;
    P3* p3;

    void processChar();
    void notifyEndOfT();
public:
    P2();
    void setReferences(P1* p1, P3* p3);
    ~P2();

    void receiveChar(bool fol, bool isa, char c, int line);
    void execute();
};

#endif