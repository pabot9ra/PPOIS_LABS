#include "tape.h"
#include <ostream>
#include <istream>

Tape::Tape() {
}

bool Tape::isMarked(long long pos) const {
    return marked_.count(pos) > 0;
}

void Tape::mark(long long pos) {
    marked_.insert(pos);
}

void Tape::unmark(long long pos) {
    marked_.erase(pos);
}

void Tape::clear() {
    marked_.clear();
}

bool Tape::empty() const {
    return marked_.empty();
}
long long Tape::count() const {
    return marked_.size();
}

long long Tape::minPos() const {
    if (marked_.empty()){
        return 0;
    }
    return *marked_.begin();
}

long long Tape::maxPos() const {
    if (marked_.empty()){
        return 0;
    }
        return *marked_.rbegin();
}

bool Tape::operator==(const Tape &other) const {
    return marked_ == other.marked_;
}

bool Tape::operator!=(const Tape &other) const {
    return (*this == other);
}

std::ostream& operator<<(std::ostream &os, const Tape &tape) {
    if (tape.empty()) {
        os << "(empty tape)";
        return os;
    }

    long long from = tape.minPos();
    long long to = tape.maxPos();

    os << "[" << from << "] ";
    for (long long i = from; i <= to; ++i) {
        if (tape.isMarked(i)) {
            os << "V ";
        } else {
            os << ". ";
        }
    }
    return os;
}