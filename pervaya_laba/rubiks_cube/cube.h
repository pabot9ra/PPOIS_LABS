#ifndef CUBE_H
#define CUBE_H
#include <array>
#include "face.h"

enum class FaceId{
    U,
    L,
    F,
    R,
    B,
    D
};

class Cube{
    private:
        std::array<Face, 6> faces_;
    public:
        Cube();
        const Face& face(FaceId id) const;
        bool isSolved() const;
};

#endif