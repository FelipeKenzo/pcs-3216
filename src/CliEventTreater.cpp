#include "../include/CliEventTreater.h"

CliEventTreater::CliEventTreater() {
}

void CliEventTreater::t0() {
    if (session == "") {
        std::cout << "$JOB: starting job [" << t.sequence << "]\n";
        session = t.sequence;
        vnm = new VonNeumannMachine(&input, &output);
        loadLoader();
    }
    else {
        std::cerr << "$JOB: Error: job [" << session << "] already runnning.\n";
    }
}

void CliEventTreater::t1() {
    if (session != "") {
        disk = t.sequence;
        std::cout << "$DISK: Disk set to [" << t.sequence << "]\n";
    }
    else {
        std::cerr << "$DISK: Error: no job is running.\n";
    }
}

void CliEventTreater::t2() {
    if (session != "") {
        if (disk != " ") {
            std::string files = std::filesystem::current_path();
            files += "/" + disk;
            std::string fileName;
            std::cout << "\n$DIRECTORY: files in disk:\n";
            for (const auto & entry : std::filesystem::directory_iterator(files)) {
                fileName = entry.path();
                fileName = eraseSubStr(fileName, files + "/");
                std::cout << fileName << "\n";
            }
            std::cout << "\n";
        } else {
            std::cerr << "$DIRECTORY: Error: no disk directory.\n";
        }
    }
    else {
        std::cerr << "$DIRECTORY: Error: no job is running.\n";
    }
    return;
}

void CliEventTreater::t3() {
    if (session != "") {
        std::string path = std::filesystem::current_path();
        path += "/" + disk + "/" + t.sequence;

        std::ofstream newFile;
        newFile.open(path);
        if (newFile.is_open()) {
            std::cout << "$CREATE: file [" << t.sequence << "] created.\n";
        }
        else {
            std::cout << "$CREATE: Error: file [" << t.sequence << "] could not be created.\n";
        }
        newFile.close();
    }
    else {
        std::cerr << "$CREATE: Error: no job is running.\n";
    }
}

void CliEventTreater::t4() {
    if (session != "") {
        std::string path = "./" + disk + "/" + t.sequence;
        if (std::remove(path.c_str())) {
            std::cerr << "$DEL: Error: Cannot not remove [" << t.sequence << "] - no such file\n";
        }
        else {
            std::cout << "$DEL: File [" << t.sequence << "] deleted.\n";
        }
    }
    else {
        std::cerr << "$DEL: Error: no job is running.\n";
    }
}

void CliEventTreater::t5() {
    if (session != "") {
        std::string path = disk + "/" + t.sequence;

        std::ifstream file;
        file.open(path);

        if (!file.is_open()) {
            std::cerr << "$LIST: Error: file [" << t.sequence << "] could not be opened.\n";
            return;
        }
        
        std::cout << "\n$LIST: listing contents of file [" << t.sequence << "]:\n\n";
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << "\n";
        }
        std::cout << "\n";
    }
    else {
        std::cerr << "$LIST: Error: no job is running.\n";
    }

}

void CliEventTreater::t6() {
    if (session != "") {
        std::string path = disk + "/" + t.sequence;
        inputPath = path;

        if (input.is_open()) {
            input.close();
        }
        
        input.open(path);

        if (input.is_open()) {
            std::cout << "$INFILE: file [" << t.sequence <<"] set as input.\n";
        }
        else {
            std::cerr << "$INFILE: Error: file [" << t.sequence << "] could not be opened.\n";
        }
    }
    else {
        std::cerr << "$INFILE: Error: no job is running.\n";
    }
}

void CliEventTreater::t7() {
    if (session != "") {
        std::string path = disk + "/" + t.sequence;
        outputPath = path;

        if (output.is_open()) {
            output.close();
        }
        
        output.open(path);

        if (output.is_open()) {
            std::cout << "$OUTFILE: file [" << t.sequence << "] set as output.\n";
        }
        else {
            std::cerr << "$OUTFILE: Error: file [" << t.sequence << "] could not be opened.\n";
        }
    }
    else {
        std::cerr << "$OUTFILE: Error: no job is running.\n";
    }
}

void CliEventTreater::t8() {
    if (session != "") {
        diskfile = disk + "/" + t.sequence;
        std::cout << "$DISKFILE: file [" << t.sequence << "] set as VNM disk.\n";
    }
    else {
        std::cerr << "$DISKFILE: Error: no job is running.\n";
    }
}

void CliEventTreater::t9() {
    if (session != "") {
        // System programs
        if (t.sequence == "vnm") {
            std::cout << "$RUN: initial PC(0-4094)?: ";
            int addr;
            std::cin >> addr;

            vnm->setPC(addr);
            vnm->run();
        }
        else if (t.sequence == "assembler") {
            a = new Assembler(diskfile, outputPath, true);
            if (a->assemble()) {
                std::cout << "$RUN: assemble: success\n";
            }
            delete(a);
        }
        else if (t.sequence == "loader") {
            if (input.is_open()) {
                input.close();
            }
            
            input.open(diskfile);
            vnm->setPC(0);
            vnm->run();

            std::cout << "$RUN: loader: done\n";

            input.close();

            if (inputPath != "") {
                input.open(inputPath);
            }
        }
        else if (t.sequence == "interpreter") {
            hli = new HighLevelInterpreter(diskfile, &input, &output);
            hli->executeScript();
            delete hli;
        }
        else {
            std::string path = disk + "/" + t.sequence;
            std::cout << "$RUN: VNM(1) or Interpreter(2)? ";
            int choice;
            std::cin >> choice;
            if (choice == 1) {
                // run Loader
                if (input.is_open()) {
                    input.close();
                }
                
                input.open(path);
                vnm->setPC(0);
                vnm->run();

                input.close();

                if (inputPath != "") {
                    input.open(inputPath);
                }
                // run vnm
                std::cout << "$RUN: initial PC? (0-4094): ";
                int addr;
                std::cin >> addr;

                vnm->setPC(addr);
                vnm->run();
            }
            else if (choice == 2) {
                // initilize and run interpreter
                hli = new HighLevelInterpreter(path, &input, &output);
                hli->executeScript();
                delete hli;
            }
            else {
                std::cerr << "$RUN: invalid choice.\n";
            }
        }
    }
    else {
        std::cerr << "$RUN: Error: no job is running.\n";
    }
}

void CliEventTreater::t10() {
    if (session != "") {
        std::cout << "$ENDJOB: ending job [" << session << "].\n";
        session = "";
        delete(vnm);
    }
    else {
        std::cerr << "$ENDJOB: Error: no job is running.\n";
    }
}

void CliEventTreater::executeTreatment() {
    nextLine = t.line + 1;
    switch(this->routine) {
        case 0: t0();
            break;
        case  1: t1();
            break;
        case  2: t2();
            break;
        case  3: t3();
            break;
        case  4: t4();
            break;
        case  5: t5();
            break;
        case  6: t6();
            break;
        case  7: t7();
            break;
        case  8: t8();
            break;
        case  9: t9();
            break;
        case 10: t10();
            break;
    }
}

void CliEventTreater::loadLoader() {
    vnm->memWrite_w(0x00, 0x800B);
    vnm->memWrite_w(0x02, 0x500B);
    vnm->memWrite_w(0x04, 0x900B);
    vnm->memWrite_w(0x06, 0x000E);
    vnm->memWrite_w(0x08, 0x0001);
    vnm->memWrite_w(0x0A, 0x9000);
    vnm->memWrite_w(0x0C, 0x00FF);
    vnm->memWrite_w(0x0E, 0x800C);
    vnm->memWrite_w(0x10, 0x500C);
    vnm->memWrite_w(0x12, 0x900C);
    vnm->memWrite_w(0x14, 0xA056);
    vnm->memWrite_w(0x16, 0x400A);
    vnm->memWrite_w(0x18, 0x902A);
    vnm->memWrite_w(0x1A, 0x500A);
    vnm->memWrite_w(0x1C, 0x500D);
    vnm->memWrite_w(0x1E, 0x1054);
    vnm->memWrite_w(0x20, 0xA056);
    vnm->memWrite_w(0x22, 0x902B);
    vnm->memWrite_w(0x24, 0xA056);
    vnm->memWrite_w(0x26, 0x9008);
    vnm->memWrite_w(0x28, 0xA056);
    vnm->memWrite_w(0x2A, 0x9000);
    vnm->memWrite_w(0x2C, 0x802B);
    vnm->memWrite_w(0x2E, 0x4009);
    vnm->memWrite_w(0x30, 0x902B);
    vnm->memWrite_w(0x32, 0x103E);
    vnm->memWrite_w(0x34, 0x8008);
    vnm->memWrite_w(0x36, 0x5009);
    vnm->memWrite_w(0x38, 0x9008);
    vnm->memWrite_w(0x3A, 0x1046);
    vnm->memWrite_w(0x3C, 0x0028);
    vnm->memWrite_w(0x3E, 0x802A);
    vnm->memWrite_w(0x40, 0x4009);
    vnm->memWrite_w(0x42, 0x902A);
    vnm->memWrite_w(0x44, 0x0034);
    vnm->memWrite_w(0x46, 0x800D);
    vnm->memWrite_w(0x48, 0x500C);
    vnm->memWrite_w(0x4A, 0x900C);
    vnm->memWrite_w(0x4C, 0xD000);
    vnm->memWrite_w(0x4E, 0x500C);
    vnm->memWrite_w(0x50, 0x100E);
    vnm->memWrite_w(0x52, 0x900B);
    vnm->memWrite_w(0x54, 0xC00E);
    vnm->memWrite_w(0x56, 0x0000);
    vnm->memWrite_w(0x58, 0xD000);
    vnm->memWrite_w(0x5A, 0x9064);
    vnm->memWrite_w(0x5C, 0x400C);
    vnm->memWrite_w(0x5E, 0x900C);
    vnm->memWrite_w(0x60, 0x8064);
    vnm->memWrite_w(0x62, 0xB056);
    
}