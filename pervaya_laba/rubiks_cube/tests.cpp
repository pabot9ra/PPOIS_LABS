#include <UnitTest++/UnitTest++.h>
#include "face.h"

//face tests

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
int main(){
    return UnitTest::RunAllTests();
}