#include "mailbag.h"
#include <cctype>
#include <stdexcept>

MailBag::MailBag(const std::string &id, int sourceBranchId, int destinationBranchId, int maximumWeight)
    : id_(id), sourceBranchId_(sourceBranchId), destinationBranchId_(destinationBranchId),
      maximumWeight_(maximumWeight), weightGrams_(0), sealed_(false) {
    if(id_.empty() || sourceBranchId_ <= 0 || destinationBranchId_ <= 0 ||
       sourceBranchId_ == destinationBranchId_ || maximumWeight_ <= 0){
        throw std::invalid_argument("Bag requires ID, different positive branch IDs and positive capacity");
    }
    for(char character : id_){
        if(std::isspace(static_cast<unsigned char>(character))){
            throw std::invalid_argument("Bag ID must not contain whitespace");
        }
    }
}

const std::string &MailBag::id() const {
    return id_;
}

int MailBag::sourceBranchId() const {
    return sourceBranchId_;
}

int MailBag::destinationBranchId() const {
    return destinationBranchId_;
}

int MailBag::maximumWeight() const {
    return maximumWeight_;
}

int MailBag::weightGrams() const {
    return weightGrams_;
}

std::size_t MailBag::shipmentCount() const {
    return trackingNumbers_.size();
}

const std::string &MailBag::trackingNumberAt(std::size_t index) const {
    return trackingNumbers_.at(index);
}

bool MailBag::sealed() const {
    return sealed_;
}

int MailBag::remainingCapacity() const {
    return maximumWeight_ - weightGrams_;
}

bool MailBag::contains(const std::string &trackingNumber) const {
    for(const std::string &number : trackingNumbers_){
        if(number == trackingNumber){
            return true;
        }
    }
    return false;
}

void MailBag::addShipment(const Shipment &shipment) {
    if(sealed_ || shipment.status() == ShipmentStatus::Delivered){
        throw std::logic_error("Bag must be open and shipment must not be delivered");
    }
    if(contains(shipment.trackingNumber())){
        throw std::invalid_argument("Shipment is already in this bag");
    }
    if(shipment.weightGrams() > remainingCapacity()){
        throw std::invalid_argument("Shipment exceeds remaining bag capacity");
    }
    trackingNumbers_.push_back(shipment.trackingNumber());
    weightGrams_ += shipment.weightGrams();
}

void MailBag::seal() {
    if(sealed_ || trackingNumbers_.empty()){
        throw std::logic_error("Only a nonempty open bag can be sealed");
    }
    sealed_ = true;
}

void MailBag::open() {
    if(!sealed_){
        throw std::logic_error("Bag is not sealed");
    }
    sealed_ = false;
}

void MailBag::clear() {
    if(sealed_){
        throw std::logic_error("Sealed bag cannot be cleared");
    }
    trackingNumbers_.clear();
    weightGrams_ = 0;
}
