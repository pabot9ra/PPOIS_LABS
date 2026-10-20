#include <iostream>
#include <sstream>
#include "postmachine.h"

int main() {
    std::istringstream input("V 2\n-> 3\nV 4\n!\n");
    Program prog;
    input >> prog;

    PostMachine m(Tape(), prog, 0);
    std::cout << m << "\n\n";

    while (m.step()) {
        std::cout << m << "\n\n";
    }
    return 0;
}