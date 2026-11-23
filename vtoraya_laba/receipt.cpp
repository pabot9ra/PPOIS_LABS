#include "receipt.h"
#include <stdexcept>

Receipt::Receipt(int registerId, const Payment &payment)
    : registerId_(registerId), paymentId_(payment.id()),
      trackingNumber_(payment.trackingNumber()), amount_(payment.amount()) {
    if(registerId_ <= 0){
        throw std::invalid_argument("Register ID must be positive");
    }
    if(!payment.captured()){
        throw std::logic_error("Receipt requires a captured payment");
    }
}

int Receipt::registerId() const {
    return registerId_;
}

int Receipt::paymentId() const {
    return paymentId_;
}

const std::string &Receipt::trackingNumber() const {
    return trackingNumber_;
}

int Receipt::amount() const {
    return amount_;
}

std::string Receipt::format() const {
    return "Register " + std::to_string(registerId_) + ", payment " + std::to_string(paymentId_) +
           ", shipment " + trackingNumber_ + ", amount " + std::to_string(amount_) + " kopecks";
}
