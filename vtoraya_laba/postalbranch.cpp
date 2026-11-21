#include "postalbranch.h"
#include <stdexcept>

PostalBranch::PostalBranch(int id, const PostalAddress &address)
    : id_(id), address_(address) {
    if(id_ <= 0){
        throw std::invalid_argument("Branch ID must be positive");
    }
}

int PostalBranch::id() const {
    return id_;
}

const PostalAddress &PostalBranch::address() const {
    return address_;
}

std::size_t PostalBranch::shipmentCount() const {
    return shipments_.size();
}

void PostalBranch::acceptShipment(const Shipment &shipment) {
    if(shipment.status() != ShipmentStatus::Accepted){
        throw std::logic_error("Only a newly accepted shipment can be registered");
    }
    for(const auto &stored : shipments_){
        if(stored->trackingNumber() == shipment.trackingNumber()){
            throw std::invalid_argument("Tracking number is already registered at this branch");
        }
    }
    shipments_.push_back(shipment.clone());
}

const Shipment &PostalBranch::findShipment(const std::string &trackingNumber) const {
    for(const auto &shipment : shipments_){
        if(shipment->trackingNumber() == trackingNumber){
            return *shipment;
        }
    }
    throw std::out_of_range("Shipment was not found at this branch");
}
