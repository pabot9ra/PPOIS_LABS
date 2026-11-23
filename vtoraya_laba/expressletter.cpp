#include "expressletter.h"
#include <limits>
#include <stdexcept>

ExpressLetter::ExpressLetter(const std::string &trackingNumber, const Customer &sender,
                             const Customer &recipient, int weightGrams, int pricePerGram,
                             int deliveryHours, int expressFee)
    : Letter(trackingNumber, sender, recipient, weightGrams, pricePerGram),
      deliveryHours_(deliveryHours), expressFee_(expressFee) {
    if(deliveryHours_ <= 0 || expressFee_ < 0){
        throw std::invalid_argument("Delivery hours must be positive; express fee must not be negative");
    }
    if(expressFee_ > std::numeric_limits<int>::max() - Letter::calculatePrice()){
        throw std::overflow_error("Express letter price is too large");
    }
}

int ExpressLetter::deliveryHours() const {
    return deliveryHours_;
}

int ExpressLetter::expressFee() const {
    return expressFee_;
}

bool ExpressLetter::isOverdue(int hoursInTransit) const {
    if(hoursInTransit < 0){
        throw std::invalid_argument("Hours in transit must not be negative");
    }
    return status() == ShipmentStatus::InTransit && hoursInTransit > deliveryHours_;
}

int ExpressLetter::calculatePrice() const {
    return Letter::calculatePrice() + expressFee_;
}

std::unique_ptr<Shipment> ExpressLetter::clone() const {
    return std::make_unique<ExpressLetter>(*this);
}
