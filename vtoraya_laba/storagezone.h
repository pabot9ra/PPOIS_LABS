#ifndef STORAGEZONE_H
#define STORAGEZONE_H

#include "storagecell.h"
#include <cstddef>
#include <vector>

class StorageZone {
private:
    std::string id_;
    int branchId_;
    std::vector<StorageCell> cells_;
    bool locked_;

public:
    StorageZone(const std::string &id, int branchId);

    const std::string &id() const;
    int branchId() const;
    bool locked() const;
    std::size_t cellCount() const;
    std::size_t occupiedCount() const;
    long long weightGrams() const;
    void addCell(const StorageCell &cell);
    const StorageCell &findCell(const std::string &id) const;
    const StorageCell &findShipmentCell(const std::string &trackingNumber) const;
    const StorageCell &findAvailableCell(const Shipment &shipment, const Dimensions &contents) const;
    bool contains(const std::string &trackingNumber) const;
    bool canStore(const Shipment &shipment, const Dimensions &contents) const;
    void storeShipment(const Shipment &shipment, const Dimensions &contents);
    void releaseShipment(const std::string &trackingNumber);
    void lock();
    void unlock();
};

#endif
