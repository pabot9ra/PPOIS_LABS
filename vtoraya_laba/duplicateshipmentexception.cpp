#include "duplicateshipmentexception.h"

DuplicateShipmentException::DuplicateShipmentException(const std::string &trackingNumber)
    : PostalException("Shipment is already registered: " + trackingNumber), trackingNumber_(trackingNumber) {
}

const std::string &DuplicateShipmentException::trackingNumber() const {
    return trackingNumber_;
}
