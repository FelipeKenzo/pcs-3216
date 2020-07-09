#ifndef EVENTTREATER_H
#define EVENTTREATER_H

#include <string>

#include "EventMotor.h"
#include "Structs.h"

class EventTreater : public EventMotor {
public:
    void execute();

    // ===============
    // == Interface ==
    // ===============

    void receiveRoutine(treatment routine);
    int nextLine = 1;
protected:
    virtual void executeTreatment() = 0; // Specific to each interpreter

    // ================
    // == Attributes ==
    // ================

    int routine;
    token t;
};

#endif