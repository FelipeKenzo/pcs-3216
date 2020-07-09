#ifndef CLIEVENTTREATER_H
#define CLIEVENTTREATER_H

#include <string>
#include <filesystem>
#include <fstream>

#include "EventTreater.h"
#include "Interface.h"
#include "VonNeumannMachine.h"
#include "Assembler.h"
#include "HighLevelInterpreter.h"

class CliEventTreater : public EventTreater {
public:
    CliEventTreater();

protected:
    void executeTreatment();

    // =====================
    // == System Programs ==
    // =====================
    VonNeumannMachine* vnm;
    Assembler* a;
    HighLevelInterpreter* hli;
    void loadLoader();

    // ===================
    // == Job Variables ==
    // ===================
    std::string session = "";
    std::string disk = "";
    std::string diskfile = "";
    std::string inputPath = "";
    std::ifstream input;
    std::ofstream output;

    // ==========================
    // == Interpreter Routines ==
    // ==========================

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
};

#endif