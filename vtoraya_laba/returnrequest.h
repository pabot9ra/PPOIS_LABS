#ifndef RETURNREQUEST_H
#define RETURNREQUEST_H

#include "customerrequest.h"

class ReturnRequest : public CustomerRequest {
private:
    PostalAddress returnAddress_;
    int returnFee_;

protected:
    void validateEligibility(const Shipment &shipment) const override;

public:
    ReturnRequest(int id, const Customer &applicant, const Shipment &shipment,
                  int openedMinute, const std::string &reason, int returnFee);

    const PostalAddress &returnAddress() const;
    int returnFee() const;
    std::string issueLabel(const Shipment &shipment);
    std::unique_ptr<CustomerRequest> clone() const override;
};

#endif
