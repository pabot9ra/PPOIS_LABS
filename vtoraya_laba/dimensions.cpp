#include "dimensions.h"
#include <limits>
#include <stdexcept>

Dimensions::Dimensions(int length, int width, int height)
    : length_(length), width_(width), height_(height) {
    if(length_ <= 0 || width_ <= 0 || height_ <= 0){
        throw std::invalid_argument("Dimensions in millimeters must be positive");
    }
    long long area = static_cast<long long>(length_) * width_;
    if(area > std::numeric_limits<long long>::max() / height_){
        throw std::overflow_error("Volume is too large");
    }
}

int Dimensions::length() const {
    return length_;
}

int Dimensions::width() const {
    return width_;
}

int Dimensions::height() const {
    return height_;
}

long long Dimensions::volume() const {
    return static_cast<long long>(length_) * width_ * height_;
}

bool Dimensions::fitsInside(const Dimensions &other) const {
    return (length_ <= other.length_ && width_ <= other.width_ && height_ <= other.height_) ||
           (length_ <= other.length_ && height_ <= other.width_ && width_ <= other.height_) ||
           (width_ <= other.length_ && length_ <= other.width_ && height_ <= other.height_) ||
           (width_ <= other.length_ && height_ <= other.width_ && length_ <= other.height_) ||
           (height_ <= other.length_ && length_ <= other.width_ && width_ <= other.height_) ||
           (height_ <= other.length_ && width_ <= other.width_ && length_ <= other.height_);
}

Dimensions Dimensions::inset(int padding) const {
    if(padding < 0 || padding > (length_ - 1) / 2 ||
       padding > (width_ - 1) / 2 || padding > (height_ - 1) / 2){
        throw std::invalid_argument("Padding must leave positive inner dimensions");
    }
    return Dimensions(length_ - 2 * padding, width_ - 2 * padding, height_ - 2 * padding);
}
