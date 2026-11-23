#include "damageclaim.h"
#include "insuredparcel.h"
#include <stdexcept>

DamageClaim::DamageClaim(int id, const Customer &applicant, const Shipment &shipment,
                         int openedMinute, const std::string &reason, int requestedAmount)
    : CustomerRequest(id, applicant, shipment, openedMinute, reason),
      requestedAmount_(requestedAmount), settledAmount_(0) {
    if(requestedAmount_ <= 0){
        throw std::invalid_argument("Requested compensation must be positive");
    }
}

int DamageClaim::requestedAmount() const {
    return requestedAmount_;
}

int DamageClaim::settledAmount() const {
    return settledAmount_;
}

void DamageClaim::validateEligibility(const Shipment &shipment) const {
    const auto *parcel = dynamic_cast<const InsuredParcel *>(&shipment);
    if(!parcel){
        throw std::invalid_argument("Damage claim requires an insured parcel");
    }
    if(parcel->status() != ShipmentStatus::Delivered){
        throw std::logic_error("Damage claim can be approved after delivery");
    }
    if(requestedAmount_ > parcel->remainingCoverage()){
        throw std::invalid_argument("Claim exceeds remaining insurance coverage");
    }
}

void DamageClaim::settle(PostalBranch &branch) {
    ensureApproved();
    const Shipment &shipment = branch.findShipment(trackingNumber());
    ensureMatches(shipment);
    validateEligibility(shipment);
    branch.recordCompensation(trackingNumber(), requestedAmount_);
    settledAmount_ = requestedAmount_;
    markCompleted();
}

std::unique_ptr<CustomerRequest> DamageClaim::clone() const {
    return std::make_unique<DamageClaim>(*this);
}
