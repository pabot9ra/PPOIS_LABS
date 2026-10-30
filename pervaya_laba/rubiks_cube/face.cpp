#include "face.h"
#include <ostream>
#include <istream>

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
    auto old = cells_;
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

bool Face::operator!=(const Face&other) const{
    return !((*this) == other);
}

std::ostream& operator<<(std::ostream &os, Color color){
    switch(color){
        case Color::Blue: 
            os << 'B';
            break;
        case Color::Red: 
            os << 'R';
            break;
        case Color::Yellow: 
            os << 'Y';
            break;
        case Color::Green: 
            os << 'G';
            break;
        case Color::White: 
            os << 'W';
            break;
        case Color::Orange: 
            os << 'O';
            break;
    }
    return os;
}

std::istream& operator>>(std::istream &is, Color &color){
    char colorSymbol;
    is >> colorSymbol;
    if(!is){
        return is;
    }
    switch(colorSymbol){
        case 'B':
            color = Color::Blue;
            break;
        case 'R':
            color = Color::Red;
            break;
        case 'Y':
            color = Color::Yellow;
            break;
        case 'G':
            color = Color::Green;
            break;
        case 'W':
            color = Color::White;
            break;
        case 'O':
            color = Color::Orange;
            break;
        default:
            is.setstate(std::ios::failbit);
            break;

    }
    return is;
}