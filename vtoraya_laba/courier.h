#ifndef COURIER_H
#define COURIER_H

#include "employee.h"
#include <cstddef>

class Courier : public Employee {
private:
    std::size_t dispatchedCount_;
    std::size_t deliveredCount_;

public:
    Courier(int id, const std::string &name, int branchId);

    std::string role() const override;
    std::size_t dispatchedCount() const;
    std::size_t deliveredCount() const;
    void dispatchShipment(PostalBranch &branch, const std::string &trackingNumber);
    void deliverShipment(PostalBranch &branch, const std::string &trackingNumber);
};

#endif
