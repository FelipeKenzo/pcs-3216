#ifndef P5_H
#define P5_H

#include <map>
#include <list>
#include "EventMotor.h"

#include "P0.h"
#include "P6.h"

class P5 : public EventMotor {
private:
    std::map<std::string, std::list<int>*>*  refTable;
    
    P0* p0;
    P6* p6;

    void orderTable();
    void printTable();
    void signalEnd();
public:
    P5(std::map<std::string, std::list<int>*>*  refTable);
    void setReferences(P0* p0, P6* p6);

    void execute();
};

#endif