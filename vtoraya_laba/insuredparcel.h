#ifndef INSUREDPARCEL_H
#define INSUREDPARCEL_H

#include "parcel.h"

class InsuredParcel : public Parcel {
private:
    int declaredValue_;
    int insuranceFee_;
    int compensationRecorded_;

public:
    InsuredParcel(const std::string &trackingNumber, const Customer &sender,
                  const Customer &recipient, int weightGrams, int basePrice,
                  int pricePerKilogram, int declaredValue, int insuranceFee);

    int declaredValue() const;
    int insuranceFee() const;
    int remainingCoverage() const;
    void recordCompensation(int amount);
    int calculatePrice() const override;
    std::unique_ptr<Shipment> clone() const override;
};

#endif
