#ifndef CUBE_H
#define CUBE_H
#include <array>
#include "face.h"
#include <iosfwd>


enum class FaceId{
    U,
    L,
    F,
    R,
    B,
    D
};

enum class Turn{
    Clockwise,
    CounterClockwise,
    DoubleMove
};

class Cube{
    private:
        std::array<Face, 6> faces_;
    public:
        Cube();
        const Face& face(FaceId id) const;
        bool isSolved() const;
        void moveU();
        void moveR();
        void moveL();
        void moveD();
        void moveF();
        void moveB();
        void move(FaceId face, Turn turn);
};

std::ostream& operator<<(std::ostream &os, const Cube &cube);

#endif