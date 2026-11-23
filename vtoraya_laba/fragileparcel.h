#ifndef FRAGILEPARCEL_H
#define FRAGILEPARCEL_H

#include "parcel.h"

class FragileParcel : public Parcel {
private:
    int packagingFee_;
    bool keepUpright_;

public:
    FragileParcel(const std::string &trackingNumber, const Customer &sender,
                  const Customer &recipient, int weightGrams, int basePrice,
                  int pricePerKilogram, int packagingFee, bool keepUpright);

    int packagingFee() const;
    bool keepUpright() const;
    std::string handlingLabel() const;
    void checkOrientation(bool upright) const;
    int calculatePrice() const override;
    std::unique_ptr<Shipment> clone() const override;
};

#endif
