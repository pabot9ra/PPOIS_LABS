#ifndef STORAGECELL_H
#define STORAGECELL_H

#include "dimensions.h"
#include "shipment.h"
#include <string>

class StorageCell {
private:
    std::string id_;
    int branchId_;
    Dimensions dimensions_;
    int maximumWeight_;
    std::string trackingNumber_;
    int weightGrams_;

public:
    StorageCell(const std::string &id, int branchId, const Dimensions &dimensions, int maximumWeight);

    const std::string &id() const;
    int branchId() const;
    const Dimensions &dimensions() const;
    int maximumWeight() const;
    const std::string &trackingNumber() const;
    int weightGrams() const;
    bool occupied() const;
    bool canFit(const Shipment &shipment, const Dimensions &contents) const;
    void store(const Shipment &shipment, const Dimensions &contents);
    void release();
};

#endif
