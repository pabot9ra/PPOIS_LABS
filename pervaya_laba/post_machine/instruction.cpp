#include "instruction.h"
#include <ostream>
#include <istream>
#include <string>

Instruction::Instruction()
    : type_(OpType::Stop), next1_(0), next2_(0){
}

Instruction::Instruction(OpType type, int next1)
    : type_(type), next1_(next1), next2_(0){

}

Instruction::Instruction(OpType type, int next1, int next2)
    : type_(type), next1_(next1), next2_(next2){
}

OpType Instruction::type() const{
    return type_;
}

int Instruction::next1() const{
    return next1_;
}

int Instruction::next2() const{
    return next2_;
}

bool Instruction::operator==(const Instruction &other) const{
    return type_ == other.type_ && next1_ == other.next1_ && next2_ == other.next2_;
}

bool Instruction::operator!=(const Instruction &other) const{
    return !(*this == other);
}

std::ostream& operator<<(std::ostream &os, const Instruction &ins){
    switch(ins.type()){
        case OpType::Mark:
            os << "V " << ins.next1();
            break;
        case OpType::Erase:
            os << "X " << ins.next1();
            break;
        case OpType::Right:
            os << "-> " << ins.next1();
            break;
        case OpType::Left:
            os << "<- " << ins.next1();
            break;
        case OpType::Branch:
            os << "? " << ins.next1() << ", " << ins.next2();
            break;
        case OpType::Stop:
            os << "!";
            break;
    }

    return os;
}


std::istream& operator>>(std::istream &is, Instruction &ins) {
    std::string op;
    is >> op;

    if (op == "V") {
        int n;
        is >> n;
        ins = Instruction(OpType::Mark, n);
    } else if (op == "X"){
        int n;
        is >> n;
        ins = Instruction(OpType::Erase, n);
    } else if (op == "->"){
        int n;
        is >> n;
        ins = Instruction(OpType::Right, n);
    } else if (op == "<-"){
        int n;
        is >> n;
        ins = Instruction(OpType::Left, n);
    } else if (op == "?"){
        int n1, n2;
        char comma;
        is >> n1 >> comma >> n2;
        ins = Instruction(OpType::Branch, n1, n2);

    } else if (op == "!"){
        ins = Instruction();
    } else {
        is.setstate(std::ios::failbit);
    } 
    
    return is;
}