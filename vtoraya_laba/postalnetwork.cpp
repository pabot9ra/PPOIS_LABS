#include "postalnetwork.h"
#include <cctype>
#include <stdexcept>

PostalNetwork::PostalNetwork(const std::string &name) : name_(name) {
    bool hasName = false;
    for(char character : name_){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasName = true;
            break;
        }
    }
    if(!hasName){
        throw std::invalid_argument("Postal network name must not be blank");
    }
}

const std::string &PostalNetwork::name() const {
    return name_;
}

std::size_t PostalNetwork::customerCount() const {
    return customers_.size();
}

std::size_t PostalNetwork::branchCount() const {
    return branches_.size();
}

std::size_t PostalNetwork::shipmentCount() const {
    return branches_.shipmentCount();
}

void PostalNetwork::registerCustomer(const Customer &customer) {
    customers_.add(customer);
}

void PostalNetwork::openBranch(int id, const PostalAddress &address) {
    branches_.add(id, address);
}

const Customer &PostalNetwork::customer(int id) const {
    return customers_.find(id);
}

const PostalBranch &PostalNetwork::branch(int id) const {
    return branches_.find(id);
}

void PostalNetwork::changeCustomerAddress(int id, const PostalAddress &address) {
    customers_.changeAddress(id, address);
}

void PostalNetwork::acceptShipment(int branchId, const Shipment &shipment) {
    if(!customers_.contains(shipment.sender().id()) || !customers_.contains(shipment.recipient().id())){
        throw std::invalid_argument("Both shipment participants must be registered in the network");
    }
    branches_.acceptShipment(branchId, shipment);
}

int PostalNetwork::shipmentBranchId(const std::string &trackingNumber) const {
    return branches_.locateShipment(trackingNumber);
}

const Shipment &PostalNetwork::findShipment(const std::string &trackingNumber) const {
    int branchId = branches_.locateShipment(trackingNumber);
    const PostalBranch &storedBranch = branches_.find(branchId);
    return storedBranch.findShipment(trackingNumber);
}

void PostalNetwork::dispatchShipment(const std::string &trackingNumber) {
    int branchId = branches_.locateShipment(trackingNumber);
    branches_.dispatchShipment(branchId, trackingNumber);
}

void PostalNetwork::deliverShipment(const std::string &trackingNumber) {
    int branchId = branches_.locateShipment(trackingNumber);
    branches_.deliverShipment(branchId, trackingNumber);
}
