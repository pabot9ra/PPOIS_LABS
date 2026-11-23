#ifndef MAILBAG_H
#define MAILBAG_H

#include "shipment.h"
#include <cstddef>
#include <string>
#include <vector>

class MailBag {
private:
    std::string id_;
    int sourceBranchId_;
    int destinationBranchId_;
    int maximumWeight_;
    int weightGrams_;
    std::vector<std::string> trackingNumbers_;
    bool sealed_;

public:
    MailBag(const std::string &id, int sourceBranchId, int destinationBranchId, int maximumWeight);

    const std::string &id() const;
    int sourceBranchId() const;
    int destinationBranchId() const;
    int maximumWeight() const;
    int weightGrams() const;
    std::size_t shipmentCount() const;
    const std::string &trackingNumberAt(std::size_t index) const;
    bool sealed() const;
    int remainingCapacity() const;
    bool contains(const std::string &trackingNumber) const;
    void addShipment(const Shipment &shipment);
    void seal();
    void open();
    void clear();
};

#endif
