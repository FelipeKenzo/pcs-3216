#include "../include/HliEventTreater.h"

HliEventTreater::HliEventTreater(std::vector<std::string>* src, std::ifstream* infile, std::ofstream* outfile) :
    src(src), infile(infile), outfile(outfile) {
    variables = new std::map<std::string, int>();
    labels = new std::map<std::string, int>();
    operandsTobeUsed = new std::stack<int>();
    operatorsAnalyzed = new std::stack<std::string>();
}

void HliEventTreater::t0() { // Adds a label
    std::map<std::string, int>::iterator it;
    it = (*labels).find(t.sequence);
    if (it != (*labels).end()) {
        if ((*it).second != t.line) {
            std::cerr << "Error(" << t.line << "): label \"" << t.sequence << "\" already defined at line (" << (*it).second << ").\n";
            error = true;
        }
    }
    else {
        (*labels)[t.sequence] = t.line;
    }
}

void HliEventTreater::t1(){ // Indicates where to jump
    std::map<std::string, int>::iterator it;
    it = (*labels).find(t.sequence);
    if (it != (*labels).end()) {
        gotoLine = (*it).second;
    }
    else { // Scans source file for the label
        for (int i = 0; i < (*src).size(); i++) {
            std::stringstream ss((*src)[i]);
            std::string labelCandidate;
            ss >> labelCandidate;

            if (labelCandidate.length() > 0) {
                labelCandidate.pop_back();
                if (labelCandidate == t.sequence) { // Found label declaration
                    (*labels)[t.sequence] = i + 1;
                    gotoLine = i + 1;
                    return;
                }
            }
        }
        // Didnt find label
        std::cerr << "Error(" << t.line << "): undeclared label \"" << t.sequence << "\"\n";
        error = true;
    }
}

void HliEventTreater::t2() { // Performs the GOTO jump
    nextLine = gotoLine;

}

void HliEventTreater::t3() { // Memorizes attribution destination
    destination = t.sequence;
}

void HliEventTreater::t4() { // Starts sequentiation process
    (*operandsTobeUsed).push(0);
    (*operatorsAnalyzed).push("+");
}

void HliEventTreater::t5() { // Starts sequentiation process (will output)
    destination = "";
    target = write;
    t4();
}

void HliEventTreater::t6() { // Starts sequentiation process (will attribute)
    target = variable;
    t4();
}

void HliEventTreater::t7() { // sequence operand
    if (target == write) {
        destination += t.sequence + " ";
    }
    if (t.type == integer) {
        (*operandsTobeUsed).push(std::stoi(t.sequence));
    }
    else {
        std::map<std::string, int>::iterator it;
        it = (*variables).find(t.sequence);
        
        if (it != (*variables).end()) {
            (*operandsTobeUsed).push((*it).second);
        }
        else {
            std::cerr << "Error(" << t.line << "): undefined variable\n";
            error = true;
        }
    }
}

void HliEventTreater::t8() { // Performs operation
    int b = (*operandsTobeUsed).top();
    (*operandsTobeUsed).pop();

    int a = (*operandsTobeUsed).top();
    (*operandsTobeUsed).pop();

    std::string op = (*operatorsAnalyzed).top();
    (*operatorsAnalyzed).pop();

    int result;

    if (op == "+") {
        result = a + b;
    }
    else if (op == "-") {
        result = a - b;
    }
    else if (op == "*") {
        result = a * b;
    }
    else if (op == "/") {
        result = a / b;
    }

    (*operandsTobeUsed).push(result);
}

void HliEventTreater::t9() { // Analyze operator
    
    std::string newOp = t.sequence;
    std::string topOp = (*operatorsAnalyzed).top();

    if (target == write) {
        destination += t.sequence + " ";
    }
    
    if (newOp == "+") {
        while (!(*operatorsAnalyzed).empty() && 
            ((*operatorsAnalyzed).top() != "*" ||
            (*operatorsAnalyzed).top() != "/")) {
            t8();
        }
        (*operatorsAnalyzed).push("+");
    }
    else if (newOp == "-") {
        while (!(*operatorsAnalyzed).empty() && 
            ((*operatorsAnalyzed).top() != "*" ||
            (*operatorsAnalyzed).top() != "/")) {

            t8();
        }
        (*operatorsAnalyzed).push("-");
    }
    else if (newOp == "*") {
        if (topOp == "+") {
            (*operatorsAnalyzed).push("*");
        }
        else if (topOp == "-") {
            (*operatorsAnalyzed).push("*");
        }
        else if (topOp == "*") {
            while (!(*operatorsAnalyzed).empty() && 
                ((*operatorsAnalyzed).top() == "*" ||
                (*operatorsAnalyzed).top() == "/")) {

                t8();
            }
            (*operatorsAnalyzed).push("*");
        }
        else if (topOp == "/") {
            while (!(*operatorsAnalyzed).empty() && 
                ((*operatorsAnalyzed).top() == "*" ||
                (*operatorsAnalyzed).top() == "/")) {

                t8();
            }
            (*operatorsAnalyzed).push("*");
        }
    }
    else if (newOp == "/") {
        if (topOp == "+") {
            (*operatorsAnalyzed).push("/");
        }
        else if (topOp == "-") {
            (*operatorsAnalyzed).push("/");
        }
        else if (topOp == "*") {
            while (!(*operatorsAnalyzed).empty() && 
                ((*operatorsAnalyzed).top() == "*" ||
                (*operatorsAnalyzed).top() == "/")) {

                t8();
            }
            (*operatorsAnalyzed).push("/");
        }
        else if (topOp == "/") {
            while (!(*operatorsAnalyzed).empty() && 
                ((*operatorsAnalyzed).top() == "*" ||
                (*operatorsAnalyzed).top() == "/")) {

                t8();
            }
            (*operatorsAnalyzed).push("/");
        }
    }
}

void HliEventTreater::t10() { // Finishes operation
    while ((*operatorsAnalyzed).size() > 0) {
        t8();
    }
    
    int result = (*operandsTobeUsed).top();
    (*operandsTobeUsed).pop();
    
    switch(target) {
        case write:
            (*outfile) << destination << "= " << result << "\n";
            (*outfile).flush();
            nextLine = t.line + 1;
            break;
        case variable:
            (*variables)[destination] = result;
            nextLine = t.line + 1;
            break;
        case goto1:
            firstOperand = result;
            comparator = t.sequence;
            target = goto2;
            t4(); // restarts sequentiation process
            break;
        case goto2:
            nextLine = t.line + 1;
            if (comparator == ">") {
                if (firstOperand > result) t2();
            }
            else if (comparator == "=") {
                if (firstOperand == result) t2();
            }
            else if (comparator == "<") {
                if (firstOperand < result) t2();
            }
            break;

    }

}

void HliEventTreater::t11() { // Starts sequentiation process (goto1)
    target = goto1;
    t4();
}

void HliEventTreater::t12() { // Reads an input and assigns to a variable 
    int input;
    (*infile) >> input;

    (*variables)[t.sequence] = input;
    nextLine = t.line + 1;
}


void HliEventTreater::executeTreatment() {
    switch(this->routine) {
        case 0: t0();
            break;
        case 1: t1();
            break;
        case 2: t2();
            break;
        case 3: t3();
            break;
        case 4: t4();
            break;
        case 5: t5();
            break;
        case 6: t6();
            break;
        case 7: t7();
            break;
        case 8: t8();
            break;
        case 9: t9();
            break;
        case 10: t10();
            break;
        case 11: t11();
            break;
        case 12: t12();
            break;
    }

    ready = true;
}