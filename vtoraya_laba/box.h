#ifndef BOX_H
#define BOX_H

#include "packaging.h"
#include <string>

class Box : public Packaging {
private:
    int padding_;
    std::string material_;

public:
    Box(const Dimensions &outerDimensions, int maximumWeight, int price,
        int padding, const std::string &material);

    int padding() const;
    const std::string &material() const;
    Dimensions outerDimensions() const;
    std::string label() const override;
};

#endif
