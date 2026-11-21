#include "letter.h"
#include <limits>
#include <stdexcept>

Letter::Letter(const std::string &trackingNumber, const Customer &sender,
               const Customer &recipient, int weightGrams, int pricePerGram)
    : Shipment(trackingNumber, sender, recipient), weightGrams_(weightGrams),
      pricePerGram_(pricePerGram) {
    if(weightGrams_ <= 0 || pricePerGram_ <= 0){
        throw std::invalid_argument("Weight and price per gram must be positive");
    }
    if(pricePerGram_ > std::numeric_limits<int>::max() / weightGrams_){
        throw std::overflow_error("Letter price is too large");
    }
}

int Letter::weightGrams() const {
    return weightGrams_;
}

int Letter::pricePerGram() const {
    return pricePerGram_;
}

int Letter::calculatePrice() const {
    return weightGrams_ * pricePerGram_;
}

std::unique_ptr<Shipment> Letter::clone() const {
    return std::make_unique<Letter>(*this);
}
