#include "cube.h"
#include <ostream>

Cube::Cube()
    : faces_{
          Face(Color::White),  // U
          Face(Color::Orange), // L
          Face(Color::Green),  // F
          Face(Color::Red),    // R
          Face(Color::Blue),   // B
          Face(Color::Yellow)  // D
      } {}

const Face &Cube::face(FaceId id) const { return faces_[static_cast<int>(id)]; }

bool Cube::isSolved() const {
    for (int i = 0; i < 6; i++) {
        if (!faces_[i].isSolved()) {
            return false;
        }
    }
    return true;
}

void Cube::moveU() {
    Cube old = *this;
    faces_[static_cast<int>(FaceId::U)].rotateClockwise();
    for (int i = 0; i < 3; i++) {
        faces_[static_cast<int>(FaceId::L)].set(0, i, old.faces_[static_cast<int>(FaceId::F)].at(0, i));
        faces_[static_cast<int>(FaceId::B)].set(0, i, old.faces_[static_cast<int>(FaceId::L)].at(0, i));
        faces_[static_cast<int>(FaceId::R)].set(0, i, old.faces_[static_cast<int>(FaceId::B)].at(0, i));
        faces_[static_cast<int>(FaceId::F)].set(0, i, old.faces_[static_cast<int>(FaceId::R)].at(0, i));
    }
}

void Cube::moveR() {
    Cube old = *this;
    faces_[static_cast<int>(FaceId::R)].rotateClockwise();
    for (int i = 0; i < 3; i++) {
        faces_[static_cast<int>(FaceId::U)].set(i, 2, old.faces_[static_cast<int>(FaceId::F)].at(i, 2));
        faces_[static_cast<int>(FaceId::B)].set(i, 0, old.faces_[static_cast<int>(FaceId::U)].at(2 - i, 2));
        faces_[static_cast<int>(FaceId::D)].set(i, 2, old.faces_[static_cast<int>(FaceId::B)].at(2 - i, 0));
        faces_[static_cast<int>(FaceId::F)].set(i, 2, old.faces_[static_cast<int>(FaceId::D)].at(i, 2));
    }
}

void Cube::moveL() {
    Cube old = *this;
    faces_[static_cast<int>(FaceId::L)].rotateClockwise();
    for (int i = 0; i < 3; i++) {
        faces_[static_cast<int>(FaceId::U)].set(i, 0, old.faces_[static_cast<int>(FaceId::B)].at(2 - i, 2));
        faces_[static_cast<int>(FaceId::B)].set(i, 2, old.faces_[static_cast<int>(FaceId::D)].at(2 - i, 0));
        faces_[static_cast<int>(FaceId::D)].set(i, 0, old.faces_[static_cast<int>(FaceId::F)].at(i, 0));
        faces_[static_cast<int>(FaceId::F)].set(i, 0, old.faces_[static_cast<int>(FaceId::U)].at(i, 0));
    }
}

void Cube::moveD() {
    Cube old = *this;
    faces_[static_cast<int>(FaceId::D)].rotateClockwise();
    for (int i = 0; i < 3; i++) {
        faces_[static_cast<int>(FaceId::R)].set(2, i, old.faces_[static_cast<int>(FaceId::F)].at(2, i));
        faces_[static_cast<int>(FaceId::L)].set(2, i, old.faces_[static_cast<int>(FaceId::B)].at(2, i));
        faces_[static_cast<int>(FaceId::B)].set(2, i, old.faces_[static_cast<int>(FaceId::R)].at(2, i));
        faces_[static_cast<int>(FaceId::F)].set(2, i, old.faces_[static_cast<int>(FaceId::L)].at(2, i));
    }
}

void Cube::moveF() {
    Cube old = *this;
    faces_[static_cast<int>(FaceId::F)].rotateClockwise();
    for (int i = 0; i < 3; i++) {
        faces_[static_cast<int>(FaceId::L)].set(i, 2, old.faces_[static_cast<int>(FaceId::D)].at(0, i));
        faces_[static_cast<int>(FaceId::U)].set(2, i, old.faces_[static_cast<int>(FaceId::L)].at(2 - i, 2));
        faces_[static_cast<int>(FaceId::R)].set(i, 0, old.faces_[static_cast<int>(FaceId::U)].at(2, i));
        faces_[static_cast<int>(FaceId::D)].set(0, i, old.faces_[static_cast<int>(FaceId::R)].at(2 - i, 0));
    }
}

void Cube::moveB() {
    Cube old = *this;
    faces_[static_cast<int>(FaceId::B)].rotateClockwise();
    for (int i = 0; i < 3; i++) {
        faces_[static_cast<int>(FaceId::L)].set(i, 0, old.faces_[static_cast<int>(FaceId::U)].at(0, 2 - i));
        faces_[static_cast<int>(FaceId::D)].set(2, i, old.faces_[static_cast<int>(FaceId::L)].at(i, 0));
        faces_[static_cast<int>(FaceId::R)].set(i, 2, old.faces_[static_cast<int>(FaceId::D)].at(2, 2 - i));
        faces_[static_cast<int>(FaceId::U)].set(0, i, old.faces_[static_cast<int>(FaceId::R)].at(i, 2));
    }
}

void Cube::move(FaceId face, Turn turn) {
    int times;
    if (turn == Turn::Clockwise) {
        times = 1;
    } else if (turn == Turn::CounterClockwise) {
        times = 3;
    } else {
        times = 2;
    }
    for (int i = 0; i < times; i++) {
        switch (face) {
        case FaceId::U:
            moveU();
            break;
        case FaceId::L:
            moveL();
            break;
        case FaceId::F:
            moveF();
            break;
        case FaceId::R:
            moveR();
            break;
        case FaceId::B:
            moveB();
            break;
        case FaceId::D:
            moveD();
            break;
        }
    }
}

std::ostream &operator<<(std::ostream &os,const Cube &cube) {

    for (int i = 0; i < 3; i++) {
        os << "      ";
        for (int j = 0; j < 3; j++) {

            os << cube.face(FaceId::U).at(i, j) << ' ';
        }
        os << '\n';

    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            os << cube.face(FaceId::L).at(i, j) << ' ';
        }
        for (int j = 0; j < 3; j++) {
            os << cube.face(FaceId::F).at(i, j) << ' ';
        }
        for (int j = 0; j < 3; j++) {
            os << cube.face(FaceId::R).at(i, j) << ' ';
        }
        for (int j = 0; j < 3; j++) {
            os << cube.face(FaceId::B).at(i, j) << ' ';
        }
        os << '\n';
    }
    
    for (int i = 0; i < 3; i++) {
        os << "      ";
        for (int j = 0; j < 3; j++) {

            os << cube.face(FaceId::D).at(i, j) << ' ';
        }
        os << '\n';
    }
    return os;
}