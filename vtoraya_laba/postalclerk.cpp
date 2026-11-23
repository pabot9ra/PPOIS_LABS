#include "postalclerk.h"

PostalClerk::PostalClerk(int id, const std::string &name, int branchId)
    : Employee(id, name, branchId), acceptedCount_(0) {
}

std::string PostalClerk::role() const {
    return "Postal clerk";
}

std::size_t PostalClerk::acceptedCount() const {
    return acceptedCount_;
}

void PostalClerk::acceptShipment(PostalBranch &branch, const Shipment &shipment) {
    ensureCanWork(branch);
    branch.acceptShipment(shipment);
    acceptedCount_++;
}
