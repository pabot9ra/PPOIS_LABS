#ifndef POSTMACHINE_H
#define POSTMACHINE_H
#include "tape.h"
#include "program.h"
#include <iosfwd>

enum class State{
    Running,
    Stopped,
    Error
};

class PostMachine{
private:
   Tape tape_;
   Program program_;
   long long pos_;
   int current_;
   State state_;



public:
    PostMachine();
    PostMachine(const Tape &tape, const Program &prog, long long pos);

    const Tape& tape() const;
    const Program& program() const;
    long long position() const;
    int current() const;
    State state() const;

    void setTape(const Tape &tape);
    void setProgram(const Program &program);
    void setPosition(long long pos);
    void reset();

    bool step();
    int run(int limit);

    PostMachine& operator++();
    PostMachine operator++(int);

    bool operator==(const PostMachine &other) const;
    bool operator!=(const PostMachine &other) const;

};

std::ostream& operator<<(std::ostream &os, const PostMachine &machine);
std::istream& operator>>(std::istream &is, PostMachine &machine);


#endif