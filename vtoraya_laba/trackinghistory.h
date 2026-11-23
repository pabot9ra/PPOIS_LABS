#ifndef TRACKINGHISTORY_H
#define TRACKINGHISTORY_H

#include "trackingevent.h"
#include <cstddef>
#include <vector>

class TrackingHistory {
private:
    std::string trackingNumber_;
    std::vector<TrackingEvent> events_;

    void validateTransition(ShipmentStatus next) const;

public:
    explicit TrackingHistory(const Shipment &shipment);

    const std::string &trackingNumber() const;
    std::size_t eventCount() const;
    const TrackingEvent &eventAt(std::size_t index) const;
    const TrackingEvent &lastEvent() const;
    void record(const TrackingEvent &event);
    int elapsedMinutes() const;
    bool hasStatus(ShipmentStatus status) const;
    const TrackingEvent &findLastAtBranch(int branchId) const;
};

#endif
