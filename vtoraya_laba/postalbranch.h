#ifndef POSTALBRANCH_H
#define POSTALBRANCH_H

#include "shipment.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class PostalBranch {
private:
    int id_;
    PostalAddress address_;
    std::vector<std::unique_ptr<Shipment>> shipments_;

    Shipment &findMutableShipment(const std::string &trackingNumber);

public:
    PostalBranch(int id, const PostalAddress &address);

    int id() const;
    const PostalAddress &address() const;
    std::size_t shipmentCount() const;

    void acceptShipment(const Shipment &shipment);
    const Shipment &findShipment(const std::string &trackingNumber) const;
    void dispatchShipment(const std::string &trackingNumber);
    void deliverShipment(const std::string &trackingNumber);
};

#endif
