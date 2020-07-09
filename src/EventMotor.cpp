#include "../include/EventMotor.h"

void EventMotor::insertEvent(int code) {
    eventQueue.push(code);
}