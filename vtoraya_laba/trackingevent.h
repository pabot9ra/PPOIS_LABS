#ifndef TRACKINGEVENT_H
#define TRACKINGEVENT_H

#include "postalbranch.h"
#include <string>

class TrackingEvent {
private:
    std::string trackingNumber_;
    ShipmentStatus status_;
    int branchId_;
    int minute_;
    std::string description_;

public:
    TrackingEvent(const PostalBranch &branch, const std::string &trackingNumber,
                  int minute, const std::string &description);

    const std::string &trackingNumber() const;
    ShipmentStatus status() const;
    int branchId() const;
    int minute() const;
    const std::string &description() const;
    std::string format() const;
};

#endif
