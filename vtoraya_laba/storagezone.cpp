#include "storagezone.h"
#include <cctype>
#include <stdexcept>

StorageZone::StorageZone(const std::string &id, int branchId)
    : id_(id), branchId_(branchId), locked_(false) {
    if(id_.empty() || branchId_ <= 0){
        throw std::invalid_argument("Zone requires ID and positive branch ID");
    }
    for(char character : id_){
        if(std::isspace(static_cast<unsigned char>(character))){
            throw std::invalid_argument("Zone ID must not contain whitespace");
        }
    }
}

const std::string &StorageZone::id() const {
    return id_;
}

int StorageZone::branchId() const {
    return branchId_;
}

bool StorageZone::locked() const {
    return locked_;
}

std::size_t StorageZone::cellCount() const {
    return cells_.size();
}

std::size_t StorageZone::occupiedCount() const {
    std::size_t count = 0;
    for(const StorageCell &cell : cells_){
        if(cell.occupied()){
            count++;
        }
    }
    return count;
}

long long StorageZone::weightGrams() const {
    long long weight = 0;
    for(const StorageCell &cell : cells_){
        weight += cell.weightGrams();
    }
    return weight;
}

void StorageZone::addCell(const StorageCell &cell) {
    if(locked_ || cell.occupied()){
        throw std::logic_error("Zone must be unlocked and new cell must be empty");
    }
    if(cell.branchId() != branchId_){
        throw std::invalid_argument("Cell belongs to another branch");
    }
    for(const StorageCell &stored : cells_){
        if(stored.id() == cell.id()){
            throw std::invalid_argument("Cell ID is already used in this zone");
        }
    }
    cells_.push_back(cell);
}

const StorageCell &StorageZone::findCell(const std::string &id) const {
    for(const StorageCell &cell : cells_){
        if(cell.id() == id){
            return cell;
        }
    }
    throw std::out_of_range("Storage cell was not found");
}

bool StorageZone::contains(const std::string &trackingNumber) const {
    for(const StorageCell &cell : cells_){
        if(cell.occupied() && cell.trackingNumber() == trackingNumber){
            return true;
        }
    }
    return false;
}

const StorageCell &StorageZone::findShipmentCell(const std::string &trackingNumber) const {
    for(const StorageCell &cell : cells_){
        if(cell.occupied() && cell.trackingNumber() == trackingNumber){
            return cell;
        }
    }
    throw std::out_of_range("Shipment was not found in this storage zone");
}

const StorageCell &StorageZone::findAvailableCell(const Shipment &shipment, const Dimensions &contents) const {
    if(locked_){
        throw std::logic_error("Storage zone is locked");
    }
    for(const StorageCell &cell : cells_){
        if(cell.canFit(shipment, contents)){
            return cell;
        }
    }
    throw std::out_of_range("No suitable storage cell is available");
}

bool StorageZone::canStore(const Shipment &shipment, const Dimensions &contents) const {
    if(locked_ || contains(shipment.trackingNumber())){
        return false;
    }
    for(const StorageCell &cell : cells_){
        if(cell.canFit(shipment, contents)){
            return true;
        }
    }
    return false;
}

void StorageZone::storeShipment(const Shipment &shipment, const Dimensions &contents) {
    if(locked_ || shipment.status() == ShipmentStatus::Delivered){
        throw std::logic_error("Zone must be unlocked and shipment must not be delivered");
    }
    if(contains(shipment.trackingNumber())){
        throw std::invalid_argument("Shipment is already stored in this zone");
    }
    for(StorageCell &cell : cells_){
        if(cell.canFit(shipment, contents)){
            cell.store(shipment, contents);
            return;
        }
    }
    throw std::out_of_range("No suitable storage cell is available");
}

void StorageZone::releaseShipment(const std::string &trackingNumber) {
    if(locked_){
        throw std::logic_error("Storage zone is locked");
    }
    for(StorageCell &cell : cells_){
        if(cell.occupied() && cell.trackingNumber() == trackingNumber){
            cell.release();
            return;
        }
    }
    throw std::out_of_range("Shipment was not found in this storage zone");
}

void StorageZone::lock() {
    if(locked_){
        throw std::logic_error("Zone is already locked");
    }
    locked_ = true;
}

void StorageZone::unlock() {
    if(!locked_){
        throw std::logic_error("Zone is already unlocked");
    }
    locked_ = false;
}
