#include "payment.h"
#include <cctype>
#include <stdexcept>

Payment::Payment(int id, const std::string &trackingNumber, int amount)
    : id_(id), trackingNumber_(trackingNumber), amount_(amount),
      captured_(false), refundedAmount_(0) {
    if(id_ <= 0 || amount_ <= 0 || trackingNumber_.empty()){
        throw std::invalid_argument("Payment requires positive ID, positive amount and tracking number");
    }
    for(char character : trackingNumber_){
        if(std::isspace(static_cast<unsigned char>(character))){
            throw std::invalid_argument("Tracking number must not contain whitespace");
        }
    }
}

int Payment::id() const {
    return id_;
}

const std::string &Payment::trackingNumber() const {
    return trackingNumber_;
}

int Payment::amount() const {
    return amount_;
}

bool Payment::captured() const {
    return captured_;
}

int Payment::refundableAmount() const {
    if(!captured_){
        return 0;
    }
    return amount_ - refundedAmount_;
}

void Payment::capture() {
    if(captured_){
        throw std::logic_error("Payment is already captured");
    }
    captured_ = true;
}

void Payment::refund(int amount) {
    if(!captured_){
        throw std::logic_error("Only a captured payment can be refunded");
    }
    if(amount <= 0 || amount > refundableAmount()){
        throw std::invalid_argument("Refund must be positive and within the remaining payment amount");
    }
    refundedAmount_ += amount;
}
