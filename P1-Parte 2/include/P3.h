#ifndef P3_H
#define P3_H

#include "EventMotor.h"
class P4;
#include "P4.h"

class P3 : public EventMotor {
private:
    bool firstOfLine;
    std::string sequence = "";
    int lineNumber = 0;

    P4* p4;
    
    void sendSequence();
    void notifyEndOfT();
public:
    P3();
    void setReferences(P4* p4);

    void receiveSequence(bool fol, std::string seq, int line);
    void execute();
    void receiveEndOfT();
};

#endif

// C#maj7 | Cm7b5 | F7 | Bbm7 | G#7 | F#maj7 
// Gm7(b5) | 