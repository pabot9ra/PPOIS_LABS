#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "customer.h"
#include <string>

enum class ShipmentStatus {
    Accepted,
    InTransit,
    Delivered
};

class Shipment {
private:
    std::string trackingNumber_;
    Customer sender_;
    Customer recipient_;
    ShipmentStatus status_;

public:
    Shipment(const std::string &trackingNumber, const Customer &sender,
             const Customer &recipient);

    const std::string &trackingNumber() const;
    const Customer &sender() const;
    const Customer &recipient() const;
    ShipmentStatus status() const;

    void dispatch();
    void deliver();
};

#endif
