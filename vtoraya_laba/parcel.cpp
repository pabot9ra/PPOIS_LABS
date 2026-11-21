#include "parcel.h"
#include <limits>
#include <stdexcept>

namespace {
const int GRAMS_PER_KILOGRAM = 1000;
}

Parcel::Parcel(const std::string &trackingNumber, const Customer &sender,
               const Customer &recipient, int weightGrams, int basePrice,
               int pricePerKilogram)
    : Shipment(trackingNumber, sender, recipient), weightGrams_(weightGrams),
      basePrice_(basePrice), pricePerKilogram_(pricePerKilogram) {
    if(weightGrams_ <= 0 || basePrice_ < 0 || pricePerKilogram_ <= 0){
        throw std::invalid_argument("Weight and kilogram rate must be positive; base price must not be negative");
    }
    int availablePrice = std::numeric_limits<int>::max() - basePrice_;
    if(pricePerKilogram_ > availablePrice / chargedKilograms()){
        throw std::overflow_error("Parcel price is too large");
    }
}

int Parcel::weightGrams() const {
    return weightGrams_;
}

int Parcel::basePrice() const {
    return basePrice_;
}

int Parcel::pricePerKilogram() const {
    return pricePerKilogram_;
}

int Parcel::chargedKilograms() const {
    int kilograms = weightGrams_ / GRAMS_PER_KILOGRAM;
    if(weightGrams_ % GRAMS_PER_KILOGRAM != 0){
        kilograms++;
    }
    return kilograms;
}

int Parcel::calculatePrice() const {
    return basePrice_ + chargedKilograms() * pricePerKilogram_;
}

std::unique_ptr<Shipment> Parcel::clone() const {
    return std::make_unique<Parcel>(*this);
}
