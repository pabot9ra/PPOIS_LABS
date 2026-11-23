#include "trackinghistory.h"
#include <stdexcept>

TrackingHistory::TrackingHistory(const Shipment &shipment)
    : trackingNumber_(shipment.trackingNumber()) {
}

const std::string &TrackingHistory::trackingNumber() const {
    return trackingNumber_;
}

std::size_t TrackingHistory::eventCount() const {
    return events_.size();
}

const TrackingEvent &TrackingHistory::eventAt(std::size_t index) const {
    return events_.at(index);
}

const TrackingEvent &TrackingHistory::lastEvent() const {
    if(events_.empty()){
        throw std::out_of_range("Tracking history is empty");
    }
    return events_.back();
}

void TrackingHistory::validateTransition(ShipmentStatus next) const {
    if(events_.empty()){
        if(next != ShipmentStatus::Accepted){
            throw std::logic_error("History must start with an accepted shipment");
        }
        return;
    }
    ShipmentStatus previous = events_.back().status();
    bool allowed = previous == next ||
                   (previous == ShipmentStatus::Accepted && next == ShipmentStatus::InTransit) ||
                   (previous == ShipmentStatus::InTransit && next == ShipmentStatus::Delivered);
    if(!allowed){
        throw std::logic_error("Tracking status cannot move backwards or skip dispatch");
    }
}

void TrackingHistory::record(const TrackingEvent &event) {
    if(event.trackingNumber() != trackingNumber_){
        throw std::invalid_argument("Event belongs to another shipment");
    }
    if(!events_.empty() && event.minute() < events_.back().minute()){
        throw std::invalid_argument("Tracking events must be chronological");
    }
    validateTransition(event.status());
    events_.push_back(event);
}

int TrackingHistory::elapsedMinutes() const {
    if(events_.empty()){
        return 0;
    }
    return events_.back().minute() - events_.front().minute();
}

bool TrackingHistory::hasStatus(ShipmentStatus status) const {
    for(const TrackingEvent &event : events_){
        if(event.status() == status){
            return true;
        }
    }
    return false;
}

const TrackingEvent &TrackingHistory::findLastAtBranch(int branchId) const {
    for(auto it = events_.rbegin(); it != events_.rend(); ++it){
        if(it->branchId() == branchId){
            return *it;
        }
    }
    throw std::out_of_range("No tracking event was recorded at this branch");
}
