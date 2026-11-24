#include "shipmentstateexception.h"

ShipmentStateException::ShipmentStateException(const std::string &trackingNumber, ShipmentStatus currentStatus,
                                               const std::string &operation)
    : PostalException("Cannot " + operation + " shipment " + trackingNumber + " in its current state"),
      trackingNumber_(trackingNumber), currentStatus_(currentStatus), operation_(operation) {
}

const std::string &ShipmentStateException::trackingNumber() const {
    return trackingNumber_;
}

ShipmentStatus ShipmentStateException::currentStatus() const {
    return currentStatus_;
}

const std::string &ShipmentStateException::operation() const {
    return operation_;
}
