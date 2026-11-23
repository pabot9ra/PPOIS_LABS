#include "trackingevent.h"
#include <cctype>
#include <stdexcept>

TrackingEvent::TrackingEvent(const PostalBranch &branch, const std::string &trackingNumber,
                             int minute, const std::string &description)
    : trackingNumber_(trackingNumber), status_(branch.findShipment(trackingNumber).status()),
      branchId_(branch.id()), minute_(minute), description_(description) {
    if(minute_ < 0){
        throw std::invalid_argument("Event minute must not be negative");
    }
    bool hasDescription = false;
    for(char character : description_){
        if(!std::isspace(static_cast<unsigned char>(character))){
            hasDescription = true;
            break;
        }
    }
    if(!hasDescription){
        throw std::invalid_argument("Event description must not be blank");
    }
}

const std::string &TrackingEvent::trackingNumber() const {
    return trackingNumber_;
}

ShipmentStatus TrackingEvent::status() const {
    return status_;
}

int TrackingEvent::branchId() const {
    return branchId_;
}

int TrackingEvent::minute() const {
    return minute_;
}

const std::string &TrackingEvent::description() const {
    return description_;
}

std::string TrackingEvent::format() const {
    return trackingNumber_ + ", branch " + std::to_string(branchId_) +
           ", minute " + std::to_string(minute_) + ": " + description_;
}
