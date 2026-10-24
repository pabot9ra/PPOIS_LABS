#ifndef TAPE_H
#define TAPE_H

#include <set>
#include <iosfwd>

class Tape {
private:
    std::set<long long> marked_;

public:
    Tape();

    bool isMarked(long long pos) const;
    void mark(long long pos);
    void unmark(long long pos);
    void clear();

    bool empty() const;
    long long count() const;
    long long minPos() const;
    long long maxPos() const;

    bool operator==(const Tape &other) const;
    bool operator!=(const Tape &other) const;
};

std::ostream& operator<<(std::ostream &os, const Tape &tape);
std::istream& operator>>(std::istream &is, Tape &tape);

#endif