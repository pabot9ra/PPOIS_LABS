#ifndef POSTALNETWORK_H
#define POSTALNETWORK_H

#include "customerdirectory.h"
#include "branchdirectory.h"
#include <cstddef>
#include <string>

class PostalNetwork {
private:
    std::string name_;
    CustomerDirectory customers_;
    BranchDirectory branches_;

public:
    explicit PostalNetwork(const std::string &name);

    const std::string &name() const;
    std::size_t customerCount() const;
    std::size_t branchCount() const;
    std::size_t shipmentCount() const;
    void registerCustomer(const Customer &customer);
    void openBranch(int id, const PostalAddress &address);
    const Customer &customer(int id) const;
    const PostalBranch &branch(int id) const;
    void changeCustomerAddress(int id, const PostalAddress &address);
    void acceptShipment(int branchId, const Shipment &shipment);
    int shipmentBranchId(const std::string &trackingNumber) const;
    const Shipment &findShipment(const std::string &trackingNumber) const;
    void dispatchShipment(const std::string &trackingNumber);
    void deliverShipment(const std::string &trackingNumber);
};

#endif
