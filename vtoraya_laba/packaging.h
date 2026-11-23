#ifndef PACKAGING_H
#define PACKAGING_H

#include "dimensions.h"
#include "shipment.h"
#include <string>

class Packaging {
private:
    Dimensions innerDimensions_;
    int maximumWeight_;
    int price_;
    std::string trackingNumber_;
    bool sealed_;

public:
    Packaging(const Dimensions &innerDimensions, int maximumWeight, int price);
    virtual ~Packaging() = default;

    const Dimensions &innerDimensions() const;
    int maximumWeight() const;
    int price() const;
    const std::string &trackingNumber() const;
    bool sealed() const;
    virtual std::string label() const = 0;

    void pack(const Shipment &shipment, const Dimensions &contents);
    void seal();
    void open();
    void unpack();
};

#endif
