#ifndef ROUTESTOP_H
#define ROUTESTOP_H

#include "postalbranch.h"
#include <string>

class RouteStop {
private:
    int branchId_;
    PostalAddress address_;
    int distanceKm_;
    int plannedMinute_;

public:
    RouteStop(const PostalBranch &branch, int distanceKm, int plannedMinute);

    int branchId() const;
    const PostalAddress &address() const;
    int distanceKm() const;
    int plannedMinute() const;
    std::string describe() const;
};

#endif
