#ifndef BRANCHDIRECTORY_H
#define BRANCHDIRECTORY_H

#include "postalbranch.h"
#include <cstddef>
#include <map>
#include <memory>

class BranchDirectory {
private:
    std::map<int, std::unique_ptr<PostalBranch>> branches_;

    PostalBranch &findMutable(int id);

public:
    std::size_t size() const;
    bool contains(int id) const;
    void add(int id, const PostalAddress &address);
    const PostalBranch &find(int id) const;
    void remove(int id);
    std::size_t shipmentCount() const;
    bool containsShipment(const std::string &trackingNumber) const;
    int locateShipment(const std::string &trackingNumber) const;
    void acceptShipment(int branchId, const Shipment &shipment);
    void dispatchShipment(int branchId, const std::string &trackingNumber);
    void deliverShipment(int branchId, const std::string &trackingNumber);
};

#endif
