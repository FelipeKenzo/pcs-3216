#include <string>
#include <iostream>
#include <map>
#include <list>

#include "../include/P0.h"
#include "../include/P1.h"
#include "../include/P2.h"
#include "../include/P3.h"
#include "../include/P4.h"
#include "../include/P5.h"
#include "../include/P6.h"

int main() {

    // Reference Table
    std::map<std::string, std::list<int>*>* refTable = new std::map<std::string, std::list<int>*>();

    // Modules
    P0* p0 = new P0();
    P1* p1 = new P1("T.txt");
    P2* p2 = new P2();
    P3* p3 = new P3();
    P4* p4 = new P4(refTable);
    P5* p5 = new P5(refTable);
    P6* p6 = new P6("listing.txt", "report.txt");

    // Setting references
    p0->setReferences(p1);
    p1->setReferences(p2, p6);
    p2->setReferences(p1, p3);
    p3->setReferences(p4);
    p4->setReferences(p5);
    p5->setReferences(p0, p6);
    
    // Event loop
    p0->start();
    while(p0->isRunning) {
        p1->execute();
        p2->execute();
        p3->execute();
        p4->execute();
        p5->execute();
        p6->execute();
    }
}

