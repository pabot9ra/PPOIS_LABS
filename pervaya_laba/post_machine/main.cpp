#include <iostream>
#include <fstream>
#include "postmachine.h"
#include <string>
#include "menu.h"
const int STEP_LIMIT = 10000;

int main(int argc, char *argv[]) {
    if(argc == 1){
        runMenu();
        return 0;
    }
    if (argc < 2){
        std::cerr << "Usage: ./prog example.txt [-log]" << '\n';
        return 1;
    }
    std::ifstream file(argv[1]);
    if(!file){
    std::cerr << "File not loaded" << '\n';
    return 1;
    }
    PostMachine machine;
    file >> machine;
    if(!file){
    std::cerr << "File format is invalid" << '\n';
    return 1;
    }
    bool isLog = false;
    if (argc >= 3 && std::string(argv[2]) == "-log"){
        isLog = true;
    }
    
    std::cout << "Starting state: " << '\n'<< machine << '\n'; 
    int counter = 0;
    for(; counter < STEP_LIMIT && machine.step();counter++){
        if (isLog){
        std::cout << machine << '\n';
        }
    }
    


    if (!isLog){
        std::cout << machine << '\n';
    }

    std::cout << "Steps made: " << counter << '\n';

    if (machine.state() == State::Running){
        std::cerr << "Machine still running after step limit" << '\n';
        return 1;
    }
    if(machine.state() == State::Error){
        std::cerr << "Machine ended in error state" << '\n';
        return 1;
    }

    return 0;
    
}