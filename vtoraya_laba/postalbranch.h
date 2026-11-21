#ifndef POSTALBRANCH_H
#define POSTALBRANCH_H

#include "shipment.h"
#include <cstddef>
#include <string>
#include <vector>

class PostalBranch {
private:
    int id_;
    PostalAddress address_;
    std::vector<Shipment> shipments_;

public:
    PostalBranch(int id, const PostalAddress &address);

    int id() const;
    const PostalAddress &address() const;
    std::size_t shipmentCount() const;

    void acceptShipment(const Shipment &shipment);
    Shipment findShipment(const std::string &trackingNumber) const;
};

#endif
