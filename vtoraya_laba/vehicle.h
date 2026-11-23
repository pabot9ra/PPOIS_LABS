#ifndef VEHICLE_H
#define VEHICLE_H

#include "mailbag.h"
#include "route.h"
#include <cstddef>
#include <string>
#include <vector>

class Vehicle {
private:
    std::string plate_;
    int maximumLoad_;
    Route route_;
    std::vector<MailBag> bags_;
    std::size_t currentStop_;
    bool moving_;

    void ensureCanLoad(const MailBag &bag) const;
    void ensureUniqueContents(const MailBag &bag) const;

public:
    Vehicle(const std::string &plate, int maximumLoad, const Route &route);

    const std::string &plate() const;
    int maximumLoad() const;
    int currentBranchId() const;
    bool moving() const;
    std::size_t bagCount() const;
    int loadGrams() const;
    int remainingCapacity() const;
    void loadBag(const MailBag &bag);
    MailBag unloadBag(const std::string &bagId);
    void depart();
    void arriveAt(int branchId);
};

#endif
