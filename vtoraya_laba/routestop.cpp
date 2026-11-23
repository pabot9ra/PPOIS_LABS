#include "routestop.h"
#include <stdexcept>

RouteStop::RouteStop(const PostalBranch &branch, int distanceKm, int plannedMinute)
    : branchId_(branch.id()), address_(branch.address()),
      distanceKm_(distanceKm), plannedMinute_(plannedMinute) {
    if(distanceKm_ < 0 || plannedMinute_ < 0){
        throw std::invalid_argument("Distance and planned minute must not be negative");
    }
}

int RouteStop::branchId() const {
    return branchId_;
}

const PostalAddress &RouteStop::address() const {
    return address_;
}

int RouteStop::distanceKm() const {
    return distanceKm_;
}

int RouteStop::plannedMinute() const {
    return plannedMinute_;
}

std::string RouteStop::describe() const {
    return "Branch " + std::to_string(branchId_) + ", " + address_.formatLabel() +
           ", planned minute " + std::to_string(plannedMinute_);
}
