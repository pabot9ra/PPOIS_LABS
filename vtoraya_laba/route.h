#ifndef ROUTE_H
#define ROUTE_H

#include "routestop.h"
#include <cstddef>
#include <string>
#include <vector>

class Route {
private:
    int id_;
    std::string name_;
    std::vector<RouteStop> stops_;

public:
    Route(int id, const std::string &name);

    int id() const;
    const std::string &name() const;
    std::size_t stopCount() const;
    const RouteStop &stopAt(std::size_t index) const;
    void addStop(const RouteStop &stop);
    int totalDistanceKm() const;
    bool canReachAfter(std::size_t currentStop, int branchId) const;
};

#endif
