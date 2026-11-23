#ifndef DAMAGECLAIM_H
#define DAMAGECLAIM_H

#include "customerrequest.h"
#include "postalbranch.h"

class DamageClaim : public CustomerRequest {
private:
    int requestedAmount_;
    int settledAmount_;

protected:
    void validateEligibility(const Shipment &shipment) const override;

public:
    DamageClaim(int id, const Customer &applicant, const Shipment &shipment,
                int openedMinute, const std::string &reason, int requestedAmount);

    int requestedAmount() const;
    int settledAmount() const;
    void settle(PostalBranch &branch);
    std::unique_ptr<CustomerRequest> clone() const override;
};

#endif
