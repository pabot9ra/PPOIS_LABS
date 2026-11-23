#ifndef POSTALCLERK_H
#define POSTALCLERK_H

#include "employee.h"
#include <cstddef>

class PostalClerk : public Employee {
private:
    std::size_t acceptedCount_;

public:
    PostalClerk(int id, const std::string &name, int branchId);

    std::string role() const override;
    std::size_t acceptedCount() const;
    void acceptShipment(PostalBranch &branch, const Shipment &shipment);
};

#endif
