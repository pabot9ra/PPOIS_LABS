#ifndef PARCEL_H
#define PARCEL_H

#include "shipment.h"

class Parcel : public Shipment {
private:
    int weightGrams_;
    int basePrice_;
    int pricePerKilogram_;

public:
    Parcel(const std::string &trackingNumber, const Customer &sender,
           const Customer &recipient, int weightGrams, int basePrice,
           int pricePerKilogram);

    int weightGrams() const override;
    int basePrice() const;
    int pricePerKilogram() const;
    int chargedKilograms() const;
    int calculatePrice() const override;
    std::unique_ptr<Shipment> clone() const override;
};

#endif
