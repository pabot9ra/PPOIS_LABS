#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>
#include <iosfwd>
#include "instruction.h"

class Program {
private:
    std::vector<Instruction> instructions_;

public:
     Program();

     int size() const;
     bool empty() const;
     void add(const Instruction &ins);
     bool remove(int n);
     bool has(int n) const;
     const Instruction& at(int n) const;
     void clear();
     
    


    

    bool operator==(const Program &other) const;
    bool operator!=(const Program &other) const;
};

std::ostream& operator<<(std::ostream &os, const Program &prog);
std::istream& operator>>(std::istream &is, Program &prog);

#endif