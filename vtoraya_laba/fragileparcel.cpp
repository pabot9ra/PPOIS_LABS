#include "fragileparcel.h"
#include <limits>
#include <stdexcept>

FragileParcel::FragileParcel(const std::string &trackingNumber, const Customer &sender,
                             const Customer &recipient, int weightGrams, int basePrice,
                             int pricePerKilogram, int packagingFee, bool keepUpright)
    : Parcel(trackingNumber, sender, recipient, weightGrams, basePrice, pricePerKilogram),
      packagingFee_(packagingFee), keepUpright_(keepUpright) {
    if(packagingFee_ < 0){
        throw std::invalid_argument("Packaging fee must not be negative");
    }
    if(packagingFee_ > std::numeric_limits<int>::max() - Parcel::calculatePrice()){
        throw std::overflow_error("Fragile parcel price is too large");
    }
}

int FragileParcel::packagingFee() const {
    return packagingFee_;
}

bool FragileParcel::keepUpright() const {
    return keepUpright_;
}

std::string FragileParcel::handlingLabel() const {
    if(keepUpright_){
        return "FRAGILE; KEEP UPRIGHT";
    }
    return "FRAGILE";
}

void FragileParcel::checkOrientation(bool upright) const {
    if(keepUpright_ && !upright){
        throw std::logic_error("This parcel must remain upright");
    }
}

int FragileParcel::calculatePrice() const {
    return Parcel::calculatePrice() + packagingFee_;
}

std::unique_ptr<Shipment> FragileParcel::clone() const {
    return std::make_unique<FragileParcel>(*this);
}
