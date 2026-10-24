#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <iosfwd>

enum class OpType {
    Mark,
    Erase,
    Right,
    Left,
    Branch,
    Stop
};

class Instruction {
private:
    OpType type_;
    int next1_;
    int next2_;

public:
    Instruction();
    Instruction(OpType type, int next1);
    Instruction(OpType type, int next1, int next2);

    OpType type() const;
    int next1() const;
    int next2() const;

    bool operator==(const Instruction &other) const;
    bool operator!=(const Instruction &other) const;
};

std::ostream& operator<<(std::ostream &os, const Instruction &ins);
std::istream& operator>>(std::istream &is, Instruction &ins);

#endif