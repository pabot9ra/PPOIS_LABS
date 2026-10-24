#include "face.h"

Face::Face(Color color){
    for(int r = 0; r < 3; r++){
        for(int c = 0; c < 3; c++){
            cells_[r][c] = color;
        }
    }
}

Color Face::at(int r, int c) const{
    return cells_[r][c];
}

void Face::set(int r, int c, Color color){
    cells_[r][c] = color;
}

void Face::rotateClockwise(){
    std::array old = cells_;
    for(int r = 0; r < 3; r++){
        for(int c = 0; c < 3; c++){
            cells_[c][2-r] = old[r][c];
        }
    }
}

bool Face::isSolved() const{
    for(int r = 0; r < 3; r++){
        for(int c = 0; c < 3; c++){
            if(cells_[r][c] != cells_[0][0]){
                return false;
            }
        }
    }
    return true;

}

bool Face::operator==(const Face &other) const{
    for(int r = 0; r < 3; r++){
        for(int c = 0; c < 3; c++){
            if(cells_[r][c] != other.cells_[r][c]){
                return false;
            }
        }
    }
    return true;
}
