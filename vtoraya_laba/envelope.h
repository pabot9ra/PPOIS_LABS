#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "packaging.h"

class Envelope : public Packaging {
private:
    bool reinforced_;
    bool waterproof_;

public:
    Envelope(const Dimensions &innerDimensions, int maximumWeight, int price,
             bool reinforced, bool waterproof);

    bool reinforced() const;
    bool waterproof() const;
    std::string label() const override;
};

#endif
