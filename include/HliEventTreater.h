#ifndef HLIEVENTTREATER_H
#define HLIEVENTTREATER_H

#include <string>
#include <map>
#include <fstream>
#include <stack>
#include <sstream>

#include "EventTreater.h"

class HliEventTreater : public EventTreater {
public:
    HliEventTreater(std::vector<std::string>* src, std::ifstream* infile, std::ofstream* outfile);
    bool ready = false;
    bool error = false;
protected:
    void executeTreatment();

    std::ifstream* infile;
    std::ofstream* outfile;

    std::vector<std::string>* src;

    std::map<std::string, int>* variables;
    std::map<std::string, int>* labels;
    std::stack<int>* operandsTobeUsed;
    std::stack<std::string>* operatorsAnalyzed;

    std::string destination = "";

    enum expressionTarget {
        write,
        variable,
        goto1,
        goto2,
        none
    };

    expressionTarget target = none;

    int firstOperand = 0;
    int gotoLine = 0;
    std::string comparator = "";

    void t0();
    void t1();
    void t2();
    void t3(); 
    void t4();
    void t5();
    void t6();
    void t7();
    void t8();
    void t9();
    void t10();
    void t11();
    void t12();

};

#endif