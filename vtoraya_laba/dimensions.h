#ifndef DIMENSIONS_H
#define DIMENSIONS_H

class Dimensions {
private:
    int length_;
    int width_;
    int height_;

public:
    Dimensions(int length, int width, int height);

    int length() const;
    int width() const;
    int height() const;
    long long volume() const;
    bool fitsInside(const Dimensions &other) const;
    Dimensions inset(int padding) const;
};

#endif
