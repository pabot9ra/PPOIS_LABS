#include "requestregistry.h"
#include "returnrequest.h"
#include "redirectionrequest.h"
#include "damageclaim.h"
#include <stdexcept>

RequestRegistry::RequestRegistry(const PostalBranch &branch) : branchId_(branch.id()) {
}

int RequestRegistry::branchId() const {
    return branchId_;
}

std::size_t RequestRegistry::requestCount() const {
    return requests_.size();
}

void RequestRegistry::ensureBranch(const PostalBranch &branch) const {
    if(branch.id() != branchId_){
        throw std::invalid_argument("Request registry belongs to another branch");
    }
}

void RequestRegistry::submit(const PostalBranch &branch, const CustomerRequest &request) {
    ensureBranch(branch);
    if(request.status() != RequestStatus::Open){
        throw std::logic_error("Only open requests can be submitted");
    }
    const Shipment &shipment = branch.findShipment(request.trackingNumber());
    if(request.applicant().id() != shipment.sender().id() && request.applicant().id() != shipment.recipient().id()){
        throw std::invalid_argument("Applicant is not a participant of the registered shipment");
    }
    for(const auto &stored : requests_){
        if(stored->id() == request.id()){
            throw std::invalid_argument("Request ID is already registered");
        }
    }
    requests_.push_back(request.clone());
}

const CustomerRequest &RequestRegistry::findRequest(int id) const {
    for(const auto &request : requests_){
        if(request->id() == id){
            return *request;
        }
    }
    throw std::out_of_range("Customer request was not found");
}

CustomerRequest &RequestRegistry::findMutableRequest(int id) {
    for(const auto &request : requests_){
        if(request->id() == id){
            return *request;
        }
    }
    throw std::out_of_range("Customer request was not found");
}

void RequestRegistry::approve(int id, const PostalBranch &branch) {
    ensureBranch(branch);
    CustomerRequest &request = findMutableRequest(id);
    request.approve(branch.findShipment(request.trackingNumber()));
}

void RequestRegistry::reject(int id, const std::string &reason) {
    CustomerRequest &request = findMutableRequest(id);
    request.reject(reason);
}

std::string RequestRegistry::issueReturnLabel(int id, const PostalBranch &branch) {
    ensureBranch(branch);
    CustomerRequest &request = findMutableRequest(id);
    auto *recall = dynamic_cast<ReturnRequest *>(&request);
    if(!recall){
        throw std::invalid_argument("Request is not a return request");
    }
    const Shipment &shipment = branch.findShipment(request.trackingNumber());
    return recall->issueLabel(shipment);
}

std::string RequestRegistry::issueRedirectionLabel(int id, const PostalBranch &branch) {
    ensureBranch(branch);
    CustomerRequest &request = findMutableRequest(id);
    auto *redirection = dynamic_cast<RedirectionRequest *>(&request);
    if(!redirection){
        throw std::invalid_argument("Request is not a redirection request");
    }
    const Shipment &shipment = branch.findShipment(request.trackingNumber());
    return redirection->issueLabel(shipment);
}

void RequestRegistry::settleDamageClaim(int id, PostalBranch &branch) {
    ensureBranch(branch);
    CustomerRequest &request = findMutableRequest(id);
    auto *claim = dynamic_cast<DamageClaim *>(&request);
    if(!claim){
        throw std::invalid_argument("Request is not a damage claim");
    }
    claim->settle(branch);
}
