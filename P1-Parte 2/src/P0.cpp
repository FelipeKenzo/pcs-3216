#include "../include/P0.h"

P0::P0() {
    //ctor
}

void P0::setReferences(P1* p1) {
    this->p1 = p1;
}

void P0::start(){
    p1->insertEvent(0);
}

void P0::stop() {
    isRunning = false;
}