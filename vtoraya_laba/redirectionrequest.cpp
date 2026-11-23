#include "redirectionrequest.h"
#include <stdexcept>

RedirectionRequest::RedirectionRequest(int id, const Customer &applicant, const Shipment &shipment,
                                       int openedMinute, const std::string &reason,
                                       const PostalAddress &destination, int redirectionFee)
    : CustomerRequest(id, applicant, shipment, openedMinute, reason),
      destination_(destination), redirectionFee_(redirectionFee) {
    if(redirectionFee_ < 0){
        throw std::invalid_argument("Redirection fee must not be negative");
    }
}

const PostalAddress &RedirectionRequest::destination() const {
    return destination_;
}

int RedirectionRequest::redirectionFee() const {
    return redirectionFee_;
}

void RedirectionRequest::validateEligibility(const Shipment &shipment) const {
    if(shipment.status() == ShipmentStatus::Delivered){
        throw std::logic_error("Delivered shipment cannot be redirected");
    }
    if(destination_.formatLabel() == shipment.recipient().address().formatLabel()){
        throw std::invalid_argument("New destination must differ from the original address");
    }
}

std::string RedirectionRequest::issueLabel(const Shipment &shipment) {
    ensureApproved();
    ensureMatches(shipment);
    validateEligibility(shipment);
    std::string label = "REDIRECT " + trackingNumber() + " TO " + destination_.formatLabel() +
                        "; fee " + std::to_string(redirectionFee_) + " kopecks";
    markCompleted();
    return label;
}

std::unique_ptr<CustomerRequest> RedirectionRequest::clone() const {
    return std::make_unique<RedirectionRequest>(*this);
}
