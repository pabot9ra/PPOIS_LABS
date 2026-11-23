#ifndef SHIPMENTNOTFOUNDEXCEPTION_H
#define SHIPMENTNOTFOUNDEXCEPTION_H

#include "postalexception.h"

class ShipmentNotFoundException : public PostalException {
private:
    std::string trackingNumber_;

public:
    explicit ShipmentNotFoundException(const std::string &trackingNumber);

    const std::string &trackingNumber() const;
};

#endif
