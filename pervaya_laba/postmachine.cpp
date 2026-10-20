#include "postmachine.h"
#include <istream>
#include <ostream>

PostMachine::PostMachine()
    : tape_(),  program_(), pos_(0), current_(1), state_(State::Running) {
}

PostMachine::PostMachine(const Tape &tape, const Program &prog, long long pos)
    : tape_(tape), program_(prog), pos_(pos), current_(1), state_(State::Running){

}
 
const Tape& PostMachine::tape() const{
    return tape_;
}

const Program& PostMachine::program() const{
    return program_;
}

long long PostMachine::position() const{
    return pos_;
}

int PostMachine::current() const{
    return current_;
}

State PostMachine::state() const{
    return state_;
}

void PostMachine::setTape(const Tape &tape){
    tape_ = tape;
}

void PostMachine::setProgram(const Program &program){
    program_ = program;
}

void PostMachine::setPosition(long long pos){
    pos_ = pos;
}

void PostMachine::reset(){
    current_ = 1;
    state_ = State::Running;
}

bool PostMachine::operator==(const PostMachine &other) const{
    return (tape_ == other.tape_ && program_ == other.program_ && pos_ == other.pos_ && current_ == other.current_ && state_ == other.state_);
}

bool PostMachine::operator!=(const PostMachine &other) const{
    return !(*this == other);
}

bool PostMachine::step(){
    if (state_ != State::Running){
        return false;
    }
    if (!program_.has(current_)){
        state_ = State::Error;
        return false;
    }

    const Instruction &ins = program_.at(current_);

    switch (ins.type()){
        case OpType::Mark: 
            if (tape_.isMarked(pos_)){
                state_ = State::Error;
                return false;
            }  
            tape_.mark(pos_);
            current_ = ins.next1();
            break;

        

        case OpType::Erase:
            if (!tape_.isMarked(pos_)){
               state_ = State::Error;
               return false;
            }
            tape_.unmark(pos_);
            current_ = ins.next1();
            break;

        case OpType::Right:
            pos_ = pos_ + 1;
            current_ = ins.next1();
            break;

        case OpType::Left:
            pos_ = pos_ - 1;
            current_ = ins.next1();
            break;

        case OpType::Branch:
            if (tape_.isMarked(pos_)){
                current_ = ins.next1();
            }
            else {
                current_ = ins.next2();
            }
            break;

        case OpType::Stop:
            state_ = State::Stopped;
            break;

    } 
    return true;
}


std::ostream &operator<<(std::ostream &os, const PostMachine &machine){
    switch(machine.state()){
        case State::Error:
            os << "[Error]" << '\n';
            break;
        case State::Running:
            os << "[Running]" << '\n';
            break;
        case State::Stopped:
            os << "[Stopped]" << '\n';
            break;
    }
    if(machine.program().has(machine.current())){
        os << "command: " << machine.current() << ' ';
        os << machine.program().at(machine.current()) << '\n';
    } else {
        os << "no such command with number: " << machine.current() << '\n';
    }

    os << "carriage: " << machine.position() << '\n';

    os << "tape: " << machine.tape() << '\n';


    return os;

}