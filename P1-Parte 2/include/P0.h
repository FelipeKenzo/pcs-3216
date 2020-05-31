#ifndef P0_H
#define P0_H

#include "P1.h"

class P0 {
private:
    P1* p1;
public:
    bool isRunning = true;
    void start();
    void stop();
    P0();
    void setReferences(P1* p1);
};

#endif