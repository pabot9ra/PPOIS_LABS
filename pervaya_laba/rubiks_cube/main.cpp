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

Cube URtest;

URtest.moveU();
URtest.moveR();
std::cout << "U[0][2] red: "  << (URtest.face(FaceId::U).at(0, 2) == Color::Red) << '\n';      // 1
std::cout << "U[1][2] green: " << (URtest.face(FaceId::U).at(1, 2) == Color::Green) << '\n';   // 1
std::cout << "D[0][2] blue: " << (URtest.face(FaceId::D).at(0, 2) == Color::Blue) << '\n';     // 1
std::cout << "D[2][2] orange: " << (URtest.face(FaceId::D).at(2, 2) == Color::Orange) << '\n'; // 1
std::cout << "R[0][2] blue: " << (URtest.face(FaceId::R).at(0, 2) == Color::Blue) << '\n';     // 1

Cube Rtest4rot;
for(int i = 0; i < 4; i++){
    Rtest4rot.moveR();
}
std::cout << "after 4R solved: " << Rtest4rot.isSolved() << '\n'; // 1

Cube ULtest;
ULtest.moveU();
ULtest.moveL();

std::cout << "U[0][0] blue: "  << (ULtest.face(FaceId::U).at(0, 0) == Color::Blue) << '\n';     // 1
std::cout << "U[2][0] orange: "  << (ULtest.face(FaceId::U).at(2, 0) == Color::Orange) << '\n'; // 1
std::cout << "D[0][0] red: "  << (ULtest.face(FaceId::D).at(0, 0) == Color::Red) << '\n';       // 1
std::cout << "D[2][0] green: "  << (ULtest.face(FaceId::D).at(2, 0) == Color::Green) << '\n';   // 1
std::cout << "B[0][2] yellow: "  << (ULtest.face(FaceId::B).at(0, 2) == Color::Yellow) << '\n'; // 1
std::cout << "F[0][0] white: "  << (ULtest.face(FaceId::F).at(0, 0) == Color::White) << '\n';   // 1
std::cout << "L[0][2] green: "  << (ULtest.face(FaceId::L).at(0, 2) == Color::Green) << '\n';   // 1


Cube Ltest4rot;

for(int i = 0; i < 4;i++){
    Ltest4rot.moveL(); 
}
std::cout << "after 4L solved: " << Ltest4rot.isSolved() << '\n'; // 1

}