#include "courier.h"

Courier::Courier(int id, const std::string &name, int branchId)
    : Employee(id, name, branchId), dispatchedCount_(0), deliveredCount_(0) {
}

std::string Courier::role() const {
    return "Courier";
}

std::size_t Courier::dispatchedCount() const {
    return dispatchedCount_;
}

std::size_t Courier::deliveredCount() const {
    return deliveredCount_;
}

void Courier::dispatchShipment(PostalBranch &branch, const std::string &trackingNumber) {
    ensureCanWork(branch);
    branch.dispatchShipment(trackingNumber);
    dispatchedCount_++;
}

void Courier::deliverShipment(PostalBranch &branch, const std::string &trackingNumber) {
    ensureCanWork(branch);
    branch.deliverShipment(trackingNumber);
    deliveredCount_++;
}
