#include "storageinventory.h"
#include <stdexcept>

StorageInventory::StorageInventory(const PostalBranch &branch, int maximumWeight)
    : branchId_(branch.id()), maximumWeight_(maximumWeight) {
    if(maximumWeight_ <= 0){
        throw std::invalid_argument("Storage capacity must be positive");
    }
}

int StorageInventory::branchId() const {
    return branchId_;
}

std::size_t StorageInventory::zoneCount() const {
    return zones_.size();
}

std::size_t StorageInventory::shipmentCount() const {
    std::size_t count = 0;
    for(const StorageZone &zone : zones_){
        count += zone.occupiedCount();
    }
    return count;
}

long long StorageInventory::weightGrams() const {
    long long weight = 0;
    for(const StorageZone &zone : zones_){
        weight += zone.weightGrams();
    }
    return weight;
}

long long StorageInventory::remainingCapacity() const {
    return maximumWeight_ - weightGrams();
}

void StorageInventory::addZone(const StorageZone &zone) {
    if(zone.branchId() != branchId_){
        throw std::invalid_argument("Zone belongs to another branch");
    }
    if(zone.occupiedCount() != 0){
        throw std::logic_error("New storage zone must be empty");
    }
    for(const StorageZone &stored : zones_){
        if(stored.id() == zone.id()){
            throw std::invalid_argument("Zone ID is already registered");
        }
    }
    zones_.push_back(zone);
}

bool StorageInventory::contains(const std::string &trackingNumber) const {
    for(const StorageZone &zone : zones_){
        if(zone.contains(trackingNumber)){
            return true;
        }
    }
    return false;
}

void StorageInventory::ensureCanStore(const Shipment &shipment) const {
    if(shipment.status() == ShipmentStatus::Delivered){
        throw std::logic_error("Delivered shipment cannot be stored");
    }
    if(contains(shipment.trackingNumber()) || shipment.weightGrams() > remainingCapacity()){
        throw std::invalid_argument("Shipment is already stored or exceeds storage capacity");
    }
}

StorageLocation StorageInventory::storeShipment(const Shipment &shipment, const Dimensions &contents) {
    ensureCanStore(shipment);
    for(StorageZone &zone : zones_){
        if(zone.canStore(shipment, contents)){
            const StorageCell &cell = zone.findAvailableCell(shipment, contents);
            StorageLocation location(branchId_, zone.id(), cell.id());
            zone.storeShipment(shipment, contents);
            return location;
        }
    }
    throw std::out_of_range("No suitable storage cell is available");
}

StorageLocation StorageInventory::locateShipment(const std::string &trackingNumber) const {
    for(const StorageZone &zone : zones_){
        if(zone.contains(trackingNumber)){
            return StorageLocation(branchId_, zone.id(), zone.findShipmentCell(trackingNumber).id());
        }
    }
    throw std::out_of_range("Shipment is not in storage");
}

int StorageInventory::storedWeightGrams(const std::string &trackingNumber) const {
    for(const StorageZone &zone : zones_){
        if(zone.contains(trackingNumber)){
            return zone.findShipmentCell(trackingNumber).weightGrams();
        }
    }
    throw std::out_of_range("Shipment is not in storage");
}

void StorageInventory::releaseShipment(const std::string &trackingNumber) {
    for(StorageZone &zone : zones_){
        if(zone.contains(trackingNumber)){
            zone.releaseShipment(trackingNumber);
            return;
        }
    }
    throw std::out_of_range("Shipment is not in storage");
}
