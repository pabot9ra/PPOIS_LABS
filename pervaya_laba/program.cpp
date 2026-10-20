#include "program.h"
#include <istream>
#include <ostream>

Program::Program(){

}

int Program::size() const{
    return static_cast<int>(instructions_.size());
}

bool Program::empty() const{
    return instructions_.empty();
}

void Program::add(const Instruction &ins){
    instructions_.push_back(ins);
}

bool Program::has(int n) const{
    return n >= 1 && n <= size();
}

bool Program::remove(int n){
    if(has(n)){
        instructions_.erase(instructions_.begin() + (n-1));
        return true;
    } else return false;
}

const Instruction& Program::at(int n) const{
    return instructions_.at(n-1);
}

void Program::clear(){
    instructions_.clear();
}

bool Program::operator==(const Program &other) const{
    return instructions_ == other.instructions_;
}

bool Program::operator!=(const Program &other) const{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream &os, const Program &prog){
    for (int i = 1; i <= prog.size(); i++){
        os << prog.at(i) << '\n'; 
    } 
    return os;
}

std::istream& operator>>(std::istream &is, Program &prog){
    prog.clear();
    Instruction ins;
    while(is >> ins){
        prog.add(ins);
    }
    if (is.eof()){
        is.clear();
    } 
    return is;
}

