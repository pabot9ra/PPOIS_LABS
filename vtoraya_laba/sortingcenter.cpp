#include "sortingcenter.h"
#include <stdexcept>

SortingCenter::SortingCenter(int id, const PostalBranch &branch, int maximumWeight)
    : id_(id), branchId_(branch.id()), inventory_(branch, maximumWeight), open_(false) {
    if(id_ <= 0){
        throw std::invalid_argument("Sorting center ID must be positive");
    }
}

int SortingCenter::id() const {
    return id_;
}

int SortingCenter::branchId() const {
    return branchId_;
}

bool SortingCenter::isOpen() const {
    return open_;
}

std::size_t SortingCenter::storedCount() const {
    return inventory_.shipmentCount();
}

std::size_t SortingCenter::bagCount() const {
    return bags_.size();
}

long long SortingCenter::storedWeightGrams() const {
    return inventory_.weightGrams();
}

void SortingCenter::ensureOpen() const {
    if(!open_){
        throw std::logic_error("Sorting center shift must be open");
    }
}

void SortingCenter::addZone(const StorageZone &zone) {
    if(open_){
        throw std::logic_error("Storage must be configured between shifts");
    }
    inventory_.addZone(zone);
}

void SortingCenter::openShift() {
    if(open_){
        throw std::logic_error("Sorting center shift is already open");
    }
    open_ = true;
}

void SortingCenter::closeShift() {
    ensureOpen();
    open_ = false;
}

StorageLocation SortingCenter::receiveShipment(const Shipment &shipment, const Dimensions &contents) {
    ensureOpen();
    for(const MailBag &bag : bags_){
        if(bag.contains(shipment.trackingNumber())){
            throw std::invalid_argument("Shipment is already sorted into an outbound bag");
        }
    }
    return inventory_.storeShipment(shipment, contents);
}

StorageLocation SortingCenter::locateShipment(const std::string &trackingNumber) const {
    return inventory_.locateShipment(trackingNumber);
}

void SortingCenter::createBag(const std::string &bagId, int destinationBranchId, int maximumWeight) {
    ensureOpen();
    for(const MailBag &bag : bags_){
        if(bag.id() == bagId){
            throw std::invalid_argument("Bag ID is already used in this center");
        }
    }
    MailBag bag(bagId, branchId_, destinationBranchId, maximumWeight);
    bags_.push_back(bag);
}

const MailBag &SortingCenter::findBag(const std::string &bagId) const {
    for(const MailBag &bag : bags_){
        if(bag.id() == bagId){
            return bag;
        }
    }
    throw std::out_of_range("Bag was not found in this sorting center");
}

MailBag &SortingCenter::findMutableBag(const std::string &bagId) {
    for(MailBag &bag : bags_){
        if(bag.id() == bagId){
            return bag;
        }
    }
    throw std::out_of_range("Bag was not found in this sorting center");
}

void SortingCenter::sortIntoBag(const Shipment &shipment, const std::string &bagId) {
    ensureOpen();
    MailBag &bag = findMutableBag(bagId);
    if(inventory_.storedWeightGrams(shipment.trackingNumber()) != shipment.weightGrams()){
        throw std::invalid_argument("Shipment weight differs from its storage record");
    }
    bag.addShipment(shipment);
    inventory_.releaseShipment(shipment.trackingNumber());
}

void SortingCenter::sealBag(const std::string &bagId) {
    ensureOpen();
    MailBag &bag = findMutableBag(bagId);
    bag.seal();
}

MailBag SortingCenter::takeBag(const std::string &bagId) {
    ensureOpen();
    for(auto it = bags_.begin(); it != bags_.end(); ++it){
        if(it->id() == bagId){
            if(!it->sealed()){
                throw std::logic_error("Only sealed bags can leave the sorting center");
            }
            MailBag result = *it;
            bags_.erase(it);
            return result;
        }
    }
    throw std::out_of_range("Bag was not found in this sorting center");
}
