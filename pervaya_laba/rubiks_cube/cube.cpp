#include "cube.h"

Cube::Cube()
    : faces_{ Face(Color::White), // U
              Face(Color::Orange), // L
              Face(Color::Green), // F
              Face(Color::Red), // R
              Face(Color::Blue), // B
              Face(Color::Yellow) // D
    }
    {
}

const Face& Cube::face(FaceId id) const{
    return faces_[static_cast<int>(id)];
}

bool Cube::isSolved() const{
    for(int i = 0; i < 6; i++){
        if(!faces_[i].isSolved()){
            return false;
        }
    }
    return true;
}

void Cube::moveU(){
    Cube old = *this;
    faces_[static_cast<int>(FaceId::U)].rotateClockwise();
    for(int i = 0; i < 3; i++){
        faces_[static_cast<int>(FaceId::L)].set(0, i, old.faces_[static_cast<int>(FaceId::F)].at(0, i));
        faces_[static_cast<int>(FaceId::B)].set(0, i, old.faces_[static_cast<int>(FaceId::L)].at(0, i));
        faces_[static_cast<int>(FaceId::R)].set(0, i, old.faces_[static_cast<int>(FaceId::B)].at(0, i));
        faces_[static_cast<int>(FaceId::F)].set(0, i, old.faces_[static_cast<int>(FaceId::R)].at(0, i));
    }
}