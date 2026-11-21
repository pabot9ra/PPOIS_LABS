#ifndef LETTER_H
#define LETTER_H

#include "shipment.h"

class Letter : public Shipment {
private:
    int weightGrams_;
    int pricePerGram_;

public:
    Letter(const std::string &trackingNumber, const Customer &sender,
           const Customer &recipient, int weightGrams, int pricePerGram);

    int weightGrams() const;
    int pricePerGram() const;
    int calculatePrice() const override;
    std::unique_ptr<Shipment> clone() const override;
};

#endif
