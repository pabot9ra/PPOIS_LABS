#include "branchdirectory.h"
#include <stdexcept>
#include <utility>

std::size_t BranchDirectory::size() const {
    return branches_.size();
}

bool BranchDirectory::contains(int id) const {
    return branches_.count(id) > 0;
}

void BranchDirectory::add(int id, const PostalAddress &address) {
    if(contains(id)){
        throw std::invalid_argument("Branch ID is already registered");
    }
    auto branch = std::make_unique<PostalBranch>(id, address);
    branches_.emplace(id, std::move(branch));
}

const PostalBranch &BranchDirectory::find(int id) const {
    auto it = branches_.find(id);
    if(it == branches_.end()){
        throw std::out_of_range("Postal branch was not found");
    }
    return *it->second;
}

PostalBranch &BranchDirectory::findMutable(int id) {
    auto it = branches_.find(id);
    if(it == branches_.end()){
        throw std::out_of_range("Postal branch was not found");
    }
    return *it->second;
}

void BranchDirectory::remove(int id) {
    const PostalBranch &branch = find(id);
    if(branch.shipmentCount() != 0){
        throw std::logic_error("Branch with shipment records cannot be removed");
    }
    branches_.erase(id);
}

std::size_t BranchDirectory::shipmentCount() const {
    std::size_t count = 0;
    for(const auto &entry : branches_){
        count += entry.second->shipmentCount();
    }
    return count;
}

bool BranchDirectory::containsShipment(const std::string &trackingNumber) const {
    for(const auto &entry : branches_){
        if(entry.second->containsShipment(trackingNumber)){
            return true;
        }
    }
    return false;
}

int BranchDirectory::locateShipment(const std::string &trackingNumber) const {
    for(const auto &entry : branches_){
        if(entry.second->containsShipment(trackingNumber)){
            return entry.first;
        }
    }
    throw std::out_of_range("Shipment was not found in the branch directory");
}

void BranchDirectory::acceptShipment(int branchId, const Shipment &shipment) {
    PostalBranch &branch = findMutable(branchId);
    if(containsShipment(shipment.trackingNumber())){
        throw std::invalid_argument("Tracking number is already registered in another branch or this branch");
    }
    branch.acceptShipment(shipment);
}

void BranchDirectory::dispatchShipment(int branchId, const std::string &trackingNumber) {
    PostalBranch &branch = findMutable(branchId);
    branch.dispatchShipment(trackingNumber);
}

void BranchDirectory::deliverShipment(int branchId, const std::string &trackingNumber) {
    PostalBranch &branch = findMutable(branchId);
    branch.deliverShipment(trackingNumber);
}
