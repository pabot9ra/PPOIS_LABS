#include "box.h"
#include <cctype>
#include <stdexcept>

Box::Box(const Dimensions &outerDimensions, int maximumWeight, int price,
         int padding, const std::string &material)
    : Packaging(outerDimensions.inset(padding), maximumWeight, price),
      padding_(padding), material_(material) {
    bool hasMaterial = false;
    for(char character : material_){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasMaterial = true;
            break;
        }
    }
    if(!hasMaterial){
        throw std::invalid_argument("Box material must not be blank");
    }
}

int Box::padding() const {
    return padding_;
}

const std::string &Box::material() const {
    return material_;
}

Dimensions Box::outerDimensions() const {
    const Dimensions &inner = innerDimensions();
    return Dimensions(inner.length() + 2 * padding_, inner.width() + 2 * padding_, inner.height() + 2 * padding_);
}

std::string Box::label() const {
    return "BOX; " + material_ + "; PADDING " + std::to_string(padding_) + " mm";
}
