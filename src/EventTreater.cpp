#include "../include/EventTreater.h"

void EventTreater::receiveRoutine(treatment routine) {
    this->routine = routine.routine;
    t = routine.t;

    insertEvent(0);
}

void EventTreater::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: executeTreatment();
            break;
        default:
            break;
    }

    eventQueue.pop();
}

