#include "envelope.h"

Envelope::Envelope(const Dimensions &innerDimensions, int maximumWeight, int price,
                   bool reinforced, bool waterproof)
    : Packaging(innerDimensions, maximumWeight, price),
      reinforced_(reinforced), waterproof_(waterproof) {
}

bool Envelope::reinforced() const {
    return reinforced_;
}

bool Envelope::waterproof() const {
    return waterproof_;
}

std::string Envelope::label() const {
    std::string result = "ENVELOPE";
    if(reinforced_){
        result += "; REINFORCED";
    }
    if(waterproof_){
        result += "; WATERPROOF";
    }
    return result;
}
