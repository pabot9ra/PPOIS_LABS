#ifndef SHIPMENTSTATEEXCEPTION_H
#define SHIPMENTSTATEEXCEPTION_H

#include "postalexception.h"
#include "shipment.h"

class ShipmentStateException : public PostalException {
private:
    std::string trackingNumber_;
    ShipmentStatus currentStatus_;
    std::string operation_;

public:
    ShipmentStateException(const std::string &trackingNumber, ShipmentStatus currentStatus, const std::string &operation);

    const std::string &trackingNumber() const;
    ShipmentStatus currentStatus() const;
    const std::string &operation() const;
};

#endif
