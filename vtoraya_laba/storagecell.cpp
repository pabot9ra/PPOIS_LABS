#include "storagecell.h"
#include <cctype>
#include <stdexcept>

StorageCell::StorageCell(const std::string &id, int branchId, const Dimensions &dimensions, int maximumWeight)
    : id_(id), branchId_(branchId), dimensions_(dimensions), maximumWeight_(maximumWeight), weightGrams_(0) {
    if(id_.empty() || branchId_ <= 0 || maximumWeight_ <= 0){
        throw std::invalid_argument("Cell requires ID, positive branch ID and positive capacity");
    }
    for(char character : id_){
        if(std::isspace(static_cast<unsigned char>(character))){
            throw std::invalid_argument("Cell ID must not contain whitespace");
        }
    }
}

const std::string &StorageCell::id() const {
    return id_;
}

int StorageCell::branchId() const {
    return branchId_;
}

const Dimensions &StorageCell::dimensions() const {
    return dimensions_;
}

int StorageCell::maximumWeight() const {
    return maximumWeight_;
}

const std::string &StorageCell::trackingNumber() const {
    return trackingNumber_;
}

int StorageCell::weightGrams() const {
    return weightGrams_;
}

bool StorageCell::occupied() const {
    return !trackingNumber_.empty();
}

bool StorageCell::canFit(const Shipment &shipment, const Dimensions &contents) const {
    return !occupied() && shipment.status() != ShipmentStatus::Delivered &&
           shipment.weightGrams() <= maximumWeight_ && contents.fitsInside(dimensions_);
}

void StorageCell::store(const Shipment &shipment, const Dimensions &contents) {
    if(occupied() || shipment.status() == ShipmentStatus::Delivered){
        throw std::logic_error("Cell must be empty and shipment must not be delivered");
    }
    if(!canFit(shipment, contents)){
        throw std::invalid_argument("Shipment exceeds cell weight or size limit");
    }
    trackingNumber_ = shipment.trackingNumber();
    weightGrams_ = shipment.weightGrams();
}

void StorageCell::release() {
    if(!occupied()){
        throw std::logic_error("Storage cell is empty");
    }
    trackingNumber_.clear();
    weightGrams_ = 0;
}
