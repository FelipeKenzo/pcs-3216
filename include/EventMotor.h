#ifndef EventMotor_H
#define EventMotor_H

#include <queue>
#include <iostream>

class EventMotor {
protected:
    std::queue<int> eventQueue;
public:
    void insertEvent(int code);
    virtual void execute() = 0;
};

#endif