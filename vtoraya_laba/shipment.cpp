#include "shipment.h"
#include "shipmentstateexception.h"
#include <cctype>
#include <stdexcept>

Shipment::Shipment(const std::string &trackingNumber, const Customer &sender,
                   const Customer &recipient)
    : trackingNumber_(trackingNumber), sender_(sender), recipient_(recipient),
      status_(ShipmentStatus::Accepted) {
    if(trackingNumber_.empty()){
        throw std::invalid_argument("Tracking number must not be empty");
    }
    for(char character : trackingNumber_){
        if(std::isspace(static_cast<unsigned char>(character))){
            throw std::invalid_argument("Tracking number must not contain whitespace");
        }
    }
}

const std::string &Shipment::trackingNumber() const {
    return trackingNumber_;
}

const Customer &Shipment::sender() const {
    return sender_;
}

const Customer &Shipment::recipient() const {
    return recipient_;
}

ShipmentStatus Shipment::status() const {
    return status_;
}

void Shipment::dispatch() {
    if(status_ != ShipmentStatus::Accepted){
        throw ShipmentStateException(trackingNumber_, status_, "dispatch");
    }
    status_ = ShipmentStatus::InTransit;
}

void Shipment::deliver() {
    if(status_ != ShipmentStatus::InTransit){
        throw ShipmentStateException(trackingNumber_, status_, "deliver");
    }
    status_ = ShipmentStatus::Delivered;
}
