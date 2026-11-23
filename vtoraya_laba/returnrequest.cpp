#include "returnrequest.h"
#include <stdexcept>

ReturnRequest::ReturnRequest(int id, const Customer &applicant, const Shipment &shipment,
                             int openedMinute, const std::string &reason, int returnFee)
    : CustomerRequest(id, applicant, shipment, openedMinute, reason),
      returnAddress_(applicant.address()), returnFee_(returnFee) {
    if(returnFee_ < 0){
        throw std::invalid_argument("Return fee must not be negative");
    }
}

const PostalAddress &ReturnRequest::returnAddress() const {
    return returnAddress_;
}

int ReturnRequest::returnFee() const {
    return returnFee_;
}

void ReturnRequest::validateEligibility(const Shipment &shipment) const {
    if(applicant().id() != shipment.sender().id()){
        throw std::invalid_argument("Only the sender can request a recall");
    }
    if(shipment.status() == ShipmentStatus::Delivered){
        throw std::logic_error("Delivered shipment cannot be recalled");
    }
}

std::string ReturnRequest::issueLabel(const Shipment &shipment) {
    ensureApproved();
    ensureMatches(shipment);
    validateEligibility(shipment);
    std::string label = "RETURN " + trackingNumber() + " TO " + returnAddress_.formatLabel() +
                        "; fee " + std::to_string(returnFee_) + " kopecks";
    markCompleted();
    return label;
}

std::unique_ptr<CustomerRequest> ReturnRequest::clone() const {
    return std::make_unique<ReturnRequest>(*this);
}
