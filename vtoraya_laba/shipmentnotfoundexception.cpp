#include "shipmentnotfoundexception.h"

ShipmentNotFoundException::ShipmentNotFoundException(const std::string &trackingNumber)
    : PostalException("Shipment was not found at this branch: " + trackingNumber),
      trackingNumber_(trackingNumber) {
}

const std::string &ShipmentNotFoundException::trackingNumber() const {
    return trackingNumber_;
}
