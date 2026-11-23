#include "packaging.h"
#include <stdexcept>

Packaging::Packaging(const Dimensions &innerDimensions, int maximumWeight, int price)
    : innerDimensions_(innerDimensions), maximumWeight_(maximumWeight),
      price_(price), sealed_(false) {
    if(maximumWeight_ <= 0 || price_ < 0){
        throw std::invalid_argument("Maximum weight must be positive; packaging price must not be negative");
    }
}

const Dimensions &Packaging::innerDimensions() const {
    return innerDimensions_;
}

int Packaging::maximumWeight() const {
    return maximumWeight_;
}

int Packaging::price() const {
    return price_;
}

const std::string &Packaging::trackingNumber() const {
    return trackingNumber_;
}

bool Packaging::sealed() const {
    return sealed_;
}

void Packaging::pack(const Shipment &shipment, const Dimensions &contents) {
    if(!trackingNumber_.empty() || shipment.status() != ShipmentStatus::Accepted){
        throw std::logic_error("Packaging must be empty and shipment must be newly accepted");
    }
    if(shipment.weightGrams() > maximumWeight_ || !contents.fitsInside(innerDimensions_)){
        throw std::invalid_argument("Shipment exceeds packaging weight or size limit");
    }
    trackingNumber_ = shipment.trackingNumber();
}

void Packaging::seal() {
    if(trackingNumber_.empty() || sealed_){
        throw std::logic_error("Only filled, unsealed packaging can be sealed");
    }
    sealed_ = true;
}

void Packaging::open() {
    if(!sealed_){
        throw std::logic_error("Packaging is not sealed");
    }
    sealed_ = false;
}

void Packaging::unpack() {
    if(trackingNumber_.empty() || sealed_){
        throw std::logic_error("Only filled, open packaging can be unpacked");
    }
    trackingNumber_.clear();
}
