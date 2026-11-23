#ifndef STORAGEINVENTORY_H
#define STORAGEINVENTORY_H

#include "storagezone.h"
#include "storagelocation.h"
#include "postalbranch.h"
#include <cstddef>
#include <vector>

class StorageInventory {
private:
    int branchId_;
    int maximumWeight_;
    std::vector<StorageZone> zones_;

    void ensureCanStore(const Shipment &shipment) const;

public:
    StorageInventory(const PostalBranch &branch, int maximumWeight);

    int branchId() const;
    std::size_t zoneCount() const;
    std::size_t shipmentCount() const;
    long long weightGrams() const;
    long long remainingCapacity() const;
    void addZone(const StorageZone &zone);
    bool contains(const std::string &trackingNumber) const;
    StorageLocation storeShipment(const Shipment &shipment, const Dimensions &contents);
    StorageLocation locateShipment(const std::string &trackingNumber) const;
    int storedWeightGrams(const std::string &trackingNumber) const;
    void releaseShipment(const std::string &trackingNumber);
};

#endif
