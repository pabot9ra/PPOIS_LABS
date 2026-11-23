#ifndef SORTINGCENTER_H
#define SORTINGCENTER_H

#include "storageinventory.h"
#include "mailbag.h"
#include <cstddef>
#include <vector>

class SortingCenter {
private:
    int id_;
    int branchId_;
    StorageInventory inventory_;
    std::vector<MailBag> bags_;
    bool open_;

    void ensureOpen() const;
    MailBag &findMutableBag(const std::string &bagId);

public:
    SortingCenter(int id, const PostalBranch &branch, int maximumWeight);

    int id() const;
    int branchId() const;
    bool isOpen() const;
    std::size_t storedCount() const;
    std::size_t bagCount() const;
    long long storedWeightGrams() const;
    void addZone(const StorageZone &zone);
    void openShift();
    void closeShift();
    StorageLocation receiveShipment(const Shipment &shipment, const Dimensions &contents);
    StorageLocation locateShipment(const std::string &trackingNumber) const;
    void createBag(const std::string &bagId, int destinationBranchId, int maximumWeight);
    const MailBag &findBag(const std::string &bagId) const;
    void sortIntoBag(const Shipment &shipment, const std::string &bagId);
    void sealBag(const std::string &bagId);
    MailBag takeBag(const std::string &bagId);
};

#endif
