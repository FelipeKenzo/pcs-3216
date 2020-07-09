#include "../include/Classifier.h"

Classifier::Classifier(std::vector<std::string>* src, std::string lstPath, std::vector<std::string> keywords, Recognizer* r, EventTreater* e) :
    src(src), keywords(keywords), r(r), e(e){
    
    lst.open(lstPath);
    if (!lst.is_open()) std::cerr << "could not open lst.\n";
}

void Classifier::readLine() {
    if (e->nextLine != lineNumber) {
        lineNumber = e->nextLine;
        if (lineNumber - 1 >= (*src).size()) { // End of file
            insertEvent(4);
            return;
        }
        currentLine = (*src)[lineNumber - 1];
        cindex = 0;

        // ignore empty lines
        while (currentLine == "") {
            if (lineNumber - 1 >= (*src).size()) { // End of file
                insertEvent(4);
                return;
            }
            lineNumber++;
            currentLine = (*src)[lineNumber - 1];
        }

        insertEvent(2); // print line to listing
        insertEvent(1); // process the first char
    }
    else {
        insertEvent(0);
    }
}

void Classifier::processChar() {
    if (cindex >= currentLine.length()) {
        insertEvent(3); // finishes token
        return;
    }

    // ignore comments
    char c = currentLine[cindex++];

    if (isAlphabetical(c)) {
        sequence += c;
        if (candidateType == integer) {
            candidateType = identifier;
        }
    }
    else if (isNumeric(c)) {
        if (sequence == "") {
            candidateType = integer;
        }
        sequence += c;
    }
    else {
        if (c != ' ') {
            if (sequence == "") {
                sequence += c;
                candidateType = specialT;
            }
            else {
                // reprocess special char
                cindex--;
            }
        }
        
        // End of token
        insertEvent(3); // Sends token
        return;
    }

    insertEvent(1); // process next char
    return;
}

void Classifier::printLine() {
    lst << lineNumber << " " << currentLine << "\n";
    lst.flush();
    return;
}

void Classifier::sendToken() {

    // Checks if it is a keyword
    if (candidateType == identifier) {
        for (int i = 0; i < keywords.size() ; i++) {
            // std::cout << i << "\n";
            if (sequence == keywords[i]) {
                candidateType = keyword;
            }
        }
    }


    if (r->isRequestingToken()) {
        token t = {
            sequence,
            candidateType,
            lineNumber,
        };

        if (sequence != "") r->receiveToken(t);
        
        // Start processing next token
        if (cindex >= currentLine.length() || sequence == ";") {
            insertEvent(0);
        }
        else {
            insertEvent(1); 
        }
        sequence = "";
        candidateType = identifier;
    }
    else {
        insertEvent(3);
    }

}

void Classifier::waitDep() {
    if (sequence != "") {
        insertEvent(3);
        insertEvent(4);
    }
    else {
        eof = true;
    }
}

bool Classifier::isAlphabetical(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || c == '.');
}

bool Classifier::isNumeric(char c) {
    return (c >= '0' && c <= '9');
}

// Abstract Methods

void Classifier::execute() {
    if (eventQueue.empty()) return;

    switch(eventQueue.front()) {
        case 0: readLine();
            break;
        case 1: processChar();
            break;
        case 2:
            printLine();
            break;
        case 3:
            sendToken();
            break;
        case 4:
            waitDep();
            break;
        default:
            break;
    }

    eventQueue.pop();
}