#include <iostream>
#include "face.h"
#include "cube.h"
int main() {
    Face f(Color::Red);
    std::cout << "solved: " << f.isSolved() << '\n';      // 1

    f.set(0, 0, Color::Blue);
    std::cout << "solved: " << f.isSolved() << '\n';      // 0

    f.rotateClockwise();
    std::cout << "[0][2] blue: " << (f.at(0, 2) == Color::Blue) << '\n';  // 1
    std::cout << "[0][0] red: "  << (f.at(0, 0) == Color::Red)  << '\n';  // 1

    Face g = f;
    for (int i = 0; i < 4; i++) g.rotateClockwise();
    std::cout << "4 turns same: " << (g == f) << '\n';    // 1

    Cube cube;
    std::cout << "cube solved: " << cube.isSolved() << '\n'; // 1
    std::cout << "L orange: " << (cube.face(FaceId::L).at(1, 1) == Color::Orange) << '\n'; // 1

    Cube c;
c.moveU();
std::cout << "after U solved: " << c.isSolved() << '\n';                                   // 0
std::cout << "L[0][0] green: " << (c.face(FaceId::L).at(0, 0) == Color::Green) << '\n';    // 1
std::cout << "F[0][0] red: "   << (c.face(FaceId::F).at(0, 0) == Color::Red) << '\n';      // 1
std::cout << "F[1][0] green: " << (c.face(FaceId::F).at(1, 0) == Color::Green) << '\n';    // 1
for (int i = 0; i < 3; i++) c.moveU();
std::cout << "after 4U solved: " << c.isSolved() << '\n';                                  // 1
}