#include <UnitTest++/UnitTest++.h>
#include "face.h"
#include "cube.h"
#include <array>
#include <sstream>
#include <string>

std::array<int, 6> colorCounts(const Cube &cube){
    std::array<int, 6> counts{};
    for(int f = 0; f < 6; f++){
        for(int r = 0; r < 3; r++){
            for(int c = 0; c < 3; c++){
                counts[static_cast<int>(cube.face(static_cast<FaceId>(f)).at(r, c))]++;
            }
        }
    }
    return counts;
}

Cube preparedCube(){
    Cube cube;
    cube.move(FaceId::U, Turn::Clockwise);
    cube.move(FaceId::R, Turn::Clockwise);
    cube.move(FaceId::F, Turn::Clockwise);
    return cube;
}

void commutatorSixTimes(Cube &cube, FaceId a, FaceId b){
    for(int i = 0; i < 6; i++){
        cube.move(a, Turn::Clockwise);
        cube.move(b, Turn::Clockwise);
        cube.move(a, Turn::CounterClockwise);
        cube.move(b, Turn::CounterClockwise);
    }
}

// face tests

TEST(FaceSolvedAfterCreate){
    CHECK_EQUAL(true, Face(Color::Red).isSolved());
}

TEST(FaceSetAt){
    Face face(Color::Red);
    face.set(0, 1, Color::Blue);
    CHECK(face.at(0, 0) == Color::Red);
    CHECK(face.at(0, 1) == Color::Blue);
}

TEST(FaceNotSolvedAfterSet){
    Face face(Color::Red);
    face.set(0, 1, Color::Blue);
    CHECK_EQUAL(false, face.isSolved());
}

TEST(FaceRotateCorner){
    Face face(Color::Red);
    face.set(0, 0, Color::Blue);
    face.rotateClockwise();
    CHECK(face.at(0, 2) == Color::Blue);
    CHECK(face.at(0, 0) == Color::Red);
}
TEST(FaceRotateEdge){
    Face face(Color::Red);
    face.set(0, 1, Color::Blue);
    face.rotateClockwise();
    CHECK(face.at(1, 2) == Color::Blue);

}
TEST(FaceFourRotations){
    Face face(Color::Red);
    face.set(0, 1, Color::Blue);
    face.set(2, 0, Color::Green);
    Face facebefore4rot = face;
    for(int i = 0; i < 4; i++){
        face.rotateClockwise();
    }
    CHECK(facebefore4rot.at(0, 1) == face.at(0, 1));
    CHECK(facebefore4rot.at(2, 0) == face.at(2, 0));
    CHECK(face.at(0, 1) == Color::Blue);
    CHECK(face.at(2, 0) == Color::Green);
    CHECK(facebefore4rot.at(0, 1) == Color::Blue);
    CHECK(facebefore4rot.at(2, 0) == Color::Green);
}

TEST(FaceEqualNotEqual){
    Face face1(Color::Red);
    Face face2(Color::Red);
    CHECK(face1 == face2);
    face2.set(1, 1, Color::Blue);
    CHECK(!(face1 == face2));
}

// cube tests

TEST(CubeSolvedAfterCreate){
    CHECK_EQUAL(true, Cube().isSolved());
}

TEST(CubeNotSolvedAfterMove){
    Cube cube;
    cube.move(FaceId::U, Turn::Clockwise);
    CHECK_EQUAL(false, cube.isSolved());
}


TEST(CubeCentersAfterCreate){
    Cube cube;
    CHECK_EQUAL(Color::White, cube.face(FaceId::U).at(1, 1));
    CHECK_EQUAL(Color::Orange, cube.face(FaceId::L).at(1, 1));
    CHECK_EQUAL(Color::Green, cube.face(FaceId::F).at(1, 1));
    CHECK_EQUAL(Color::Red, cube.face(FaceId::R).at(1, 1));
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::B).at(1, 1));
    CHECK_EQUAL(Color::Yellow, cube.face(FaceId::D).at(1, 1));
}

TEST(CubeEqualNotEqual){
    Cube a;
    Cube b;
    CHECK(a == b);
    CHECK_EQUAL(false, a != b);
    b.move(FaceId::R, Turn::Clockwise);
    CHECK_EQUAL(false, a == b);
    CHECK(a != b);
}

TEST(CubeEqualAfterSameMoves){
    Cube a = preparedCube();
    Cube b = preparedCube();
    CHECK(a == b);
    a.move(FaceId::L, Turn::Clockwise);
    b.move(FaceId::L, Turn::CounterClockwise);
    CHECK(a != b);
}


TEST(MoveU){
    Cube cube;
    cube.move(FaceId::U, Turn::Clockwise);
    CHECK_EQUAL(Color::Green, cube.face(FaceId::L).at(0, 0));
    CHECK_EQUAL(Color::Red, cube.face(FaceId::F).at(0, 0));
    CHECK_EQUAL(Color::Green, cube.face(FaceId::F).at(1, 0));
}

TEST(MoveR){
    Cube cube;
    cube.move(FaceId::U, Turn::Clockwise);
    cube.move(FaceId::R, Turn::Clockwise);
    CHECK_EQUAL(Color::Red, cube.face(FaceId::U).at(0, 2));
    CHECK_EQUAL(Color::Green, cube.face(FaceId::U).at(1, 2));
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::D).at(0, 2));
    CHECK_EQUAL(Color::Orange, cube.face(FaceId::D).at(2, 2));
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::R).at(0, 2));
}

TEST(MoveRFlipsColumnOnB){
    Cube cube;
    cube.move(FaceId::F, Turn::Clockwise);
    cube.move(FaceId::R, Turn::Clockwise);
    CHECK_EQUAL(Color::Orange, cube.face(FaceId::B).at(0, 0));
    CHECK_EQUAL(Color::White, cube.face(FaceId::B).at(2, 0));
}

TEST(MoveL){
    Cube cube;
    cube.move(FaceId::U, Turn::Clockwise);
    cube.move(FaceId::L, Turn::Clockwise);
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::U).at(0, 0));
    CHECK_EQUAL(Color::Orange, cube.face(FaceId::U).at(2, 0));
    CHECK_EQUAL(Color::Red, cube.face(FaceId::D).at(0, 0));
    CHECK_EQUAL(Color::Green, cube.face(FaceId::D).at(2, 0));
    CHECK_EQUAL(Color::Yellow, cube.face(FaceId::B).at(0, 2));
    CHECK_EQUAL(Color::White, cube.face(FaceId::F).at(0, 0));
    CHECK_EQUAL(Color::Green, cube.face(FaceId::L).at(0, 2));
}

TEST(MoveD){
    Cube cube;
    cube.move(FaceId::R, Turn::Clockwise);
    cube.move(FaceId::D, Turn::Clockwise);
    CHECK_EQUAL(Color::Green, cube.face(FaceId::R).at(2, 0));
    CHECK_EQUAL(Color::Yellow, cube.face(FaceId::R).at(2, 2));
    CHECK_EQUAL(Color::Red, cube.face(FaceId::B).at(2, 0));
    CHECK_EQUAL(Color::White, cube.face(FaceId::L).at(2, 0));
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::L).at(2, 2));
    CHECK_EQUAL(Color::Orange, cube.face(FaceId::F).at(2, 0));
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::D).at(2, 2));
}

TEST(MoveF){
    Cube cube;
    cube.move(FaceId::U, Turn::Clockwise);
    cube.move(FaceId::F, Turn::Clockwise);
    CHECK_EQUAL(Color::Orange, cube.face(FaceId::U).at(2, 0));
    CHECK_EQUAL(Color::Green, cube.face(FaceId::U).at(2, 2));
    CHECK_EQUAL(Color::White, cube.face(FaceId::R).at(0, 0));
    CHECK_EQUAL(Color::Red, cube.face(FaceId::D).at(0, 0));
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::D).at(0, 2));
    CHECK_EQUAL(Color::Yellow, cube.face(FaceId::L).at(0, 2));
    CHECK_EQUAL(Color::Green, cube.face(FaceId::F).at(0, 0));
    CHECK_EQUAL(Color::Red, cube.face(FaceId::F).at(0, 2));
}

TEST(MoveB){
    Cube cube;
    cube.move(FaceId::U, Turn::Clockwise);
    cube.move(FaceId::B, Turn::Clockwise);
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::U).at(0, 0));
    CHECK_EQUAL(Color::Red, cube.face(FaceId::U).at(0, 2));
    CHECK_EQUAL(Color::Yellow, cube.face(FaceId::R).at(0, 2));
    CHECK_EQUAL(Color::Green, cube.face(FaceId::D).at(2, 0));
    CHECK_EQUAL(Color::Orange, cube.face(FaceId::D).at(2, 2));
    CHECK_EQUAL(Color::White, cube.face(FaceId::L).at(0, 0));
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::B).at(0, 0));
    CHECK_EQUAL(Color::Orange, cube.face(FaceId::B).at(0, 2));
}


TEST(FourU){
    Cube cube = preparedCube();
    Cube before = cube;
    for(int i = 0; i < 4; i++) cube.move(FaceId::U, Turn::Clockwise);
    CHECK(before == cube);
}

TEST(FourL){
    Cube cube = preparedCube();
    Cube before = cube;
    for(int i = 0; i < 4; i++) cube.move(FaceId::L, Turn::Clockwise);
    CHECK(before == cube);
}

TEST(FourF){
    Cube cube = preparedCube();
    Cube before = cube;
    for(int i = 0; i < 4; i++) cube.move(FaceId::F, Turn::Clockwise);
    CHECK(before == cube);
}

TEST(FourR){
    Cube cube = preparedCube();
    Cube before = cube;
    for(int i = 0; i < 4; i++) cube.move(FaceId::R, Turn::Clockwise);
    CHECK(before == cube);
}

TEST(FourB){
    Cube cube = preparedCube();
    Cube before = cube;
    for(int i = 0; i < 4; i++) cube.move(FaceId::B, Turn::Clockwise);
    CHECK(before == cube);
}

TEST(FourD){
    Cube cube = preparedCube();
    Cube before = cube;
    for(int i = 0; i < 4; i++) cube.move(FaceId::D, Turn::Clockwise);
    CHECK(before == cube);
}


TEST(CounterEqualsThreeClockwise){
    Cube counter = preparedCube();
    Cube three = preparedCube();
    counter.move(FaceId::R, Turn::CounterClockwise);
    for(int i = 0; i < 3; i++) three.move(FaceId::R, Turn::Clockwise);
    CHECK(counter == three);
}

TEST(DoubleEqualsTwoClockwise){
    Cube twice = preparedCube();
    Cube two = preparedCube();
    twice.move(FaceId::F, Turn::DoubleMove);
    two.move(FaceId::F, Turn::Clockwise);
    two.move(FaceId::F, Turn::Clockwise);
    CHECK(twice == two);
}

TEST(ClockwiseThenCounterIsIdentity){
    Cube cube = preparedCube();
    Cube before = cube;
    cube.move(FaceId::B, Turn::Clockwise);
    cube.move(FaceId::B, Turn::CounterClockwise);
    CHECK(before == cube);
}

TEST(CommutatorRUSixTimes){
    Cube cube;
    commutatorSixTimes(cube, FaceId::R, FaceId::U);
    CHECK_EQUAL(true, cube.isSolved());
}

TEST(CommutatorFLSixTimes){
    Cube cube;
    commutatorSixTimes(cube, FaceId::F, FaceId::L);
    CHECK_EQUAL(true, cube.isSolved());
}

TEST(CommutatorBDSixTimes){
    Cube cube;
    commutatorSixTimes(cube, FaceId::B, FaceId::D);
    CHECK_EQUAL(true, cube.isSolved());
}


TEST(MixNotSolved){
    Cube cube;
    cube.mix(25);
    CHECK_EQUAL(false, cube.isSolved());
}

TEST(MixKeepsColorsAndCenters){
    Cube cube;
    cube.mix(25);
    std::array<int, 6> counts = colorCounts(cube);
    for(int i = 0; i < 6; i++){
        CHECK_EQUAL(9, counts[i]);
    }
    CHECK_EQUAL(Color::White, cube.face(FaceId::U).at(1, 1));
    CHECK_EQUAL(Color::Orange, cube.face(FaceId::L).at(1, 1));
    CHECK_EQUAL(Color::Green, cube.face(FaceId::F).at(1, 1));
    CHECK_EQUAL(Color::Red, cube.face(FaceId::R).at(1, 1));
    CHECK_EQUAL(Color::Blue, cube.face(FaceId::B).at(1, 1));
    CHECK_EQUAL(Color::Yellow, cube.face(FaceId::D).at(1, 1));
}


TEST(ColorOutput){
    std::ostringstream os;
    os << Color::Green;
    CHECK_EQUAL("G", os.str());
}

TEST(ColorInput){
    std::istringstream input("G X");
    Color color;
    input >> color;
    CHECK_EQUAL(Color::Green, color);
    CHECK_EQUAL(false, !input);
    input >> color;
    CHECK_EQUAL(true, !input);
}


TEST(ReadRoundTrip){
    Cube original;
    original.mix(25);
    std::stringstream buffer;
    buffer << original;
    Cube loaded;
    buffer >> loaded;
    CHECK_EQUAL(false, !buffer);
    CHECK(original == loaded);
}

TEST(ReadBadLetter){
    std::istringstream input("W W W W W W W W X");
    Cube cube;
    input >> cube;
    CHECK_EQUAL(true, !input);
    CHECK_EQUAL(true, cube.isSolved());
}

TEST(ReadShortInput){
    std::istringstream input("W W W");
    Cube cube;
    input >> cube;
    CHECK_EQUAL(true, !input);
    CHECK_EQUAL(true, cube.isSolved());
}

TEST(ReadAllWhite){
    std::string text;
    for(int i = 0; i < 54; i++){
        text += "W ";
    }
    std::istringstream input(text);
    Cube cube;
    input >> cube;
    CHECK_EQUAL(true, !input);
    CHECK_EQUAL(true, cube.isSolved());
}

TEST(ReadDuplicateCenters){
    std::istringstream input(
        "W W W\n"
        "W G W\n"
        "W W W\n"
        "O O O G W G R R R B B B\n"
        "O O O G G G R R R B B B\n"
        "O O O G G G R R R B B B\n"
        "Y Y Y\n"
        "Y Y Y\n"
        "Y Y Y\n");
    Cube cube;
    input >> cube;
    CHECK_EQUAL(true, !input);
    CHECK_EQUAL(true, cube.isSolved());
}


int main(){
    return UnitTest::RunAllTests();
}
