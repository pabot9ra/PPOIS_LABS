#ifndef SHIPMENT_H
#define SHIPMENT_H

#include "customer.h"
#include <memory>
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
    virtual ~Shipment() = default;

    virtual int calculatePrice() const = 0;
    virtual int weightGrams() const = 0;
    virtual std::unique_ptr<Shipment> clone() const = 0;

    const std::string &trackingNumber() const;
    const Customer &sender() const;
    const Customer &recipient() const;
    ShipmentStatus status() const;

    void dispatch();
    void deliver();
};

#endif
