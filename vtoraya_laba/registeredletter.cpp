#include "registeredletter.h"
#include <cctype>
#include <limits>
#include <stdexcept>

RegisteredLetter::RegisteredLetter(const std::string &trackingNumber, const Customer &sender,
                                   const Customer &recipient, int weightGrams, int pricePerGram,
                                   int registrationFee)
    : Letter(trackingNumber, sender, recipient, weightGrams, pricePerGram),
      registrationFee_(registrationFee) {
    if(registrationFee_ < 0){
        throw std::invalid_argument("Registration fee must not be negative");
    }
    if(registrationFee_ > std::numeric_limits<int>::max() - Letter::calculatePrice()){
        throw std::overflow_error("Registered letter price is too large");
    }
}

int RegisteredLetter::registrationFee() const {
    return registrationFee_;
}

const std::string &RegisteredLetter::signedBy() const {
    return signedBy_;
}

void RegisteredLetter::confirmReceipt(const std::string &signer) {
    if(status() != ShipmentStatus::Delivered || !signedBy_.empty()){
        throw std::logic_error("Receipt can be signed once, after delivery");
    }
    bool hasName = false;
    for(char character : signer){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasName = true;
            break;
        }
    }
    if(!hasName){
        throw std::invalid_argument("Signer name must not be blank");
    }
    signedBy_ = signer;
}

int RegisteredLetter::calculatePrice() const {
    return Letter::calculatePrice() + registrationFee_;
}

std::unique_ptr<Shipment> RegisteredLetter::clone() const {
    return std::make_unique<RegisteredLetter>(*this);
}
