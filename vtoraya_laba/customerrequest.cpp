#include "customerrequest.h"
#include <cctype>
#include <stdexcept>

CustomerRequest::CustomerRequest(int id, const Customer &applicant, const Shipment &shipment,
                                 int openedMinute, const std::string &reason)
    : id_(id), applicant_(applicant), trackingNumber_(shipment.trackingNumber()),
      openedMinute_(openedMinute), reason_(reason), status_(RequestStatus::Open) {
    if(id_ <= 0 || openedMinute_ < 0 || !hasText(reason_)){
        throw std::invalid_argument("Request requires positive ID, nonnegative minute and a reason");
    }
    ensureMatches(shipment);
}

bool CustomerRequest::hasText(const std::string &value) {
    for(char character : value){
        if(!std::isspace(static_cast<unsigned char>(character))){
            return true;
        }
    }
    return false;
}

int CustomerRequest::id() const {
    return id_;
}

const Customer &CustomerRequest::applicant() const {
    return applicant_;
}

const std::string &CustomerRequest::trackingNumber() const {
    return trackingNumber_;
}

int CustomerRequest::openedMinute() const {
    return openedMinute_;
}

const std::string &CustomerRequest::reason() const {
    return reason_;
}

RequestStatus CustomerRequest::status() const {
    return status_;
}

const std::string &CustomerRequest::decisionReason() const {
    return decisionReason_;
}

void CustomerRequest::ensureMatches(const Shipment &shipment) const {
    if(shipment.trackingNumber() != trackingNumber_ ||
       (applicant_.id() != shipment.sender().id() && applicant_.id() != shipment.recipient().id())){
        throw std::invalid_argument("Request must belong to this shipment and one of its participants");
    }
}

void CustomerRequest::approve(const Shipment &shipment) {
    if(status_ != RequestStatus::Open){
        throw std::logic_error("Only an open request can be approved");
    }
    ensureMatches(shipment);
    validateEligibility(shipment);
    status_ = RequestStatus::Approved;
}

void CustomerRequest::reject(const std::string &reason) {
    if(status_ != RequestStatus::Open){
        throw std::logic_error("Only an open request can be rejected");
    }
    if(!hasText(reason)){
        throw std::invalid_argument("Rejection reason must not be blank");
    }
    decisionReason_ = reason;
    status_ = RequestStatus::Rejected;
}

void CustomerRequest::ensureApproved() const {
    if(status_ != RequestStatus::Approved){
        throw std::logic_error("Request must be approved before completion");
    }
}

void CustomerRequest::markCompleted() {
    ensureApproved();
    status_ = RequestStatus::Completed;
}
