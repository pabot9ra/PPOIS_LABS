#include "insuredparcel.h"
#include <limits>
#include <stdexcept>

InsuredParcel::InsuredParcel(const std::string &trackingNumber, const Customer &sender,
                             const Customer &recipient, int weightGrams, int basePrice,
                             int pricePerKilogram, int declaredValue, int insuranceFee)
    : Parcel(trackingNumber, sender, recipient, weightGrams, basePrice, pricePerKilogram),
      declaredValue_(declaredValue), insuranceFee_(insuranceFee), compensationRecorded_(0) {
    if(declaredValue_ <= 0 || insuranceFee_ < 0){
        throw std::invalid_argument("Declared value must be positive; insurance fee must not be negative");
    }
    if(insuranceFee_ > std::numeric_limits<int>::max() - Parcel::calculatePrice()){
        throw std::overflow_error("Insured parcel price is too large");
    }
}

int InsuredParcel::declaredValue() const {
    return declaredValue_;
}

int InsuredParcel::insuranceFee() const {
    return insuranceFee_;
}

int InsuredParcel::remainingCoverage() const {
    return declaredValue_ - compensationRecorded_;
}

void InsuredParcel::recordCompensation(int amount) {
    if(status() != ShipmentStatus::Delivered){
        throw std::logic_error("Damage compensation can be recorded after delivery");
    }
    if(amount <= 0 || amount > remainingCoverage()){
        throw std::invalid_argument("Compensation must be positive and within remaining coverage");
    }
    compensationRecorded_ += amount;
}

int InsuredParcel::calculatePrice() const {
    return Parcel::calculatePrice() + insuranceFee_;
}

std::unique_ptr<Shipment> InsuredParcel::clone() const {
    return std::make_unique<InsuredParcel>(*this);
}
