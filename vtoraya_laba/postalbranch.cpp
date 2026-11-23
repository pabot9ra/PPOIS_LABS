#include "postalbranch.h"
#include "shipmentnotfoundexception.h"
#include "registeredletter.h"
#include "insuredparcel.h"
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

bool PostalBranch::containsShipment(const std::string &trackingNumber) const {
    for(const auto &shipment : shipments_){
        if(shipment->trackingNumber() == trackingNumber){
            return true;
        }
    }
    return false;
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
    throw ShipmentNotFoundException(trackingNumber);
}

Shipment &PostalBranch::findMutableShipment(const std::string &trackingNumber) {
    for(const auto &shipment : shipments_){
        if(shipment->trackingNumber() == trackingNumber){
            return *shipment;
        }
    }
    throw ShipmentNotFoundException(trackingNumber);
}

void PostalBranch::dispatchShipment(const std::string &trackingNumber) {
    Shipment &shipment = findMutableShipment(trackingNumber);
    shipment.dispatch();
}

void PostalBranch::deliverShipment(const std::string &trackingNumber) {
    Shipment &shipment = findMutableShipment(trackingNumber);
    shipment.deliver();
}

void PostalBranch::confirmReceipt(const std::string &trackingNumber, const std::string &signer) {
    Shipment &shipment = findMutableShipment(trackingNumber);
    auto *letter = dynamic_cast<RegisteredLetter *>(&shipment);
    if(!letter){
        throw std::invalid_argument("Receipt confirmation requires a registered letter");
    }
    letter->confirmReceipt(signer);
}

void PostalBranch::recordCompensation(const std::string &trackingNumber, int amount) {
    Shipment &shipment = findMutableShipment(trackingNumber);
    auto *parcel = dynamic_cast<InsuredParcel *>(&shipment);
    if(!parcel){
        throw std::invalid_argument("Compensation requires an insured parcel");
    }
    parcel->recordCompensation(amount);
}
