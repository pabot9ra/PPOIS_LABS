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