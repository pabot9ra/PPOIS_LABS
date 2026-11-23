#include "refund.h"
#include <cctype>
#include <stdexcept>

Refund::Refund(int id, int paymentId, int amount, const std::string &reason)
    : id_(id), paymentId_(paymentId), amount_(amount), reason_(reason) {
    if(id_ <= 0 || paymentId_ <= 0 || amount_ <= 0){
        throw std::invalid_argument("Refund and payment IDs and refund amount must be positive");
    }
    bool hasReason = false;
    for(char character : reason_){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasReason = true;
            break;
        }
    }
    if(!hasReason){
        throw std::invalid_argument("Refund reason must not be blank");
    }
}

int Refund::id() const {
    return id_;
}

int Refund::paymentId() const {
    return paymentId_;
}

int Refund::amount() const {
    return amount_;
}

const std::string &Refund::reason() const {
    return reason_;
}
