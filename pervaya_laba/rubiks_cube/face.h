#ifndef FACE_H
#define FACE_H
#include <array>
#include <iosfwd>

enum class Color{
    Blue,
    Red,
    Yellow,
    Green,
    White,
    Orange
};

class Face{
    private:
        std::array<std::array<Color, 3>, 3> cells_;
    public:
        explicit Face(Color color);
        Color at(int r, int c) const;
        void set(int r, int c, Color color);
        void rotateClockwise();
        bool isSolved() const;
        bool operator==(const Face &other) const;
        bool operator!=(const Face &other) const;
};

std::ostream& operator<<(std::ostream &os, Color color);

std::istream& operator>>(std::istream &is, Color &color);

#endif